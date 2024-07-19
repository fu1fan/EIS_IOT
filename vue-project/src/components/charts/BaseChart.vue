<template>
  <div ref="EchartRef" :style="style"></div>
</template>

<script>
import { defineComponent } from "vue";
//第一步：先在官网中的指令进行安装
// npm install echarts --save
//第二步：导入echarts
import * as echarts from "echarts";
// import * as ecStat from 'echarts-stat';

// 保存原始的addEventListener方法
const originalAddEventListener = EventTarget.prototype.addEventListener;

// 覆盖addEventListener方法
EventTarget.prototype.addEventListener = function (type, listener, options) {
  // 检查是否是mousewheel或wheel事件
  if (type === 'mousewheel' || type === 'wheel') {
    // 确保options是一个对象
    options = options || {};
    // 设置passive属性为true
    options = Object.assign({}, options, { passive: true });
  }
  // 调用原始的addEventListener方法
  originalAddEventListener.call(this, type, listener, options);
};

export default defineComponent({
  //第三步：创建Dom结构（并且设定div的大小）用来echars图表
  //第四步：通过ref获取到创建的Dom结构 (！！！！！Dom加载完成的生命周期！！！！！)
  mounted() {
    fetch('customed.project.json')
      .then(r => r.json())
      .then(theme => {
        const main = this.$refs.EchartRef;

        //第五步：初始化咱们的图表内容（到这里基础的引入工作已经完成了）
        echarts.registerTheme('customed', theme);
        const myChars = echarts.init(main, 'customed');
        // echarts.registerTransform(ecStat.transform.regression);
        //第六步：需要配置对应的option数据可以采用对象拼接的数据
        //第七步：通过setOption进行数据的设置
        myChars.setOption(this.Option);

        //监听窗口变化，重新渲染
        window.addEventListener('resize', this.handleWindowResize);
      })
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
  watch: {
    Option: {
      handler(newOption) {
        // Call your function here
        this.yourFunction(newOption);
      },
      deep: true
    }
  },
  methods: {
    yourFunction(newOption) {
      // Your code logic here
      // This function will be called when the Option prop changes
      const myChart = echarts.getInstanceByDom(this.$refs.EchartRef); // Declare myChart variable
      if (myChart!=null){
      myChart.setOption(newOption);
      }
      console.log("Option changed");
    },
    handleWindowResize() {
      console.log("resize")
      const myChart = echarts.getInstanceByDom(this.$refs.EchartRef);
      // 使用setTimeout将resize操作推迟到下一个事件循环
      setTimeout(() => {
        myChart.resize();
      }, 0);
    }
  },
  beforeUnmount() {
    // Remove event listener when component is unmounted
    window.removeEventListener('resize', this.handleWindowResize);
  },
});
</script>