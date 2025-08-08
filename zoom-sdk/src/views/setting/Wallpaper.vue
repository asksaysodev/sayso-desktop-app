<template>
  <div class="setting">
    <el-button type="primary" round plain @click="setOnMeetingWallpaperChangedCB">SetOnMeetingWallpaperChangedCB</el-button>
    <el-button type="primary" round plain @click="setOnMeetingWallpaperImageDownloadStatusCB">SetOnMeetingWallpaperImageDownloadStatusCB</el-button>
    <el-button type="primary" round plain @click="setOnPersonalWallpaperChangedCB">SetOnPersonalWallpaperChangedCB</el-button>
    <el-button type="primary" round plain @click="setOnPersonalWallpaperImageDownloadStatusCB">SetOnPersonalWallpaperImageDownloadStatusCB</el-button>
    <el-button type="primary" round plain @click="isMeetingWallpaperEnabled">IsMeetingWallpaperEnabled</el-button>
    <el-button type="primary" round plain @click="isMeetingWallpaperThumbsReady">IsMeetingWallpaperThumbsReady</el-button>
    <el-button type="primary" round plain @click="getCurrentMeetingWallpaperItem">GetCurrentMeetingWallpaperItem</el-button>
    <el-button type="primary" round plain @click="getMeetingWallpaperList">GetMeetingWallpaperList</el-button>
    <div class="in-line">
      <el-select v-model="meetingWallpaperId" placeholder="Select item">
        <el-option v-for="(item, key) in meetingWallpaperList" :key="key" :label="item.title" :value="item.wallpaperId"></el-option>
      </el-select>
      <el-input-number v-model="transparency" :min="-1" :max="255" />
      <el-select v-model="mode" placeholder="Select mode">
        <el-option v-for="(item, key) in ZoomSDKWallpaperLayoutMode" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="getMeetingWallpaperItemByID">GetMeetingWallpaperItemByID</el-button>
      <el-button type="primary" round plain @click="setMeetingWallpaper">SetMeetingWallpaper</el-button>
    </div>
    <el-button type="primary" round plain @click="isPersonalWallpaperEnabled">IsPersonalWallpaperEnabled</el-button>
    <el-button type="primary" round plain @click="getCurrentPersonalWallpaperItem">GetCurrentPersonalWallpaperItem</el-button>
    <el-button type="primary" round plain @click="getPersonalWallpaperList">GetPersonalWallpaperList</el-button>
    <div class="in-line">
      <el-select v-model="personalWallpaperId" placeholder="Select item">
        <el-option v-for="(item, key) in personalWallpaperList" :key="key" :label="item.title" :value="item.wallpaperId"></el-option>
      </el-select>
      <el-input-number v-model="transparency" :min="-1" :max="255" />
      <el-select v-model="mode" placeholder="Select mode">
        <el-option v-for="(item, key) in ZoomSDKWallpaperLayoutMode" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="getPersonalWallpaperItemByID">GetPersonalWallpaperItemByID</el-button>
      <el-button type="primary" round plain @click="setPersonalWallpaper">SetPersonalWallpaper</el-button>
    </div>
  </div>
  <div class="result">Result: {{result}}</div>
  <div class="result">
    <div class="result-item">
      <span class="title">onMeetingWallpaperChanged <el-button size="small" @click="onMeetingWallpaperChanged_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onMeetingWallpaperChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onMeetingWallpaperImageDownloadStatus <el-button size="small" @click="onMeetingWallpaperImageDownloadStatus_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onMeetingWallpaperImageDownloadStatus_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item">
      <span class="title">onPersonalWallpaperChanged <el-button size="small" @click="onPersonalWallpaperChanged_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onPersonalWallpaperChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onPersonalWallpaperImageDownloadStatus <el-button size="small" @click="onPersonalWallpaperImageDownloadStatus_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onPersonalWallpaperImageDownloadStatus_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import { getSDKResult } from '../../assets/ts/util'
import { ZoomSDKWallpaperLayoutMode, ZoomSDKWallpaperSettingStatus } from '../../../lib/settings.js'
const remote = window.require('@electron/remote')
const result = ref()
const meetingWallpaperList = ref<any[]>([])
const transparency = ref(-1)
const mode = ref(0)
const meetingWallpaperId = ref<number>()
const onMeetingWallpaperChanged_options = ref<string[]>([])
const personalWallpaperList = ref<any[]>([])
const personalWallpaperId = ref<number>()
const onMeetingWallpaperImageDownloadStatus_options = ref<string[]>([])
const onPersonalWallpaperChanged_options = ref<string[]>([])
const onPersonalWallpaperImageDownloadStatus_options = ref<string[]>([])

