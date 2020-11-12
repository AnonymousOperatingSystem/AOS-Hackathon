#include "aosgarden.hpp"
#include "utils.hpp"
#include <math.h> 

const static string SPLITTER = "|";
const static uint64_t SETTLE_DURATION = 10 * 60; // 10 minutes
const static uint64_t FAED_DURATION = 24 * 60 * 60; // 1 day
const static uint64_t VIP_NUMBER = 1; // first 100 users are VIPs

void aosgarden::ontransfer(name from, name to, asset quantity, string memo)
{
    on_transfer(from, to, quantity, memo);
}

void aosgarden::ontransferpp(name from, name to, asset quantity, string memo)
{
    on_transfer(from, to, quantity, memo);
}

void aosgarden::withdraw(name player, uint64_t pid, const name& token_contract, const asset& token_quantity)
{
    require_auth( player );

    check(token_quantity.amount > 0, "withdraw quantity must be positive");

    users us(get_self(), player.value);
    auto u = us.begin();
    check(u != us.end(), "player does not exists.");
    auto fsi = std::find_if(u->stakes.begin(), u->stakes.end(), [=](const stake_info& s) -> bool { return s.pool_id == pid; });
    check(fsi != u->stakes.end(), "player does not participate this pool.");
    check(fsi->staked.quantity >= token_quantity, "player does not have enough staked token in this pool.");
    check(fsi->staked.contract == token_contract, "something wrong, mismatch stake contract.");

    globals glbs(get_self(), _self.value);
    auto g = glbs.begin();
    check(g != glbs.end(), "there is no pools right now.");
    auto fd = std::find_if(g->pairs.begin(),g->pairs.end(), [=](const token_pair & tp) -> bool { return tp.id == pid; });
    check(fd != g->pairs.end(), "No such pool found.");

    // update pool state
    pools pls(get_self(), pid);
    auto p = pls.begin();
    check(p != pls.end(), "Something wrong, no such pool.");
    check(p->token_staked.begin()->staked.contract == token_contract, "mismatch stake contract.");
    check(p->token_staked.begin()->staked.quantity.symbol == token_quantity.symbol, "mismatch stake token.");
    check(p->token_staked.back().staked.quantity >= token_quantity, "token quantity overdraw");

    uint64_t now_s = get_currenttime_s();
    if (fsi->claim_time < p->start_time + p->duration) {
        check((fsi->claim_time + SETTLE_DURATION) > now_s, "you have unclaimed rewards, claim first");
    }

    // update stake supply
    auto ext_asset_out = extended_asset{token_quantity, token_contract};
    update_pool(pid, -ext_asset_out, now_s, false);

    // update user state
    auto si = *fsi;
    si.staked -= ext_asset_out;
    auto osi = u->stakes;
    std::replace_if(osi.begin(), osi.end(), [=](const stake_info& s) -> bool { return s.pool_id == pid; }, si);
    us.modify(*u, _self, [&](auto& a) {
        a.stakes = osi;
    });

    // if player has inviter, update inviter's state
    if (u->inviter) {
        users ref(get_self(), u->inviter.value);
        auto rf = ref.begin();
        check(rf != ref.end(), "something wrong, inviter not exists.");

        auto osi = rf->stakes;
        auto fsi = std::find_if(osi.begin(), osi.end(), [=](const stake_info& s) -> bool { return s.pool_id == pid; });
        if (fsi != osi.end()) {
            auto nsi = *fsi;
            nsi.invite_staked -= ext_asset_out;
            nsi.claim_time = now_s;
            std::replace_if(osi.begin(), osi.end(), [=](const stake_info& s) -> bool { return s.pool_id == pid; }, nsi);
            ref.modify(*rf, _self, [&](auto& a) {
                a.stakes = osi;
            });
        }
    }

    string_view act = "transfer";
    if (token_contract == "ciphertokens"_n) {
        act = "transferpp";
    }
    action(permission_level{ get_self(), "active"_n }, token_contract, name(act),
        std::make_tuple( get_self(), player, token_quantity, string("withdraw staked tokens from pool ") + std::to_string(pid)) ).send();
}

void aosgarden::claim(name player, uint64_t pid)
{
    require_auth( player );

    users us(get_self(), player.value);
    claim_harvest(us, pid);
}

