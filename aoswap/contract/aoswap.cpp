#include "aoswap.hpp"
#include "utils.hpp"

using namespace aoswap;

void aoswap::openext( const name& user, const name& payer, const name& token_contract, const symbol& token_sym) {
    check( is_account( user ), "user account does not exist" );
    require_auth( payer );
    evodexacnts acnts( get_self(), user.value );
    auto index = acnts.get_index<"extended"_n>();
    const auto& acnt_balance = index.find( 
      make128key(token_contract.value, token_sym.raw()) );
    if( acnt_balance == index.end() ) {
        acnts.emplace( payer, [&]( auto& a ){
          auto ext_symbol = extended_symbol{token_sym, token_contract} ;
          a.balance = extended_asset{0, ext_symbol};
          a.id = acnts.available_primary_key();
        });
    }
}

void aoswap::closeext ( const name& user, const name& to, const name& token_contract, const symbol& token_sym, string memo) {
    require_auth( user );
    evodexacnts acnts( get_self(), user.value );
    auto index = acnts.get_index<"extended"_n>();
    const auto& acnt_balance = index.find( make128key(token_contract.value, token_sym.raw()) );
    check( acnt_balance != index.end(), "User does not have such token" );
    auto ext_balance = acnt_balance->balance;
    if (ext_balance.quantity.amount > 0) {
      string_view act = "transfer";
      if (ext_balance.contract == "ciphertokens"_n) {
        act = "transferpp";
      }
      action(permission_level{ get_self(), "active"_n }, ext_balance.contract, name(act),
        std::make_tuple( get_self(), to, ext_balance.quantity, memo) ).send();
    }
    index.erase( acnt_balance );
}

void aoswap::ontransfer(name from, name to, asset quantity, string memo) {
  on_transfer(from, to, quantity, memo);
}

void aoswap::ontransferpp(name from, name to, asset quantity, string memo) {
  on_transfer(from, to, quantity, memo);
}

void aoswap::on_transfer(name from, name to, asset quantity, string memo) {
    constexpr string_view DEPOSIT_TO = "deposit to:";
    constexpr string_view EXCHANGE   = "exchange:";

    if (from == get_self()) return;
    check(to == get_self(), "This transfer is not for aoswap");
    check(quantity.amount >= 0, "quantity must be positive");

    auto incoming = extended_asset{quantity, get_first_receiver()};
    string_view memosv(memo);
    if ( starts_with(memosv, EXCHANGE) ) {
      memoexchange(from, incoming, memosv.substr(EXCHANGE.size()) );
    } else {
      if ( starts_with(memosv, DEPOSIT_TO) ) {
          from = name(trim(memosv.substr(DEPOSIT_TO.size())));
          check(from != get_self(), "Donation not accepted");
      }
      add_signed_ext_balance(from, incoming);
    }
}

void aoswap::withdraw(name user, name to, const name& token_contract, const asset& token_quantity, string memo) {
    require_auth( user );
    check(token_quantity.amount > 0, "quantity must be positive");

    extended_asset to_withdraw;
    to_withdraw.contract = token_contract;
    to_withdraw.quantity = token_quantity;
    add_signed_ext_balance(user, -to_withdraw);

    string_view act = "transfer";
    if (token_contract == "ciphertokens"_n) {
      act = "transferpp";
    }
    action(permission_level{ get_self(), "active"_n }, token_contract, name(act),
      std::make_tuple( get_self(), to, token_quantity, memo) ).send();
}

void aoswap::addliquidity(name user, asset to_buy, 
  asset max_asset1, asset max_asset2) {
    require_auth(user);
    check( (to_buy.amount > 0), "to_buy amount must be positive");
    check( (max_asset1.amount >= 0) && (max_asset2.amount >= 0), "assets must be nonnegative");
    add_signed_liq(user, to_buy, true, max_asset1, max_asset2);
}