const onMeetingWallpaperChanged = (item: any) => {
  console.log('onMeetingWallpaperChanged', item)
  onMeetingWallpaperChanged_options.value.push(JSON.stringify(item))
}
const onMeetingWallpaperImageDownloadStatus = (status: string, wallpaperId: string) => {
  console.log('onMeetingWallpaperImageDownloadStatus', status, wallpaperId)
  let statusContent = `status: ${Object.keys(ZoomSDKWallpaperSettingStatus).find(i => ZoomSDKWallpaperSettingStatus[i] == status)}, wallpaperId: ${wallpaperId}`
  onMeetingWallpaperImageDownloadStatus_options.value.push(statusContent)
}
const onPersonalWallpaperChanged = (item: any) => {
  console.log('onPersonalWallpaperChanged', item)
  onPersonalWallpaperChanged_options.value.push(JSON.stringify(item))
}
const onPersonalWallpaperImageDownloadStatus = (status: string, wallpaperId: string) => {
  console.log('onPersonalWallpaperImageDownloadStatus', status, wallpaperId)
  let statusContent = `status: ${Object.keys(ZoomSDKWallpaperSettingStatus).find(i => ZoomSDKWallpaperSettingStatus[i] == status)}, wallpaperId: ${wallpaperId}`
  onPersonalWallpaperImageDownloadStatus_options.value.push(statusContent)
}
const zoomSettingWallpaper = remote.app.zoomSdkModule.Setting.GetWallpaperSetting({ onMeetingWallpaperChanged, onMeetingWallpaperImageDownloadStatus,
  onPersonalWallpaperChanged, onPersonalWallpaperImageDownloadStatus })

const setOnMeetingWallpaperChangedCB = () => {
  let ret = zoomSettingWallpaper.SetOnMeetingWallpaperChangedCB(onMeetingWallpaperChanged)
  result.value = getSDKResult('SetOnMeetingWallpaperChangedCB', ret)
}

const setOnMeetingWallpaperImageDownloadStatusCB = () => {
  let ret = zoomSettingWallpaper.SetOnMeetingWallpaperImageDownloadStatusCB(onMeetingWallpaperImageDownloadStatus)
  result.value = getSDKResult('SetOnMeetingWallpaperImageDownloadStatusCB', ret)
}

const setOnPersonalWallpaperChangedCB = () => {
  let ret = zoomSettingWallpaper.SetOnPersonalWallpaperChangedCB(onPersonalWallpaperChanged)
  result.value = getSDKResult('SetOnPersonalWallpaperChangedCB', ret)
}

const setOnPersonalWallpaperImageDownloadStatusCB = () => {
  let ret = zoomSettingWallpaper.SetOnPersonalWallpaperImageDownloadStatusCB(onPersonalWallpaperImageDownloadStatus)
  result.value = getSDKResult('SetOnPersonalWallpaperImageDownloadStatusCB', ret)
}

const isMeetingWallpaperEnabled = () => {
  let ret = zoomSettingWallpaper.IsMeetingWallpaperEnabled()
  result.value = getSDKResult('IsMeetingWallpaperEnabled', ret)
}

const isMeetingWallpaperThumbsReady = () => {
  let ret = zoomSettingWallpaper.IsMeetingWallpaperThumbsReady()
  result.value = getSDKResult('IsMeetingWallpaperThumbsReady', ret)
}

const getCurrentMeetingWallpaperItem = () => {
  let ret = zoomSettingWallpaper.GetCurrentMeetingWallpaperItem()
  result.value = getSDKResult('GetCurrentMeetingWallpaperItem', ret)
}

const getMeetingWallpaperList = () => {
  let list = zoomSettingWallpaper.GetMeetingWallpaperList()
  meetingWallpaperList.value = list.filter((item: any) => item.wallpaperId)
  result.value = getSDKResult('GetMeetingWallpaperList', list)
}

const setMeetingWallpaper = () => {
  let param = { 
    wallpaperId: meetingWallpaperId.value,
    transparency: transparency.value,
    mode: mode.value
  }
  let ret = zoomSettingWallpaper.SetMeetingWallpaper(param)
  result.value = getSDKResult('SetMeetingWallpaper', ret, param)
}

const getMeetingWallpaperItemByID = () => {
  let param = { wallpaperId: meetingWallpaperId.value }
  let ret = zoomSettingWallpaper.GetMeetingWallpaperItemByID(param)
  result.value = getSDKResult('GetMeetingWallpaperItemByID', ret, param)
}

const isPersonalWallpaperEnabled = () => {
  let ret = zoomSettingWallpaper.IsPersonalWallpaperEnabled()
  result.value = getSDKResult('IsPersonalWallpaperEnabled', ret)
}

const getCurrentPersonalWallpaperItem = () => {
  let ret = zoomSettingWallpaper.GetCurrentPersonalWallpaperItem()
  result.value = getSDKResult('GetCurrentPersonalWallpaperItem', ret)
}

const getPersonalWallpaperList = () => {
  let list = zoomSettingWallpaper.GetPersonalWallpaperList()
  personalWallpaperList.value = list.filter((item: any) => item.wallpaperId)
  result.value = getSDKResult('GetPersonalWallpaperList', list)
}

const setPersonalWallpaper = () => {
  let param = {
    wallpaperId: personalWallpaperId.value,
    transparency: transparency.value, 
    mode: mode.value }
  let ret = zoomSettingWallpaper.SetPersonalWallpaper(param)
  result.value = getSDKResult('SetPersonalWallpaper', ret, param)
}

const getPersonalWallpaperItemByID = () => {
  let param = { wallpaperId: personalWallpaperId.value }
  let ret = zoomSettingWallpaper.GetPersonalWallpaperItemByID(param)
  result.value = getSDKResult('GetPersonalWallpaperItemByID', ret, param)
}
</script>

<style scoped lang="scss">
.result {
  .title {
    width: 340px;
  }
}
</style>