<template>
	<el-card shadow="hover">
		<template #header>
			<div class="card-header">
				<span class="card-name">
					<span class="align-middle">电池选择</span>
				</span>
			</div>
		</template>
		<div class="battery-list">
			<div
				v-for="index in batteryCount"
				:key="index"
				class="battery-item"
				:class="{ selected: index === selectedIndex }"
				@click="selectBattery(index)"
			>
				Battery {{ index }}
			</div>
		</div>
	</el-card>
</template>

<script setup>
import { ref, defineProps, defineEmits, onMounted } from 'vue';

// 定义props
// const props = defineProps({
// 	batteryCount:{
// 		type: Number,
// 		required: true
// 	},
// });
const batteryCount = ref(0);

onMounted(() => {
	// 获取电池数量
	fetch('/api/c/battery_count')
	.then((response) => response.json())
	.then((data) => {
		batteryCount.value = data.data
	})
	.catch((error) => {
		console.error('Error:', error);
	});
});

// 定义emits
const emits = defineEmits(['update:selectedIndex']);

const selectedIndex = ref(0);

// 选择电池的方法
const selectBattery = (index) => {
  console.log('selectBattery', index);
  selectedIndex.value = index; // 更新本地状态
  emits('update:selectedIndex', index); // 通知父组件
};
</script>

<style scoped>
.battery-list {
	display: flex;
	flex-wrap: wrap;
}

.battery-item {
	padding: 10px;
	margin: 5px;
	border: 1px solid #ccc;
	cursor: pointer;
}

.selected {
	color: white;
	background-color: #409eff;
	box-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);
}
</style>