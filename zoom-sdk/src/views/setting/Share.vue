<template>
  <div class="setting">
    <el-checkbox v-model="enable_enableAutoFitToWindowWhenViewSharing" label="EnableAutoFitToWindowWhenViewSharing" @change="enableAutoFitToWindowWhenViewSharing"></el-checkbox>
    <el-checkbox v-model="enable_enableAccelerateGPUWhenShare" label="EnableAccelerateGPUWhenShare" @change="enableAccelerateGPUWhenShare"></el-checkbox>
    <el-checkbox v-model="enable_enableRemoteControlAllApplications" label="EnableRemoteControlAllApplications" @change="enableRemoteControlAllApplications"></el-checkbox>
    <el-button type="primary" round plain @click="isAutoFitToWindowWhenViewSharingEnabled">IsAutoFitToWindowWhenViewSharingEnabled</el-button>
    <el-button type="primary" round plain @click="isCurrentOSSupportAccelerateGPUWhenShare">IsCurrentOSSupportAccelerateGPUWhenShare</el-button>
    <el-button type="primary" round plain @click="isAccelerateGPUWhenShareEnabled">IsAccelerateGPUWhenShareEnabled</el-button>
    <el-button type="primary" round plain @click="isRemoteControlAllApplicationsEnabled">IsRemoteControlAllApplicationsEnabled</el-button>
  </div>
  <div class="result">Result: {{result}}</div>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const zoomSettingShare = remote.app.zoomSdkModule.Setting.GetShareSetting()
const enable_enableAutoFitToWindowWhenViewSharing = ref(false)
const enable_enableAccelerateGPUWhenShare = ref(false)
const enable_enableRemoteControlAllApplications = ref(false)
const result = ref()

const enableAutoFitToWindowWhenViewSharing = () => {
  let param = {bEnable: enable_enableAutoFitToWindowWhenViewSharing.value}
  let ret = zoomSettingShare.Setting_EnableAutoFitToWindowWhenViewSharing(param)
  result.value = getSDKResult('Setting_EnableAutoFitToWindowWhenViewSharing', ret, param)
}

const enableAccelerateGPUWhenShare = () => {
  let param = {bEnable: enable_enableAccelerateGPUWhenShare.value}
  let ret = zoomSettingShare.Setting_EnableAccelerateGPUWhenShare(param)
  result.value = getSDKResult('Setting_EnableAccelerateGPUWhenShare', ret, param)
}

const enableRemoteControlAllApplications = () => {
  let param = {bEnable: enable_enableRemoteControlAllApplications.value}
  let ret = zoomSettingShare.Setting_EnableRemoteControlAllApplications(param)
  result.value = getSDKResult('Setting_EnableRemoteControlAllApplications', ret, param)
}

const isAutoFitToWindowWhenViewSharingEnabled = () => {
  let ret = zoomSettingShare.Setting_IsAutoFitToWindowWhenViewSharingEnabled()
  result.value = getSDKResult('Setting_IsAutoFitToWindowWhenViewSharingEnabled', ret)
}

const isCurrentOSSupportAccelerateGPUWhenShare = () => {
  let ret = zoomSettingShare.Setting_IsCurrentOSSupportAccelerateGPUWhenShare()
  result.value = getSDKResult('Setting_IsCurrentOSSupportAccelerateGPUWhenShare', ret)
}

const isAccelerateGPUWhenShareEnabled = () => {
  let ret = zoomSettingShare.Setting_IsAccelerateGPUWhenShareEnabled()
  result.value = getSDKResult('Setting_IsAccelerateGPUWhenShareEnabled', ret)
}

const isRemoteControlAllApplicationsEnabled = () => {
  let ret = zoomSettingShare.Setting_IsRemoteControlAllApplicationsEnabled()
  result.value = getSDKResult('Setting_IsRemoteControlAllApplicationsEnabled', ret)
}
</script>
