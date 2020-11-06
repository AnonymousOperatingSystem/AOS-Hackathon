# horsebet
An AOS based DAPP of horse betting.

## About
该DAPP的游戏规则是：
每一期，开始的时候，先离线选择中奖号码，然后再拼上一段随机数，将他们做hash，存入合约，开启一期。
大家可以自由的选择购买号码，号码范围是1到16，可重复，可累加。这一期结束的时候，将中奖号码和随机数以明文方式传入合约，完成hash比对后，开奖。
买中的人，获得所购金额13倍的奖励。
玩家投注的金额没有限制。玩家需要手动领取奖励。

鉴于上述规则，我们设计的合约分为：

数据结构部分：
```
gloabl
全局属性信息，以合约名为scope

round
各期的投注和中奖号码等信息，每一期一张表，以期数作为scope

player
每个用户的信息，以用户名作为scope

```

合约调用接口部分：
```
合约所有者或管理员执行创建新的一期操作，需传入期数和hash
open()

合约所有者或管理员执行开奖操作，需传入期数，中奖号码，和随机数
reveal()

玩家领取奖励
claim()

监听aosio.token的transfer操作，读取memo的传入数字，给每个用户记录下注信息
transfer()
```

## test command

0. build contract
```
mkdir build & cd build
eosio-cpp -abigen ../horsebet.cpp -o horsebet.wasm -I ./
```

1. deploy contract
```
claos  set contract pickanimal11 ./build/ horsebet.wasm horsebet.abi -p pickanimal11
```

2. set code permission
```
claos  set account permission pickanimal11 active --add-code
```

3. open new period
```
claos  push action pickanimal11 open '[1, "7cfae5817b5ad58b9e6a58443f34fc6bdc9986cbade28315ff7e712e0463c297"]' -p pickanimal11
```

4. reveal period
```
claos  push action pickanimal11 reveal '[1, 12, "e752e69bd6be58adf32d5278943b4bd3adc988f5074b214c7d304508328a180b"]' -p pickanimal11
```

5. 下注，user pick a number
```
claos  push action eosio.token transfer '["alicealiceaa", "pickanimal11", "1.0000 AOS", "PICK:12"]' -p alicealiceaa

POST push_transaction
actions: [{
        account: 'aosio.token', // 下注token的合约账户，如aosio.token
        name: 'transfer',           // 抵押token的转账操作，一般都是transfer，但对account是ciphertokens来说，是transferpp
        authorization: [{
            actor: 'YOUR_ACCOUNT',  // 用户账户
            permission: 'active',
        }],
        data: {
            from: 'YOUR_ACCOUNT',   // 用户账户
            to: 'CONTRACT',  // 本合约账户
            quantity: '40.0000 AOS', // 抵押token金额
            memo: "PICK:NUMBER" // 格式是：PICK:号码（例如PICK:12），号码范围是1~16
        },
    }]

```

6. 领奖，user try to claim rewards
```
claos  push action pickanimal11 claim '["alicealiceaa"]' -p alicealiceaa

POST push_transaction
actions: [{
        account: 'CONTRACT', // 本合约账户
        name: 'claim',              // 固定写死即可
        authorization: [{
            actor: 'YOUR_ACCOUNT',  // 用户账户
            permission: 'active',
        }],
        data: {
            user: 'YOUR_ACCOUNT'   // 用户账户
        },
    }]
  
```

7. 获取总信息，query global infos
```
claos  get table pickanimal11 pickanimal11 globals

POST get_table_rows
参数：
    {
        "code": "CONTRACT",  // 本合约账户
        "json": true,
        "limit": -1,
        "lower_bound": "",
        "scope": "CONTRACT", // 本合约账户
        "table": "globals",         // 固定写死
        "table_key": "",
        "upper_bound": ""
    }

{
  "rows": [{
      "owner": "pickanimal11",  // 合约名
      "on": 1,                  // 是否运行
      "period": 1,              // 当前期数
      "income": "0.0000 AOS",   // 收到的总下注金额
      "outcome": "0.0000 AOS"   // 产生的总奖金
    }
  ],
  "more": false,
  "next_key": ""
}
```

8. 获取某一期的情况，query period info
```
claos  get table pickanimal11 1 rounds

POST get_table_rows
参数：
    {
        "code": "CONTRACT",  // 本合约账户
        "json": true,
        "limit": -1,
        "lower_bound": "",
        "scope": 1, // 期数
        "table": "rounds",         // 固定写死
        "table_key": "",
        "upper_bound": ""
    }

{
  "rows": [{
      "period": 1,  // 期数，从1开始，依次递增
      "open": 1,    // 本期是否开启，0为结束
      "lucky": 0,   // 本期中奖号码，未开奖前是0，开奖后在1~16之间
      "hash": "7cfae5817b5ad58b9e6a58443f34fc6bdc9986cbade28315ff7e712e0463c297", // 中奖号码的hash
      "random": "", // 随机数
      "animals": [{
          "number": 1,  // 号码
          "invests": "0.0000 AOS" // 该号码总下注金额
        },{
          "number": 2,
          "invests": "0.0000 AOS"
        },{
          "number": 3,
          "invests": "0.0000 AOS"
        },{
          "number": 4,
          "invests": "0.0000 AOS"
        },{
          "number": 5,
          "invests": "0.0000 AOS"
        },{
          "number": 6,
          "invests": "0.0000 AOS"
        },{
          "number": 7,
          "invests": "0.0000 AOS"
        },{
          "number": 8,
          "invests": "0.0000 AOS"
        },{
          "number": 9,
          "invests": "0.0000 AOS"
        },{
          "number": 10,
          "invests": "0.0000 AOS"
        },{
          "number": 11,
          "invests": "0.0000 AOS"
        },{
          "number": 12,
          "invests": "2.0000 AOS"
        },{
          "number": 13,
          "invests": "1.0000 AOS"
        },{
          "number": 14,
          "invests": "0.0000 AOS"
        },{
          "number": 15,
          "invests": "0.0000 AOS"
        },{
          "number": 16,
          "invests": "0.0000 AOS"
        }
      ]
    }
  ],
  "more": false,
  "next_key": ""
}
```

9. 获取某用户的信息，query player info
```
claos  get table pickanimal11 alicealiceaa players

POST get_table_rows
  参数：
    {
        "code": "CONTRACT",  // 本合约账户
        "json": true,
        "limit": -1,
        "lower_bound": "",
        "scope": "alicealiceaa", // 用户名
        "table": "players",         // 固定写死
        "table_key": "",
        "upper_bound": ""
    }

{
  "rows": [{
      "id": "alicealiceaa",     // 用户名
      "invests": "3.0000 AOS",  // 总下注金额
      "profits": "0.0000 AOS",  // 总收益
      "claimed": 0,             // 已领奖的最新期数，比如现在游戏期数是10，这个claimed是5，则说明用户已经领了1~5期的奖了，但是还没有领6~9期的奖
      "pickings": [{      // 下注记录
          "period": 1,            // 期数
          "number": 12,           // 号码
          "invest": "2.0000 AOS"  // 下注金额
        },{
          "period": 1,
          "number": 13,
          "invest": "1.0000 AOS"
        }
      ]
    }
  ],
  "more": false,
  "next_key": ""
}
```