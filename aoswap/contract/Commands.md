**NOTE: In this example we use the PAIR AOS/USDT, which creates the evotoken AOSUSDT. We assume an AOS token located in the aosio.token contract as usual, and a USDT token located in the contract usdtusdttest. You would need to replace these variables depending on your trading pairs.**

First, let us describe the single actions of the smart contract.

Open a channel in the contract. This channel will store your trading tokens. You need to create one channel for each token you plan to trade. The second input below is the ram payer, and the authorizer must be the ram payer.

    claos push action aoswapaoswap openext '["alice22alice", "alice22alice", {"contract":"aosio.token", "sym":"4,AOS"}]' -p alice22alice

Open a channel for the second token you wish to trade in evodex:

    claos push action aoswapaoswap openext '["alice22alice", "alice22alice", {"contract":"usdtusdttest", "sym":"4,USDT"}]' -p alice22alice

Close the contract's channel for a specific token. In case there are funds there,
it returns them to the account "TO".

    claos push action aoswapaoswap closeext '["YOUR_ACCOUNT", "TO", {"contract":"aosio.token", "sym":"4,AOS"}, "memo"]' -p YOUR_ACCOUNT

Fill your account with the desired tokens:

    claos push action aosio.token transfer '["alice11alice", "aoswapaoswap", "10.0000 AOS", "memo1"]' -p alice11alice

    claos push action usdtusdttest transfer '["alice11alice", "aoswapaoswap", "30.0000 USDT", "memo2"]' -p alice11alice

Check your open channels and balances:

    claos get table aoswapaoswap alice11alice evodexacnts

Withdraw funds from your opened channels, to the account "TO":

    claos push action aoswapaoswap withdraw '["YOUR_ACCOUNT", "TO", {"contract":"aosio.token", "quantity":"1.0000 AOS"}, "memo"]' -p YOUR_ACCOUNT

Create the AOS/USDT evotoken. Set the initial liquidity, the initial fee for the trading pair and the fee controller.

    claos push action aoswapaoswap inittoken '["alice11alice", "4,AOSUSDT", {"contract":"aosio.token", "quantity":"1.0000 AOS"}, {"contract":"usdtusdttest", "quantity":"3.0000 USDT"}, 10, "alice11alice"]' -p alice11alice

Check your evotokens balance:

    claos get table aoswapaoswap alice11alice accounts


Create pool
```
    {
        "actions":
        [
        {
            "account": "aoswapaoswap",
            "name": "openext",
            "authorization": [{"actor": "YOUR_ACCOUNT","permission": "active"}],
            "data": {
                "user": "YOUR_ACCOUNT",
                "payer": "YOUR_ACCOUNT",
                "ext_symbol": {"contract":"aosio.token", "sym":"4,AOS"}
            }
        },{
            "account": "aoswapaoswap",
            "name": "openext",
            "authorization": [{"actor": "YOUR_ACCOUNT","permission": "active"}],
            "data": {
                "user": "YOUR_ACCOUNT",
                "payer": "YOUR_ACCOUNT",
                "ext_symbol": {"contract":"usdtusdttest", "sym":"4,USDT"}
            }
        },{
            "account": "aosio.token",
            "name": "transfer",
            "authorization": [{"actor": "YOUR_ACCOUNT","permission": "active"}],
            "data": {
                "from": "YOUR_ACCOUNT",
                "to": "aoswapaoswap",
                "quantity": "2.0000 AOS",
                "memo": ""
            }
        },{
            "account": "usdtusdttest",
            "name": "transfer",
            "authorization": [{"actor": "YOUR_ACCOUNT","permission": "active"}],
            "data": {
                "from": "YOUR_ACCOUNT",
                "to": "aoswapaoswap",
                "quantity": "2.0000 USDT",
                "memo": ""
            }
        },{
            "account": "aoswapaoswap",
            "name": "inittoken",
            "authorization": [{"actor": "YOUR_ACCOUNT","permission": "active"}],
            "data": {
                "fee_contract":"wevotethefee",
                "initial_fee":10,
                "initial_pool1":{
                    "contract":"aosio.token",
                    "quantity":"2.0000 AOS"
                },
                "initial_pool2":{
                    "contract":"usdtusdttest",
                    "quantity":"2.0000 USDT"
                },
                "new_symbol": "4,AOSUSDT",
                "user": "YOUR_ACCOUNT"
            }
        },{
            "account": "aosio.token",
            "name": "transfer",
            "authorization": [{"actor": "YOUR_ACCOUNT","permission": "active"}],
            "data": {
                "from": "YOUR_ACCOUNT",
                "to": "AOSwapteam11",
                "quantity": "2.0000 AOS",
                "memo": ""
            }
        }
        ]
    } 
```

Add more liquidity to a pool. Set the exact amount of evotoken to obtain, in this case 
1.5000 AOSUSDT, and the maximum you are willing to pay of each token of the pair.

    claos push action aoswapaoswap addliquidity '["alice22alice", "1.7320 AOSUSDT",  "1.0000 AOS", "3.0000 USDT"]' -p alice22alice

