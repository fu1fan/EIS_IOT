<script setup>
import { ref } from 'vue'
import { computed } from 'vue'
import BaseChart from './BaseChart.vue'

const props = defineProps({
  symbolSize:{
    type: Number,
    default: 5
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
  add_data(x, y) {
    data.value.push([x, y])
  },
  clear_data() {
    data.value = []
  }
})

let option = computed(() => ({
  title: {
    text: 'Nyquist Chart of Battery',
    left: 'center'
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
  grid: {
    left: '9%',
    right: '9%',
    bottom: '10%',
    containLabel: true
  },
  xAxis: {
    type: 'value',
    name: 'Re(Z)  mΩ',
    nameLocation: 'middle',
    nameGap: 25,
    nameTextStyle: {
      fontSize: 14
    }
  },
  yAxis: {
    type: 'value',
    name: '-Im(Z)  mΩ',
    nameLocation: 'middle',
    nameGap: 25,
    nameTextStyle: {
      fontSize: 14
    }
  },
  series: [{
    data: data.value,
    type: 'scatter',
    symbolSize: props.symbolSize
  }]
}))
</script>

<template>
  <BaseChart :Option="option" :width="width" :height="height"/>
</template>

<style scoped>

</style>