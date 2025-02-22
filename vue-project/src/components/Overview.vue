<script setup>
import BaseChart from './charts/BaseChart.vue'
import { ref, computed, onMounted, onUnmounted } from 'vue'

const props = defineProps({
  states: Object
})

const { states } = props

const tableRowClassName = ({
  row,
  rowIndex,
}) => {
  if (rowIndex === 1) {
    return 'warning-row'
  } else if (rowIndex === 3) {
    return 'success-row'
  }
  return ''
}

const battery_status = {
  "正常": {
    icon: "success",
    title: "正常",
    sub_title: "电池状态正常"
  },
  "未知": {
    icon: "warning",
    title: "未知",
    sub_title: "请等待第一次扫描结果"
  },
  "离线": {
    icon: "error",
    title: "离线",
    sub_title: "设备离线"
  },
  "均衡": {
    icon: "warning",
    title: "电压不均衡",
    sub_title: "请到就近维修店检修电池"
  },
  "异常": {
    icon: "error",
    title: "存在异常电池",
    sub_title: "已断开动力源，请到就近维修店检修电池"
  },
  "低压": {
    icon: "error",
    title: "电压过低",
    sub_title: "部分电池电压低于临界值"
  },
  "高压": {
    icon: "error",
    title: "电压过高",
    sub_title: "部分电池电压高于临界值"
  },
  "未响应": {
    icon: "info",
    title: "等待测量结果",
    sub_title: "这比预期的等待时间长"
  }
}

// let status = ref("未知")
// let voltage_mean = ref(0)
// let battery_count = ref(0)
// let ohmages_mean = ref(0)
// let last_update = ref(0)

// // Make a request to /api/c/is_online
// let intervalId = null;

// onMounted(() => {
//   intervalId = setInterval(() => {
//     fetch('/api/c/is_online')
//       .then(response => response.json())
//       .then(data => {
//         if (data.data == true) {
//           fetch('/api/c/get_state')
//             .then(response => response.json())
//             .then(data => {
//               if (data.status == "success") {
//                 status.value = data.data.state
//                 voltage_data.value = data.data.voltages_his
//                 voltage_mean.value = data.data.voltage_mean.toFixed(3)
//                 battery_count.value = data.data.battery_count
//                 ohmages_mean.value = data.data.ohmages_mean.toFixed(3)
//                 last_update.value = new Date(data.data.last_update * 1000).toLocaleTimeString()
//               }
//             })
//             .catch(error => {
//               console.error('Error:', error);
//             });
//         } else {
//           status.value = "离线"
//         }
//       })
//       .catch(error => {
//         console.error('Error:', error);
//       });
//   }, 1000);
// });

// onUnmounted(() => {
//   clearInterval(intervalId);
// });

const option = computed(() => {
  const data = states.voltages_his.slice(-7).map((value) => {
    return value.toFixed(2)
  })

  const xAxisData = ['-6min', '-5min', '-4min', '-3min', '-2min', '-1min', 'cur']
  const yAxisData = data.length < 7 ? Array(7 - data.length).fill(0).concat(data) : data

  return {
    title: {
      text: '电压变化趋势图',
      left: 'center'
    },
    grid: {
      left: '0%',
      // right: '3%',
      bottom: '0%',
      containLabel: true
    },
    xAxis: {
      type: 'category',
      data: xAxisData
    },
    yAxis: {
      type: 'value'
    },
    tooltip: {
      trigger: 'axis',
      axisPointer: {
        type: 'cross'
      }
    },
    series: [
      {
        data: yAxisData,
        type: 'line',
        smooth: true
      }
    ]
  }
})
const deviceData = computed(() => [
  {
    item: '设备状态',
    content: states.text,
  },
  {
    item: '上次更新',
    content: states.last_update,
  },
])

const batteryData = computed(() => [
  {
    item: '电池数量',
    content: states.battery_count,
  },
  {
    item: '平均内阻(mΩ)',
    content: states.ohmages_mean,
  },
  {
    item: "平均电压(V)",
    content: states.voltage_mean,
  },
])
</script>

<template>
  <el-row gutter=20>
    <el-col :span=16><el-card shadow="hover">
        <template #header>
          <div class="card-header"><span class="card-name"><span class="align-middle">
                概要
              </span></span></div>
        </template>
        <div>
          <el-row gutter=10>
            <el-col :span=14>
              <BaseChart :Option="option" width="100%" height="330px" />
            </el-col>
            <el-col :span=10>
              <el-table :data="deviceData" style="width: 100%" :row-class-name="tableRowClassName">
                <el-table-column prop="item" />
                <el-table-column prop="content" />
              </el-table>
              <br />
              <el-table :data="batteryData" style="width: 100%" :row-class-name="tableRowClassName">
                <el-table-column prop="item" label="项目" />
                <el-table-column prop="content" label="参数" />
              </el-table>
            </el-col>
          </el-row>
        </div>
      </el-card>
    </el-col>
    <el-col :span=8>
      <el-row gutter=10>
        <el-col :span=24>
          <el-card shadow="hover">
            <template #header>
              <div class="card-header">
                <span>电池状态</span>
              </div>
            </template>
            <el-result :icon="battery_status[states.text].icon" :title="battery_status[states.text].title"
              :sub-title="battery_status[states.text].sub_title">
            </el-result>
          </el-card>
        </el-col>
      </el-row>
      <el-row>
        <el-col :span=24>
          <el-card shadow="hover">
            <template #header>
              <div class="card-header">
                <span>运行模式</span>
              </div>
            </template>
            <p>定时扫描</p>
          </el-card>
        </el-col>
      </el-row>
    </el-col>
  </el-row>
</template>

<style lang="scss" scoped>
.el-card {
  height: 100%;
}

.el-table .warning-row {
  --el-table-tr-bg-color: var(--el-color-warning-light-9);
}

.el-table .success-row {
  --el-table-tr-bg-color: var(--el-color-success-light-9);
}
</style>