Sell your evotokens and retire liquidity. The amount of evotoken is exact and the other two are minima required.

    claos push action aoswapaoswap remliquidity '["YOUR_ACCOUNT", "1.0000 AOSUSDT", 
    "0.1000 USDT", "1.0000 AOS"]' -p YOUR_ACCOUNT

Exchange your tokens.
There two methods. The first one is to do a transfer to the contract with a memo starting with "exchange:" and followed by the details of your operation, with the format "EVOTOKN, min_expected_asset, memo". Blank spaces before EVOTOKN, min_expected_asset and memo are ignored. The amount to be obtained by the user will be computed by the contract and executed only if it is at least min_expected_asset. 

    claos push action aosio.token '["YOUR_ACCOUNT", "aoswapaoswap", "1.0000 AOS", "exchange: AOSUSDT, 0.1000 USDT, memo for the transfer]' -p YOUR_ACCOUNT

The other method operates between funds already deposited in the contract. The structure
of the input is account, evotoken, extended_asset to pay (exact), asset to receive (limiting).

    claos push action aoswapaoswap exchange '["YOUR_ACCOUNT", "AOSUSDT", 
    {"contract":"aosio.token", "quantity":"1.0000 AOS"}, "0.1000 USDT"]' -p YOUR_ACCOUNT

It is also possible to set the exact amount to obtain and limit the amount to pay.
To do this, use negative amounts. The following example means that you want to receive exactly 0.1000 USDT and pay at most 1.0000 AOS. 

    claos push action aoswapaoswap exchange '["YOUR_ACCOUNT", "AOSUSDT", 
    {"contract":"aosio.token", "quantity":"-0.1000 USDT"}, "-1.0000 AOS"]' -p YOUR_ACCOUNT

Transfer your evotokens to another account:

    claos push action aoswapaoswap transfer '["YOUR_ACCOUNT", "argentinaAOS", "0.0001 AOSUSDT", "ITS ALIVE"]' -p YOUR_ACCOUNT

See evotoken stats:

    claos get table aoswapaoswap AOSUSDT stat

In many practical cases, users will prefer to run many actions in a single transaction.
For example, if you want to add liquidity, you will probably prefer to close the accounts in the contract aoswapaoswap corresponding to the external tokens, to avoid spending RAM. To that end, you may run:

    claos push transaction addliquidity.json

where the file addliquidity.json contains:

    {
        "actions":
        [
        {
            "account": "aoswapaoswap",
            "name": "openext",
            "authorization": [{"actor": "YOUR_ACCOUNT","permission": "active"}],
            "data": {
                "user": "YOUR_ACCOUNT",
                "payer": "YOUR_ACCOUNT",
                "ext_symbol": {"contract":"aosio.token", "sym":"4,AOS"}
            }
        },{
            "account": "aoswapaoswap",
            "name": "openext",
            "authorization": [{"actor": "YOUR_ACCOUNT","permission": "active"}],
            "data": {
                "user": "YOUR_ACCOUNT",
                "payer": "YOUR_ACCOUNT",
                "ext_symbol": {"contract":"usdtusdttest", "sym":"4,USDT"}
            }
        },{
            "account": "aosio.token",
            "name": "transfer",
            "authorization": [{"actor": "YOUR_ACCOUNT","permission": "active"}],
            "data": {
                "from": "YOUR_ACCOUNT",
                "to": "aoswapaoswap",
                "quantity": "2.0000 AOS",
                "memo": ""
            }
        },{
            "account": "usdtusdttest",
            "name": "transfer",
            "authorization": [{"actor": "YOUR_ACCOUNT","permission": "active"}],
            "data": {
                "from": "YOUR_ACCOUNT",
                "to": "aoswapaoswap",
                "quantity": "2.0000 USDT",
                "memo": ""
            }
        },{
            "account": "aoswapaoswap",
            "name": "addliquidity",
            "authorization": [{"actor": "YOUR_ACCOUNT","permission": "active"}],
            "data": {
                "user": "YOUR_ACCOUNT",
                "to_buy": "1.5000 AOSUSDT",
                "max_asset1": "2.0000 AOS",
                "max_asset2": "2.0000 USDT",                
            }
        },{
            "account": "aoswapaoswap",
            "name": "closeext",
            "authorization": [{"actor": "YOUR_ACCOUNT","permission": "active"}],
            "data": {
                "user": "YOUR_ACCOUNT",
                "to": "TO",
                "ext_symbol": {"contract":"usdtusdttest", "sym":"4,USDT"},
                "memo": ""
            }
        },{
            "account": "aoswapaoswap",
            "name": "closeext",
            "authorization": [{"actor": "YOUR_ACCOUNT","permission": "active"}],
            "data": {
                "user": "YOUR_ACCOUNT",
                "to": "TO",                
                "ext_symbol": {"contract":"aosio.token", "sym":"4,AOS"},
                "memo": ""
            }
        }
        ]
    }    

The same idea applies to the operations of removing liquidity and inittoken.
Typically, a graphical user interface will perform this kind of multiaction transactions.

Finally, to change the fee for operating through AOSUSDT run:

    claos push action aoswapaoswap changefee '["4,AOSUSDT", "37"]' -p FEE_CONTROLLER

and the fee will now be set to 0.37%. Or you can configure a contract to perform
the *changefee* action.