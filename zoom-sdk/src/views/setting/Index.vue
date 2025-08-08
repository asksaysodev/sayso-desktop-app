<template>
  <div class="sub-router">
    <el-menu
      :default-active="router.currentRoute.value.path"
      background-color="#545c64"
      text-color="#fff"
      router
      class="menu"
    >
      <el-menu-item v-for="(item, index) in menuList" :key="index" :index="`/setting/${item.toLowerCase()}`">
        <span>{{item}}</span>
      </el-menu-item>
    </el-menu>
    <div class="content">
      <div class="router-name">{{router.currentRoute.value.name}}</div>
      <el-divider></el-divider>
      <router-view v-slot="{ Component }">
        <keep-alive>
          <component :is="Component"/>
        </keep-alive>
      </router-view>
    </div>
  </div>
</template>

<script setup lang="ts">
import { useRouter } from 'vue-router'
const router = useRouter()
const menuList = ['Dialog', 'General', 'Video', 'Audio', 'Share', 'Recording', 'Wallpaper', 'VirtualBG', 'Accessibility', 'UI', 'Statistic']
</script>
