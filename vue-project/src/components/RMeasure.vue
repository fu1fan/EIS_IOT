<template>
    <el-row gutter="20">
      <el-col :span="8">
        <BatterySelect />
      </el-col>
      <el-col :span="16">
        <el-card shadow="hover">
          <el-row gutter="20">
            <el-col :span="16">
              暂无
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
              <el-button type="primary">开始测量</el-button>
            </div>
          </template>
        </el-card>
      </el-col>
    </el-row>
  </template>
  
  <script setup>
  import BatterySelect from './BatterySelect.vue';
  
  </script>
  
  <style scoped>
  .el-card {
    height: 100%;
  }
  </style>
  