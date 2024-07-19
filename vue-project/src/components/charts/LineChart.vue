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

const line_data = ref([])

defineExpose({
  add_data(x, y) {
    line_data.value.push([x, y])
  },
  clear_data() {
    line_data.value = []
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
  // legend: {
  //   data: legend.value
  // },
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
  series: {
    // name: name,
    type: 'line',
    data: line_data.value,
    smooth: props.smooth,
    symbolSize: props.symbolSize
  },
}))
</script>

<template>
  <BaseChart :Option="option" :width="width" :height="height" />
</template>

<style scoped></style>