<!--
 * @Descripttion: 
 * @Author: v_aoswap@users.noreply.github.com
 * @Date: 2020-09-01 15:38:21
-->
<template>
    <div class="con">
        <img src="../../static/image/logo_new.png" alt="" class="logo">
        <div class="exchange">
            <div class="tab">
                <span :class="{checked: exchange, unchecked: !exchange, lefttab: true}" @click="exchange = true;">
                    Swap
                </span>
                <span :class="{checked: !exchange, unchecked: exchange, righttab: true}" @click="exchange = false;">
                    Liquidity
                </span>
            </div>
            <div class="exchage-tab" v-if="exchange">
                <div class="pay">
                    <p class="pay-tit">
                        <span>
                            Pool size: {{oriPool.pool1.quantity}}
                        </span>
                        <span>
                            send
                        </span>
                    </p>
                    <div class="pay-money">
                        <img :src="tokenxImg.token1.logo" alt="" v-if="poolFlag && tokenxImg.token1.logo.match(/http/)">
                        <img :src="tokenxImg.token2.logo" alt="" v-else-if="!poolFlag && tokenxImg.token2.logo.match(/http/)">
                        <img src="../../static/image/null.png" alt="" v-else>
                        <span class="pay-name">
                            {{oriPool.pool1.quantity.split(' ')[1]}}
                        </span>
                        <span class="pay-addr">
                            {{oriPool.pool1.contract}}
                        </span>
                        <input type="number" placeholder="0.00" v-model="amount1" @change="amountChange">
                    </div>
                </div>
                <img src="../../static/image/change.png" alt="" class="change-img" @click="changePayFn">

                <div class="pay">
                    <p class="pay-tit">
                        <span>
                            Pool size: {{oriPool.pool2.quantity}}
                        </span>
                        <span>
                            receive
                        </span>
                    </p>
                    <div class="pay-money">
                        <img :src="tokenxImg.token2.logo" alt="" v-if="poolFlag && tokenxImg.token2.logo.match(/http/)">
                        <img :src="tokenxImg.token1.logo" alt="" v-else-if="!poolFlag && tokenxImg.token1.logo.match(/http/)">
                        <img src="../../static/image/null.png" alt="" v-else>
                        <span class="pay-name">
                            {{oriPool.pool2.quantity.split(' ')[1]}}
                        </span>
                        <span class="pay-addr">
                            {{oriPool.pool2.contract}}
                        </span>
                        <p class="amount2">{{amount2}}</p>
                    </div>
                </div>

                <div class="price">
                    <span class="tit">
                        Price
                    </span>
                    <span class="rate">
                        <span>
                            {{price}} {{oriPool.pool1.quantity.split(' ')[1]}}/{{oriPool.pool2.quantity.split(' ')[1]}}
                        </span>
                        <img src="../../static/image/change-rate.png" alt="" @click="changePayFn">
                    </span>
                </div>
                <div class="btn" @click="swapFn">
                    Swap
                </div>
            </div>
            <div class="market" v-if="!exchange">
                <div :class="{'market-tab': true, checkedin: checkedin, checkedout: !checkedin}">
                    <span :class="{'checked-tab-in': checkedin, 'checked-tab-out': !checkedin}" @click="checkedin = true">
                        Provide
                    </span>
                    <span :class="{'checked-tab-in': !checkedin, 'checked-tab-out': checkedin}" @click="checkedin = false">
                        Withdraw
                    </span>
                </div>
                <div v-if="checkedin">
                    <div class="pay">
                        <p class="pay-tit">
                            <span>
                                Pool size: {{pool.pool1.quantity}}
                            </span>
                            <span>
                                receive
                            </span>
                        </p>
                        <div class="pay-money">
                            <img :src="tokenxImg.token1.logo" alt="" v-if="tokenxImg.token1.logo.match(/http/)">
                            <img src="../../static/image/null.png" alt="" v-else>
                            <span class="pay-name" @click="showList">
                                {{pool.pool1.quantity.split(' ')[1]}}
                                <span class="iconfont icon-arrow-down">
                                </span>
                            </span>
                            <span class="pay-addr" @click="showList">
                                {{pool.pool1.contract}}
                            </span>
                            <input type="number" placeholder="0.00" v-model="token1" @change="amountChange" @input="inputToken1">
                        </div>
                    </div>
                    <p class="iconfont icon-add-bold add">
                    </p>
                    <div class="pay" style="margin-bottom: 0.24rem">
                        <p class="pay-tit">
                            <span>
                                Pool size: {{pool.pool2.quantity}}
                            </span>
                            <span>
                                receive
                            </span>
                        </p>
                        <div class="pay-money">
                            <img :src="tokenxImg.token2.logo" alt="" v-if="tokenxImg.token2.logo.match(/http/)">
                            <img src="../../static/image/null.png" alt="" v-else>
                            <span class="pay-name"  @click="showList">
                                {{pool.pool2.quantity.split(' ')[1]}}
                                <span class="iconfont icon-arrow-down">
                                </span>
                            </span>
                            <span class="pay-addr"  @click="showList">
                                {{pool.pool2.contract}}
                            </span>
                            <input type="number" placeholder="0.00" v-model="token2" @change="amountChange" @input="inputToken2">
                        </div>
                    </div>

                    <div class="btn" style="margin-bottom: 0.2rem" @click="addliquidityFn">
                        Provide
                    </div>
                    <p class="pool">
                        <span>
                            Pool price
                        </span>
                        <span>
                            {{poolPrice}} {{this.pool1.symbol.code().to_string()}}/{{this.pool2.symbol.code().to_string()}}
                        </span>
                    </p>
                    <p class="pool">
                        <span>
                            Pool token supply
                        </span>
                        <span>
                            {{pool.supply}}
                        </span>
                    </p>
                </div>
                <div v-if="!checkedin">
                    <!-- //Withdraw -->
                    <div class="key-val" @click="showList">
                        <p>
                            {{pool1.symbol.code().to_string()}} / {{pool2.symbol.code().to_string()}}
                        </p>
                        <p>
                           {{pool.pool1.contract}} / {{pool.pool2.contract}}
                        </p>
                        <span class="iconfont icon-arrow-down arrow">

                        </span>
                    </div>
                    <div class="idcard">
                        <p>
                            <span>
                                New Token
                            </span>
                            <span>
                                My New Token : {{myNewTokenAmount}}
                            </span>
                        </p>
                        <input type="number" class="amount" placeholder="0.0" v-model="amount" @change="amountChangeRm">
                    </div>
                    <p class="get-val">
                        <span>
                            receive
                        </span>
                        <span>
                            {{baseReceive}} + {{quoteReceive}}
                        </span>
                    </p>
                    <div class="btn" @click="remliquidity">
                        receive
                    </div>
                    <p class="pool-det">
                        <span>
                            Pool token supply
                        </span>
                        <span>
                            {{pool.supply}}
                        </span>
                    </p>
                </div>
            </div>
        </div>
        <div class="receive" v-if="!exchange && checkedin">
            <p>
                <span>
                    Liquidity token receive
                </span>
                <span>
                    {{to_buy}}
                </span>
            </p>
        </div>
        <div class="dialog" v-show="showDialog">
            <chooseMarket title="dialog" v-on:closeDialogFn="closeDialogFn"></chooseMarket>
        </div>
    </div>
