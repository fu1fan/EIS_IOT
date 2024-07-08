<template>
  <el-row gutter="20">
    <el-col :span="8">
      <BatterySelect @update:selectedIndex="updateSelectedIndex" />
    </el-col>
    <el-col :span="16">
      <el-card shadow="hover">
        <template #header>
          <div class="card-header">
            <span class="card-name">
              <span class="align-middle">测量结果</span>
            </span>
          </div>
        </template>
        <el-text size="large">
          <p>{{ result }}</p>
        </el-text>
        <template #footer>
          <div style="display: flex; justify-content: flex-end;">
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
import { ref, computed, onMounted, onUnmounted } from 'vue'
import BatterySelect from './BatterySelect.vue';

let button_disabled = ref(false);
let button_text = ref('开始测量');

const selected_cell_id = ref(-1);
const result = ref('未开始测量');

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
          result.value = data.data.zabs.toFixed(4) + "Ω (" + data.data.zarg.toFixed(4) + ")";
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
      "type": "single",
      "freq": 100,
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