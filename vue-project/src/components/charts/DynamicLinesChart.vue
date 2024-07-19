<script setup>
import { ref } from 'vue'
import { computed } from 'vue'
import BaseChart from './BaseChart.vue'

const props = defineProps({
  title: {
    type: String,
    default: "Stacked Line"
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
  width: {
    type: String,
    default: '100%'
  },
  height: {
    type: String,
    default: '400px'
  }
})

let series = ref([])
let legend = ref([])
let yAxis = ref([])
const limit = -1

defineExpose({
  add_yAxis(name) {
    yAxis.value.push({
      type: 'value',
      name: name,
      nameLocation: 'middle',
      nameGap: 26,
      nameTextStyle: {
        fontSize: 12
      }
    })
  },
  add_series(name, yAxisIndex=0) {
    series.value.push({
      name: name,
      type: 'line',
      data: [],
      smooth: props.smooth,
      symbolSize: props.symbolSize,
      yAxisIndex: yAxisIndex
    })
    legend.value = series.value.map(item => item.name)
  },
  add_data(index, x, y) {
    series.value[index].data.push([x, y])
    if (limit > 0 && series.value[index].data.length > limit) {
      series.value[index].data.shift()
    }
  },
  clear_series() {
    series.value = []
  },
  clear_data() {
    series.value.forEach(element => {
      element.data = []
    });
  },
  set_limit(limit) {
    limit = limit
  }
})

const option = computed(() => ({
  title: {
    text: props.title,
    left: '15px',
    textStyle: {
      fontSize: 12,
    }
  },
  tooltip: {
    trigger: 'axis'
  },
  legend: {
    data: legend.value
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
  tooltip: {
    trigger: 'axis',
    axisPointer: {
      type: 'cross'
    }
  },
  xAxis: {
    type: 'value',
    name: props.xName,
    nameLocation: 'middle',
    nameGap: 20,
    nameTextStyle: {
      fontSize: 12
    }
  },
  yAxis: yAxis.value,
  // yAxis: {
  //   type: 'value',
  //   name: props.yName,
  //   nameLocation: 'middle',
  //   nameGap: 24,
  //   nameTextStyle: {
  //     fontSize: 10
  //   }
  // },
  series: series.value,
}))
</script>

<template>
  <BaseChart :Option="option" :width="width" :height="height" />
</template>

<style scoped></style>