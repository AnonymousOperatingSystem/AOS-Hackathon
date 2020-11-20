#pragma once

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
#include <eosio/crypto.hpp>
#include <vector>
#include <string>

using namespace eosio;
using namespace std;

class [[eosio::contract("rps")]] rps : public contract {
   public:
      using contract::contract;

      [[eosio::on_notify("aosio.token::transfer")]] void ontransfer(name from, name to, asset quantity, string memo);

      [[eosio::action]] void hashfmsr(uint64_t id, string hashfromserver);
      [[eosio::action]] void hashsdfmsr(uint64_t id, uint64_t chooseserver, string uuidserver, string hashserver);
      [[eosio::action]] void erase(uint64_t id);

   struct [[eosio::table]]  hashseedque {
      uint64_t id;
      std::string hashServer;
      uint64_t createTime;
      uint64_t primary_key() const { return id; }
   };

   struct [[eosio::table]]  workingdice {
      uint64_t id;
      name    userName;

      uint64_t serverChoose;
      std::string uuidServer;
      std::string hashServer;

      uint64_t clientChoose;
      uint64_t endtime;
      
      asset quantity;
      uint64_t primary_key() const { return id; }
   };


private:
   uint64_t get_currenttime_s();

   typedef eosio::multi_index< "workingdices"_n, workingdice > workingdices;
   typedef eosio::multi_index< "hashseedques"_n, hashseedque > hashseedques;
};
