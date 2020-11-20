#include "dicedicedice.hpp"
#include "utils.hpp"
#include <math.h> 

const static string SPLITTER = "|";

void dicedicedice::ontransfer(name from, name to, asset quantity, string memo)
{   
    print_f("you just call ontransfer: %  --- %  %---%---",from,to,quantity,memo);
    auto parts = split(memo, SPLITTER);
    
    //auto it =parts.begin();
    // while (it != parts.end())
    // {
    //   print_f("%-", string(*it));
    //   it++;
    // }

    if (from==name("cryptocasino"))
    {
      return;
    }
    //id|rand
    if (from==name("thegodofdice"))
    {
      return;
    }

    if (from==name("kangboaosdep"))
    {
      return;
    }

    
    check(parts.size() == 3, "memo format not right");

    bool has_only_digits = (string(parts[0]).find_first_not_of( "0123456789" ) == std::string::npos);
    check(has_only_digits, "id must be number");

    has_only_digits = (string(parts[1]).find_first_not_of( "0123456789" ) == std::string::npos);
    check(has_only_digits, "rate parameter must be number");

    uint64_t ratePar = std::stoi(std::string(parts[1]));
    check(ratePar>=2&&ratePar<=96, "rate parameter must 2<=seed<=96");

    has_only_digits = (string(parts[2]).find_first_not_of( "0123456789" ) == std::string::npos);
    check(has_only_digits, "rand must be positive number");

    uint64_t randNumber = std::stoi(std::string(parts[2]));
    check(randNumber>=1&&randNumber<=100, "randNumber parameter must 1<=randNumber<=100");
    
    
    uint64_t id = std::stoi(std::string(parts[0]));
    workingdices wd(get_self(), _self.value);
    auto itx = wd.find(id);
    check(itx==wd.end(), "id already exist,not repeat");

    hashseedques hs(get_self(), _self.value);
    
    auto iths = hs.begin();
    check(iths!=hs.end(), "no cache hash can use,wait for a while");
    
      wd.emplace( _self, [&](auto& a) {
        a.id = id;
        a.username = from;
        a.ratePar= ratePar;
        a.randNumber=randNumber;
        a.hashServer=iths->hashServer;
        a.contractname="aosio.token";
        a.quantity=quantity;
      });

      hs.erase(iths);
}

void dicedicedice::hashfmsr(uint64_t id, string hashfromserver)
{
    require_auth( get_self());

    hashseedques hs(get_self(), _self.value);
    auto iths = hs.find(id);
    check(iths==hs.end(), "id already in cache now");
      hs.emplace( _self, [&](auto& a) {
      a.id = id;
      a.hashServer=hashfromserver;
      a.createTime=get_currenttime_s();
    });
}

void dicedicedice::hashsdfmsr(uint64_t id, uint64_t seed, string hashfromserver)
{
    require_auth( get_self());

    workingdices wd(get_self(), _self.value);
    auto itwd = wd.find(id);
    check(itwd!=wd.end(), "id not in dice cache now");
    check(itwd->hashServer==hashfromserver, "hashfromserver not right");
    check(itwd->endtime==0, "this dice already finished");

    uint64_t total = seed+itwd->randNumber;
    uint64_t resultRandom=total%100+1;

    wd.modify(*itwd, _self, [&](auto& a) {
            a.seedServer=seed;
            a.resultRandom=resultRandom;
            a.endtime=get_currenttime_s();
        });


    asset quantity = itwd->quantity;
    
    if (resultRandom<itwd->ratePar)
    {
          double rate = (98.5 / (itwd->ratePar - 1));
          rate = ( (double)( (int)( (rate+0.005)*100 ) ) )/100;
          quantity.amount= quantity.amount*rate;

          print_f("rate=%---quantity=%", rate, quantity);
           action(permission_level{ get_self(), "active"_n }, name(itwd->contractname), name("transfer"),
           std::make_tuple(get_self(), itwd->username, quantity, string("you win")+std::to_string(itwd->id))).send();
    }else{

    }

    auto itwddel = wd.begin();
    while (itwddel!=wd.end())
    {
      if(itwddel->endtime!=0 && itwddel->endtime+5<get_currenttime_s()){
      itwddel = wd.erase(itwddel);
    }else{
      itwddel++;
    }
  }
    
}

uint64_t dicedicedice::get_currenttime_s() {
    return static_cast<uint64_t>(current_time_point().sec_since_epoch());
}