void aosgarden::on_transfer(name from, name to, asset quantity, string memo)
{
    // memo of create_pool:CREATE:CONTRACT|SYMBOL|FADE_FACTOR|DAYS
    // memo of stake:STAKE:POOL_ID|REFERRER
    constexpr string_view CREATE = "CREATE:";
    constexpr string_view STAKE = "STAKE:";

    if (from == get_self()) return;
    check(to == get_self(), "This transfer is not for aosgarden");
    check(quantity.amount > 0, "quantity must be positive");

    auto incoming = extended_asset{quantity, get_first_receiver()};
    string_view memosv(memo);
    if (starts_with(memosv, CREATE)) {
      create_pool( from, incoming, memosv.substr(CREATE.size()) );
    } else if (starts_with(memosv, STAKE)) {
      stake(from, incoming, memosv.substr(STAKE.size()));
    }
}

void aosgarden::create_pool(const name& from, extended_asset ext_asset_in, string_view details)
{
    // details:CONTRACT|SYMBOL|FADE_FACTOR|DAYS
    auto parts = split(details, SPLITTER);
    check(parts.size() >= 4, "Expected format 'CREATE:contract|symbol|fade_factor|days'");
    bool has_only_digits = (string(parts[2]).find_first_not_of( "0123456789" ) == std::string::npos);
    check(has_only_digits, "fade factor should be only integer.");
    has_only_digits = (string(parts[3]).find_first_not_of( "0123456789" ) == std::string::npos);
    check(has_only_digits, "days should be only integer.");

    auto contract2 = name(parts[0]);
    uint64_t fade_factor = std::stoi(string(parts[2]));
    check(fade_factor >= 1 && fade_factor <= 100, "fade factor should be in 1~100.");
    uint64_t days = std::stoull(string(parts[3]));
    check(days >= 1, "days should be bigger than 1.");

    // calculate init_supply
    /*
    设挖矿Token总数量为x，从区块高度为0区块开始,进行释放Token.
    单次释放周期为1200个区块（10min，产生一个区块时间是0.5s）
    单次释放数量为a
    单次衰减周期为172800个区块（约1天）
    单次衰减因子b%
    总时间N天
    */
    //a=x*b%/[144*[1-(1-b%)^N]]
    asset init_supply = ext_asset_in.quantity * fade_factor / 100 / (144 * (1 - pow( 1 - fade_factor*1.0/100, days)));
    auto sym_parts = split(parts[1], ",");
    check(sym_parts.size() == 2, "Expected symbol format 'precision,symbol_code'");
    auto precision = std::stoi(string(sym_parts[0]));
    auto symbol2 = symbol(sym_parts[1], precision);
    check(symbol2.is_valid(), "stake token symbol is invliad");

    string profile = ext_asset_in.contract.to_string() + ext_asset_in.quantity.symbol.code().to_string() + string(parts[0]) + string(sym_parts[1]);
    token_pair tp;
    tp.profile = profile;
    tp.id = 1;

    stake_supply ss;
    ss.timestamp = get_currenttime_s();
    ss.staked = extended_asset{asset{0, symbol2}, contract2};

    globals glbs(get_self(), _self.value);
    auto g = glbs.begin();
    if (g != glbs.end()) {
        auto fd = std::find_if( g->pairs.begin(),g->pairs.end(),
                                [=](const token_pair & tp) -> bool { return tp.profile == profile; });
        check(fd == g->pairs.end(), "same pool already exists.");

        // generate pool id and profile
        tp.id = g->pairs.size() + 1;
        glbs.modify(*g, _self, [&](auto& a) {
            a.pairs.push_back(tp);
        });
    } else {
        // first one to create pool
        glbs.emplace( _self, [&](auto& a) {
            a.owner = get_self();
            a.users = 0;
            a.pairs.push_back(tp);
      });
    }

    // save pool details
    pools pls(get_self(), tp.id);
    pls.emplace( _self, [&](auto& a) {
        a.creater = from;
        a.total_supply = ext_asset_in;
        a.cur_supply = ext_asset_in;
        a.init_supply = extended_asset{init_supply, ext_asset_in.contract};
        a.fade_factor = fade_factor;
        a.duration = days * FAED_DURATION;
        a.start_time = get_currenttime_s();
        a.token_staked.push_back(ss);
    });
}

