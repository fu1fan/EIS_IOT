<template>
  <el-row gutter="20">
    <el-col :span="8">
      <BatterySelect />
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
            <el-button type="primary" @click="add_data(1, 2)">开始测量</el-button>
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

const nequistChartRef = ref(null);

const add_data = (x, y) => {
  if (nequistChartRef.value) {
    nequistChartRef.value.add_data(x, y);
  }
};

</script>

<style scoped>
.el-card {
  height: 100%;
}
</style>
