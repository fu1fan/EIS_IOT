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

let series = ref([])
let legend = ref([])

defineExpose({
  add_series(name, _data) {
    series.value.push({
      name: name,
      type: 'line',
      data: _data,
      smooth: props.smooth,
      symbolSize: props.symbolSize
    })
    legend.value = series.value.map(item => item.name)
  },
  clear_data() {
    series.value = []
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
      fontSize: 10
    }
  },
  yAxis: {
    type: 'value',
    name: props.yName,
    nameLocation: 'middle',
    nameGap: 24,
    nameTextStyle: {
      fontSize: 10
    }
  },
  series: series.value,
}))
</script>

<template>
  <BaseChart :Option="option" :width="width" :height="height" />
</template>

<style scoped></style>