void aoswap::remliquidity(name user, asset to_sell,
  asset min_asset1, asset min_asset2) {
    require_auth(user);
    check(to_sell.amount > 0, "to_sell amount must be positive");
    check( (min_asset1.amount >= 0) && (min_asset2.amount >= 0), "assets must be nonnegative");
    add_signed_liq(user, -to_sell, false, -min_asset1, -min_asset2);
}

// computes x * y / z plus the fee
int64_t aoswap::compute(int64_t x, int64_t y, int64_t z, int fee) {
    check( (x != 0) && (y > 0) && (z > 0), "invalid parameters");
    int128_t prod = int128_t(x) * int128_t(y);
    int128_t tmp = 0;
    int128_t tmp_fee = 0;
    if (x > 0) {
        tmp = 1 + (prod - 1) / int128_t(z);
        check( (tmp <= MAX), "computation overflow" );
        tmp_fee = (tmp * fee + 9999) / 10000;
    } else {
        tmp = prod / int128_t(z);
        check( (tmp >= -MAX), "computation underflow" );
        tmp_fee =  (-tmp * fee + 9999) / 10000;
    }
    tmp += tmp_fee;
    return int64_t(tmp);
}

void aoswap::add_signed_liq(name user, asset to_add, bool is_buying,
  asset max_asset1, asset max_asset2){
    check( to_add.is_valid(), "invalid asset");
    stats statstable( get_self(), to_add.symbol.code().raw() );
    const auto& token = statstable.find( to_add.symbol.code().raw() );
    check ( token != statstable.end(), "pair token does not exist" );
    auto A = token-> supply.amount;
    auto P1 = token-> pool1.quantity.amount;
    auto P2 = token-> pool2.quantity.amount;

    int fee = is_buying? token->fee : 0;
    auto to_pay1 = extended_asset{ asset{compute(to_add.amount, P1, A, fee),
      token->pool1.quantity.symbol}, token->pool1.contract};
    auto to_pay2 = extended_asset{ asset{compute(to_add.amount, P2, A, fee),
      token->pool2.quantity.symbol}, token->pool2.contract};
    check( (to_pay1.quantity.symbol == max_asset1.symbol) && 
           (to_pay2.quantity.symbol == max_asset2.symbol), "incorrect symbol");
    check( (to_pay1.quantity.amount <= max_asset1.amount) && 
           (to_pay2.quantity.amount <= max_asset2.amount), "available is less than expected");

    add_signed_ext_balance(user, -to_pay1);
    add_signed_ext_balance(user, -to_pay2);
    (to_add.amount > 0)? add_balance(user, to_add, user) : sub_balance(user, -to_add);
    if (token->fee_contract) require_recipient(token->fee_contract);
    statstable.modify( token, same_payer, [&]( auto& a ) {
      a.supply += to_add;
      a.pool1 += to_pay1;
      a.pool2 += to_pay2;
    });
    if (token-> supply.amount == 0) {
      statstable.erase(token);

      // Remove pair
      globals globaltable(get_self(), _self.value);
      auto g = globaltable.begin();
      if (g != globaltable.end()) {
        globaltable.modify(*g, same_payer, [&](auto& a) {
          for (auto i = a.pairs.begin(); i != a.pairs.end(); ++i) {
            if (i->tokenx == to_add.symbol.code().to_string()) {
              a.pairs.erase(i);
              break;
            }
          }
        });
      }
    }
}


void aoswap::exchange( name user, symbol_code pair_token,
  const name& token_contract, const asset& token_quantity, asset min_expected ) {
    require_auth(user);
    extended_asset ext_asset_in;
    ext_asset_in.contract = token_contract;
    ext_asset_in.quantity = token_quantity;
    check( ((ext_asset_in.quantity.amount > 0) && (min_expected.amount >= 0)) ||
           ((ext_asset_in.quantity.amount < 0) && (min_expected.amount <= 0)), 
           "ext_asset_in must be nonzero and min_expected must have same sign or be zero");
    auto ext_asset_out = process_exch(pair_token, ext_asset_in, min_expected);
    add_signed_ext_balance(user, -ext_asset_in);
    add_signed_ext_balance(user, ext_asset_out);
}