void aosgarden::stake(const name& from, extended_asset ext_asset_in, string_view details)
{
    // details:POOL_ID|REFERRER
    auto parts = split(details, SPLITTER);
    check(parts.size() >= 2, "Expected format 'STAKE:pool_id|referrer'");

    uint64_t pid = std::stoi(string(parts[0]));
    auto referrer = name(parts[1]);
    uint64_t now_s = get_currenttime_s();

    // check pool id
    globals glbs(get_self(), _self.value);
    auto g = glbs.begin();
    check(g != glbs.end(), "there is no pools right now.");
    auto fd = std::find_if(g->pairs.begin(),g->pairs.end(),
                            [=](const token_pair & tp) -> bool { return tp.id == pid; });
    check(fd != g->pairs.end(), "No such pool found.");

    // check stake token and its contract
    update_pool(pid, ext_asset_in, now_s);

    // save stake_info
    auto invite_staked = extended_asset{0, ext_asset_in.get_extended_symbol()};
    pools pls(get_self(), pid);
    auto p = pls.begin();
    auto zero_rewards = extended_asset{0, p->total_supply.get_extended_symbol()};
    auto si = stake_info{ pid, ext_asset_in, invite_staked, zero_rewards, zero_rewards, now_s};

    // update user state
    users us(get_self(), from.value);
    auto u = us.begin();
    if (u == us.end()) {
        // first time user login
        if (g->users >= VIP_NUMBER) {
            // need inviter
            users ref(get_self(), referrer.value);
            auto rf = ref.begin();
            check(rf != ref.end(), "Your inviter is not exists!");
            ref.modify(*rf, _self, [&](auto& a) {
                a.invitees.push_back(from);
            });
        }

        // increase user number
        glbs.modify(*g, _self, [&](auto& a) {
            a.users += 1;
        });

        us.emplace(_self, [&](auto& a) {
            a.id = from;
            a.inviter = (g->users >= VIP_NUMBER) ? referrer : name();
            a.create_time = now_s;
            a.stakes.push_back(si);  
        });
    } else {
        // just use table inviter, not memo referrer
        auto fsi = std::find_if(u->stakes.begin(), u->stakes.end(), [=](const stake_info& s) -> bool { return s.pool_id == pid; });
        if (fsi != u->stakes.end()) {
            if (fsi->staked.quantity.amount > 0 && (fsi->claim_time + SETTLE_DURATION) <= now_s) {
                check(false, "you have unclaimed rewards, claim first");
            }

            // add old data
            si = *fsi;
            si.staked += ext_asset_in;
            auto osi = u->stakes;
            std::replace_if(osi.begin(), osi.end(), [=](const stake_info& s) -> bool { return s.pool_id == pid; }, si);
            us.modify(*u, _self, [&](auto& a) {
                a.stakes = osi;
            });
        } else {
            us.modify(*u, _self, [&](auto& a) {
                a.stakes.push_back(si);
            });
        }
    }

    // update referrer state
    if (us.begin()->inviter) {
        users ref(get_self(), us.begin()->inviter.value);
        auto rf = ref.begin();
        check(rf != ref.end(), "Your inviter is not exists!");
        auto fsi = std::find_if(rf->stakes.begin(), rf->stakes.end(), [=](const stake_info& s) -> bool { return s.pool_id == pid; });
        if (fsi != rf->stakes.end()) {
            // update referrer stake info
            bool need_claim = ((fsi->claim_time + SETTLE_DURATION) < get_currenttime_s()) && (fsi->staked.quantity.amount > 0);
            if (need_claim) {
                claim_harvest(ref, pid);
            }

            fsi = std::find_if(rf->stakes.begin(), rf->stakes.end(), [=](const stake_info& s) -> bool { return s.pool_id == pid; });
            auto nsi = *fsi;
            auto osi = rf->stakes;
            nsi.invite_staked += ext_asset_in;
            nsi.claim_time = now_s;
            std::replace_if(osi.begin(), osi.end(), [=](const stake_info& s) -> bool { return s.pool_id == pid; }, nsi);
            ref.modify(*rf, _self, [&](auto& a) {
                // if inviter has same stake info, need add invite_staked 
                a.stakes = osi;
            });
        }
    }
}

uint64_t aosgarden::get_currenttime_s() {
    return static_cast<uint64_t>(current_time_point().sec_since_epoch());
}

uint64_t aosgarden::get_settle_time(uint64_t start_time, uint64_t now)
{
    if (start_time >= now)
        return 0;

    uint64_t elapse = now - start_time;
    uint64_t factor = elapse / SETTLE_DURATION;

    if ((elapse % SETTLE_DURATION) != 0) {
        factor += 1; 
    }

    return start_time + factor * SETTLE_DURATION;
}

