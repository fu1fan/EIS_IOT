<script setup>
import BaseChart from './charts/BaseChart.vue'

const option = {
  title: {
    text: '电压变化趋势图',
    left: 'center'
  },
  xAxis: {
    type: 'category',
    data: ['-6min', '-5min', '-4min', '-3min', '-2min', '-1min', 'cur']
  },
  yAxis: {
    type: 'value'
  },
  series: [
    {
      data: [4.2, 4.15, 4.1, 4, 3.9, 3.92, 3.88],
      type: 'line',
      smooth: true
    }
  ]
}

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

const deviceData = [
  {
    item: '设备状态',
    content: '离线',
  },
  {
    item: '上次更新',
    content: '...',
  },
  {
    item: "电池数量",
    content: 1
  }
]

const batteryData = [
  {
    item: '电池数量',
    content: '0',
  },
  {
    item: '平均内阻',
    content: '0',
  },
  {
    item: "平均电压",
    content: '0'
  }
]

</script>

<template>
  <el-row gutter=20>
    <el-col :span="16"><el-card shadow="hover">
        <template #header>
          <div class="card-header"><span class="card-name"><span class="align-middle">
                概要
              </span></span></div>
        </template>
        <div>
          <el-row gutter=10>
            <el-col :span="16"><BaseChart :Option="option" width="100%" height="370px" /></el-col>
            <el-col :span="8">
              <el-table
                :data="deviceData"
                style="width: 100%"
                :row-class-name="tableRowClassName"
              >
                <el-table-column prop="item" />
                <el-table-column prop="content" />
              </el-table>
              <br />
              <el-table
                :data="batteryData"
                style="width: 100%"
                :row-class-name="tableRowClassName"
              >
                <el-table-column prop="item" label="项目" />
                <el-table-column prop="content" label="参数" />
              </el-table>
            </el-col>
          </el-row>
        </div>
      </el-card>
    </el-col>
    <el-col :span="8">
      <el-row gutter=10>
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

.el-table .warning-row {
  --el-table-tr-bg-color: var(--el-color-warning-light-9);
}

.el-table .success-row {
  --el-table-tr-bg-color: var(--el-color-success-light-9);
}
</style>