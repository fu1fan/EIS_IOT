<template>
  <el-row gutter=20>
    <el-col :span=8>
      <BatterySelect @update:selectedIndex="updateSelectedIndex" :states="props.states" />
    </el-col>
    <el-col :span=16>
      <el-card shadow="hover">
        <template #header>
          <div class="card-header">
            <span class="card-name">
              <span class="align-middle">测量</span>
            </span>
          </div>
        </template>
        <div>
          <el-space direction="vertical" :size="20" style="width:100%;" fill>
            <div style="display: flex; justify-content: space-between; align-items: center;">
              <el-space :size="20">
                <span><a><el-text size="large" style="font-weight: bold;">Freq(Hz):</el-text></a></span>
                <el-input-number v-model="measure_freq" controls-position="right" :step="100">
                  <template #decrease-icon>
                    <el-icon>
                      <Minus />
                    </el-icon>
                  </template>
                  <template #increase-icon>
                    <el-icon>
                      <Plus />
                    </el-icon>
                  </template>
                </el-input-number>
              </el-space>
              <el-text size="large" style="width:auto;" line-clamp=1>常用频率：100Hz:分析动力特性、1000+Hz:测量欧姆阻抗</el-text>
            </div>
          </el-space>
          <br />
          <br />
          <el-divider content-position="center">测量结果</el-divider>
          <el-row gutter=10>
            <el-col :span=14>
              <el-space direction="vertical" :size="5" fill style="width:100%;">
                <span style="display: flex; justify-content: center;"><a><el-text>阻抗信息</el-text></a></span>
                <el-table :data="eisData" border style="width:100%;">
                  <el-table-column prop="real" label="REAL(mΩ)" />
                  <el-table-column prop="imag" label="IMAG(mΩ)" />
                </el-table>
                <el-table :data="eisData" border style="width:100%;">
                  <el-table-column prop="abs" label="ABS(mΩ)" />
                  <el-table-column prop="phase" label="PHAS(rad)" />
                </el-table>
              </el-space>
            </el-col>
            <el-col :span=10>
              <div style="display: flex; justify-content: flex-end;">
                <ComplexChart ref="compChartRef" title="阻抗图" width="200px" height="200px" />
              </div>
            </el-col>
          </el-row>
        </div>
        <template #footer>
          <div style="display: flex; justify-content: space-between; align-items: center;">
            <el-text size="large">
              <a>{{ result }}</a>
            </el-text>
            <el-button :disabled="button_disabled" type="primary" @click="start_measure">
              {{ button_text }}
            </el-button>
          </div>
        </template>
      </el-card>
    </el-col>
  </el-row>
</template>

<script setup>
import { ref } from 'vue'
import BatterySelect from './BatterySelect.vue';
import ComplexChart from './charts/ComplexChart.vue';

const props = defineProps({
  states: Object
})

let button_disabled = ref(false);
let button_text = ref('开始测量');

const measure_freq = ref(100);

const selected_cell_id = ref(-1);
const result = ref('未开始测量');

const compChartRef = ref(null);

const eisData = ref([
  {
    real: "-",
    imag: "-",
    abs: "-",
    phase: "-",
  }
]);

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
          // data.imags存放着纵坐标
          // data.reals存放着横坐标
          // 遍历添加
          result.value = data.data.zabs.toFixed(4) + "mΩ (angel: " + data.data.zarg.toFixed(4) + ")";
          eisData.value = [
            {
              real: data.data.real.toFixed(4),
              imag: data.data.imag.toFixed(4),
              abs: data.data.zabs.toFixed(4),
              phase: data.data.zarg.toFixed(4),
            }
          ];
          if (compChartRef.value) {
            compChartRef.value.set_data(data.data.real, data.data.imag);
          }
        } else {
          alert('错误代码：' + data.code);
        }
        button_disabled.value = false;
        button_text.value = '开始测量';
      } else if (data.status == "warning") {
        if (data.message == "waiting") {
          button_text.value = '任务排队中';
        } else if (data.message == "processing") {
          button_text.value = '任务进行中';
        }
        // 如果任务未完成，稍后再次检查
        setTimeout(() => checkResult(task_id), 500)
      } else {
        alert('任务丢失')
      }
    })
    .catch((error) => {
      alert('获取数据失败');
      console.error('Error:', error);
      // 可以在这里添加错误处理逻辑，比如重试或者终止
    });
}

const start_measure = () => {
  if (selected_cell_id.value == -1) {
    console.log(selected_cell_id.value);
    alert('请选择电池');
  }
  if (measure_freq.value < 1) {
    alert('频率过低');
    return;
  }
  if (measure_freq.value > 6000) {
    alert('频率过高');
    return;
  }
  console.log('start measure');
  fetch('/api/c/add_task', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify({
      "type": "single",
      "freq": measure_freq.value,
      "cell_id": selected_cell_id.value - 1,
    }),
  })
    .then((response) => response.json())
    .then((data) => {
      if (data.status == "success") {
        let task_id = data.id;
        result.value = '等待测量结果';
        button_disabled.value = true;
        button_text.value = '等待...';
        checkResult(task_id);
      } else {
        ElMessageBox.alert('设备离线或正忙', '错误', {
          confirmButtonText: '确定',
          type: 'error',
        });
      }
    })
    .catch((error) => {
      console.error('Error:', error);
    });
};
</script>

<style scoped>
.el-card {
  height: 100%;
}
</style>