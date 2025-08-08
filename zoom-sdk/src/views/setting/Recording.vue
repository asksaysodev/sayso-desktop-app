<template>
  <div class="setting">
    <el-checkbox v-model="enable_selectRecordFileLocationAfterMeeting" label="EnableSelectRecordFileLocationAfterMeeting" @change="enableSelectRecordFileLocationAfterMeeting"></el-checkbox>
    <el-checkbox v-model="enable_multiAudioStreamRecord" label="EnableMultiAudioStreamRecord" @change="enableMultiAudioStreamRecord"></el-checkbox>
    <el-checkbox v-model="enable_addTimestampWatermark" label="EnableAddTimestampWatermark" @change="enableAddTimestampWatermark"></el-checkbox>
    <el-checkbox v-model="enable_optimizeFor3rdPartyVideoEditor" label="EnableOptimizeFor3rdPartyVideoEditor" @change="enableOptimizeFor3rdPartyVideoEditor"></el-checkbox>
    <el-checkbox v-model="enable_showVideoThumbnailWhenShare" label="EnableShowVideoThumbnailWhenShare" @change="enableShowVideoThumbnailWhenShare"></el-checkbox>
    <el-checkbox v-model="enable_placeVideoNextToShareInRecord" label="EnablePlaceVideoNextToShareInRecord" @change="enablePlaceVideoNextToShareInRecord"></el-checkbox>
    <el-button type="primary" round plain @click="setonCloudRecordingStorageInfoCB">SetonCloudRecordingStorageInfoCB</el-button>
    <el-button type="primary" round plain @click="setRecordingPath">SetRecordingPath</el-button>
    <el-button type="primary" round plain @click="getRecordingPath">GetRecordingPath</el-button>
    <el-button type="primary" round plain @click="canGetCloudRecordingStorageInfo">CanGetCloudRecordingStorageInfo</el-button>
    <el-button type="primary" round plain @click="getCloudRecordingStorageInfo">GetCloudRecordingStorageInfo</el-button>
    <el-button type="primary" round plain @click="getRecordingManagementURL">GetRecordingManagementURL</el-button>
    <el-button type="primary" round plain @click="canGetRecordingManagementURL">CanGetRecordingManagementURL</el-button>
    <el-button type="primary" round plain @click="isSelectRecordFileLocationAfterMeetingEnabled">IsSelectRecordFileLocationAfterMeetingEnabled</el-button>
    <el-button type="primary" round plain @click="isMultiAudioStreamRecordEnabled">IsMultiAudioStreamRecordEnabled</el-button>
    <el-button type="primary" round plain @click="isAddTimestampWatermarkEnabled">IsAddTimestampWatermarkEnabled</el-button>
    <el-button type="primary" round plain @click="isOptimizeFor3rdPartyVideoEditorEnabled">IsOptimizeFor3rdPartyVideoEditorEnabled</el-button>
    <el-button type="primary" round plain @click="isShowVideoThumbnailWhenShareEnabled">IsShowVideoThumbnailWhenShareEnabled</el-button>
    <el-button type="primary" round plain @click="isPlaceVideoNextToShareInRecordEnabled">IsPlaceVideoNextToShareInRecordEnabled</el-button>
  </div>
  <div class="result">Result: {{result}}</div>
  <div class="result"> 
    <div class="result-item">
      <span class="title">onCloudRecordingStorageInfo <el-button size="small" @click="onCloudRecordingStorageInfo_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onCloudRecordingStorageInfo_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const enable_selectRecordFileLocationAfterMeeting = ref(false)
const enable_multiAudioStreamRecord = ref(false)
const enable_addTimestampWatermark = ref(false)
const enable_optimizeFor3rdPartyVideoEditor = ref(false)
const enable_showVideoThumbnailWhenShare = ref(false)
const enable_placeVideoNextToShareInRecord = ref(false)
const result = ref()
const clientHeight = ref((window.innerHeight - 240) + 'px')
const onCloudRecordingStorageInfo_options = ref<string[]>([])

const onCloudRecordingStorageInfo = (storage_total_size: string, storage_used_size: string, allow_exceed_storage: string) => {
  console.log('onCloudRecordingStorageInfo', storage_total_size, storage_used_size, allow_exceed_storage)
  let content = `total_size:${storage_total_size}-used_size:${storage_used_size}-allow_exceed_storage:${allow_exceed_storage}`
  onCloudRecordingStorageInfo_options.value.push(content)
}
const zoomSettingRecording = remote.app.zoomSdkModule.Setting.GetRecordingSetting({onCloudRecordingStorageInfo})

const enableSelectRecordFileLocationAfterMeeting = () => {
  let param = {bEnable: enable_selectRecordFileLocationAfterMeeting.value}
  let ret = zoomSettingRecording.Setting_EnableSelectRecordFileLocationAfterMeeting(param)
  result.value = getSDKResult('Setting_EnableSelectRecordFileLocationAfterMeeting', ret, param)
}

const enableMultiAudioStreamRecord = () => {
  let param = {bEnable: enable_multiAudioStreamRecord.value}
  let ret = zoomSettingRecording.Setting_EnableMultiAudioStreamRecord(param)
  result.value = getSDKResult('Setting_EnableMultiAudioStreamRecord', ret, param)
}

