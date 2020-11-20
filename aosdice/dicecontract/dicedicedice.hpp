#pragma once

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
#include <eosio/crypto.hpp>
#include <vector>
#include <string>

using namespace eosio;
using namespace std;

class [[eosio::contract("dicedicedice")]] dicedicedice : public contract {
   public:
      using contract::contract;

      [[eosio::on_notify("aosio.token::transfer")]] void ontransfer(name from, name to, asset quantity, string memo);

      [[eosio::action]] void hashfmsr(uint64_t id, string hashfromserver);
      [[eosio::action]] void hashsdfmsr(uint64_t id, uint64_t seed, string hashfromserver);


   struct [[eosio::table]]  hashseedque {
      uint64_t id;
      std::string hashServer;
      uint64_t createTime;
      uint64_t primary_key() const { return id; }
   };

   struct [[eosio::table]]  workingdice {
      uint64_t id;
      name    username;

      uint64_t ratePar;
      std::string hashServer;
      uint64_t seedServer;
      uint64_t resultRandom;
      uint64_t endtime;
      string contractname;
      asset quantity;
      uint64_t randNumber;
      uint64_t primary_key() const { return id; }
   };


private:
   uint64_t get_currenttime_s();

   typedef eosio::multi_index< "workingdices"_n, workingdice > workingdices;
   typedef eosio::multi_index< "hashseedques"_n, hashseedque > hashseedques;
};