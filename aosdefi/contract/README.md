# eosgarden
An AOS based DAPP for Defi.

## command

```
build contract:

1. install eosio.cdt: https://github.com/EOSIO/eosio.cdt#binary-releases

2. use eosio.cdt to build contract
rm -rf build
mkdir build
cd build
eosio-cpp -abigen ../aosgarden.cpp -o aosgarden.wasm -I ./
```

```
contract: aosgarden111

token1 contract: usdtusdtusdt
token2 contract: tokentoken22

set contract:
claos set contract aosgarden111 ./build aosgarden.wasm aosgarden.abi

set permission:
claos set account permission aosgarden111 active --add-code

create_pool:
claos push action usdtusdtusdt transfer '["usdtusdtusdt", "aosgarden111", "40000.0000 APS", "CREATE:tokentoken22|4,AAA|1|365"]' -p usdtusdtusdt

query global state:
claos get table aosgarden111 aosgarden111 globals

query pool:
claos get table aosgarden111 1 pools

query user state:
claos get table aosgarden111 alice11alice users

stake:
claos push action tokentoken22 transfer '["alice11alice", "aosgarden111", "100.0000 AAA", "STAKE:1|"]' -p alice11alice

claim:
claos push action aosgarden111 claim '["alice11alice", 1]' -p alice11alice

withdraw:
claos push action aosgarden111 withdraw '["alice11alice", 1, "tokentoken22", "10.0000 AAA"]' -p alice11alice

clear:
claos push action aosgarden111 foo '[]' -p aosgarden111

```

接口信息：

1. 创建挖矿池
```
POST push_transaction

actions: [{
        account: 'TOKEN1_CONTRACT', // 矿池token的合约账户，如aosio.token
        name: 'transfer',           // 矿池token的转账操作，一般都是transfer，但对account是ciphertokens来说，是transferpp
        authorization: [{
            actor: 'YOUR_ACCOUNT',  // 创建者账户
            permission: 'active',
        }],
        data: {
            from: 'YOUR_ACCOUNT',   // 创建者账户
            to: 'AOSWAP_CONTRACT',  // 本合约账户
            quantity: '40.0000 ASP', // 矿池token总金额
            memo: "CREATE:TOKEN2_CONTRACT|SYMBOL|FADE|DAYS" // 格式是：CREATE:抵押token的合约|抵押token的symbol(包含精度，如:4,AOS)|衰减因子|总挖矿天数
        },
    }]
```

2. 查询基本信息
```
POST get_table_rows

    参数：
    {
        "code": "AOSWAP_CONTRACT",  // 本合约账户
        "json": true,
        "limit": -1,
        "lower_bound": "",
        "scope": "AOSWAP_CONTRACT", // 本合约账户
        "table": "globals",         // 固定写死
        "table_key": "",
        "upper_bound": ""
    }

    结果示例：
    {
        "rows": [{
            "owner": "aosgarden111",                            // 合约名
            "users": 2,                                         // 当前参与抵押的用户数
            "pairs": [{                                         // 挖矿池列表
                "id": 1,                                        // 矿池id，唯一的
                "profile": "usdtusdtusdtAPStokentoken22AAA"     // 矿池合约名和token名组成的字符串，唯一的
                }
            ]
            }
        ],
        "more": false,
        "next_key": ""
    }
```

3. 查询某矿池详细信息
```
POST get_table_rows

    参数：
    {
        "code": "AOSWAP_CONTRACT",// 本合约账户
        "json": true,
        "limit": -1,
        "lower_bound": "",
        "scope": POOL_ID,         // 矿池id，int类型
        "table": "pools",         // 固定写死
        "table_key": "",
        "upper_bound": ""
    }

    结果示例：
    {
        "rows": [{
            "creater": "usdtusdtusdt",          // 矿池创建者
            "total_supply": {                   // 矿池token总供应量
                "quantity": "40000.0000 APS",   // 数量
                "contract": "usdtusdtusdt"      // 矿池token的合约名
            },
            "cur_supply": {                     // 矿池token当前剩余量
                "quantity": "40000.0000 APS",
                "contract": "usdtusdtusdt"
            },
            "start_time": 1600328268,           // 矿池启动时间
            "init_supply": {                    // 初期每个释放周期的释放量，用于计算APY
                "quantity": "2.8571 APS",
                "contract": "usdtusdtusdt"
            },
            "fade_factor": 1,                   // 衰减因子，需除以100
            "duration": 365,                    // 总挖矿天数
            "token_staked": [{                  // 抵押token统计，一般可能有多个，按照时间排序
                "timestamp": 1600333068,        // 统计时间
                "staked": {
                    "quantity": "700.0000 AAA", // 数量
                    "contract": "tokentoken22"  // 抵押token合约名
                }
                }
            ]
            }
        ],
        "more": false,
        "next_key": ""
    }
```

