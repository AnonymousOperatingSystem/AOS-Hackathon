<!--
 * @Descripttion: 
 * @Author: v_aoswap@users.noreply.github.com
 * @Date: 2020-09-01 10:40:31
-->
<template>
    <div class="create-market">
        <ul id="con">
            <li>
                <p class="tit">
                    Token A
                </p>
                <p class="subtit">
                    Contract address
                </p>
                <input type="text" placeholder="Enter contract address" :class="{null: !addressA && clickFlag}" v-model="addressA">
                <p class="subtit">
                    Token name
                </p>
                <input type="text" placeholder="Enter token name" :class="{null: !nameA && clickFlag}" v-model="nameA">
                <p class="subtit">
                    Amount
                </p>
                <input type="number" placeholder="Enter Amount" :class="{null: !amountA && clickFlag}" v-model="amountA">
                <p class="subtit">
                    Site
                </p>
                <input type="text" placeholder="Enter Site" v-model="siteA">
                <p class="subtit">
                    Logo
                </p>
                <input type="text" placeholder="Enter Logo" v-model="logoA">
            </li>
            <li>
                <p class="tit">
                    Token B
                </p>
                <p class="subtit">
                    Contract address
                </p>
                <input type="text" placeholder="Enter contract address" :class="{null: !addressB && clickFlag}" v-model="addressB">
                <p class="subtit">
                    Token name
                </p>
                <input type="text" placeholder="Enter token name" :class="{null: !nameB && clickFlag}" v-model="nameB">
                <p class="subtit">
                    Amount
                </p>
                <input type="number" placeholder="Enter Amount" :class="{null: !amountB && clickFlag}" v-model="amountB">
                <p class="subtit">
                    Site
                </p>
                <input type="text" placeholder="Enter Site" v-model="siteB">
                <p class="subtit">
                    Logo
                </p>
                <input type="text" placeholder="Enter Logo" v-model="logoB">
            </li>
            <div class="btn" @click="createFn">
                Create
            </div>
        </ul>
    </div>