extended_asset aoswap::process_exch(symbol_code pair_token,
  extended_asset ext_asset_in, asset min_expected){
    stats statstable( get_self(), pair_token.raw() );
    const auto token = statstable.find( pair_token.raw() );
    check ( token != statstable.end(), "pair token does not exist" );
    bool in_first;
    if ((token->pool1.get_extended_symbol() == ext_asset_in.get_extended_symbol()) && 
        (token->pool2.quantity.symbol == min_expected.symbol)) {
        in_first = true;
    } else if ((token->pool1.quantity.symbol == min_expected.symbol) &&
               (token->pool2.get_extended_symbol() == ext_asset_in.get_extended_symbol())) {
        in_first = false;
    }
    else check(false, "extended_symbol mismatch");
    int64_t P_in, P_out;
    if (in_first) { 
      P_in = token-> pool1.quantity.amount;
      P_out = token-> pool2.quantity.amount;
    } else {
      P_in = token-> pool2.quantity.amount;
      P_out = token-> pool1.quantity.amount;
    }
    auto A_in = ext_asset_in.quantity.amount;
    int64_t A_out = compute(-A_in, P_out, P_in + A_in, token->fee);
    check(min_expected.amount <= -A_out, "available is less than expected");
    extended_asset ext_asset1, ext_asset2, ext_asset_out;
    if (in_first) { 
      ext_asset1 = ext_asset_in;
      ext_asset2 = extended_asset{A_out, token-> pool2.get_extended_symbol()};
      ext_asset_out = -ext_asset2;
    } else {
      ext_asset1 = extended_asset{A_out, token-> pool1.get_extended_symbol()};
      ext_asset2 = ext_asset_in;
      ext_asset_out = -ext_asset1;
    }
    statstable.modify( token, same_payer, [&]( auto& a ) {
      a.pool1 += ext_asset1;
      a.pool2 += ext_asset2;
    });
    return ext_asset_out;
}

void aoswap::memoexchange(name user, extended_asset ext_asset_in, string_view details){
    auto parts = split(details, ",");
    check(parts.size() >= 2, "Expected format 'EVOTOKEN,min_expected_asset,optional memo'");

    auto pair_token   = symbol_code(parts[0]);
    auto min_expected = asset_from_string(parts[1]);
    auto second_comma_pos = details.find(",", 1 + details.find(","));
    auto memo = (second_comma_pos == string::npos)? "" : details.substr(1 + second_comma_pos);

    check(min_expected.amount >= 0, "min_expected must be expressed with a positive amount");
    auto ext_asset_out = process_exch(pair_token, ext_asset_in, min_expected);
    string_view act = "transfer";
    if (ext_asset_out.contract == "ciphertokens"_n) {
      act = "transferpp";
    }
    action(permission_level{ get_self(), "active"_n }, ext_asset_out.contract, name(act),
      std::make_tuple( get_self(), user, ext_asset_out.quantity, std::string(memo)) ).send();
}

