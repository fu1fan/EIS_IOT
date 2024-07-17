<template>
  <el-card shadow="hover">
    <template #header>
      <div class="card-header">
        <span class="card-name">
          <span class="align-middle">电池选择</span>
        </span>
      </div>
    </template>
    <el-scrollbar>
      <div v-if="props.states.battery_count > 0" class="battery-list">
        <div v-for="index in props.states.battery_count" :key="index" class="battery-item"
          :class="{ selected: index === selectedIndex }" @click="selectBattery(index)">
          Battery {{ index }}
        </div>
      </div>
      <div v-else class="battery-none">
        <el-text type="info" size="large">无电池</el-text>
      </div>
    </el-scrollbar>
    <template #footer>
      <el-space direction="vertical" :size=10 fill style="width:100%;">
        <el-text>电池信息</el-text>
        <el-table :data="batteryData" border style="width:100%;">
          <el-table-column prop="voltage" label="Vol(V)" />
          <el-table-column prop="ohmage" label="Imp(mΩ)" />
        </el-table>
        <div style="display: flex; justify-content: space-between; align-items: center;">
          <el-text type="info">最后更新:</el-text>
          <el-text type="info">{{ props.states.last_update }}</el-text>
        </div>
      </el-space>
    </template>
  </el-card>
</template>

<script setup>
import { ref } from 'vue';

// 定义props
// const props = defineProps({
// 	batteryCount:{
// 		type: Number,
// 		required: true
// 	},
// });
const batteryData = ref([
  {
    voltage: "-",
    ohmage: "-",
  }
]);

const props = defineProps({
  states: Object
})

// 定义emits
const emits = defineEmits(['update:selectedIndex']);

const selectedIndex = ref(0);

// 选择电池的方法
const selectBattery = (index) => {
  console.log('selectBattery', index);
  selectedIndex.value = index; // 更新本地状态
  batteryData.value = [
    {
      voltage: props.states.voltages_cur[index - 1],
      ohmage: props.states.ohmages[index - 1],
    }
  ];
  emits('update:selectedIndex', index); // 通知父组件
};
</script>

<style scoped>
.battery-list {
  display: flex;
  flex-wrap: wrap;
  justify-content: center;
  min-height: 240px;
  align-content: flex-start;
}

.battery-none {
  display: flex;
  flex-wrap: wrap;
  justify-content: center;
  min-height: 240px;
}

.battery-item {
  padding: 10px;
  margin: 5px;
  border: 1px solid #ccc;
  cursor: pointer;
  min-width: 90px;
  text-align: center;
}

.selected {
  color: white;
  background-color: #409eff;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);
}
</style>