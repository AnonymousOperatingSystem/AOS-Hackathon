#include <eosiolib/eosio.hpp>
#include <eosiolib/time.hpp>
#include <eosiolib/contract.hpp>
#include <string>

using namespace eosio;
using std::string;

class ballot : public eosio::contract
{
public:
  const uint32_t OPENTIME = 24 * 60 * 60;


  ballot(account_name self)
      : eosio::contract(self),
        goodss(self, self),
        sites(self, self),
        ppslsiters(self, self) {}

 
  void addsite(account_name account, string site_name)
  {
    require_auth(account);
  
    for (auto &item : sites)
    {
      if (item.site_name == site_name)
      {
        print("site ", site_name, " exist!");
        return;
      }
    }
   
    sites.emplace(_self, [&](auto &site) {
      site.key = sites.available_primary_key();
      site.site_name = site_name;
      site.created = eosio::time_point_sec(now());
      site.deadline = eosio::time_point_sec(0);
      site.owner = account;
      site.voting = false;
    });
  }


  void startsite(account_name account, uint64_t site_key)
  {
    require_auth(account);
    auto vtit = sites.find(site_key);
    eosio_assert(vtit != sites.end(), "site not exist");
    eosio_assert(vtit->owner == account, "only owner can start the site");
    eosio_assert(!vtit->voting, "site is already voting");
    
    sites.modify(vtit, 0, [&](auto &site) {
      site.deadline = time_point_sec(time_point_sec(now()) + OPENTIME);
      site.voting = true;
    });
  }

  void addgoods(account_name account, uint64_t site_key, string goods_name)
  {
    require_auth(account);
    auto it = sites.find(site_key);
    eosio_assert(it != sites.end(), "site does not exist");
    eosio_assert(!it->voting, "only can add goods before site starts");
    eosio_assert(it->owner == account, "only site owner can add goods");
    auto site_index = goodss.get_index<N(site_key)>();
    auto itr = site_index.find(site_key);
    for (; itr != site_index.end() && itr->site_key == site_key; ++itr)
    {
      if (itr->name == goods_name)
      {
        print("goods ", goods_name, " exists!");
        return;
      }
    }
    goodss.emplace(_self, [&](auto &goods) {
      goods.key = goodss.available_primary_key();
      goods.site_key = site_key;
      goods.name = goods_name;
      goods.site_count = 0;
    });
  }

  void sitegoods(account_name siter, uint64_t goods_key)
  {
    require_auth(siter);
    auto pit = goodss.find(goods_key);
    eosio_assert(pit != goodss.end(), "invalid goods");
    auto vtit = sites.find(pit->site_key);
    eosio_assert(vtit != sites.end(), "invalid site");
    eosio_assert(vtit->voting, "site is not started");
    eosio_assert(time_point_sec(now()) < vtit->deadline, "site is ended");
    auto owner_index = ppslsiters.get_index<N(owner)>();
    auto itr = owner_index.find(siter);
    for (; itr != owner_index.end() && itr->owner == siter; ++itr)
    {
      if (itr->goods_key == goods_key)
      {
        print("goods sited!");
        return;
      }
    }

    ppslsiters.emplace(_self, [&](auto &ppslsiter) {
      ppslsiter.key = ppslsiters.available_primary_key();
      ppslsiter.owner = siter;
      ppslsiter.goods_key = goods_key;
    });

    goodss.modify(pit, _self, [&](auto &a) {
      a.site_count += 1;
    });
  }

  

private:
  struct site
  {
    uint64_t key;                 
    string site_name;             
    account_name owner;          
    time_point_sec created;       
    time_point_sec deadline;      
    bool voting;                  
    std::vector<uint64_t> winner;

    uint64_t
    primary_key() const
    {
      return key;
    }

    EOSLIB_SERIALIZE(site, (key)(site_name)(owner)(created)(deadline)(voting)(winner)); // 定义序列化的相关配置
  };
  typedef eosio::multi_index<N(site), site> site_index; // 定义索引

  

  struct goods
  {
    uint64_t key;
    uint64_t site_key;   
    string name;         
    uint64_t site_count; 

    uint64_t primary_key() const { return key; }
    uint64_t by_site_key() const { return site_key; }  

    EOSLIB_SERIALIZE(goods, (key)(site_key)(name)(site_count));
  };
  typedef eosio::multi_index<N(goods), goods,
                             indexed_by<N(site_key), const_mem_fun<goods, uint64_t, &goods::by_site_key>>>
      goods_index;

  // 成员变量
  site_index sites;
  ppslsiter_index ppslsiters;
  goods_index goodss;
};

EOSIO_ABI(ballot, (addsite)(startsite)(addgoods)(sitegoods)(delsiteppsl)(wingoods))