void aoswap::inittoken(name user, symbol new_symbol, name contract_pool1, asset quantity_pool1, name contract_pool2, 
asset quantity_pool2, int initial_fee, name fee_contract, string site1, string site2, string logo1, string logo2)
{
    require_auth( user );
    check((quantity_pool1.amount > 0) && (quantity_pool2.amount > 0), "Both assets must be positive");
    check((quantity_pool1.amount < INIT_MAX) && (quantity_pool2.amount < INIT_MAX), "Initial amounts must be less than 10^15");
    int128_t geometric_mean = sqrt(int128_t(quantity_pool1.amount) * int128_t(quantity_pool2.amount));
    auto new_token = asset{int64_t(geometric_mean), new_symbol};
    //check( initial_pool1.get_extended_symbol() != initial_pool2.get_extended_symbol(), "extended symbols must be different");
    check(contract_pool1 != contract_pool2 || quantity_pool1.symbol.code() != quantity_pool2.symbol.code(), "extended symbols must be different");
    stats statstable( get_self(), new_token.symbol.code().raw() );
    const auto& token = statstable.find( new_token.symbol.code().raw() );
    check ( token == statstable.end(), "token symbol already exists" );
    check( (0 <= initial_fee) && (initial_fee <= 500), "initial fee out of reasonable range");
    check( is_account(fee_contract) || !fee_contract, "fee_contract account must exist or be empty");

    token_info token1, token2;
    token1.symbol = quantity_pool1.symbol.code().to_string();
    token1.contract = contract_pool1.to_string();
    token1.site = site1;
    token1.logo = logo1;

    token2.symbol = quantity_pool2.symbol.code().to_string();
    token2.contract = contract_pool2.to_string();
    token2.site = site2;
    token2.logo = logo2;

    token_pair pair;
    pair.tokenx = new_token.symbol.code().to_string();
    pair.token1 = token1;
    pair.token2 = token2;

    globals globaltable(get_self(), _self.value);
    auto g = globaltable.begin();
    if (g == globaltable.end()) {
      // not exist
      globaltable.emplace( user, [&](auto& a) {
        a.owner = get_self();
        a.pairs.push_back(pair);
      });
    } else {
      globaltable.modify(*g, user, [&](auto& a) {
        a.pairs.push_back(pair);
      });
    }

    extended_asset initial_pool1, initial_pool2;
    initial_pool1.contract = contract_pool1;
    initial_pool1.quantity = quantity_pool1;
    initial_pool2.contract = contract_pool2;
    initial_pool2.quantity = quantity_pool2;

    statstable.emplace( user, [&]( auto& a ) {
        a.supply = new_token;
        a.max_supply = asset{MAX,new_token.symbol};
        a.issuer = get_self();
        a.pool1 = initial_pool1;
        a.pool2 = initial_pool2;
        a.fee = initial_fee;
        a.fee_contract = fee_contract;
    } );

    add_balance(user, new_token, user);
    add_signed_ext_balance(user, -initial_pool1);
    add_signed_ext_balance(user, -initial_pool2);
}

void aoswap::changefee(symbol_code pair_token, int newfee) {
    check( (0 <= newfee) && (newfee <= 500), "new fee out of reasonable range");
    stats statstable( get_self(), pair_token.raw() );
    const auto& token = statstable.find( pair_token.raw() );
    check ( token != statstable.end(), "pair token does not exist" );
    check( (token->fee_contract) != ""_n, "this pair token has fixed fee parameter" );
    require_auth(token->fee_contract);
    statstable.modify( token, same_payer, [&]( auto& a ) {
      a.fee = newfee;
    } );
}

uint128_t aoswap::make128key(uint64_t a, uint64_t b) {
    uint128_t aa = a;
    uint128_t bb = b;
    return (aa << 64) + bb;
}

void aoswap::add_signed_ext_balance( const name& user, const extended_asset& to_add )
{
    check( to_add.quantity.is_valid(), "invalid asset" );
    evodexacnts acnts( get_self(), user.value );
    auto index = acnts.get_index<"extended"_n>();
    const auto& acnt_balance = index.find( make128key(to_add.contract.value, to_add.quantity.symbol.raw() ) );
    check( acnt_balance != index.end(), "extended_symbol not registered for this user,\
 please run openext action or write exchange details in the memo of your transfer");
    index.modify( acnt_balance, same_payer, [&]( auto& a ) {
        a.balance += to_add;
        check( a.balance.quantity.amount >= 0, "insufficient funds");
    });
}