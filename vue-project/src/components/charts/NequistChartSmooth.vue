<script setup>
import { ref } from 'vue'
import { computed } from 'vue'
import BaseChart from './BaseChart.vue'

const props = defineProps({
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
    text: 'Nyquist Chart',
    left: 'center'
  },
  tooltip: {
    trigger: 'axis',
    axisPointer: {
      type: 'cross'
    }
  },
  xAxis: {
    type: 'value',
    name: 'Re(Z)',
    nameLocation: 'middle',
    nameGap: 25,
    nameTextStyle: {
      fontSize: 14
    }
  },
  yAxis: {
    type: 'value',
    name: '-Im(Z)',
    nameLocation: 'middle',
    nameGap: 25,
    nameTextStyle: {
      fontSize: 14
    }
  },
  series: [{
    data: data.value,
    type: 'line',
    smooth: true
  }]
}))
</script>

<template>
  <BaseChart :Option="option" :width="width" :height="height"/>
</template>

<style scoped>

</style>