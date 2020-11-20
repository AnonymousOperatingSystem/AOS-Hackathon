#include "rps.hpp"
#include "utils.hpp"
#include <math.h> 

const static string SPLITTER = "|";

void rps::ontransfer(name from, name to, asset quantity, string memo)
{   
    print_f("you just call ontransfer: %  --- %  %---%---",from,to,quantity,memo);
    auto parts = split(memo, SPLITTER);

    //id|rand
    if (from==name("rockpapescis"))
    {
      return;
    }

    if (from==name("rockpapescic"))
    {
      return;
    }

    if (from==name("kangboaosdep"))
    {
      return;
    }

    
    check(parts.size() == 2, "memo format not right");

    bool has_only_digits = (string(parts[0]).find_first_not_of( "0123456789" ) == std::string::npos);
    check(has_only_digits, "id must be number");

    bool has_only_123 = (string(parts[1]).find_first_not_of( "123" ) == std::string::npos);
    check(has_only_123, "Rock-paper-scissors must choose 1, 2, 3");

    uint64_t clientChoose = std::stoi(std::string(parts[1]));
    check(clientChoose>=1&&clientChoose<=3, "Rock-paper-scissors must choose 1, 2, 3");

    
    uint64_t id = std::stoi(std::string(parts[0]));
    workingdices wd(get_self(), _self.value);
    auto itx = wd.find(id);
    check(itx==wd.end(), "id already exist,not repeat");

    hashseedques hs(get_self(), _self.value);
    
    auto iths = hs.begin();
    check(iths!=hs.end(), "no cache hash can use,wait for a while");
    
      wd.emplace( _self, [&](auto& a) {
        a.id = id;
        a.userName = from;
        a.clientChoose= clientChoose;
        a.hashServer=iths->hashServer;
        a.quantity=quantity;
      });

      hs.erase(iths);
}

void rps::erase(uint64_t id)
{
  require_auth( get_self());
  workingdices wd(get_self(), _self.value);
  auto itwd = wd.find(id);
  check(itwd!=wd.end(), "id not exist!");
  wd.erase(itwd);
}

void rps::hashfmsr(uint64_t id, string hashfromserver)
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

void rps::hashsdfmsr(uint64_t id, uint64_t chooseserver, string uuidserver, string hashserver)
{
    require_auth( get_self());

    workingdices wd(get_self(), _self.value);
    auto itwd = wd.find(id);
    check(itwd!=wd.end(), "id not exist!");
    check(itwd->hashServer==hashserver, "hashfromserver not right");
    check(itwd->endtime==0, "this game already finished");


    wd.modify(*itwd, _self, [&](auto& a) {
            a.serverChoose=chooseserver;
            a.uuidServer=uuidserver;
            a.endtime=get_currenttime_s();
        });


    asset quantity = itwd->quantity;
    
    //1石头 2剪刀 3布，打成平手，则玩家需要支付1%的服务费
    if(chooseserver==itwd->clientChoose){
          quantity.amount= quantity.amount*0.99;
          if(quantity.amount <1){
            quantity.amount = 1;
          }
          print_f("---quantity=%", quantity);
          action(permission_level{ get_self(), "active"_n }, name("aosio.token"), name("transfer"),
          std::make_tuple(get_self(), itwd->userName, quantity, string("you guys are tied, you only need to pay 1% platform usage fee")+std::to_string(itwd->id))).send();
    }else //server win
    if((chooseserver==1 && itwd->clientChoose==2)||
    (chooseserver==2 && itwd->clientChoose==3)||
    (chooseserver==3 && itwd->clientChoose==1)){

    }//server lost,2倍返还
    else{
          quantity.amount= quantity.amount*1.99;
          print_f("---quantity=%",  quantity);
          action(permission_level{ get_self(), "active"_n }, name("aosio.token"), name("transfer"),
          std::make_tuple(get_self(), itwd->userName, quantity, string("you win 199% of the bet amount")+std::to_string(itwd->id))).send();
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

uint64_t rps::get_currenttime_s() {
    return static_cast<uint64_t>(current_time_point().sec_since_epoch());
}