</template>
<script>
    import chooseMarket from '@/components/chooseMarket'
    import { asset, number_to_asset } from 'eos-common'
    export default {
        data() {
            return {
                myNewTokenAmount: 0,
                oriPool: '',
                amount: '', //receive数量
                to_buy: '',
                amount1: 0,
                amount2: 0,
                token1: 0,
                token2: 0,
                exchange: true,
                showDialog: false,
                pay: [
                    {
                        size: '39323.321 AOS',
                        img:'',
                        name: 'AOS',
                        addr: 'eosio.token'
                    },{
                        size: '1234567 USDT',
                        img:'',
                        name: 'USDT',
                        addr: 'USDT.token'
                    }
                ],
                checkedin: true, //默认Provide
                pool: '',
                poolFlag: true,
                currentTokenx: '', //默认从route里取
                poolPrice: '',
                tokenxlists: '', //交易对列表
                tokenxImg: '', //交易图片
            }
        },
        components: {
            chooseMarket
        },
        watch: {
            amount1(newVal, oldVal) {
                this.updateAmounts()
            }
        },
        computed: {
            baseReceive() {
                if (!this.amount) {
                    return number_to_asset(0, asset(this.pool.supply).symbol)
                } else {
                    let amount = this.inputToAsset(this.amount, asset(this.pool.supply).symbol.precision()).amount
                    amount = this.computeForward(amount.multiply(-1), this.pool1.amount, asset(this.pool.supply).amount, 0).abs()
                    return asset(amount, this.pool1.symbol).to_string()
                }

            },
            quoteReceive() {
                if (!this.amount) {
                    return number_to_asset(0, asset(this.pool.supply).symbol)
                } else {
                    let amount = this.inputToAsset(this.amount, asset(this.pool.supply).symbol.precision()).amount
                    amount = this.computeForward(amount.multiply(-1), this.pool2.amount, asset(this.pool.supply).amount, 0).abs()
                    return asset(amount, this.pool2.symbol).to_string()
                }
            },
            tokenReceive() {
                const token1 = this.inputToAsset(this.token1, this.pool1.symbol.precision())

                const to_buy = this.computeBackward(
                    token1.amount,
                    asset(this.pool.supply).amount,
                    this.pool1.amount,
                    this.pool.fee
                )
                
                return to_buy
            },
            poolOne() {
                return this.poolFlag ? this.pool1 : this.pool2
            },

            poolTwo() {
                return this.poolFlag ? this.pool2 : this.pool1
            },
            price() {
                const price = this.calcPrice(
                    this.inputToAsset(this.amount1, this.poolOne.symbol.precision()),
                    this.inputToAsset(this.amount2, this.poolTwo.symbol.precision())
                )

                return price || this.calcPrice(this.poolOne, this.poolTwo).toFixed(8)
            },
        },
        methods: {
            amountChangeRm() {
                try {
                    this.amount = (parseFloat(this.amount) || 0).toFixed(asset(this.pool.supply).symbol.precision())
                } catch(err) {
                    this.amount = '';
                    this.amountChangeRm()
                }
            },
            remliquidity() {
                let actions = [
                    {
                        account: this.$aoswapAccount,
                        name: 'openext',
                        authorization: [
                            {
                                actor: this.eosAccount,
                                permission: 'active'
                            }
                        ],
                        data: {
                            user: this.eosAccount,
                            payer: this.eosAccount,
                            token_contract: this.pool.pool1.contract,
                            token_sym: this.pool1.symbol.toString()
                        }
                    },{
                        account: this.$aoswapAccount,
                        name: 'openext',
                        authorization: [
                            {
                                actor: this.eosAccount,
                                permission: 'active'
                            }
                        ],
                        data: {
                            user: this.eosAccount,
                            payer: this.eosAccount,
                            token_contract: this.pool.pool2.contract,
                            token_sym: this.pool2.symbol.toString()
                        }
                    },{
                        account: this.$aoswapAccount,
                        name: 'remliquidity',
                        authorization: [
                            {
                                actor: this.eosAccount,
                                permission: 'active'
                            }
                        ],
                        data: {
                            user: this.eosAccount,
                            to_sell: this.amount + ' ' + asset(this.pool.supply).symbol.code().to_string(),
                            min_asset1: this.baseReceive,
                            min_asset2: this.quoteReceive
                        }
                    },{
                        account: this.$aoswapAccount,
                        name: 'closeext',
                        authorization: [{
                            actor: this.eosAccount,
                            permission: 'active'
                        }],
                        data: {
                            user: this.eosAccount,
                            to: this.eosAccount,
                            token_contract: this.pool.pool2.contract,
                            token_sym: this.pool2.symbol.toString(),
                            memo: ''
                        }
                    },{
                        account: this.$aoswapAccount,
                        name: 'closeext',
                        authorization: [{
                            actor: this.eosAccount,
                            permission: 'active'
                        }],
                        data: {
                            user: this.eosAccount,
                            to: this.eosAccount,                
                            token_contract: this.pool.pool1.contract,
                            token_sym: this.pool1.symbol.toString(),
                            memo: ''
                        }
                    }
                ]
                this.eosUrl.transaction({actions}).then(
                    res => {
                        this.getPoolInfo()
                        this.Toast({
                            duration: 2500,
                            message: 'success!',
                        });
                    }
                ).catch(
                    err => {
                        this.Toast({
                            duration: 2500,
                            message: JSON.parse(err).error.details[0].message,
                        });
                        this.getPoolInfo()
                    }
                )
            },
            inputToken1(value) {
                let token1 = (parseFloat(this.token1) || 0).toFixed(this.pool1.symbol.precision())
                token1 = asset(token1 + ' ' + this.pool1.symbol.code().to_string()).amount
                
                const to_buy = this.computeBackward(
                    token1,
                    asset(this.pool.supply).amount,
                    this.pool1.amount,
                    this.pool.fee
                )

                const token2 = number_to_asset(0, this.pool2.symbol)

                token2.set_amount(
                this.computeForward(to_buy, this.pool2.amount, asset(this.pool.supply).amount, this.pool.fee)
                )
                this.to_buy = asset(to_buy, asset(this.pool.supply).symbol).to_string()
                this.token2 = token2.to_string().split(' ')[0]
            },
            inputToken2(value) {
                let token2 = (parseFloat(this.token2) || 0).toFixed(this.pool2.symbol.precision())

                token2 = asset(token2 + ' ' + this.pool2.symbol.code().to_string()).amount

                const to_buy = this.computeBackward(
                    token2,
                    asset(this.pool.supply).amount,
                    this.pool2.amount,
                    this.pool.fee
                )

                const token1 = number_to_asset(0, this.pool1.symbol)

                token1.set_amount(
                    this.computeForward(to_buy.multiply(-1), this.pool1.amount, asset(this.pool.supply).amount, this.pool.fee).multiply(-1)
                )
                this.to_buy = asset(to_buy, asset(this.pool.supply).symbol).to_string()
                console.log('this.to_buy', this.to_buy)
                this.token1 = token1.to_string().split(' ')[0]
            },
            computeBackward(x, y, z, fee) {
                const fee_amount = x.multiply(fee).plus(9999).divide(10000)
                x = x.minus(fee_amount)
                x = x.multiply(y).divide(z)

                return x
            },
            addliquidityFn() {
                const to_buy = asset(this.tokenReceive, asset(this.pool.supply).symbol).to_string()
                const token1 = asset(`${this.token1} ${this.pool1.symbol.code().to_string()}`).to_string()
                const token2 = asset(`${this.token2} ${this.pool2.symbol.code().to_string()}`).to_string()
                //Provide
                let data =  {
                    user: this.eosAccount,
                    payer: this.eosAccount,
                    token_contract: this.pool.pool1.contract,
                    token_sym: this.pool1.symbol.toString()
                }
                let actions = 
                [
                    {
                        account: this.$aoswapAccount,
                        name: 'openext',
                        authorization: [
                            {
                                actor: this.eosAccount, 
                                permission: 'active'
                            }
                        ],
                        data: {
                            user: this.eosAccount,
                            payer: this.eosAccount,
                            token_contract: this.pool.pool1.contract,
                            token_sym: this.pool1.symbol.toString()
                        }
                    },
                    {
                        account: this.$aoswapAccount,
                        name: 'openext',
                        authorization: [
                            {
                                actor: this.eosAccount,
                                permission: 'active'
                            }
                        ],
                        data: {
                            user: this.eosAccount,
                            payer: this.eosAccount,
                            token_contract: this.pool.pool2.contract,
                            token_sym: this.pool2.symbol.toString()
                        }
                    }
                    ,{
                        account: this.pool.pool1.contract,
                        name: this.pool.pool1.contract == 'ciphertokens' ? 'transferpp' : 'transfer',
                        authorization: [
                            {
                                actor: this.eosAccount,
                                permission: 'active'
                            }
                        ],
                        data: {
                            from: this.eosAccount,
                            to: this.$aoswapAccount,
                            quantity: this.token1+' '+this.pool.pool1.quantity.split(' ')[1],
                            memo: ''
                        }
                    },
                    {
                        account: this.pool.pool2.contract,
                        name: this.pool.pool2.contract == 'ciphertokens' ? 'transferpp' : 'transfer',
                        authorization: [
                            {
                                actor: this.eosAccount,
                                permission: 'active'
                            }
                        ],
                        data: {
                            from: this.eosAccount,
                            to: this.$aoswapAccount,
                            quantity: this.token2+' '+this.pool.pool2.quantity.split(' ')[1],
                            memo: ''
                        }
                    },
                    {
                        account: this.$aoswapAccount,
                        name: 'addliquidity',
                        authorization: [
                            {
                                actor: this.eosAccount,
                                permission: 'active'
                            }
                        ],
                        data: {
                            user: this.eosAccount,
                            to_buy,
                            max_asset1: this.token1+' '+this.pool.pool1.quantity.split(' ')[1],
                            max_asset2: this.token2+' '+this.pool.pool2.quantity.split(' ')[1],            
                        }
                    },
                    {
                        account: this.$aoswapAccount,
                        name: 'closeext',
                        authorization: [
                            {
                                actor: this.eosAccount,
                                permission: 'active'
                            }
                        ],
                        data: {
                            user: this.eosAccount,
                            to: this.eosAccount,
                            token_contract: this.pool.pool2.contract,
                            token_sym: this.pool2.symbol.toString(),
                            memo: ''
                        }
                    },{
                        account: this.$aoswapAccount,
                        name: 'closeext',
                        authorization: [
                            {
                                actor: this.eosAccount,
                                permission: 'active'
                            }
                        ],
                        data: {
                            user: this.eosAccount,
                            to: this.eosAccount,                
                            token_contract: this.pool.pool1.contract,
                            token_sym: this.pool1.symbol.toString(),
                            memo: ''
                        }
                    }
                ]

                this.eosUrl.transaction({actions}).then(
                    res => {
                        this.getPoolInfo()
                        this.Toast({
                            duration: 2500,
                            message: 'success!',
                        });
                    }
                ).catch(
                    err => {
                        this.getPoolInfo()
                        this.Toast({
                            duration: 2500,
                            message: JSON.parse(err).error.details[0].message,
                        });
                    }
                )
            },
            transactionFn() {
                //交易
                return new Promise((resolve, reject)=>{
                    let account = this.oriPool.pool1.contract
                    let quantity1 = Number(this.amount1).toFixed(this.oriPool.pool1.quantity.split(' ')[0].split('.')[1].length) + ' ' + 
                    this.oriPool.pool1.quantity.split(' ')[1];
                    let quantity2 = Number(this.amount2).toFixed(this.oriPool.pool2.quantity.split(' ')[0].split('.')[1].length) + ' ' + 
                    this.oriPool.pool2.quantity.split(' ')[1];
                    this.eosUrl.transaction({
                        actions: [{
                            account: account,
                            name: account == 'ciphertokens' ? 'transferpp' : 'transfer',
                            authorization: [{
                                actor: this.eosAccount,
                                permission: 'active',
                            }],
                            data: {
                                from: this.eosAccount,
                                to: this.$aoswapAccount,
                                quantity: quantity1,
                                memo: `exchange:${this.oriPool.supply.split(' ')[1]},${quantity2}`
                                // "exchange:EOSOCT,8.0000 OCT" // 格式是：exchange:tokenx,token2的数量
                            },
                        }]
                    }).then(
                        (res) => {
                            resolve(res)
                        }
                    ).catch(
                        (err) => {
                            reject(err)
                        }
                    )
                });
            },
            async swapFn() {
                try {
                    await this.transactionFn()
                    //成功
                    this.getPoolInfo();
                    this.Toast({
                        duration: 2500,
                        message: 'success!',
                    });
                }catch(err){
                    this.getPoolInfo();
                    this.Toast({
                        duration: 2500,
                        message: JSON.parse(err).error.details[0].message,
                    });
                }
            },

            calcPrice(a, b) {
                const diff_precision = a.symbol.precision() - b.symbol.precision()
                return (a.amount / b.amount) / (10 ** diff_precision)
            },
            
            inputToAsset(input, precision) {
                return asset((parseFloat(input) || 0).toFixed(precision) + ' XXX')
            },
            amountChange() {
                if (!this.amount1) {
                    this.amount1 = 0.0
                }
                this.amount1 = Number(this.amount1).toFixed(this.pool1.symbol.precision())
                this.amount2 = Number(this.amount2).toFixed(this.pool2.symbol.precision())

                this.token1 = Number(this.token1).toFixed(this.pool1.symbol.precision())
                this.token2 = Number(this.token2).toFixed(this.pool2.symbol.precision())
            },
            getLists() {
                /**
                 * @msg:  receive交易对详情
                 * @param {type} 
                 * @return {type} 
                 */   
                const val = {
                    'table': 'globals',
                    'scope': this.$aoswapAccount,
                    'code': this.$aoswapAccount,
                    'json': true,
                    'limit': -1,
                    'lower_bound': '',
                    'upper_bound': '',
                    'table_key': ''
                }
                this.$jq.ajax({
                    type: 'POST',
                    dataType: 'JSON',
                    data: JSON.stringify(val),
                    url: this.$url+ '/v1/chain/get_table_rows',
                    success: (res) => {
                        this.tokenxlists = res.rows[0].pairs;
                        this.tokenxImg = this.tokenxlists.find(
                            (item) => {
                                return item.tokenx == this.pool.supply.split(' ')[1]
                            }
                        );
                        let tokenx = this.oriPool.supply.split(' ')[1];
                    },
                    error: (err) => {
                        console.log('err',err);
                    }
                })
            },
            getPoolInfo() {
                const val = {
                    'code': this.$aoswapAccount,  // aoswap合约名
                    'json': true,
                    'limit': -1,
                    'lower_bound': '',
                    'scope': this.$route.query.tokenx,          // tokenx名称
                    'table': 'stat',            
                    'table_key': '',
                    'upper_bound': ''
                }
                this.$jq.ajax({
                    type: 'POST',
                    dataType: 'JSON',
                    data: JSON.stringify(val),
                    url: this.$url+ '/v1/chain/get_table_rows',
                    success: (res) => {
                        this.pool = res.rows[0];
                        this.oriPool = Object.assign({}, res.rows[0]);
                        let tokenAPrice = this.pool.pool1.quantity.split(' ')[0];
                        let tokenBPrice = this.pool.pool2.quantity.split(' ')[0];
                        let resVal = (tokenAPrice / tokenBPrice).toFixed(10).toString();
                        this.poolPrice = resVal.split('.')[0] + '.' + resVal.split('.')[1].slice(0,4);
                        this.pool1 = asset(this.pool.pool1.quantity);
                        this.pool2 = asset(this.pool.pool2.quantity);
                        this.getTokenAmount();
                        this.getLists();
                    },
                    error: (err) => {
                    }
                })
            },
            getTokenAmount() {
                // my new Token 
                const val = {
                    'code': this.$aoswapAccount,  // aoswap合约名
                    'json': true,
                    'limit': -1,
                    'lower_bound': '',
                    'scope': this.$eosAccount,          // 用户账号
                    'table': 'accounts',            
                    'table_key': '',
                    'upper_bound': ''
                }
                this.$jq.ajax({
                    type: 'POST',
                    dataType: 'JSON',
                    data: JSON.stringify(val),
                    url: this.$url+ '/v1/chain/get_table_rows',
                    success: (res) => {
                        let result = res.rows;
                        let tokenx = this.$route.query.tokenx;
                        result.forEach(ele => {
                            if (ele.balance.indexOf(tokenx) !== -1) {
                                // 存在
                                this.myNewTokenAmount = ele.balance.split(' ')[0];
                            }
                        });
                        this.myNewTokenAmount = this.myNewTokenAmount ? this.myNewTokenAmount : 0;
                    },
                    error: (err) => {
                    }
                })
            },
            computeForward(x, y, z, fee) {
                const prod = x.multiply(y)
                let tmp, tmp_fee

                if (x > 0) {
                    tmp = prod.minus(1).divide(z).plus(1)
                    tmp_fee = tmp.multiply(fee).plus(9999).divide(10000)
                } else {
                    tmp = prod.divide(z)
                    tmp_fee = tmp.multiply(-1).multiply(fee).plus(9999).divide(10000)
                }
                return tmp.plus(tmp_fee)
            },
            updateAmounts() {
                if (isNaN(this.amount1)) return

                let a = (parseFloat(this.amount1) || 0).toFixed(this.poolOne.symbol.precision())
                a = asset(a + ' ' + this.pool1.symbol.code().to_string()).amount
                const p1 = this.poolOne.amount
                const p2 = this.poolTwo.amount

                if (this.poolFlag) {
                    const r = number_to_asset(0, this.poolTwo.symbol)
                    r.set_amount(Math.abs(this.computeForward(a.multiply(-1), p2, p1.plus(a), this.pool.fee)))
                    this.amount2 = r.to_string().split(' ')[0];
                } else {
                    const result = this.computeForward(
                        a.multiply(-1),
                        this.pool1.amount,
                        this.pool2.amount + a,
                        this.pool.fee
                    ).abs()
                
                    const r = number_to_asset(0, this.pool1.symbol)
                    r.set_amount(result)
                    this.amount2 = r.to_string().split(' ')[0]
                }
            },
            closeDialogFn() {
                console.log(this.$route.query);
                this.getPoolInfo()
                this.showDialog = false;
            },
            showList() {
                /**
                 * @msg: 展示dialog
                 * @param {type} 
                 * @return {type} 
                 */    
                this.showDialog = true;
            },
            changePayFn() {
                /**
                 * @msg: 互换sendreceive的值
                 * @param {type} 
                 * @return {type} 
                 */   
                this.poolFlag = !this.poolFlag;
                this.amount1 = this.amount2 = 0;
                this.amountChange()
                this.updateAmounts()
                let temp = {};
                temp = this.oriPool.pool2;
                this.oriPool.pool2 = this.oriPool.pool1;
                this.oriPool.pool1 = temp;
            }
        },
        mounted() {
            this.getPoolInfo()
        }
    }
