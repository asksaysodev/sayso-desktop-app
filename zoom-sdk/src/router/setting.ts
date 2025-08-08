import { RouteRecordRaw } from 'vue-router'

const setting_routes: Array<RouteRecordRaw> = [
  { 
    path: '/setting',
    name: 'Setting',
    component: () => import(/* webpackChunkName: "setting_index" */ '../views/setting/Index.vue'),
    children: [
      {
        path: 'dialog',
        name: 'Setting Dialog',
        component: () => import(/* webpackChunkName: "setting_dialog" */ '../views/setting/Dialog.vue'),
      },
      {
        path: 'general',
        name: 'Setting General',
        component: () => import(/* webpackChunkName: "setting_general" */ '../views/setting/General.vue'),
      },
      {
        path: 'video',
        name: 'Setting Video',
        component: () => import(/* webpackChunkName: "setting_video" */ '../views/setting/Video.vue'),
      },
      {
        path: 'audio',
        name: 'Setting Audio',
        component: () => import(/* webpackChunkName: "setting_audio" */ '../views/setting/Audio.vue'),
      },
      {
        path: 'share',
        name: 'Setting Share',
        component: () => import(/* webpackChunkName: "setting_share" */ '../views/setting/Share.vue'),
      },
      {
        path: 'recording',
        name: 'Setting Recording',
        component: () => import(/* webpackChunkName: "setting_recording" */ '../views/setting/Recording.vue'),
      },
      {
        path: 'wallpaper',
        name: 'Setting Wallpaper',
        component: () => import(/* webpackChunkName: "setting_statistic" */ '../views/setting/Wallpaper.vue'),
      },
      {
        path: 'virtualbg',
        name: 'Setting VirtualBG',
        component: () => import(/* webpackChunkName: "setting_virtual_bg" */ '../views/setting/VirtualBG.vue'),
      },
      {
        path: 'statistic',
        name: 'Setting Statistic',
        component: () => import(/* webpackChunkName: "setting_statistic" */ '../views/setting/Statistic.vue'),
      },
      {
        path: 'accessibility',
        name: 'Setting Accessibility',
        component: () => import(/* webpackChunkName: "setting_accessibility" */ '../views/setting/Accessibility.vue'),
      },
      {
        path: 'ui',
        name: 'Setting UI',
        component: () => import(/* webpackChunkName: "setting_ui" */ '../views/setting/UI.vue'),
      },
    ],
  }
]

export default setting_routes
