<!--
 * @Descripttion: 
 * @Author: v_aoswap@users.noreply.github.com
 * @Date: 2020-09-01 10:40:31
-->
<template>
    <div class="choose-mk">
        <div class="top">
            <span class="tit">
                select pair
            </span>
            <span class="iconfont icon-close close" v-show="title" @click="closeFn">
            </span>
            <span class="btn" @click="toCreate">
                create pair
            </span>
        </div>
        <input type="text" placeholder="search pair" class="search" v-model="keyword" @input="searchFn">
        <ul class="list" id="list">
            <li @click="toExchange(item.tokenx)" v-for="item in lists" :key="item.index" :style="{'padding-left': paddVal + 'px','padding-right': paddVal + 'px'}">
                <div class="item">
                    <img :src="item.token1.logo" alt="logor" v-if="item.token1.logo.match(/http/)">
                    <img src="../../static/image/null.png" alt="logo" v-else>
                    <div>
                        <p class="name">
                            {{item.token1.symbol}}
                        </p>
                        <p class="addr">
                            {{item.token1.contract}}
                        </p>
                    </div>
                </div>
                <div class="item">
                    <img :src="item.token2.logo" alt="logo" v-if="item.token2.logo.match(/http/)">
                    <img src="../../static/image/null.png" alt="logo" v-else>
                    <div>
                        <p class="name">
                            {{item.token2.symbol}}
                        </p>
                        <p class="addr">
                            {{item.token2.contract}}
                        </p>
                    </div>
                </div>
            </li>
        </ul>
    </div>
</template>
<script>
    export default {
        props: ['title'],
        data() {
            return {
                paddVal: '20',
                lists: [],
                allLists: [],
                keyword: '',
                inter: '',
            }
        },
        mounted() {
            document.getElementById('list').style.height = document.body.clientHeight - 150 + 'px';
            if( this.title ) {
                this.paddVal = 10;
                document.getElementById('list').style.height = document.body.clientHeight - 350 + 'px';
            }else{
                document.getElementById('list').style.height = document.body.clientHeight - 150 + 'px';
            }
            this.getLists();
        },
    
        methods: {
            searchFn() {
                /**
                 * @msg: 搜索接口
                 * @param {keyword} 
                 * @return  
                 */    
                if(!this.keyword.length) {
                    this.lists = this.allLists;
                }else{
                    this.lists = this.allLists.filter((item)=>{
                        let reg = new RegExp(`${this.keyword}`);
                        return item.token1.symbol.match(reg) || item.token2.symbol.match(reg)
                    })
                }
            },
            closeFn() {
                /**
                 * @msg: 关闭按钮
                 * @param {type} 
                 * @return {type} 
                 */   
                this.$emit('closeDialogFn') 
            },
            toCreate() {
                /**
                 * @msg: 跳转路由
                 * @param {type} 
                 * @return {type} 
                 */
                this.$router.push({name: 'createMarket'})
            },
            toExchange(tokenx) {
                /**
                 * @msg: 跳转到交易页面
                 * @param {type} 
                 * @return {type} 
                 */    
                if(this.title){
                    this.$route.query.tokenx = tokenx;
                    this.$emit('closeDialogFn') 
                }else{
                    this.$router.push({
                        name: 'exchange',
                        query: {
                            tokenx: tokenx
                        }
                    })
                }
                
            },
            getLists() {
                /**
                 * @msg:  receive列表详情
                 * @param {type} 
                 * @return {type} 
                 */   
                console.log('this.$aoswapAccount', this.$aoswapAccount)
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
                        this.allLists = res.rows[0].pairs;
                        this.lists = res.rows[0].pairs;
                    },
                    error: (err) => {
                        console.log('err',err);
                    }
                })
            }
        }
    }
</script>
<style lang="less" scoped>
    @import '../assets/style/iconfont/iconfont.css';
    .choose-mk {
        border-radius: 0.12rem;
        background-color: #fff;
        width: calc(~"100% - 40px");
        margin: 0 auto;
        padding: 0.2rem 0;
        font-size: 0.16rem;
        .top {
            padding: 0 0.2rem;
            overflow: hidden;
            font-size: 0.12rem;
            line-height: 0.24rem;
            .tit {
                font-size: 0.14rem;
                color: #000;
                float: left;
            }
            .close {
                float: right;
                display: inline-block;
                padding-left: 0.1rem;
            }
            .btn {
                background: #4D5CFF;
                border-radius: 4px;
                color: #fff;
                font-size: 0.13rem;
                width: 0.78rem;
                display: inline-block;
                float: right;
                height: 0.24rem;
                line-height: 0.24rem;
            }
        }
        .search {
            margin: 0.14rem 0.2rem 0;
            display: inline-block;
            height: 0.38rem;
            background: #F0F2F5;
            border-radius: 8px;
            font-size: 0.14rem;
            width: calc(~"100% - 0.4rem");
            padding-left: 0.1rem;
        }
        .list {
            overflow-y: scroll;
            // height: 700px;
            height: calc(~"100% - 1rem");
            padding: 0 0.2rem 0.5rem;
            li {
                padding: 0.1rem 0.2rem;
                display: flex;
                background: #FFFFFF;
                border: 1px solid #EBEBEB;
                border-radius: 8px;
                margin-top: 0.12rem;
                justify-content: space-between;
                .item {
                    text-align: left;
                    display: flex;
                    align-items: center;
                    img {
                        width: 0.3rem;
                        height: 0.3rem;
                        display: inline-block;
                        vertical-align: middle;
                        margin-right: 0.05rem;
                    }
                    .name {
                        font-size: 0.14rem;
                        color: #000000;
                        line-height: 0.14rem;
                    }
                    .addr {
                        font-size: 0.13rem;
                        color: #ACACBD;
                        line-height: 0.13rem;
                    }
                }
            }
        }
    }
</style>