void aosgarden::update_pool(uint64_t pid, extended_asset ext_asset, uint64_t now_s, bool stake)
{
    pools pls(get_self(), pid);
    auto p = pls.begin();
    check(p != pls.end(), "Something wrong, no such pool.");    // hardly happen
    check(p->token_staked.begin()->staked.get_extended_symbol() == ext_asset.get_extended_symbol(), 
        "mismatch stake token.");
    check(p->token_staked.begin()->staked.contract == ext_asset.contract, "mismatch stake contract.");
    if (stake) {
        check(now_s < p->start_time + p->duration, "the pool is already time ended");
    } else {
        if (now_s >= p->start_time + p->duration) {
            now_s = p->start_time + (p->duration / SETTLE_DURATION + 1) * SETTLE_DURATION;  // all outdated stake info's timestamp should be endtime+SETTLE_DURATION
        }
    }

    // update stake supply
    auto ots = p->token_staked;
    auto st = get_settle_time(p->start_time, now_s);
    stake_supply nss {st, ots.back().staked + ext_asset};    // add old data
    auto ts = std::find_if(ots.begin(), ots.end(), [=](const stake_supply& ss) -> bool { return ss.timestamp == st; });
    if (ts != ots.end()) {
        std::replace_if(ots.begin(), ots.end(), [=](const stake_supply& ss) -> bool { return ss.timestamp == st; }, nss);
    } else {
        ots.push_back(nss);
    }

    // remove old useless stake data
    // we should keep at least 1 data which timestamp is out of 2 fade durations
    bool stale = false;
    for (auto o = ots.rbegin(); o != ots.rend(); ++o) {
        if (o->timestamp + 2 * FAED_DURATION < st) {
            if (stale) {
                ots.erase(o.base()-1);
            } else {
                stale = true;
            }
        }
    }

    pls.modify(*p, _self, [&](auto& a) {
        a.token_staked = ots;
    });
}

void aosgarden::claim_harvest(users& us, uint64_t pid)
{
    pools pls(get_self(), pid);
    auto p = pls.begin();
    check(p != pls.end(), "Something wrong, no such pool.");
    check(p->cur_supply.quantity.amount > 0, "No enough token supply");

    auto u = us.begin();
    check(u != us.end(), "player does not exists.");
    auto fsi = std::find_if(u->stakes.begin(), u->stakes.end(), [=](const stake_info& s) -> bool { return s.pool_id == pid; });
    check(fsi != u->stakes.end(), "player does not participate this pool.");
    check((fsi->claim_time + SETTLE_DURATION) < get_currenttime_s(), "player already claimed in past 10 minutes");
    check(fsi->claim_time < (p->start_time + p->duration), "player claim all harvest while pool ended.");
    check(fsi->staked.quantity.amount > 0, "player don't have any staked tokens to claim");
    if (p->start_time + SETTLE_DURATION > get_currenttime_s()) {
        return; //The pool is created within 10 minutes, you can't calim now.
    }

    // calculate every period rewards
    auto now_s = std::min(get_currenttime_s(), (p->start_time + p->duration));
    auto end = p->start_time + (now_s - p->start_time) / SETTLE_DURATION  * SETTLE_DURATION;
    auto endtime = p->start_time + (p->duration / SETTLE_DURATION + 1) * SETTLE_DURATION;
    uint64_t start = (fsi->claim_time + 2 * FAED_DURATION) < end ? (end - (2 * FAED_DURATION)) : fsi->claim_time;   // only within 2 days
    start = p->start_time + (start - p->start_time) / SETTLE_DURATION  * SETTLE_DURATION;
    int64_t last_period_base_amount = p->token_staked.back().staked.quantity.amount;    // in case last staked was 2 days ago, we use last amount
    auto rwds = asset{0, fsi->rewards.quantity.symbol};
    auto inv_rwds = rwds;

    // temp_supply = a * (1-b%)^N
    double factor1 = 1.0, factor2 = 1.0, base = 1 - p->fade_factor/100.0;
    if (end > p->start_time + FAED_DURATION) {
        for (int32_t i = 0; i < (int32_t)((end - FAED_DURATION - p->start_time)/FAED_DURATION); ++i) {
            factor1 *= base;
        }
        factor2 = factor1 * base;
    }
    auto temp_supply0 = p->init_supply.quantity * int64_t(100 * factor1) / 100; // yestoday's supply
    auto temp_supply1 = p->init_supply.quantity * int64_t(100 * factor2) / 100; // today's supply

    for (uint32_t i = 0; i < (end - start)/SETTLE_DURATION; ++i) {
        auto ts = start + (i + 1) * SETTLE_DURATION;
        auto fts = std::find_if(p->token_staked.begin(), p->token_staked.end(), [=](const stake_supply& ss) -> bool {
            return (ss.timestamp >= ts) && (ss.timestamp < endtime);
        });
        if (fts != p->token_staked.end()) {
            last_period_base_amount = fts->staked.quantity.amount;
        }

        if (last_period_base_amount == 0) {
            continue;
        }

        auto temp_supply = (ts <= end - FAED_DURATION) ? temp_supply0 : temp_supply1;

        // 80% self staked rewards
        rwds += temp_supply * 8/10 * fsi->staked.quantity.amount / last_period_base_amount;

        // 20% invitee staked rewards
        inv_rwds += temp_supply * 2/10 * fsi->invite_staked.quantity.amount / last_period_base_amount;
    }

    // update user's stake info
    auto si = *fsi;
    si.rewards += extended_asset{rwds, fsi->rewards.contract};
    si.invite_rewards += extended_asset{inv_rwds, fsi->rewards.contract};
    si.claim_time = get_currenttime_s();
    auto osi = u->stakes;
    std::replace_if(osi.begin(), osi.end(), [=](const stake_info& s) -> bool { return s.pool_id == pid; }, si);
    us.modify(*u, _self, [&](auto& a) {
        a.stakes = osi;
    });

    auto all_rwds = rwds + inv_rwds;
    check(all_rwds <= p->cur_supply.quantity, "There's no such amount supply token to claim.");

    if (all_rwds.amount > 0) {
        pls.modify(p, _self, [&](auto& a) {
            a.cur_supply -= extended_asset{all_rwds, fsi->rewards.contract};
        });

        string_view act = "transfer";
        if (fsi->rewards.contract == "ciphertokens"_n) {
            act = "transferpp";
        }
        action(permission_level{ get_self(), "active"_n }, fsi->rewards.contract, name(act),
            std::make_tuple( get_self(), u->id, all_rwds, string("claim rewards from pool ") + std::to_string(pid)) ).send();
    } 
}

