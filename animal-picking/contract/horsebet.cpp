#include "horsebet.hpp"
#include <eosio/crypto.hpp>
#include <eosio/system.hpp>
#include <algorithm>

const static string_view TOKEN_CONTRACT = "aosio.token";
const static string TOKEN_SYMBOL = "AOS";
const static int LOW_NUMBER = 1;
const static int HIG_NUMBER = 16;
const static int RWD_FACTOR = 13;

bool starts_with(string_view sv, string_view s) {
    return sv.size() >= s.size() && sv.compare(0, s.size(), s) == 0;
}

// reference from https://github.com/MitchPierias/advanced-eos-examples/blob/master/01_Hashes/hashes.cpp
static string to_hex(const checksum256 &hashed) {
    // Construct variables
    string result;
    const char *hex_chars = "0123456789abcdef";
    const auto bytes = hashed.extract_as_byte_array();
    // Iterate hash and build result
    for (uint32_t i = 0; i < bytes.size(); ++i) {
        (result += hex_chars[(bytes.at(i) >> 4)]) += hex_chars[(bytes.at(i) & 0x0f)];
    }
    // Return string
    return result;
}

string cal_hash(int64_t number, const string& random)
{
    auto str = std::to_string(number) + random;
    checksum256 hashed = sha256(str.c_str(), str.length());
    return to_hex(hashed);
}

void horsebet::ontransfer(name from, name to, asset quantity, string memo)
{
    // memo of bet:PICK:NUMBER
    constexpr string_view PICK = "PICK:";

    if (from == get_self()) return;
    check(to == get_self(), "This transfer is not for animal picking");
    check(quantity.amount > 0, "quantity must be positive");
    check(quantity.is_valid(), "invalid token transfer");

    string_view memosv(memo);
    if (starts_with(memosv, PICK)) {
        check(get_first_receiver() == name(TOKEN_CONTRACT), "Only aosio.token supported.");
        check(quantity.symbol.code() == symbol_code(TOKEN_SYMBOL), "only AOS support.");

        pick(from, quantity, memosv.substr(PICK.size()) );
    }
}

void horsebet::pick(const name& user,const asset& quantity, const string_view& detail)
{
    auto number = std::stoi(string(detail));
    check(number >= LOW_NUMBER && number <= HIG_NUMBER, "picked number should be within 1 ~ 16.");

    auto g = _glbs.begin();
    check(g != _glbs.end() && g->on, "game is not started yet.");
    auto period = g->period;

    rounds rds(get_self(), period);
    auto r = rds.begin();
    check(r != rds.end(), "this period deos not exists");
    check(r->open, "this period is already closed");

    auto zero_token = asset{0, symbol(TOKEN_SYMBOL, 4)};    
    auto pk = picking{period, number, quantity};
    
    players plys(get_self(), user.value);
    auto p = plys.begin();
    if ( p == plys.end() ) {
        // first time in
        plys.emplace(get_self(), [&](auto& a) {
            a.id = user;
            a.invests = quantity;
            a.profits = zero_token;
            a.claimed = 0;
            a.pickings.push_back(pk);
        });
    } else {
        // already in
        plys.modify(*p, get_self(), [&](auto& a) {
           auto fpk = std::find_if(p->pickings.begin(), p->pickings.end(), [=](const picking& pik) -> bool { return pik.period == period && pik.number == number; });
           if (fpk == p->pickings.end()) {
               a.pickings.push_back(pk);
           } else {
               auto pks = *fpk;
               pks.invest += quantity;
               auto old_pickings = p->pickings;
               std::replace_if(old_pickings.begin(), old_pickings.end(), [=](const picking& pik) -> bool { return pik.period == period && pik.number == number; }, pks);
               a.pickings = old_pickings;
           }
           a.invests += quantity;
        });
    }

    rds.modify(*r, get_self(), [&](auto& a) {
        auto fan = std::find_if(r->animals.begin(), r->animals.end(), [=](const animal& an) -> bool { return an.number == number; });
        auto ani = *fan;
        ani.invests += quantity;
        auto old_animals = r->animals;
        std::replace_if(old_animals.begin(), old_animals.end(), [=](const animal& an) -> bool { return an.number == number; }, ani);
        a.animals = old_animals;
    });

    _glbs.modify(*g, get_self(), [&](auto& a) {
        a.income += quantity;
    });
}

