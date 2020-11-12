#pragma once

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <vector>
#include <string>

using namespace eosio;
using namespace std;

class [[eosio::contract("horsebet")]] horsebet : public contract {
public:
    using contract::contract;

    horsebet(name receiver, name code, datastream<const char *> ds)
    : contract(receiver, code, ds),
        _glbs(_self, _self.value) {}

    [[eosio::on_notify("aosio.token::transfer")]] void ontransfer(name from, name to, asset quantity, string memo);
    [[eosio::action]] void open(uint64_t period, string hash);
    [[eosio::action]] void reveal(uint64_t period, int64_t number, string random);
    [[eosio::action]] void claim(name user);

private:
    struct [[eosio::table]] global {
        name        owner;      // self
        bool        on;         // emergency button
        uint64_t    period;     // current period
        asset       income;     // income of history period
        asset       outcome;    // outcome of history period

        uint64_t primary_key() const { return owner.value; }
    };

    struct animal {
        int64_t         number;     // number
        asset           invests;    // total balance of this numbers
    };

    struct [[eosio::table]] round {
        uint64_t        period;     // current period
        bool            open;       // open or closed
        int64_t         start;      // start time, second
        int64_t         endtime;    // revealed time
        int64_t         lucky;      // lucky number of the round, 1 ~ 16
        string          hash;       // hash of lucky number
        string          random;     // random string

        vector<animal>  animals;    // 16 animals for this round

        uint64_t primary_key() const { return period; }
    };

    struct picking {
        uint64_t        period;     // period of this picking
        int64_t         number;     // picked number
        asset           invest;     // total invest for this number
    };

    struct [[eosio::table]] player {
        name            id;         // player's name
        asset           invests;    // total invests for all round
        asset           profits;    // total profits for all round
        int64_t         claimed;    // claimed period
        vector<picking> pickings;   // all pickings

        uint64_t primary_key() const { return id.value; }
    };

    struct [[eosio::table]] account {
        asset    balance;
        uint64_t primary_key()const { return balance.symbol.code().raw(); }
     };

    typedef eosio::multi_index< "globals"_n, global > globals;
    typedef eosio::multi_index< "rounds"_n, round > rounds;
    typedef eosio::multi_index< "players"_n, player > players;
    typedef eosio::multi_index< "accounts"_n, account> accounts;

    globals _glbs;
private:
    void pick(const name& user, const asset& quantity, const string_view& detail);
};