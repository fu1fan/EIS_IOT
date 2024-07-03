<script setup>
</script>

<template>
  <el-row :gutter="20">
    <el-col :span="16"><el-card shadow="hover">
        <template #header>
          <div class="card-header"><span class="card-name"><span class="align-middle">
                概要
              </span></span></div>
        </template>
        <p>测试</p>
      </el-card>
    </el-col>
    <el-col :span="8">
      <el-row gutter="10">
        <el-col :span="24">
          <el-card shadow="hover">
            <template #header>
              <div class="card-header">
                <span>电池状态</span>
              </div>
            </template>
            <el-result icon="warning" title="未知" sub-title="请检查是否已启动EIS模块">
            </el-result>
          </el-card>
        </el-col>
      </el-row>
      <el-row>
        <el-col :span="24">
          <el-card shadow="hover">
            <template #header>
              <div class="card-header">
                <span>上次更新</span>
              </div>
            </template>
            <p>测试</p>
          </el-card>
        </el-col>
      </el-row>
    </el-col>
  </el-row>
</template>

<style lang="scss" scoped>
.el-row {
  margin-bottom: 20px;
}

.el-row:last-child {
  margin-bottom: 0;
}

.el-col {
  margin-right: 0px;
  border-radius: 4px;
}

.grid-content {
  border-radius: 4px;
  min-height: 36px;
}

.el-card {
  height: 100%;
}
</style>