void aosgarden::foo()
{
    require_auth( get_self() );

    pools pls1(get_self(), 1);
    auto p1 = pls1.begin();
    if (p1 != pls1.end()) {
        pls1.modify(*p1, _self, [&](auto& a) {
            a.duration = 1602313200 - p1->start_time;   // endtime = starttime + duration, 1602313200 = 1601114823 + 1198377
        });
    }

    pools pls2(get_self(), 2);
    auto p2 = pls2.begin();
    if (p2 != pls2.end()) {
        pls2.modify(*p2, _self, [&](auto& a) {
            a.duration = 1602313200 - p2->start_time;   // endtime = starttime + duration, 1602313200 = 1601449171 + 864029
        });
    }

    // pools pls2(get_self(), 2);
    // auto p2 = pls2.begin();
    // if (p2 != pls2.end()) {
    //     pls2.erase(p2);
    // }

    // pools pls3(get_self(), 3);
    // auto p3 = pls3.begin();
    // if (p3 != pls3.end()) {
    //     pls3.erase(p3);
    // }

    // pools pls4(get_self(), 4);
    // auto p4 = pls4.begin();
    // if (p4 != pls4.end()) {
    //     pls4.erase(p4);
    // }

    // globals glbs(get_self(), _self.value);
    // auto g = glbs.begin();
    // if (g != glbs.end()) {
    //     glbs.erase(g);
    // }

    // users us1(get_self(), "alice11alice"_n.value);
    // auto u1 = us1.begin();
    // if (u1 != us1.end()) {
    //     //us1.erase(u1);
    //     auto si = u1->stakes.back();
    //     si.claim_time = 1600503274;

    //     auto osi = u1->stakes;
    //     std::replace_if(osi.begin(), osi.end(), [=](const stake_info& s) -> bool { return s.pool_id == 1; }, si);
    //     us1.modify(*u1, _self, [&](auto& a) {
    //         a.stakes = osi;
    //     });
    // }

    // users us2(get_self(), "alice22alice"_n.value);
    // auto u2 = us2.begin();
    // if (u2 != us2.end()) {
    //     us2.erase(u2);
    // }

    // users us3(get_self(), "alice55alice"_n.value);
    // auto u3 = us3.begin();
    // if (u3 != us3.end()) {
    //     us3.erase(u3);
    // }

    // users us4(get_self(), "dogdogcatcat"_n.value);
    // auto u4 = us4.begin();
    // if (u4 != us4.end()) {
    //     us4.erase(u4);
    // }
}