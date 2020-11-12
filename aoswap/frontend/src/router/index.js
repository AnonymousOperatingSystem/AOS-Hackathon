/*
 * @Descripttion: 
 * @Author: v_aoswap@users.noreply.github.com
 * @Date: 2020-09-01 10:35:10
 */
import Vue from 'vue'
import Router from 'vue-router'
import CreateMarket from '@/components/createMarket'
import Exchange from '@/components/exchange'
import ChooseMarket from '@/components/chooseMarket'

Vue.use(Router)

export default new Router({
  routes: [
    {
      path: '/',
      name: 'chooseMarket',
      component: ChooseMarket
    },
    {
      path: '/createMarket',
      name: 'createMarket',
      component: CreateMarket
    },{
      path: '/exchange',
      name: 'exchange',
      component: Exchange
    }
  ]
})
