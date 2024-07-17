<template>
  <el-space direction="vertical" :size=20 fill style="width: 100%;">
    <el-row gutter=20>
      <el-col :span=16>
        <el-card shadow="hover">
          <template #header>
            <div class="card-header">
              <span class="card-name">
                <span class="align-middle">总览</span>
              </span>
            </div>
          </template>
          <div>
            <BatterysChart :states="props.states" ref="batteryChartRef" height="350px" />
            <div style="text-align: center; width: 100%; margin-top: 5px;">
              <el-text type="warning" size="small">
                *阻抗超出量程的故障电池由于阻抗无法测量，故不显示在图中
              </el-text>
            </div>
          </div>

        </el-card>
      </el-col>

      <el-col :span=8>
        <el-card shadow="hover" style="display: flex; flex-direction: column; height: 100%;" body-style="flex-grow: 1;">
          <template #header>
            <div class="card-header">
              <span class="card-name">
                <span class="align-middle">快速诊断</span>
              </span>
            </div>
          </template>
          <div style="height: 100%">
            <el-table :data="quickInsightData" style="width:100%;" :row-class-name="tableRowClassName">
              <el-table-column prop="item" label="项目" />
              <el-table-column prop="value" label="信息" />
            </el-table>
          </div>
          <template #footer>
            <div style="display: flex; justify-content: space-between; align-items: center;">
              <el-text type="info">最后更新:</el-text>
              <el-text type="info">{{ props.states.last_update }}</el-text>
            </div>
          </template>
        </el-card>
      </el-col>
    </el-row>

    <div>
      <el-space direction="vertical" :size=10 fill style="width: 100%; margin-top: 20px;">
        <el-divider content-position="left">详细信息</el-divider>
        <el-table :data="batterysData" stripe style="width:100%;"
          :default-sort="{ prop: 'battery', order: 'ascending' }">
          <el-table-column sortable prop="battery" label="电池" />
          <el-table-column sortable prop="voltage" label="电压(V)" />
          <el-table-column sortable prop="ohmage" label="阻抗(mΩ)" />
          <el-table-column prop="tag" label="评估">
            <template #default="scope">
              <el-space>
                <span v-for="item in scope.row.tag">
                  <el-tag :type="scope.row.type" disable-transitions>{{ item }}</el-tag>
                </span>
              </el-space>
            </template>
          </el-table-column>
        </el-table>
      </el-space>
    </div>
  </el-space>
</template>

<script setup>
import { ref, computed } from 'vue'
import BatterysChart from './charts/BatterysChart.vue'

const props = defineProps({
  states: Object
})

const batteryChartRef = ref(null)

const quickInsightData = computed(() => {
  let data = [
    {
      item: "Count",
      value: props.states.battery_count
    },
    {
      item: "Vmax (V)",
      value: Math.max(...props.states.voltages_cur).toFixed(4)
    },
    {
      item: "Vmin (V)",
      value: Math.min(...props.states.voltages_cur).toFixed(4)
    },
    {
      item: "△V (V)",
      value: (Math.max(...props.states.voltages_cur) - Math.min(...props.states.voltages_cur)).toFixed(4)
    },
    {
      item: "Omax (mΩ)",
      value: isNaN(Math.max(...props.states.ohmages)) ? ">50" : Math.max(...props.states.ohmages)
    },
    {
      item: "Omean (mΩ)",
      value: props.states.ohmages_mean
    }
  ]
  return data;
});

const batterysData = computed(() => {
  let data = [
    // {
    //   battery: "Battery 1",
    //   voltage: "1",
    //   ohmage: "1",
    //   tag: ["正常", "666666666"]
    // },
  ];
  for (let i = 0; i < props.states.battery_count; i++) {
    let tag = [];
    let type = "success"
    if (props.states.ohmages[i] > 25) {
      tag.push("动力不足");
      type = "warning";
    }
    if (props.states.voltages_cur[i] > 4.5) {
      tag.push("过压");
      type = "danger";
    }
    if (props.states.voltages_cur[i] < 3.2) {
      tag.push("欠压");
      type = "warning";
    }
    if (props.states.ohmages[i] == ">50") {
      tag.push("故障");
      type = "danger";
    }
    if (tag.length == 0) {
      tag.push("正常");
    }
    data.push({
      battery: "Battery " + (i + 1),
      voltage: props.states.voltages_cur[i].toFixed(4),
      ohmage: props.states.ohmages[i] == 0 ? ">50" : props.states.ohmages[i],
      tag: tag,
      type: type
    });
  }
  return data;
});

const tableRowClassName = ({
  row,
  rowIndex,
}) => {
  switch (quickInsightData.value[rowIndex].item) {
    case "Count":
      if (quickInsightData.value[rowIndex].value < 1) {
        return "warning-row";
      }
      break;
    case "Vmax (V)":
      if (quickInsightData.value[rowIndex].value > 4.5) {
        return "warning-row";
      }
      break;
    case "Vmin (V)":
      if (quickInsightData.value[rowIndex].value < 3.2) {
        return "warning-row";
      }
      break;
    case "△V (V)":
      if (quickInsightData.value[rowIndex].value > 0.1) {
        return "warning-row";
      }
      break;
    case "Omax (mΩ)":
      if (isNaN(quickInsightData.value[rowIndex].value) || quickInsightData.value[rowIndex].value > 30) {
        return "warning-row";
      }
      break;
    case "Omean (mΩ)":
      if (quickInsightData.value[rowIndex].value < 20) {
        return "success-row";
      }
      break;
  }
  return ''
}

</script>

<style scoped>
:deep() .el-table .warning-row {
  --el-table-tr-bg-color: var(--el-color-warning-light-9);
}

:deep() .el-table .success-row {
  --el-table-tr-bg-color: var(--el-color-success-light-9);
}
</style>