</script>
<style lang="less" scoped>
    @import '../assets/style/iconfont/iconfont.css';
    .con {
        .logo {
            // width: 1rem;
            // height: 0.28rem;
            width: 1.56rem;
            height: 1.34rem;
            margin: 0 0.2rem 0.2rem;
        }
    }
    .exchange {
        border-radius: 0.12rem;
        background-color: #fff;
        width: calc(~"100% - 40px");
        margin: 0 auto;
        padding: 0 0.2rem 0.3rem;
        font-size: 0.16rem;
        .btn {
            background: #4D5CFF;
            border-radius: 12px;
            height: 0.48rem;
            line-height: 0.48rem;
            color: #fff;
            margin-top: 0.1rem;
            margin-bottom: 0.1rem;
        }
        .pool {
            font-size: 0.14rem;
            color: #000000;
            overflow: hidden;
            margin-top: 0.1rem;
            span:first-child{
                float: left;
            }
            span:last-child{
                float: right;
            }
        }
        .tab {
          .checked {
                font-size: 0.16rem;
                color: #000000;
                height: 0.6rem;
                line-height: 0.6rem;
                position: relative;
            }
            .lefttab {
                cursor: pointer;
                padding-right: 0.25rem;
            }
            .righttab {
                cursor: pointer;
                padding-right: 0.25rem;
            }
            .checked:after{
                content: '';
                width: 30px;
                background-color: #000;
                height: 1px;
                position: absolute;
                left: 0.01rem;
                bottom: -0.1rem;
            }
            .unchecked {
                font-size: 0.16rem;
                color: #ACACBD;
            }  
        }
        .pay {
            border: 1px solid #EBEBEB;
            border-radius: 0.08rem;
            height: 0.72rem;
            padding: 0.1rem;
            .pay-tit {
                font-size: 0.13rem;
                overflow: hidden;
                color: #ACACBD;
            }
            .pay-tit span:first-child{
                float: left;
            }
            .pay-tit span:last-child{
                float: right;
            }
            .pay-money {
                position: relative;
                height: 0.34rem;
                .amount2 {
                    float: right;
                    width: 0.6rem;
                    text-align: left;
                    height: 0.34rem;
                    line-height: 0.34rem;
                    font-size: 0.13rem;
                }
                img {
                    width: 0.3rem;
                    height: 0.3rem;
                    display: inline-block;
                    float: left;
                }
                .pay-name {
                    font-size: 0.14rem;
                    color: #000000;
                    position: absolute;
                    top: 0;
                    left: 0.34rem;
                }
                .pay-addr {
                    font-size: 0.13rem;
                    color: #ACACBD;
                    position: absolute;
                    left: 0.34rem;
                    bottom: 0;
                }
                input {
                    height: 0.3rem;
                    width: 0.6rem;
                    float: right;
                }
            }
        }
        .exchage-tab {
            .change-img {
                width: 0.19rem;
                height: 0.16rem;
                margin: 0.04rem auto;
                cursor: pointer;
            }
            .price {
                height: 0.3rem;
                font-size: 0.14rem;
                line-height: 0.3rem;
                .tit {
                    color: #ACACBD;
                    float: left;
                }
                .rate {
                    float: right;
                    img {
                        display: inline-block;
                        width: 0.16rem;
                        height: 0.14rem;
                        vertical-align: middle;
                        margin-top: -0.02rem;
                    }
                }
            }
        }
        .market {
            .checkedin {
                border-right: 1px solid #EBEBEB;
                border-radius: 0.08rem;
            }
            .checkedout {
                border-left: 1px solid #EBEBEB;
                border-radius: 0.08rem;
            }
            .market-tab {
                width: 100%;
                display: flex;
                border-top: 1px solid #EBEBEB;
                border-bottom: 1px solid #EBEBEB;
                height: 0.38rem;
                line-height: 0.34rem;
                margin-bottom: 0.12rem;
                .checked-tab-in {
                    border: 1px solid #4D5CFF;
                    border-radius: 8px;
                    color: #4D5CFF;
                    flex: 1;
                }
                .checked-tab-out {
                    flex: 1;
                }
            }
            .add {
                margin: 0.05rem 0;
                font-weight: 900;
                color: #9EA6BB;
            }
            .key-val {
                height: 0.62rem;
                border: 1px solid #EBEBEB;
                border-radius: 8px;
                padding: 0.15rem 0.2rem;
                text-align: left;
                position: relative;
                .arrow {
                   line-height: 0.62rem;
                   position: absolute;
                   right: 0.2rem;
                   top: 50%;
                   transform: translate(0, -50%);
                }
                p:first-child {
                    font-size: 0.14rem;
                    color: #000000;
                    line-height: 0.14rem;
                }
                p:nth-child(2) {
                    font-size: 0.13rem;
                    color: #ACACBD;
                }
            }
            .idcard {
                height: 0.62rem;
                border: 1px solid #EBEBEB;
                border-radius: 8px;
                margin-top: 0.12rem;
                margin-bottom: 0.18rem;
                padding: 0.1rem 0.14rem;
                .amount {
                    float: left;
                }
                p:first-child {
                    overflow: hidden;
                    span:first-child {
                        font-size: 0.13rem;
                        color: #ACACBD;
                        float: left;
                    }
                    span:nth-child(2) {
                        color: #000000;
                        font-size: 0.13rem;
                        float: right;
                    }
                }
                p:nth-child(2) {
                    font-size: 0.24rem;
                    color: #ACACBD;
                    text-align: left;
                }
            }
            .get-val {
                font-size: 0.14rem;
                overflow: hidden;
                margin-bottom: 0.24rem;
                span:first-child {
                    color: #ACACBD;
                    float: left;
                }
                span:last-child {
                    color: #000;
                    float: right;
                }
            }
            .btn {
                background: #4D5CFF;
                border-radius: 12px;
                color: #fff;
                margin-bottom: 0.15rem;
            }
            .pool-det {
                font-size: 0.14rem;
                overflow: hidden;
                span:first-child {
                    float: left;
                }
                span:last-child {
                    float: right;
                }
            }
        }
        
    }
    .dialog {
        position: fixed;
        top: 0;
        bottom: 0;
        left: 0;
        right: 0;
        z-index: 99;
        background-color: rgba(0, 0, 0, 0.5);
        padding: 1rem 0.2rem;
    }
    .receive {
        background: #EBEDFF;
        border-radius: 0 0 0.12rem 0.12rem;
        height: 0.56rem;
        width: calc(~"100% - 60px");
        font-size: 0.14rem;
        margin: 0 auto;
        line-height: 0.56rem;
        padding: 0 0.2rem;
        p{
            overflow: hidden;
            span:first-child {
                color: #ACACBD;
                float: left;
            }
            span:last-child {
                color: #000;
                float: right;
            }
        }
    }
    .receive-out {
        height: 0.84rem;
        padding: 0.2rem;
        line-height: 0.24rem;
    }
</style>