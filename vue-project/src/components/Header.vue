<template>
<el-menu
    :default-active="activeIndex"
    class="el-header"
    mode="horizontal"
    :ellipsis="false"
    @select="handleSelect"
>
    <el-menu-item class="logo" disabled>
    <img
        style="width: 100px"
        src="/images/logo.png"
        alt="Element logo"
    />
    </el-menu-item>
    
</el-menu>
</template>

<script lang="ts" setup>
import { ref } from 'vue'

const activeIndex = ref('1')
const handleSelect = (key: string, keyPath: string[]) => {
    console.log(key, keyPath)
}
</script>

<style scoped>
.flex-grow {
    flex-grow: 1;
}
.logo:hover {
    background-color: transparent !important;
}
.logo{
    opacity: 1;
}
</style>