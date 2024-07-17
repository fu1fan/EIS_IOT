<script setup>
import { ref } from 'vue'
import { computed } from 'vue'
import BaseChart from './BaseChart.vue'

const props = defineProps({
  title: {
    type: String,
    default: "Complex"
  },
  symbolSize: {
    type: Number,
    default: 5
  },
  smmoth: {
    type: Boolean,
    default: true
  },
  xName: {
    type: String,
    default: ''
  },
  yName: {
    type: String,
    default: ''
  },
  width: {
    type: String,
    default: '100%'
  },
  height: {
    type: String,
    default: '400px'
  }
})

let data = ref([])

defineExpose({
  set_data(x, y) {
    data.value = []
    data.value.push([0, 0])
    data.value.push([x, y])
  },
  clear_data() {
    data.value = []
  }
})

let option = computed(() => ({
  title: {
    text: props.title,
    left: 'center',
    textStyle: {
      fontSize: 12,
      fontWeight: 'normal'
    }
  },
  tooltip: {
    trigger: 'axis'
  },
  grid: {
    left: '7%',
    right: '6%',
    bottom: '7%',
    containLabel: true
  },
  toolbox: {
    feature: {
      saveAsImage: {}
    }
  },
  xAxis: {
    type: 'value',
    name: props.xName,
    nameLocation: 'middle',
    nameGap: 20,
    nameTextStyle: {
      fontSize: 10
    },
    min: -30,
    max: 30,
    scale: true
  },
  yAxis: {
    type: 'value',
    name: props.yName,
    nameLocation: 'middle',
    nameGap: 24,
    nameTextStyle: {
      fontSize: 10
    },
    min: -30,
    max: 30,
    scale: true
  },
  series: [{
    data: data.value,
    type: 'line',
  }]
}))
</script>

<template>
  <BaseChart :Option="option" :width="width" :height="height" />
</template>

<style scoped></style>