<script setup>
  import Header from './components/Header.vue';
  import Menu from './components/Menu.vue';
</script>

<template>
  <div class="common-layout">
    <el-container class="main-view">
      <el-header><Header /></el-header>
      <el-container>
        <el-aside width="200px"><Menu /></el-aside>
        <el-main>Main</el-main>
      </el-container>
    </el-container>
  </div>
</template>

<style scoped>
  #app {
    width: 100%;
    height: 100%;
  }
  .common-layout {
    width: 100%;
    height: 100vh;
  }
  .Header{
    text-align: left;
  }
  .main-view{
    height: 100%;
  }

</style>
