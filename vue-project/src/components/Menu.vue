<template>
    <el-menu default-active="2" class="el-menu-vertical" :collapse="false" @select="select_callback">
      <el-menu-item index="1">
        <el-icon><icon-menu /></el-icon>
        <template #title>概览</template>
      </el-menu-item>
      <el-sub-menu index="2" :disabled="!isOnline">
        <template #title>
          <el-icon>
            <location />
          </el-icon>
          <span>测量</span>
        </template>
        <el-menu-item-group>
          <template #title><span>单节测量</span></template>
          <el-menu-item index="2-1">EIS测量</el-menu-item>
          <el-menu-item index="2-2">内阻测量</el-menu-item>
        </el-menu-item-group>
        <el-menu-item-group>
          <template #title><span>电池组分析</span></template>
          <!-- <el-menu-item index="2-3">电压均衡</el-menu-item>
          <el-menu-item index="2-4">动力分析</el-menu-item> -->
          <el-menu-item index="2-7">所有电池</el-menu-item>
        </el-menu-item-group>
        <el-menu-item-group>
          <template #title><span>其他</span></template>
          <el-menu-item index="2-5">热失控（演示）</el-menu-item>
          <!-- <el-menu-item index="2-6">机械损伤（实验）</el-menu-item> -->
        </el-menu-item-group>
      </el-sub-menu>
      <!-- <el-menu-item index="3">
      <el-icon>
        <document />
      </el-icon>
      <template #title>历史</template>
    </el-menu-item> -->
      <el-menu-item index="4">
        <el-icon>
          <setting />
        </el-icon>
        <template #title>设置</template>
      </el-menu-item>
    </el-menu>
</template>

<script setup>
import {
  Document,
  Menu as IconMenu,
  Location,
  Setting,
} from '@element-plus/icons-vue'

const props = defineProps({
  select_callback: Function,
  isOnline: Boolean,
})

screenWidth: document.body.clientWidth
</script>

<style scoped>
.el-menu-vertical {
  height: 100%;
}

.el-menu-vertical:not(.el-menu--collapse) {
  width: 180px;
}
</style>