const enableAddTimestampWatermark = () => {
  let param = {bEnable: enable_addTimestampWatermark.value}
  let ret = zoomSettingRecording.Setting_EnableAddTimestampWatermark(param)
  result.value = getSDKResult('Setting_EnableAddTimestampWatermark', ret, param)
}

const enableOptimizeFor3rdPartyVideoEditor = () => {
  let param = {bEnable: enable_optimizeFor3rdPartyVideoEditor.value}
  let ret = zoomSettingRecording.Setting_EnableOptimizeFor3rdPartyVideoEditor(param)
  result.value = getSDKResult('Setting_EnableOptimizeFor3rdPartyVideoEditor', ret, param)
}

const enableShowVideoThumbnailWhenShare = () => {
  let param = {bEnable: enable_showVideoThumbnailWhenShare.value}
  let ret = zoomSettingRecording.Setting_EnableShowVideoThumbnailWhenShare(param)
  result.value = getSDKResult('Setting_EnableShowVideoThumbnailWhenShare', ret, param)
}

const enablePlaceVideoNextToShareInRecord = () => {
  let param = {bEnable: enable_placeVideoNextToShareInRecord.value}
  let ret = zoomSettingRecording.Setting_EnablePlaceVideoNextToShareInRecord(param)
  result.value = getSDKResult('Setting_EnablePlaceVideoNextToShareInRecord', ret, param)
}

const setonCloudRecordingStorageInfoCB = () => {
  let ret = zoomSettingRecording.Setting_SetonCloudRecordingStorageInfoCB(onCloudRecordingStorageInfo)
  result.value = getSDKResult('Setting_SetonCloudRecordingStorageInfoCB', ret)
}

const setRecordingPath = () => {
  remote.dialog.showOpenDialog({
    properties: ['openDirectory'],
  }).then((data: any) => {
    console.log(data);
    let param = {zn_szPath: data.filePaths[0]}
    let ret = zoomSettingRecording.Setting_SetRecordingPath(param)
    result.value = getSDKResult('Setting_SetRecordingPath', ret, param)
  }).catch((err: any) => {
    console.warn(err);
    result.value = getSDKResult('Setting_SetRecordingPath', `${err}`) 
  });
}

const getRecordingPath = () => {
  let ret = zoomSettingRecording.Getting_GetRecordingPath()
  result.value = getSDKResult('Getting_GetRecordingPath', ret)
}

const canGetCloudRecordingStorageInfo = () => {
  let ret = zoomSettingRecording.Setting_CanGetCloudRecordingStorageInfo()
  result.value = getSDKResult('Setting_CanGetCloudRecordingStorageInfo', ret)
}

const getCloudRecordingStorageInfo = () => {
  let ret = zoomSettingRecording.Getting_GetCloudRecordingStorageInfo()
  result.value = getSDKResult('Getting_GetCloudRecordingStorageInfo', ret)
}

const getRecordingManagementURL = () => {
  let ret = zoomSettingRecording.Getting_GetRecordingManagementURL()
  result.value = getSDKResult('Getting_GetRecordingManagementURL', ret)
}

const canGetRecordingManagementURL = () => {
  let ret = zoomSettingRecording.Setting_CanGetRecordingManagementURL()
  result.value = getSDKResult('Setting_CanGetRecordingManagementURL', ret)
}

const isSelectRecordFileLocationAfterMeetingEnabled = () => {
  let ret = zoomSettingRecording.Setting_IsSelectRecordFileLocationAfterMeetingEnabled()
  result.value = getSDKResult('Setting_IsSelectRecordFileLocationAfterMeetingEnabled', ret)
}

const isMultiAudioStreamRecordEnabled = () => {
  let ret = zoomSettingRecording.Setting_IsMultiAudioStreamRecordEnabled()
  result.value = getSDKResult('Setting_IsMultiAudioStreamRecordEnabled', ret)
}

const isAddTimestampWatermarkEnabled = () => {
  let ret = zoomSettingRecording.Setting_IsAddTimestampWatermarkEnabled()
  result.value = getSDKResult('Setting_IsAddTimestampWatermarkEnabled', ret)
}

const isOptimizeFor3rdPartyVideoEditorEnabled = () => {
  let ret = zoomSettingRecording.Setting_IsOptimizeFor3rdPartyVideoEditorEnabled()
  result.value = getSDKResult('Setting_IsOptimizeFor3rdPartyVideoEditorEnabled', ret)
}

const isShowVideoThumbnailWhenShareEnabled = () => {
  let ret = zoomSettingRecording.Setting_IsShowVideoThumbnailWhenShareEnabled()
  result.value = getSDKResult('Setting_IsShowVideoThumbnailWhenShareEnabled', ret)
}

const isPlaceVideoNextToShareInRecordEnabled = () => {
  let ret = zoomSettingRecording.Setting_IsPlaceVideoNextToShareInRecordEnabled()
  result.value = getSDKResult('Setting_IsPlaceVideoNextToShareInRecordEnabled', ret)
}

onMounted(() => {
  window.addEventListener('resize', ()=>{
    clientHeight.value = (window.innerHeight - 240) + 'px'
  })
})
</script>
<style scoped lang="scss">
.setting {
  height: v-bind(clientHeight);
}
</style>