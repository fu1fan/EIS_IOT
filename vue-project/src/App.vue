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

import { reactive, watch, ref, shallowRef, onMounted, onUnmounted } from 'vue'
import { ElMessageBox } from 'element-plus';
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

let states = ref({
  text: "未知",
  battery_count: "",
  ohmages:  [],
  omages_mean: [],
  voltages_cur: [],
  voltages_his: [],
  voltages_mean: 0,
  voltages_total: 0,
  last_update: "--:--:--"
})

onMounted(() => {
  window.alert = ElMessageBox.alert
  setInterval(() => {
    fetch('/api/c/is_online')
      .then(response => response.json())
      .then(data => {
        if (data.data == true) {
          fetch('/api/c/get_state')
            .then(response => response.json())
            .then(data => {
              if (data.status == "success") {
                states.value.text = data.data.state
                states.value.battery_count = data.data.battery_count
                states.value.ohmages = data.data.ohmages
                states.value.ohmages_mean = data.data.ohmages_mean.toFixed(2)
                states.value.voltages_cur = data.data.voltages_cur
                states.value.voltages_his = data.data.voltages_his
                states.value.voltage_mean = data.data.voltage_mean.toFixed(4)
                states.value.voltage_total = data.data.voltage_total
                states.value.last_update = data.data.last_update != 0 ? new Date(data.data.last_update * 1000).toLocaleTimeString() : "--:--:--";
              }
              // 遍历states.value.ohmages，把值为0的数据改为>50
              for (let i = 0; i < states.value.ohmages.length; i++) {
                if (states.value.ohmages[i] == 0) {
                  states.value.ohmages[i] = ">50";
                }
              }
            })
            .catch(error => {
              console.error('Error:', error);
            });
        } else {
          states.value.text = "离线"
        }
      })
      .catch(error => {
        console.error('Error:', error);
      });
  }, 2000);
});

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
              <component :is="cur_view" :states="states"></component> 
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
