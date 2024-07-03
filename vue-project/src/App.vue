<script setup>
  import Header from './components/Header.vue';
  import Menu from './components/Menu.vue';
  import Overview from './components/Overview.vue';

  import { reactive, watch, ref } from 'vue'
  // import { isDark } from '~/composables/dark'
  const isDark = ref(false)

  const font = reactive({
    color: 'rgba(0, 0, 0, .15)',
  })

  watch(
    isDark,
    () => {
      font.color = isDark.value
        ? 'rgba(255, 255, 255, .05)'
        : 'rgba(0, 0, 0, .05)'
    },
    {
      immediate: true,
    }
)
</script>

<template>
  <el-watermark :font="font" :content="['2024·共创芯未来', 'by Loong的传人']">
  <div class="common-layout">
    <el-container class="main-view">
      <el-header><Header /></el-header>
      <el-container>
        <el-aside width="200px"><Menu /></el-aside>
        <el-main><Overview /></el-main>
      </el-container>
    </el-container>
  </div>
  </el-watermark>
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
    width: 100%;
  }
  .el-main{
    padding: 25px;
  }

</style>
