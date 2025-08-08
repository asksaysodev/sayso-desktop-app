import { createRouter, createWebHashHistory, RouteRecordRaw } from 'vue-router'
import setting_routes from './setting'
import meeting_routes from './meeting'

const routes: Array<RouteRecordRaw> = [
  {
    path: '/',
    name: 'Domain',
    component: () => import(/* webpackChunkName: "domain" */ '../views/Domain.vue')
  },
  {
    path: '/login',
    name: 'Login',
    component: () => import(/* webpackChunkName: "login" */ '../views/Login.vue')
  },
  {
    path: '/board',
    name: 'Board',
    component: () => import(/* webpackChunkName: "before" */ '../views/Board.vue')
  },
  ...setting_routes,
  ...meeting_routes,
]

const router = createRouter({
  history: createWebHashHistory(),
  routes
})

export default router
