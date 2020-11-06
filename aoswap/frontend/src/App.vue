<!--
 * @Descripttion: 
 * @Author: v_aoswap@users.noreply.github.com
 * @Date: 2020-09-01 10:35:10
-->
<template>
  <div id="app">
    <router-view/>
  </div>
</template>

<script>
export default {
  name: 'App'
}
</script>

<style>
#app {
  font-family: PingFangSC-Medium;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  background-color: #4D5CFF;
  height: 100%;
  padding-top: 0.2rem;
  padding-bottom: 0.2rem;
  overflow-y: hidden;
}
</style>
