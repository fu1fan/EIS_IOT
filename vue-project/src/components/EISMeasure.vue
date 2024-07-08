<template>
  <el-row gutter="20">
    <el-col :span="8">
      <BatterySelect :batteryCount=battery_count @update:selectedIndex="updateSelectedIndex" />
    </el-col>
    <el-col :span="16">
      <el-card shadow="hover">
        <el-row gutter="20">
          <el-col :span="16">
            <nequist-chart ref="nequistChartRef" />
          </el-col>
          <el-col :span="8">
            <el-card shadow="never">
              <template #header>
                <div class="card-header">
                  <span class="card-name">
                    <span class="align-middle">设备状态</span>
                  </span>
                </div>
              </template>
              暂无
            </el-card>
          </el-col>
        </el-row>
        <template #footer>
          <div style="display: flex; justify-content: flex-end;">
            <el-button type="primary" @click="start_measure">开始测量</el-button>
          </div>
        </template>
      </el-card>
    </el-col>
  </el-row>
</template>

<script setup>
import { ref, onMounted } from 'vue';
import NequistChart from './charts/NequistChart.vue';
import BatterySelect from './BatterySelect.vue';
import { ElMessageBox } from 'element-plus';

let battery_count = ref(0)

// 获取电池数量
fetch('/api/c/get_battery_list')
  .then((response) => response.json())
  .then((data) => {
    battery_count.value = data.data;
  })
  .catch((error) => {
    console.error('Error:', error);
  });

const selected_cell_id = ref(-1);

function updateSelectedIndex(index) {
  //console.log('updateSelectedIndex', index);
  selected_cell_id.value = index;
  console.log(selected_cell_id.value);
}

const nequistChartRef = ref(null);

const add_data = (x, y) => {
  if (nequistChartRef.value) {
    nequistChartRef.value.add_data(x, y);
  }
};

// {  执行时格式
//   "message": "processing",
//     "status": "warning"
// }

// {  成功时格式
//     "code": 0,
//     "data": {
//         "cell_id": 1,
//         "freqs": [
//             8.0,
//             13.0,
//             20.0,
//         ],
//         "imags": [
//             -0.412315,
//             -0.329344,
//             -0.462629,
//         ],
//         "reals": [
//             14.431007,
//             14.340127,
//             13.979543,
//         ]
//     },
//     "status": "success"
// }

// {  失败时格式
//     "code": 0x90（不等于0）
//     "data": {
//         "cell_id": 1,
//         "freqs": [
//         ],
//         "imags": [
//         ],
//         "reals": [
//         ]
//     },
//     "status": "success"
// }

// {  任务丢失时格式
//     "message": "notfound",
//     "status": "error"
// }

function checkResult(task_id) {
  fetch('/api/c/get_result', {
    method: 'GET',
    params: {
      "id": task_id,
    }
  })
    .then((response) => response.json())
    .then((data) => {
      if (data.status == "success") {
        if (data.code == 0) {
          // data.imags存放着纵坐标
          // data.reals存放着横坐标
          // 遍历添加
          for (let i = 0; i < data.data.freqs.length; i++) {
            add_data(data.data.reals[i], data.data.imags[i]);
          }
        } else {
          alert('错误代码：' + data.code);
        }
      } else if (data.status == "warning") {
        // 如果任务未完成，稍后再次检查
        setTimeout(() => checkResult(task_id), 500)
      } else {
        alert('任务丢失')
      }
    })
    .catch((error) => {
      alert('获取数据失败');
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
      "cell_id": selected_cell_id.value,
    }),
  })
    .then((response) => response.json())
    .then((data) => {
      if (data.status == "success") {
        let task_id = data.data.id;
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