4. 查询用户信息

```
POST get_table_rows

    参数：
    {
        "code": "AOSWAP_CONTRACT",// 本合约账户
        "json": true,
        "limit": -1,
        "lower_bound": "",
        "scope": USER_NAME,       // 用户名
        "table": "users",         // 固定写死
        "table_key": "",
        "upper_bound": ""
    }

    结果示例：
    {
        "rows": [{
            "id": "alice11alice",               // 用户名
            "inviter": "",                      // 邀请者
            "invitees": [],                     // 邀请来的人
            "create_time": 1600328293,          // 第一次参与时间
            "stakes": [{                        // 抵押信息，数组，可能参与抵押多个池子
                "pool_id": 1,                   // 矿池id
                "staked": {
                    "quantity": "300.0000 AAA", // 抵押金额
                    "contract": "tokentoken22"  // 抵押token的合约
                },
                "invite_staked": {              // 邀请来的人总共抵押的金额
                    "quantity": "0.0000 AAA",   // 金额
                    "contract": "tokentoken22"  // 抵押token的合约
                },
                "rewards": {                    // 我抵押挖矿领取的历史奖励
                    "quantity": "0.4383 APS",   // 金额
                    "contract": "usdtusdtusdt"  // 矿池token的合约
                },
                "invite_rewards": {             // 我邀请的人抵押给我带来的历史奖励
                    "quantity": "324.1872 APS", // 金额
                    "contract": "usdtusdtusdt"  // 矿池token的合约
                },
                "claim_time": 1600333087        // 上次领取奖励的时间
                }
            ]
            }
        ],
        "more": false,
        "next_key": ""
    }


```

5. 抵押挖矿
```
POST push_transaction

actions: [{
        account: 'TOKEN2_CONTRACT', // 抵押token的合约账户，如aosio.token
        name: 'transfer',           // 抵押token的转账操作，一般都是transfer，但对account是ciphertokens来说，是transferpp
        authorization: [{
            actor: 'YOUR_ACCOUNT',  // 用户账户
            permission: 'active',
        }],
        data: {
            from: 'YOUR_ACCOUNT',   // 用户账户
            to: 'AOSWAP_CONTRACT',  // 本合约账户
            quantity: '40.0000 ASP', // 抵押token金额
            memo: "STAKE:POOL_ID|INVITER" // 格式是：STAKE:矿池id|邀请者（若是前100的用户，此为空，但分隔符|不能少）
        },
    }]
```

6. 领取收益
```
POST push_transaction

actions: [{
        account: 'AOSWAP_CONTRACT', // 本合约账户
        name: 'claim',              // 固定写死即可
        authorization: [{
            actor: 'YOUR_ACCOUNT',  // 用户账户
            permission: 'active',
        }],
        data: {
            player: 'YOUR_ACCOUNT',   // 用户账户
            pid: POOL_ID,             // 矿池id
        },
    }]
```

7.取回抵押
```
POST push_transaction

actions: [{
        account: 'AOSWAP_CONTRACT', // 本合约账户
        name: 'withdraw',           // 固定写死即可
        authorization: [{
            actor: 'YOUR_ACCOUNT',  // 用户账户
            permission: 'active',
        }],
        data: {
            player: 'YOUR_ACCOUNT',             // 用户账户
            pid: POOL_ID,                       // 矿池id
            token_contract: 'TOKEN2_CONTRACT',  // 抵押token的合约名
            token_quantity: '100.0000 AOS'      // 抵押token的数量
        },
    }]
```