<template>
  <div class="setting">
    <div class="selecter">
      <span>Select Camera</span>
      <el-select v-model="selectCameraListId" placeholder="Select" @change="selectVideoCamera">
        <el-option v-for="(item, index) in cameraList" :key="index" :label="item.deviceName" :value="item.deviceId"></el-option>
      </el-select>
    </div>
    <el-divider></el-divider>
    <el-checkbox v-model="enable_videoMirrorEffect" label="EnableVideoMirrorEffect" @change="enableVideoMirrorEffect"></el-checkbox>
    <el-checkbox v-model="enable_faceBeautyEffect" label="EnableFaceBeautyEffect" @change="enableFaceBeautyEffect"></el-checkbox>
    <el-checkbox v-model="enable_hDVideo" label="EnableHDVideo" @change="enableHDVideo"></el-checkbox>
    <el-checkbox v-model="enable_alwaysShowNameOnVideo" label="EnableAlwaysShowNameOnVideo" @change="enableAlwaysShowNameOnVideo"></el-checkbox>
    <el-checkbox v-model="enable_autoTurnOffVideoWhenJoinMeeting" label="EnableAutoTurnOffVideoWhenJoinMeeting" @change="enableAutoTurnOffVideoWhenJoinMeeting"></el-checkbox>
    <el-checkbox v-model="enable_spotlightSelf" label="EnableSpotlightSelf" @change="enableSpotlightSelf"></el-checkbox>
    <div class="in-line">
      <el-checkbox v-model="enable_hardwareEncode" label="EnableHardwareEncode" @change="enableHardwareEncode"></el-checkbox>
      <el-select v-model="encodeType_hardwareEncode" placeholder="Select" @change="enableHardwareEncode">
        <el-option v-for="(item, key) in ZoomSDKVideoHardwareEncodeType" :key="item" :label="key" :value="item"></el-option>
      </el-select>
    </div>
    <el-checkbox v-model="enable_49VideoesInGallaryView" label="Enable49VideoesInGallaryView" @change="enable49VideoesInGallaryView"></el-checkbox>
    <el-checkbox v-model="enable_hideNoVideoUsersOnWallView" label="EnableHideNoVideoUsersOnWallView" @change="enableHideNoVideoUsersOnWallView"></el-checkbox>
    <el-checkbox v-model="enable_videoPreviewDialog" label="EnableVideoPreviewDialog" @change="enableVideoPreviewDialog"></el-checkbox>
    <el-button type="primary" round plain @click="setComputerCamDeviceChangedCB">SetComputerCamDeviceChangedCB</el-button>
    <el-button type="primary" round plain @click="setDefaultCamDeviceChangedCB">SetDefaultCamDeviceChangedCB</el-button>
    <el-button type="primary" round plain @click="isMirrorEffectEnabled">IsMirrorEffectEnabled</el-button>
    <el-button type="primary" round plain @click="isFaceBeautyEffectEnabled">IsFaceBeautyEffectEnabled</el-button>
    <el-button type="primary" round plain @click="isHDVideoEnabled">IsHDVideoEnabled</el-button>
    <el-button type="primary" round plain @click="isAlwaysShowNameOnVideoEnabled">IsAlwaysShowNameOnVideoEnabled</el-button>
    <el-button type="primary" round plain @click="isAutoTurnOffVideoWhenJoinMeetingEnabled">IsAutoTurnOffVideoWhenJoinMeetingEnabled</el-button>
    <el-button type="primary" round plain @click="isSpotlightSelfEnabled">IsSpotlightSelfEnabled</el-button>
    <el-button type="primary" round plain @click="isHardwareEncodeEnabled">IsHardwareEncodeEnabled</el-button>
    <el-button type="primary" round plain @click="is49VideoesInGallaryViewEnabled">Is49VideoesInGallaryViewEnabled</el-button>
    <el-button type="primary" round plain @click="isHideNoVideoUsersOnWallViewEnabled">IsHideNoVideoUsersOnWallViewEnabled</el-button>
    <el-button type="primary" round plain @click="isVideoPreviewDialogEnabled">IsVideoPreviewDialogEnabled</el-button>
  </div>
  <div class="result">Result: {{result}}</div>
  <div class="result">
    <div class="result-item">
      <span class="title">onComputerCamDeviceChanged <el-button size="small" @click="onComputerCamDeviceChanged_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onComputerCamDeviceChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onDefaultCamDeviceChanged <el-button size="small" @click="onDefaultCamDeviceChanged_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onDefaultCamDeviceChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { getSDKResult } from '../../assets/ts/util'
