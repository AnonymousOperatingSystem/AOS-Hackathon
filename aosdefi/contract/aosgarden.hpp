#pragma once

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
#include <vector>
#include <string>

using namespace eosio;
using namespace std;

class [[eosio::contract("aosgarden")]] aosgarden : public contract {
public:
    using contract::contract;

    [[eosio::on_notify("*::transfer")]] void ontransfer(name from, name to, asset quantity, string memo);
    [[eosio::on_notify("ciphertokens::transferpp")]] void ontransferpp(name from, name to, asset quantity, string memo);

    [[eosio::action]] void withdraw(name player, uint64_t pid, const name& token_contract, const asset& token_quantity);
    [[eosio::action]] void claim(name player, uint64_t pid);
    [[eosio::action]] void foo();

private:
    struct stake_info {
        uint64_t                pool_id;        // pool id
        extended_asset          staked;         // staked  balance，{ "contract": "tokencontract", "quantity": "10.0000 XXX" }
        extended_asset          invite_staked;  // staked  balance of invitees 
        extended_asset          rewards;        // history rewards of my staked mining
        extended_asset          invite_rewards; // history rewards of my invitees' staked mining
        uint64_t                claim_time;     // last claim time
    };

    struct [[eosio::table]] user {
        name                    id;             // name of user
        name                    inviter;        // inviter of user
        vector<name>            invitees;       // invitees of user
        uint64_t                create_time;    // register time of user
        vector<stake_info>      stakes;         // all stake infos of user

        uint64_t primary_key() const { return id.value; }
    };

    struct stake_supply {
        uint64_t                timestamp;      // snapshot time, second
        extended_asset          staked;         // staked tokens at this time
    };

    struct [[eosio::table]] pool {
        name                    creater;        // creater of pool
        extended_asset          total_supply;   // total supply
        extended_asset          cur_supply;     // current left supply for mining
        uint64_t                start_time;     // start time of pool, seconds
        extended_asset          init_supply;    // first time supply
        uint64_t                fade_factor;    // fade factor every 1 day
        uint64_t                duration;       // duration for pool, seconds

        vector<stake_supply>    token_staked;   // staked token quantity

        uint64_t primary_key() const { return creater.value; }
    };

    struct token_pair {
        uint64_t                id;             // id of pool
        string                  profile;        // profile of pool, e.g: contract1-symbol1-contract2-symbol2 
    };

    struct [[eosio::table]] global {
        name                    owner;          // self
        uint64_t                users;          // amount of users
        vector<token_pair>      pairs;          // token pairs, pools

        uint64_t primary_key() const { return owner.value; }
    };

    typedef eosio::multi_index< "users"_n, user > users;
    typedef eosio::multi_index< "pools"_n, pool > pools;
    typedef eosio::multi_index< "globals"_n, global > globals;

private:
    void on_transfer(name from, name to, asset quantity, string memo);
    void create_pool(const name& from, extended_asset ext_asset_in, string_view details);
    void stake(const name& from, extended_asset ext_asset_in, string_view details);
    void claim_harvest(users& us, uint64_t pid);

    uint64_t get_settle_time(uint64_t start_time, uint64_t now);
    uint64_t get_currenttime_s();

    void update_pool(uint64_t pid, extended_asset ext_asset, uint64_t now_s, bool stake = true);
};