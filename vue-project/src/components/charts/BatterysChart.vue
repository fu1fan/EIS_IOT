<script setup>
import { ref } from 'vue'
import { computed } from 'vue'
import BaseChart from './BaseChart.vue'

const props = defineProps({
  states: {
    type: Object,
    required: true
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

let xAxis = computed(() => {
  let data = [];
  for (let i = 0; i < props.states.battery_count; i++) {
    data.push(`BATTERY ${i + 1}`)
  };
  return data;
})

defineExpose({
  clear_data() {
    data.value = []
  }
})

const labelOption = {
  show: true,
  position: 'insideBottom',
  distance: 15,
  align: 'left',
  verticalAlign: 'middle',
  rotate: 90,
  formatter: '{c}  {name|{a}}',
  fontSize: 16,
  fontWeight: "bold",
  rich: {
    name: {}
  }
};

let option = computed(() => ({
  title: {
    text: "",
    left: 'center',
    textStyle: {
      fontSize: 20,
      fontWeight: 'bold'
    }
  },
  tooltip: {
    trigger: 'axis',
    axisPointer: {
      type: 'shadow'
    }
  },
  legend: {
    left: 'center',
    data: ['Voltage', 'Impedance']
  },
  toolbox: {
    show: true,
    orient: 'vertical',
    left: 'right',
    top: 'top',
    feature: {
      saveAsImage: { show: true }
    }
  },
  xAxis: [
    {
      type: 'category',
      axisTick: { show: false },
      data: xAxis.value
    }
  ],
  grid: {
    left: '3%',
    right: '7%',
    bottom: '2%',
    containLabel: true
  },
  yAxis: [
    {
      type: 'value',
      splitLine: {
        show: true
      }
    },
    {
      type: 'value',
      splitLine: {
        show: false
      }
    }
  ],
  series: [
    {
      name: 'Voltage',
      type: 'bar',
      barGap: 0,
      label: labelOption,
      emphasis: {
        focus: 'series'
      },
      data: props.states.voltages_cur
    },
    {
      name: 'Impedance',
      type: 'bar',
      label: labelOption,
      emphasis: {
        focus: 'series'
      },
      yAxisIndex: 1,
      data: props.states.ohmages
    },
  ]
}));
</script>

<template>
  <BaseChart :Option="option" :width="width" :height="height" />
</template>

<style scoped></style>