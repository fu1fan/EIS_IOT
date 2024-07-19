<template>
  <el-row gutter=20>
    <el-col :span=8>
      <BatterySelect @update:selectedIndex="updateSelectedIndex" :states="props.states" />
    </el-col>
    <el-col :span=16>
      <el-card shadow="hover" style="display: flex; flex-direction: column; height: 100%;" body-style="flex-grow: 1;">
        <template #header>
          充电热失控监控
        </template>
        <!-- <LineChart ref="lineChartRef" :states="props.states" height="300px" title="70Hz阻抗变化" /> -->
        <el-space dirction="vertical" style="width: 100%;" fill>
          <DynamicLinesChart ref="lineChartRef" height="300px" title="70Hz阻抗变化" />
          <div style="width: 100%; display: flex; justify-content: center; align-items: center;">
            <el-space>
              <el-text type="warning">阻抗变化率</el-text>
            </el-space>
          </div>
        </el-space>
        <template #footer>
          <div style="display: flex; justify-content: space-between; align-items: center;">
            <el-text size="large">
              <a>{{ text }}</a>
            </el-text>
            <el-button :disabled="button_disabled" type="primary" @click="start">
              {{ button_text }}
            </el-button>
          </div>
        </template>
      </el-card>
    </el-col>
  </el-row>
</template>

<script setup>
import { ref, onMounted, onUnmounted } from 'vue'
// import LineChart from './charts/LineChart.vue';
import DynamicLinesChart from './charts/DynamicLinesChart.vue';
import BatterySelect from './BatterySelect.vue';
import { ElMessage, ElMessageBox } from 'element-plus';

const props = defineProps({
  states: Object
})

const lineChartRef = ref(null)

const text = ref('')
const button_disabled = ref(false)
const button_text = ref('开始测量')
const selected_cell_id = ref(-1)

const rate = ref(0)

let from = 0
let stop_flag = 0
let retry = 0

function updateSelectedIndex(index) {
  //console.log('updateSelectedIndex', index);
  selected_cell_id.value = index;
  console.log(selected_cell_id.value);
}

function checkResult(task_id) {
  fetch('/api/c/get_result?id=' + task_id, { method: 'GET' })
    .then((response) => response.json())
    .then((data) => {
      if (data.status == "success") {
        if (data.code == 0) {
          console.info(data.data)
          lineChartRef.value.add_data(0, ((Date.now() - from) / 1000).toFixed(2), data.data.zabs)
          lineChartRef.value.add_data(1, ((Date.now() - from) / 1000).toFixed(2), data.data.voltage)
          add_task()
        } else {
          alert('错误代码：' + data.code);
          add_task()
        }
        // button_disabled.value = false;
        // button_text.value = '开始测量';
      } else if (data.status == "warning") {
        // if (data.message == "waiting") {
        //   button_text.value = '任务排队中';
        // } else if (data.message == "processing") {
        //   button_text.value = '任务进行中';
        // }
        // 如果任务未完成，稍后再次检查
        setTimeout(() => checkResult(task_id), 500)
      } else {
        // alert('任务丢失')
        // button_disabled.value = false;
        // button_text.value = '开始测量';
        retry += 1
        if (retry < 3) {
          add_task()
        } else {
          ElMessageBox.confirm('任务连续失败，是否继续测量？', '错误', {
            confirmButtonText: '重新开始',
            cancelButtonText: '取消',
            type: 'error',
          }).then(() => {
            retry = 0
            add_task()
          }).catch(() => {
            button_text.value = '开始测量'
          })
        }
      }
    })
    .catch((error) => {
      alert(error);
      console.error('Error:', error);
      setTimeout(() => checkResult(task_id), 500)
      // 可以在这里添加错误处理逻辑，比如重试或者终止
    });
}

const start = () => {
  if (button_text.value == "开始测量") {
    if(selected_cell_id.value == -1) {
      ElMessage.error('请选择电池')
      return
    }
    from = Date.now()
    lineChartRef.value.clear_data()
    button_text.value = '停止测量'
    stop_flag = 0
    retry = 0
    add_task()
  }
  else if (button_text.value == "停止测量") {
    stop()
  }
  else {
    button_text.value = '开始测量'
  }
}

const stop = () => {
  stop_flag = 1
  button_text.value = '正在停止'
}

const add_task = () => {
  if (!stop_flag) {
    console.info('add_task')
    fetch('/api/c/add_task', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify({
        "type": "single",
        "freq": 70,
        "cell_id": selected_cell_id.value - 1,
      }),
    })
      .then((response) => response.json())
      .then((data) => {
        if (data.status == "success") {
          let task_id = data.id;
          // button_disabled.value = true;
          // button_text.value = '等待...';
          checkResult(task_id);
        } else {
          ElMessageBox.alert('设备离线或正忙', '错误', {
            confirmButtonText: '确定',
            type: 'error',
          });
          button_text.value = '开始测量'
        }
      })
      .catch((error) => {
        console.error('Error:', error);
        button_text.value = '开始测量'
      });
  }
  else {
    button_text.value = '开始测量'
    stop_flag = 0
  }
};

onMounted(() => {
  alert("请勿在无保护条件下损坏锂电池！")
  lineChartRef.value.add_yAxis('阻抗(mΩ)')
  lineChartRef.value.add_yAxis('电压(V)')
  lineChartRef.value.add_series('阻抗')
  lineChartRef.value.add_series('电压', 1)
})

</script>

<style scoped></style>
