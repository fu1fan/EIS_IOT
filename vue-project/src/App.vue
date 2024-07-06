<script setup>

import Header from './components/Header.vue';
import Menu from './components/Menu.vue';
import Overview from './components/Overview.vue';
import EISMeasure from './components/EISMeasure.vue';
import RMeasure from './components/RMeasure.vue';
import BalanceTest from './components/BalanceTest.vue';
import PowerTest from './components/PowerTest.vue';
import History from './components/History.vue';
import Setting from './components/Setting.vue';
import Thermorunaway from './components/Thermorunaway.vue';
import MechanicalDamage from './components/MechanicalDamage.vue';

import { reactive, watch, ref, shallowRef } from 'vue'
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

let view_index = "1"

const views = {
  "1": Overview,
  "2-1": EISMeasure,
  "2-2": RMeasure,
  "2-3": BalanceTest,
  "2-4": PowerTest,
  "2-5": Thermorunaway,
  "2-6": MechanicalDamage,
  "3": History,
  "4": Setting,
}

var cur_view = shallowRef(Overview)

const menu_callback = (index) => {
  console.log(index)
  cur_view.value = views[index]
}
</script>

<template>
  <el-watermark :font="font" :content="['2024·共创芯未来', 'by Loong的传人']">
      <div class="common-layout">
        <el-container class="main-view">
          <el-header>
            <Header />
          </el-header>
          <el-container>
            <el-aside width="auto" height="100%">
              <Menu :select_callback="menu_callback" isOnline/>
            </el-aside>
            <el-main>
              <component :is="cur_view"></component> 
            </el-main>
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

.Header {
  text-align: left;
}

.main-view {
  height: 100%;
  width: 100%;
}

.el-main {
  padding: 25px;
}
</style>
