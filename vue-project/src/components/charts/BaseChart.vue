<template>
    <div ref="EchartRef" :style="style"></div>
  </template>
  
  <script lang="ts">
  import { defineComponent } from "vue";
  //第一步：先在官网中的指令进行安装
  // npm install echarts --save
  //第二步：导入echarts
  import * as echarts from "echarts";

  export default defineComponent({
    //第三步：创建Dom结构（并且设定div的大小）用来echars图表
    //第四步：通过ref获取到创建的Dom结构 (！！！！！Dom加载完成的生命周期！！！！！)
    mounted() {
      const main = this.$refs.EchartRef;
      //第五步：初始化咱们的图表内容（到这里基础的引入工作已经完成了）
      const myChars = echarts.init(main);
      //第六步：需要配置对应的option数据可以采用对象拼接的数据
      //第七步：通过setOption进行数据的设置
      myChars.setOption(this.Option);
    },
    //父组件传过来的数据
    props: {
        Option: Object,
        width: {
          type: String,
          default: '300px'
        },
        height: {
          type: String,
          default: '300px'
        }
    },
    data() {
      return {
        style: {
          width: this.width,
          height: this.height
        }
      };
    },
  
  });
  </script>