import { ZoomSDKVideoHardwareEncodeType } from '../../../lib/settings.js'
const remote = window.require('@electron/remote')
const enable_videoMirrorEffect = ref(false)
const enable_faceBeautyEffect = ref(false)
const enable_hDVideo = ref(false)
const enable_alwaysShowNameOnVideo = ref(false)
const enable_autoTurnOffVideoWhenJoinMeeting = ref(false)
const enable_spotlightSelf = ref(false)
const enable_hardwareEncode = ref(false)
const encodeType_hardwareEncode = ref(0)
const enable_49VideoesInGallaryView = ref(false)
const enable_hideNoVideoUsersOnWallView = ref(false)
const enable_videoPreviewDialog = ref(false)
const result = ref()
const onComputerCamDeviceChanged_options = ref<any[]>([])
const onDefaultCamDeviceChanged_options = ref<any[]>([])
const clientHeight = ref((window.innerHeight - 280) + 'px')

const onComputerCamDeviceChanged = (newCameraList: any) => {
  console.log('onComputerCamDeviceChanged', newCameraList)
  onComputerCamDeviceChanged_options.value.push(newCameraList)
}
const onDefaultCamDeviceChanged = (deviceId: string, deviceName: string) => {
  console.log('onDefaultCamDeviceChanged', deviceId, deviceName)
  onDefaultCamDeviceChanged_options.value.push({deviceId, deviceName})
}
const zoomSettingVideo = remote.app.zoomSdkModule.Setting.GetVideoSetting({onComputerCamDeviceChanged, onDefaultCamDeviceChanged})

const cameraList = ref(zoomSettingVideo.Setting_GetCameraList())
const selectCameraListId = ref()

const selectVideoCamera = () => {
  cameraList.value = zoomSettingVideo.Setting_GetCameraList()
  let param = {zn_deviceId: selectCameraListId.value}
  let ret = zoomSettingVideo.Setting_SelectVideoCamera(param)
  result.value = getSDKResult('Setting_SelectVideoCamera', ret, param)
}

const enableVideoMirrorEffect = () => {
  let param = {zn_bEnable: enable_videoMirrorEffect.value}
  let ret = zoomSettingVideo.Setting_EnableVideoMirrorEffect(param)
  result.value = getSDKResult('Setting_EnableAutoJoinAudio', ret, param)
}

const enableFaceBeautyEffect = () => {
  let param = {zn_bEnable: enable_faceBeautyEffect.value}
  let ret = zoomSettingVideo.Setting_EnableFaceBeautyEffect(param)
  result.value = getSDKResult('Setting_EnableFaceBeautyEffect', ret, param)
}

const enableHDVideo = () => {
  let param = {bEnable: enable_hDVideo.value}
  let ret = zoomSettingVideo.Setting_EnableHDVideo(param)
  result.value = getSDKResult('Setting_EnableHDVideo', ret, param)
}

const enableAlwaysShowNameOnVideo = () => {
  let param = {bEnable: enable_alwaysShowNameOnVideo.value}
  let ret = zoomSettingVideo.Setting_EnableAlwaysShowNameOnVideo(param)
  result.value = getSDKResult('Setting_EnableAlwaysShowNameOnVideo', ret, param)
}

const enableAutoTurnOffVideoWhenJoinMeeting = () => {
  let param = {bEnable: enable_autoTurnOffVideoWhenJoinMeeting.value}
  let ret = zoomSettingVideo.Setting_EnableAutoTurnOffVideoWhenJoinMeeting(param)
  result.value = getSDKResult('Setting_EnableAutoTurnOffVideoWhenJoinMeeting', ret, param)
}

const enableSpotlightSelf = () => {
  let param = {bEnable: enable_spotlightSelf.value}
  let ret = zoomSettingVideo.Setting_EnableSpotlightSelf(param)
  result.value = getSDKResult('Setting_EnableSpotlightSelf', ret, param)
}

const enableHardwareEncode = () => {
  let param = {bEnable: enable_hardwareEncode.value, encodeType: encodeType_hardwareEncode.value}
  let ret = zoomSettingVideo.Setting_EnableHardwareEncode(param)
  result.value = getSDKResult('Setting_EnableHardwareEncode', ret, param)
}