void horsebet::open(uint64_t period, string hash)
{
    require_auth( get_self() );

    check(hash.size() == 64, "hash length should be 64");
    transform(hash.begin(),hash.end(),hash.begin(),(int (*)(int))tolower);

    auto zero_token = asset{0, symbol(TOKEN_SYMBOL, 4)};

    auto g = _glbs.begin();
    if (g == _glbs.end()) {
        check(period == 1, "first period should be 1");
        _glbs.emplace(get_self(), [&](auto& a) {
            a.owner = get_self();
            a.on = true;
            a.period = 1;
            a.income = zero_token;
            a.outcome = zero_token;
        });
    } else {
        // last one should be close first
        auto last_period = g->period;
        rounds rds(get_self(), last_period);
        auto r = rds.begin();
        check(r != rds.end(), "something wrong, last period not exists.");
        check(!r->open, "last period is still open, close it first.");
        check(period == last_period + 1, "new period should be last period + 1");

        _glbs.modify(*g, get_self(), [&](auto& a) {
            a.period += 1;
        });
    }

    rounds nrds(get_self(), period);
    auto nr = nrds.begin();
    check(nr == nrds.end(), "this period is already exists");
    nrds.emplace(get_self(), [&](auto& a) {
        a.period = period;
        a.open = true;
        a.start = static_cast<uint64_t>(current_time_point().sec_since_epoch());
        a.endtime = 0;
        a.lucky = 0;
        a.hash = hash;
        for (auto i = LOW_NUMBER; i <= HIG_NUMBER; ++i) {
            animal ani{i, zero_token};
            a.animals.push_back(ani);
        }
    });
}

void horsebet::reveal(uint64_t period, int64_t number, string random)
{
    require_auth( get_self() );

    check(number >= LOW_NUMBER && number <= HIG_NUMBER, "number should be within 1 ~ 16.");

    auto g = _glbs.begin();
    check (g != _glbs.end(), "you should open first");
    check(g->period == period, "period doesn't match");

    rounds rds(get_self(), period);
    auto r = rds.begin();
    check(r != rds.end(), "this period deos not exists");
    check(r->open, "this period is already closed");

    auto hash = cal_hash(number, random);
    transform(hash.begin(),hash.end(),hash.begin(),(int (*)(int))tolower);
    check(r->hash == hash, "hash(number + random) doesn't match pre-saved hash");

    rds.modify(*r, get_self(), [&](auto& a) {
        a.open = false;
        a.lucky = number;
        a.random = random;
        a.endtime = static_cast<uint64_t>(current_time_point().sec_since_epoch());
    });
}

void horsebet::claim(name user)
{
    require_auth( user );

    auto g = _glbs.begin();
    check(g != _glbs.end() && g->on, "game is not started yet.");

    players plys(get_self(), user.value);
    auto p = plys.begin();
    check(p != plys.end(), "player does not exists");

    rounds rds(get_self(), g->period);
    auto r = rds.begin();
    check(r != rds.end(), "something wrong, current period deos not exists"); // hardly happen
    check(g->period >= 2 || (g->period == 1 && !r->open), "you need wait first period to end.");

    // if current period is closed, we claim it; otherwise, we claim last period
    auto claimed = r->open ? (g->period - 1) : g->period;

    auto rewards = asset{0, symbol(TOKEN_SYMBOL, 4)}; 
    plys.modify(*p, get_self(), [&](auto& a) {
        for (auto& pk : a.pickings) {
            if (pk.period <= p->claimed || pk.period > claimed) continue;

            rounds rd(get_self(), pk.period);
            if (rd.begin()->lucky == pk.number) {
                rewards += pk.invest * RWD_FACTOR;
            }
        }

        a.claimed = claimed;
        a.profits += rewards;
    });

    if (rewards.amount > 0) {
        // check if contract has enough balance
        accounts self_acnts( name(TOKEN_CONTRACT), get_self().value );
        const auto& from = self_acnts.get( rewards.symbol.code().raw(), "no balance object found" );
        check( from.balance.amount >= rewards.amount, "contract account has no enough balance" );

        _glbs.modify(*g, get_self(), [&](auto& a) {
            a.outcome += rewards;
        });

        action(permission_level{ get_self(), "active"_n }, name(TOKEN_CONTRACT), name("transfer"),
            std::make_tuple( get_self(), user, rewards, string("claim rewards"))).send();
    }
}