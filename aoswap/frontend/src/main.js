/*
 * @Descripttion: 
 * @Author: v_aoswap@users.noreply.github.com
 * @Date: 2020-09-01 10:35:10
 */
// The Vue build version to load with the `import` command
// (runtime-only or standalone) has been set in webpack.base.conf with an alias.
import Vue from 'vue'
import App from './App'
import router from './router'
import Vuex from 'vuex'
// import store from './store'
import Eos from 'eosjs';
import ScatterJs from 'scatterjs-core';
import ScatterEos from 'scatterjs-plugin-eosjs';


Vue.use(Vuex)
Vue.config.productionTip = false

import { 
  Toast, 
} from 'mint-ui'

import jq from 'jquery'
Vue.prototype.$jq = jq;
/* 正式环境
*/
Vue.prototype.$url = 'http://api.aos.plus:8888';

Vue.prototype.$aoswapAccount = 'defiaoswapss';
Vue.prototype.Toast = Toast

ScatterJS.plugins( new ScatterEOS() );

//正式版*/
const network = {
  port:8888,
  protocol:'http',
  blockchain:'eos',
  host:'api.aos.plus',
  chainId:'907345e081e731497946845186a03a50030c6c9ee14bacfcb1922feae873f31b',
}


ScatterJS.scatter.connect('AOSWAP').then(connected => {
    if(!connected) { return false };
    const scatter = ScatterJS.scatter;
    Vue.prototype.eosUrl = scatter.eos(network, Eos, { expireInSeconds:60 },'http'); // 正式
    scatter.getIdentity({ accounts:[network] }).then(() => {
      Vue.prototype.eosAccount = scatter.identity.accounts.find(x => x.blockchain === 'eos').name;
    })
})

import '@/assets/style/media.css'
import '@/assets/style/reset.css'


/* eslint-disable no-new */
new Vue({
  el: '#app',
  router,
  components: { App },
  template: '<App/>'
})