const enable49VideoesInGallaryView = () => {
  let param = {bEnable: enable_49VideoesInGallaryView.value}
  let ret = zoomSettingVideo.Setting_Enable49VideoesInGallaryView(param)
  result.value = getSDKResult('Setting_Enable49VideoesInGallaryView', ret, param)
}

const enableHideNoVideoUsersOnWallView = () => {
  let param = {bEnable: enable_hideNoVideoUsersOnWallView.value}
  let ret = zoomSettingVideo.Setting_EnableHideNoVideoUsersOnWallView(param)
  result.value = getSDKResult('Setting_EnableHideNoVideoUsersOnWallView', ret, param)
}

const enableVideoPreviewDialog = () => {
  let param = {bEnable: enable_videoPreviewDialog.value}
  let ret = zoomSettingVideo.Setting_EnableVideoPreviewDialog(param)
  result.value = getSDKResult('Setting_EnableVideoPreviewDialog', ret, param)
}

const setComputerCamDeviceChangedCB = () => {
  let ret = zoomSettingVideo.Setting_SetComputerCamDeviceChangedCB(onComputerCamDeviceChanged)
  result.value = getSDKResult('Setting_SetComputerCamDeviceChangedCB', ret)
}

const setDefaultCamDeviceChangedCB = () => {
  let ret = zoomSettingVideo.Setting_SetDefaultCamDeviceChangedCB(onDefaultCamDeviceChanged)
  result.value = getSDKResult('Setting_SetDefaultCamDeviceChangedCB', ret)
}

const isMirrorEffectEnabled = () => {
  let ret = zoomSettingVideo.Checking_IsMirrorEffectEnabled()
  result.value = getSDKResult('Checking_IsMirrorEffectEnabled', ret)
}

const isFaceBeautyEffectEnabled = () => {
  let ret = zoomSettingVideo.Checking_IsFaceBeautyEffectEnabled()
  result.value = getSDKResult('Checking_IsFaceBeautyEffectEnabled', ret)
}

const isHDVideoEnabled = () => {
  let ret = zoomSettingVideo.Setting_IsHDVideoEnabled()
  result.value = getSDKResult('Setting_IsHDVideoEnabled', ret)
}

const isAlwaysShowNameOnVideoEnabled = () => {
  let ret = zoomSettingVideo.Setting_IsAlwaysShowNameOnVideoEnabled()
  result.value = getSDKResult('Setting_IsAlwaysShowNameOnVideoEnabled', ret)
}

const isAutoTurnOffVideoWhenJoinMeetingEnabled = () => {
  let ret = zoomSettingVideo.Setting_IsAutoTurnOffVideoWhenJoinMeetingEnabled()
  result.value = getSDKResult('Setting_IsAutoTurnOffVideoWhenJoinMeetingEnabled', ret)
}

const isSpotlightSelfEnabled = () => {
  let ret = zoomSettingVideo.Setting_IsSpotlightSelfEnabled()
  result.value = getSDKResult('Setting_IsSpotlightSelfEnabled', ret)
}

const isHardwareEncodeEnabled = () => {
  let param = {encodeType: encodeType_hardwareEncode.value}
  let ret = zoomSettingVideo.Setting_IsHardwareEncodeEnabled(param)
  result.value = getSDKResult('Setting_IsHardwareEncodeEnabled', ret, param)
}

const is49VideoesInGallaryViewEnabled = () => {
  let ret = zoomSettingVideo.Setting_Is49VideoesInGallaryViewEnabled()
  result.value = getSDKResult('Setting_Is49VideoesInGallaryViewEnabled', ret)
}

const isHideNoVideoUsersOnWallViewEnabled = () => {
  let ret = zoomSettingVideo.Setting_IsHideNoVideoUsersOnWallViewEnabled()
  result.value = getSDKResult('Setting_IsHideNoVideoUsersOnWallViewEnabled', ret)
}

const isVideoPreviewDialogEnabled = () => {
  let ret = zoomSettingVideo.Setting_IsVideoPreviewDialogEnabled()
  result.value = getSDKResult('Setting_IsVideoPreviewDialogEnabled', ret)
}

onMounted(() => {
  for (let item of cameraList.value) {
    if (item.isSelected) {
      selectCameraListId.value = item.deviceId
      break
    }
  }
  window.addEventListener('resize', ()=>{
    clientHeight.value = (window.innerHeight - 280) + 'px'
  })
})
</script>
<style scoped lang="scss">
.setting {
  height: v-bind(clientHeight);
}
</style>
