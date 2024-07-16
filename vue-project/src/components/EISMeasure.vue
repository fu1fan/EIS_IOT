<template>
  <el-row gutter=20>
    <el-col :span="8">
      <BatterySelect @update:selectedIndex="updateSelectedIndex" />
    </el-col>
    <el-col :span="16">
      <el-card shadow="hover">
        <nequist-chart ref="nequistChartRef" />
        <template #footer>
          <div style="display: flex; justify-content: flex-end;">
            <!-- <el-button type="primary" @click="start_measure">开始测量</el-button> -->
            <el-button :disabled="button_disabled" type="primary" @click="start_measure">
              {{ button_text }}
            </el-button>
          </div>
        </template>
      </el-card>
    </el-col>
  </el-row>
  <br />
  <el-divider content-position="center">详细数据</el-divider>
  <div>
    <el-row gutter=20>
      <el-col :span="15">
        <el-space direction="vertical" style="width: 100%">
          <span style="width: 100%; display: flex; justify-content: center;"><a><el-text>阻抗表</el-text></a></span>
          <el-table :data="eisData" stripe style="width: 100%">
            <el-table-column sortable prop="freq" label="FREQ" />
            <el-table-column prop="real" label="REAL(mΩ)" />
            <el-table-column prop="imag" label="IMAG(mΩ)" />
            <el-table-column prop="abs" label="ABS(mΩ)" />
            <el-table-column prop="phase" label="PHAS(rad)" />
          </el-table>
        </el-space>
      </el-col>
      <el-col :span="9">
        <span style="width: 100%; display: flex; justify-content: center;"><a><el-text>其他图表</el-text></a></span>
        <div style="width: 100%;">
          <el-divider border-style="dashed" content-position="center"></el-divider>
          <LinesChart ref="amplFreqChartRef" title="幅频特性曲线" height="240px" xName="Freq(Hz)" yName="Impedance(Ω)" />
          <el-divider border-style="dashed" content-position="center"></el-divider>
          <LinesChart ref="compFreqChartRef" title="复数特性曲线" height="240px" xName="Freq(Hz)" yName="Impedance(Ω)" />
          <el-divider border-style="dashed" content-position="center"></el-divider>
          <LinesChart ref="phasFreqChartRef" title="相频特性曲线" height="240px" xName="Freq(Hz)" yName="Phase(rad)"  />
          <el-divider border-style="dashed" content-position="center"></el-divider>
        </div>
      </el-col>
    </el-row>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue';
import NequistChart from './charts/NequistChart.vue';
import LinesChart from './charts/LinesChart.vue';
import BatterySelect from './BatterySelect.vue';
import { ElMessageBox } from 'element-plus';

let battery_count = ref(0)

let button_disabled = ref(false);
let button_text = ref('开始测量');

const selected_cell_id = ref(-1);

function updateSelectedIndex(index) {
  //console.log('updateSelectedIndex', index);
  selected_cell_id.value = index;
  console.log(selected_cell_id.value);
}

const nequistChartRef = ref(null);
const amplFreqChartRef = ref(null);
const compFreqChartRef = ref(null);
const phasFreqChartRef = ref(null);
const eisData = ref([
  { freq: 0, real: 0, imag: 0, abs: 0, phase: 0 },
]);

let imagFreq = []
let realFreq = []
let absFreq = []
let phaseFreq = []

const add_data = (f, x, y) => {
  if (nequistChartRef.value) {
    nequistChartRef.value.add_data(x, y);
    realFreq.push([f, x])
    imagFreq.push([f, y])
    absFreq.push([f, Math.sqrt(x * x + y * y)])
    phaseFreq.push([f, Math.atan2(y, x)])
  }
  eisData.value.push({
    freq: f,
    real: x.toFixed(4),
    imag: y.toFixed(4),
    abs: Math.sqrt(x * x + y * y).toFixed(4),
    phase: Math.atan2(y, x).toFixed(4),
  });
};

const set_series = () => {
  amplFreqChartRef.value.add_series("abs", absFreq);
  compFreqChartRef.value.add_series("real", realFreq);
  compFreqChartRef.value.add_series("imag", imagFreq);
  phasFreqChartRef.value.add_series("phase", phaseFreq);
};

const clear_data = () => {
  imagFreq = []
  realFreq = []
  absFreq = []
  phaseFreq = []
  if (nequistChartRef.value) {
    nequistChartRef.value.clear_data();
  }
  if (eisData.value.length > 0) {
    eisData.value = [];
  }
  if (amplFreqChartRef.value) {
    amplFreqChartRef.value.clear_data();
  }
  if (phasFreqChartRef.value) {
    phasFreqChartRef.value.clear_data();
  }
  if (compFreqChartRef.value) {
    compFreqChartRef.value.clear_data();
  }
};

function checkResult(task_id) {
  fetch('/api/c/get_result?id=' + task_id, { method: 'GET' })
    .then((response) => response.json())
    .then((data) => {
      if (data.status == "success") {
        if (data.code == 0) {
          // data.imags存放着纵坐标
          // data.reals存放着横坐标
          // 遍历添加
          for (let i = 0; i < data.data.freqs.length; i++) {
            add_data(data.data.freqs[i], data.data.reals[i], 0 - data.data.imags[i]);
          }
          set_series();
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
  console.log('start measure');
  fetch('/api/c/add_task', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify({
      "type": "eis",
      "cell_id": selected_cell_id.value - 1,
    }),
  })
    .then((response) => response.json())
    .then((data) => {
      if (data.status == "success") {
        let task_id = data.id;
        clear_data();
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

.h1 {
  font-size: 22em;
}
</style>