</template>
<script>
    export default {
        data() {
            return {
                username: '',
                clickFlag: false,
                addressA: '',
                nameA: '',
                amountA: '',
                siteA: '',
                logoA: '',
                addressB: '',
                nameB: '',
                amountB: '',
                siteB: '',
                logoB: '',
                inter: '',
                limitA: '', //tokenA精度
                limitB: '', //tokenB精度
            }
        },
        mounted() {
            document.getElementById('con').style.height = document.body.clientHeight - 40 + 'px';
            this.getInfo()
        },
        methods: {
            getInfo() {
                /**
                 * @msg: 定时器receive用户账户
                 */   
                if(!this.eosUrl){
                    this.inter = setInterval(()=>{
                        if(!this.eosUrl){
                            return false;
                        }else{
                            
                            clearInterval(this.inter)
                        }
                    } ,500)
                } 
            },
            checkToken(address, name) {
                return new Promise(
                    (resolve, reject) => {
                        const val = {
                            'code': address,  // aoswap合约名
                            'json': true,
                            'limit': -1,
                            'lower_bound': '',
                            'scope': name,          // tokenx名称
                            'table': 'stat',            // 固定写死
                            'table_key': '',
                            'upper_bound': ''
                        }
                        this.$jq.ajax({
                            type: 'POST',
                            dataType: 'JSON',
                            data: JSON.stringify(val),
                            url: this.$url+ '/v1/chain/get_table_rows',
                            success: (res) => {
                                resolve(res);
                            },
                            error: (err) => {
                                resolve(err);
                            }
                        })
                    }
                )
            },
            transactionFn() {
                /**
                 * @msg: 执行transaction方法
                 * @param {type} 
                 * @return {type} 
                 */    
                this.eosUrl.transaction({
                    actions:
                    [
                        {
                            name: 'openext', // 固定的
                            account: this.$aoswapAccount,  // aoswap合约账户，可变的
                            authorization: [{
                                permission: 'active',   // 固定的
                                actor: this.eosAccount,  // 用户账号，可变的
                            }],
                            data:{
                                user: this.eosAccount,   // 用户账号，可变的
                                payer: this.eosAccount,  // 用户账号，可变的
                                token_contract: this.addressA,   // token1合约账户，可变的
                                token_sym: this.limitA+','+this.nameA           // token1的名字，可变的
                                
                            }
                        },{
                            name: 'openext', // 固定的
                            account: this.$aoswapAccount,  // aoswap合约账户，可变的
                            authorization: [{
                                permission: 'active',   // 固定的
                                actor: this.eosAccount,  // 用户账号，可变的
                            }],
                            data:{
                                user: this.eosAccount,   // 用户账号，可变的
                                payer: this.eosAccount,  // 用户账号，可变的
                                token_contract: this.addressB,   // token2合约账户，可变的
                                token_sym: this.limitB+','+this.nameB              // token2的名字，可变的
                            }
                        },
                        {
                            name: this.addressA == 'ciphertokens' ? 'transferpp' : 'transfer', // 固定的
                            account: this.addressA,  // token1合约账户，可变的
                            authorization: [{
                                permission: 'active',   // 固定的
                                actor: this.eosAccount,  // 用户账号，可变的
                            }],
                            data:{
                                from: this.eosAccount,   // 用户账号，可变的
                                to: this.$aoswapAccount,     // aoswap合约账户，可变的
                                quantity: Number(this.amountA).toFixed(this.limitA) + ' ' + this.nameA, // token1的转账金额，可变的
                                memo: ''                // 空格或者任意字符串即可
                            }
                        },{
                            name: this.addressB == 'ciphertokens' ? 'transferpp' : 'transfer', // 固定的
                            account:  this.addressB,  // token2合约账户，可变的
                            authorization: [{
                                permission: 'active',   // 固定的
                                actor: this.eosAccount,  // 用户账号，可变的
                            }],
                            data:{
                                from: this.eosAccount,       // 用户账号，可变的
                                to: this.$aoswapAccount,     // aoswap合约账户，可变的
                                quantity: Number(this.amountB).toFixed(this.limitB) + ' ' + this.nameB, // token2的转账金额，可变的
                                memo: ''                    // 空格或者任意字符串即可
                            }
                        },{
                            name: 'inittoken',        // 固定的
                            account: this.$aoswapAccount,  // aoswap合约账户，可变的
                            authorization: [{
                                actor: this.eosAccount,    // 用户账号，可变的
                                permission: 'active'
                            }],
                            data: {
                                fee_contract: this.eosAccount,   // aoswap合约账户，可变的
                                initial_fee:10,                   // 交易手续费，万分之几
                                contract_pool1: this.addressA,
                                quantity_pool1: Number(this.amountA).toFixed(this.limitA) + ' ' + this.nameA,
                                contract_pool2: this.addressB,
                                quantity_pool2: Number(this.amountB).toFixed(this.limitB) + ' ' + this.nameB,
                                new_symbol: Math.floor((parseInt(this.limitA) + parseInt(this.limitB)) / 2)+','+this.nameA+this.nameB,           // 根据token1和token2的名字，新生成的tokenx
                                user: this.eosAccount,             // 用户账号，可变的
                                site1: this.siteA,                         // token1的网址，可为空
                                site2: this.siteB,                         // token2的网址，可为空
                                logo1: this.logoA,                         // token1的logo，可为空
                                logo2: this.logoB                         // token2的logo，可为空
                            }
                        }
                    ]
                }).then(
                    res=>{
                        this.$router.push({
                            name: 'exchange',
                            query: {
                                tokenx: this.nameA + this.nameB
                            }
                        })
                    }
                ).catch(
                    (err)=>{
                        this.Toast({
                            duration: 2500,
                            message: JSON.parse(err).error.details[0].message,
                        });
                    }
                )
            },
            async createFn() {
                /**
                 * @msg: create pair
                 * @param {type} 
                 * @return {type} 
                 */    
                this.clickFlag = true;
                if(this.addressA && this.nameA && this.amountA && this.addressB && this.nameB && this.amountB) {
                    try {
                        //tokenA没问题
                        let tokenARes = await this.checkToken(this.addressA, this.nameA);
                        this.limitA = tokenARes.rows[0].supply.split('.')[1].split(' ')[0].length;
                        try {
                            // tokenA tokenB 都验证成功
                            let tokenBRes = await this.checkToken(this.addressB, this.nameB);
                            this.limitB = tokenBRes.rows[0].supply.split('.')[1].split(' ')[0].length;
                            this.transactionFn()
                        }catch(error) {
                            //tokanB 不存在
                            this.Toast({
                                duration: 2500,
                                message: error.responseJSON.error.details[0].message,
                            });
                        }
                    }catch(err) {
                        //tokanA 不存在
                        this.Toast({
                            duration: 2500,
                            message: err.responseJSON.error.details[0].message,
                        });
                    }
                } else {
                    //信息不完善
                    this.Toast({
                        duration: 2500,
                        message: 'Please complete info.',
                    });
                }
            }
        }
    }
</script>
<style lang="less" scoped>
    .create-market {
        border-radius: 0.12rem;
        background-color: #fff;
        width: calc(~"100% - 40px");
        margin: 0 auto;
        ul {
            padding: 0.2rem;
            height: 500px;
            overflow-y: scroll;
        }
        .null {
            border: .01rem solid red;
        }
        input {
            border: .01rem solid #F0F2F5;
            padding-left: .14rem;
            font-size:0.14rem;
            font-family:PingFangSC-Regular;
            color:#000;
            background: #F0F2F5;
            border-radius: 0.08rem;
            display: block;
            height: 0.48rem;
            line-height: 0.48rem;
            width: 100%;
            margin-top: 0.05rem;
        }
        .tit {
            font-size: 0.16rem;
            color: #000;
            text-align: left;
        }
        .subtit {
            font-size: 0.14rem;
            color: #ACACBD;
            text-align: left;
            margin-top: 0.1rem;
        }
        .btn {
            height: 0.48rem;
            background: #4D5CFF;
            border-radius: 0.12rem;
            font-size: 0.16rem;
            color: #FFFFFF;
            line-height: 0.48rem;
            margin-top: 0.2rem;
        }
        ul li:nth-child(2) {
            .tit {
                margin-top: 0.2rem;
            }
        }
    }
</style>