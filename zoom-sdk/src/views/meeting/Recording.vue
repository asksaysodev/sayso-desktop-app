<template>
  <div class="meeting">
    <el-button type="primary" round plain @click="setonRecording2MP4DoneCB">SetonRecording2MP4DoneCB</el-button>
    <el-button type="primary" round plain @click="setonRecording2MP4ProcessingCB">SetonRecording2MP4ProcessingCB</el-button>
    <el-button type="primary" round plain @click="setonRecordingStatusCB">SetonRecordingStatusCB</el-button>
    <el-button type="primary" round plain @click="setonCloudRecordingStatusCB">SetonCloudRecordingStatusCB</el-button>
    <el-button type="primary" round plain @click="setonRecordPrivilegeChangedCB">SetonRecordPrivilegeChangedCB</el-button>
    <el-button type="primary" round plain @click="setonCloudRecordingStorageFullCB">SetonCloudRecordingStorageFullCB</el-button>
    <el-button type="primary" round plain @click="setonRequestCloudRecordingResponseCB">SetonRequestCloudRecordingResponseCB</el-button>
    <el-button type="primary" round plain @click="setonStartCloudRecordingRequestedCB">SetonStartCloudRecordingRequestedCB</el-button>
    <el-button type="primary" round plain @click="setonEnableAndStartSmartRecordingRequestedCB">SetonEnableAndStartSmartRecordingRequestedCB</el-button>
    <el-button type="primary" round plain @click="setonSmartRecordingEnableActionCallback">SetonSmartRecordingEnableActionCallback</el-button>
    <el-button type="primary" round plain @click="startRecording">StartRecording</el-button>
    <el-button type="primary" round plain @click="stopRecording">StopRecording</el-button>
    <el-button type="primary" round plain @click="canAllowDisAllowLocalRecording">CanAllowDisAllowLocalRecording</el-button>
    <el-button type="primary" round plain @click="startCloudRecording">StartCloudRecording</el-button>
    <el-button type="primary" round plain @click="stopCloudRecording">StopCloudRecording</el-button>
    <div class="in-line">
      <el-select v-model="participants_value" placeholder="Select" @focus="getParticipantsList">
        <el-option v-for="(item, index) in participantsList" :key="index" :label="item.userName" :value="item.userID"></el-option>
      </el-select>
      <el-checkbox v-model="cloud_recording" label="Cloud Recording" @change="canStartRecording"></el-checkbox>
      <el-button type="primary" round plain @click="canStartRecording">CanStartRecording</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="participants_value" placeholder="Select" @focus="getParticipantsList">
        <el-option v-for="(item, index) in participantsList" :key="index" :label="item.userName" :value="item.userID"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="isSupportLocalRecording">IsSupportLocalRecording</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="participants_value" placeholder="Select" @focus="getParticipantsList">
        <el-option v-for="(item, index) in participantsList" :key="index" :label="item.userName" :value="item.userID"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="allowLocalRecording">AllowLocalRecording</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="participants_value" placeholder="Select" @focus="getParticipantsList">
        <el-option v-for="(item, index) in participantsList" :key="index" :label="item.userName" :value="item.userID"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="disAllowLocalRecording">DisAllowLocalRecording</el-button>
    </div>
    <el-button type="primary" round plain @click="requestStartCloudRecording">RequestStartCloudRecording</el-button>
    <div class="in-line">
      <el-button type="primary" round plain @click="canEnableSmartRecordingFeature">CanEnableSmartRecordingFeature</el-button>
      <el-button type="primary" round plain @click="enableSmartRecording">EnableSmartRecording</el-button>
      <el-button type="primary" round plain @click="isSmartRecordingEnabled">IsSmartRecordingEnabled</el-button>
    </div>
  </div>
  <div>
    <div class="result">Result: {{result}}</div>
    <div class="result"> 
      <div class="result-item"> 
        <span class="title">onRecording2MP4Done <el-button size="small" @click="onRecording2MP4Done_options=[]">Clear</el-button></span> 
        <div class="callback-list">
          <span v-for="(item, index) in onRecording2MP4Done_options" :key="index">{{index}} : {{item}}</span>
        </div>
      </div>
      <div class="result-item"> 
        <span class="title">onRecording2MP4Processing <el-button size="small" @click="onRecording2MP4Processing_options=[]">Clear</el-button></span> 
        <div class="callback-list">
          <span v-for="(item, index) in onRecording2MP4Processing_options" :key="index">{{index}} : {{item}}</span>
        </div>
      </div>
      <div class="result-item"> 
        <span class="title">onRecordingStatus <el-button size="small" @click="onRecordingStatus_options=[]">Clear</el-button></span> 
        <div class="callback-list">
          <span v-for="(item, index) in onRecordingStatus_options" :key="index">{{index}} : {{item}}</span>
        </div>
      </div>
      <div class="result-item"> 
        <span class="title">onCloudRecordingStatus <el-button size="small" @click="onCloudRecordingStatus_options=[]">Clear</el-button></span> 
        <div class="callback-list">
          <span v-for="(item, index) in onCloudRecordingStatus_options" :key="index">{{index}} : {{item}}</span>
        </div>
      </div>
      <div class="result-item"> 
        <span class="title">onRecordPrivilegeChanged <el-button size="small" @click="onRecordPrivilegeChanged_options=[]">Clear</el-button></span> 
        <div class="callback-list">
          <span v-for="(item, index) in onRecordPrivilegeChanged_options" :key="index">{{index}} : {{item}}</span>
        </div>  
      </div>
      <div class="result-item"> 
        <span class="title">onCloudRecordingStorageFull <el-button size="small" @click="onCloudRecordingStorageFull_options=[]">Clear</el-button></span> 
        <div class="callback-list">
          <span v-for="(item, index) in onCloudRecordingStorageFull_options" :key="index">{{index}} : {{item}}</span>
        </div>  
      </div>
      <div class="result-item"> 
        <span class="title">onRequestCloudRecordingResponse <el-button size="small" @click="onRequestCloudRecordingResponse_options=[]">Clear</el-button></span> 
        <div class="callback-list">
          <span v-for="(item, index) in onRequestCloudRecordingResponse_options" :key="index">{{index}} : {{item}}</span>
        </div>  
      </div>
      <div class="result-item"> 
        <span class="title">onStartCloudRecordingRequested <el-button size="small" @click="onStartCloudRecordingRequested_value=0">Clear</el-button></span> 
        <span v-if="onStartCloudRecordingRequested_value">updated {{onStartCloudRecordingRequested_value}}</span> 
        <div>
          <el-button type="primary" size="small" round plain @click="getRequesterId">GetRequesterId</el-button>
          <el-button type="primary" size="small" round plain @click="getRequesterName">GetRequesterName</el-button>
          <el-button type="primary" size="small" round plain @click="start">Start</el-button>
          <el-button type="primary" size="small" round plain @click="deny(false)">Deny</el-button>
          <el-button type="primary" size="small" round plain @click="deny(true)">Deny All</el-button>
        </div>
      </div>
      <div class="result-item"> 
        <span class="title">onEnableAndStartSmartRecordingRequested <el-button size="small" @click="onEnableAndStartSmartRecordingRequested_value=0">Clear</el-button></span> 
        <span v-if="onEnableAndStartSmartRecordingRequested_value">updated {{onEnableAndStartSmartRecordingRequested_value}}</span> 
        <div>
          <el-button type="primary" size="small" round plain @click="getRequestEnableAndStartSmartRecordingUserId">GetRequestUserId</el-button>
          <el-button type="primary" size="small" round plain @click="getRequestEnableAndStartSmartRecordingTipString">GetTipString</el-button>
          <el-button type="primary" size="small" round plain @click="startCloudRecordingWithoutEnableSmartRecording">StartCloudRecordingWithoutEnableSmartRecording</el-button>
          <el-button type="primary" size="small" round plain @click="agreeToEnableAndStartSmartRecording(false)">AgreeToEnableAndStart</el-button>
          <el-button type="primary" size="small" round plain @click="agreeToEnableAndStartSmartRecording(true)">AgreeToEnableAndStart All</el-button>
          <el-button type="primary" size="small" round plain @click="declineEnableAndStartSmartRecording(false)">Decline</el-button>
          <el-button type="primary" size="small" round plain @click="declineEnableAndStartSmartRecording(true)">Decline All</el-button>
        </div>
      </div>
      <div class="result-item"> 
        <span class="title">onSmartRecordingEnableActionCallback <el-button size="small" @click="onSmartRecordingEnableActionCallback_value=0">Clear</el-button></span> 
        <span v-if="onSmartRecordingEnableActionCallback_value">updated {{onSmartRecordingEnableActionCallback_value}}</span> 
        <div>
          <el-button type="primary" size="small" round plain @click="getSmartRecordingEnableActionTipString">GetTipString</el-button>
          <el-button type="primary" size="small" round plain @click="actionConfirmEnableSmartRecording(false)">ActionConfirm</el-button>
          <el-button type="primary" size="small" round plain @click="actionConfirmEnableSmartRecording(true)">ActionConfirm All</el-button>
          <el-button type="primary" size="small" round plain @click="actionCancelEnableSmartRecording">ActionCancel</el-button>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { RecordingStatus } from '../../../lib/settings.js'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const result = ref()
const clientHeight = ref((window.innerHeight - 495) + 'px')
const onRecording2MP4Done_options = ref<any[]>([])
const onRecording2MP4Processing_options = ref<any[]>([])
const onRecordingStatus_options = ref<any[]>([])
const onCloudRecordingStatus_options = ref<any[]>([])
const onRecordPrivilegeChanged_options = ref<any[]>([])
const onCloudRecordingStorageFull_options = ref<string[]>([])
const onRequestCloudRecordingResponse_options = ref<string[]>([])
const onStartCloudRecordingRequested_value = ref(0)
const onEnableAndStartSmartRecordingRequested_value = ref(0)
const onSmartRecordingEnableActionCallback_value = ref(0)

const onRecording2MP4Done = (success: boolean, result: string, path: string) => {
  console.log('onRecording2MP4Done', success, result, path)
  let content = `${success} - ${result} - ${path}`
  onRecording2MP4Done_options.value.push(content)
}
const onRecording2MP4Processing = (percentage: string) => {
  console.log('onRecording2MP4Processing', percentage)
  onRecording2MP4Processing_options.value.push(percentage)
}
const onRecordingStatus = (status: string) => {
  let statusContent = Object.keys(RecordingStatus).find(k => RecordingStatus[k] == status)
  console.log('onRecordingStatus', status, statusContent)
  onRecordingStatus_options.value.push(statusContent)
}
const onCloudRecordingStatus = (status: string) => {
  let statusContent = Object.keys(RecordingStatus).find(k => RecordingStatus[k] == status)
  console.log('onCloudRecordingStatus', status, statusContent)
  onCloudRecordingStatus_options.value.push(statusContent)
}
const onRecordPrivilegeChanged = (canRec: boolean) => {
  console.log('onRecordPrivilegeChanged', canRec)
  onRecordPrivilegeChanged_options.value.push(canRec)
}
const onCloudRecordingStorageFull = (gracePeriodDate: number) => {
  console.log('onCloudRecordingStorageFull', gracePeriodDate)
  onCloudRecordingStorageFull_options.value.push(`gracePeriodDate: ${gracePeriodDate}`)
}
const onRequestCloudRecordingResponse = (status: number) => {
  console.log('onRequestCloudRecordingResponse', status)
  onRequestCloudRecordingResponse_options.value.push(`status: ${status}`)
}
const onStartCloudRecordingRequested = () => {
  console.log('onStartCloudRecordingRequested')
  onStartCloudRecordingRequested_value.value++
}
const onEnableAndStartSmartRecordingRequested = () => {
  console.log('onEnableAndStartSmartRecordingRequested')
  onEnableAndStartSmartRecordingRequested_value.value++
}
const onSmartRecordingEnableActionCallback = () => {
  console.log('onSmartRecordingEnableActionCallback')
  onSmartRecordingEnableActionCallback_value.value++
}
const zoomMeetingRecording = remote.app.zoomSdkModule.Meeting.GetMeetingRecording({ onRecording2MP4Done, onRecording2MP4Processing, onRecordingStatus,
  onCloudRecordingStatus, onRecordPrivilegeChanged, onCloudRecordingStorageFull, onRequestCloudRecordingResponse, onStartCloudRecordingRequested,
  onEnableAndStartSmartRecordingRequested, onSmartRecordingEnableActionCallback })
const zoomMeetingParticipants = remote.app.zoomSdkModule.Meeting.GetMeetingParticipantsCtrl()
const participantsList = ref<any[]>([])
const participants_value = ref()
const cloud_recording = ref(true)

const setonRecording2MP4DoneCB = () => {
  let ret = zoomMeetingRecording.SetonRecording2MP4DoneCB(onRecording2MP4Done)
  result.value = getSDKResult('SetonRecording2MP4DoneCB', ret)
}

const setonRecording2MP4ProcessingCB = () => {
  let ret = zoomMeetingRecording.SetonRecording2MP4ProcessingCB(onRecording2MP4Processing)
  result.value = getSDKResult('SetonRecording2MP4ProcessingCB', ret)
}

const setonRecordingStatusCB = () => {
  let ret = zoomMeetingRecording.SetonRecordingStatusCB(onRecordingStatus)
  result.value = getSDKResult('SetonRecordingStatusCB', ret)
}

const setonCloudRecordingStatusCB = () => {
  let ret = zoomMeetingRecording.SetonCloudRecordingStatusCB(onCloudRecordingStatus)
  result.value = getSDKResult('SetonRecordingStatusCB', ret)
}

const setonRecordPrivilegeChangedCB = () => {
  let ret = zoomMeetingRecording.SetonRecordPrivilegeChangedCB(onRecordPrivilegeChanged)
  result.value = getSDKResult('SetonRecordPrivilegeChangedCB', ret)
}

const setonCloudRecordingStorageFullCB = () => {
  let ret = zoomMeetingRecording.SetonCloudRecordingStorageFullCB(onCloudRecordingStorageFull)
  result.value = getSDKResult('SetonCloudRecordingStorageFullCB', ret)
}

const setonRequestCloudRecordingResponseCB = () => {
  let ret = zoomMeetingRecording.SetonRequestCloudRecordingResponseCB(onRequestCloudRecordingResponse)
  result.value = getSDKResult('SetonRequestCloudRecordingResponseCB', ret)
}

const setonStartCloudRecordingRequestedCB = () => {
  let ret = zoomMeetingRecording.SetonStartCloudRecordingRequestedCB(onStartCloudRecordingRequested)
  result.value = getSDKResult('SetonStartCloudRecordingRequestedCB', ret)
}

const setonEnableAndStartSmartRecordingRequestedCB = () => {
  let ret = zoomMeetingRecording.SetonEnableAndStartSmartRecordingRequestedCB(onEnableAndStartSmartRecordingRequested)
  result.value = getSDKResult('SetonEnableAndStartSmartRecordingRequestedCB', ret)
}

const setonSmartRecordingEnableActionCallback = () => {
  let ret = zoomMeetingRecording.SetonSmartRecordingEnableActionCallback(onSmartRecordingEnableActionCallback)
  result.value = getSDKResult('SetonSmartRecordingEnableActionCallback', ret)
}

const startRecording = () => {
  let ret = zoomMeetingRecording.StartRecording()
  result.value = getSDKResult('StartRecording', ret)
}

const stopRecording = () => {
  let ret = zoomMeetingRecording.StopRecording()
  result.value = getSDKResult('StopRecording', ret)
}

const getParticipantsList = () => {
  let list = zoomMeetingParticipants.GetParticipantsList()
  result.value = getSDKResult('GetParticipantsList', list)
  participantsList.value = []
  let obj
  for (let item of list) {
    obj = zoomMeetingParticipants.GetUserInfoByUserID(item.userid)
    participantsList.value.push(obj)
  }
  if (participantsList.value.length == 0) {
    participants_value.value = ''
  } else {
    participants_value.value = obj.userID
  }
}

const canStartRecording = () => {
  let param = {userid: participants_value.value, cloud_recording: cloud_recording.value}
  let ret = zoomMeetingRecording.CanStartRecording(param)
  result.value = getSDKResult('CanStartRecording', ret, param)
}

const canAllowDisAllowLocalRecording = () => {
  let ret = zoomMeetingRecording.CanAllowDisAllowLocalRecording()
  result.value = getSDKResult('CanAllowDisAllowLocalRecording', ret)
}

const startCloudRecording = () => {
  let ret = zoomMeetingRecording.StartCloudRecording()
  result.value = getSDKResult('StartCloudRecording', ret)
}

const stopCloudRecording = () => {
  let ret = zoomMeetingRecording.StopCloudRecording()
  result.value = getSDKResult('StopCloudRecording', ret)
}

const isSupportLocalRecording = () => {
  let param = {userid: participants_value.value}
  let ret = zoomMeetingRecording.IsSupportLocalRecording(param)
  result.value = getSDKResult('IsSupportLocalRecording', ret, param)
}

const allowLocalRecording = () => {
  let param = {userid: participants_value.value}
  let ret = zoomMeetingRecording.AllowLocalRecording(param)
  result.value = getSDKResult('AllowLocalRecording', ret, param)
}

const disAllowLocalRecording = () => {
  let param = {userid: participants_value.value}
  let ret = zoomMeetingRecording.DisAllowLocalRecording(param)
  result.value = getSDKResult('DisAllowLocalRecording', ret, param)
}

const requestStartCloudRecording = () => {
  let ret = zoomMeetingRecording.RequestStartCloudRecording()
  result.value = getSDKResult('RequestStartCloudRecording', ret)
}

const canEnableSmartRecordingFeature = () => {
  let ret = zoomMeetingRecording.CanEnableSmartRecordingFeature()
  result.value = getSDKResult('CanEnableSmartRecordingFeature', ret)
}

const enableSmartRecording = () => {
  let ret = zoomMeetingRecording.EnableSmartRecording()
  result.value = getSDKResult('EnableSmartRecording', ret)
}

const isSmartRecordingEnabled = () => {
  let ret = zoomMeetingRecording.IsSmartRecordingEnabled()
  result.value = getSDKResult('IsSmartRecordingEnabled', ret)
}

const getRequesterId = () => {
  let ret = zoomMeetingRecording.GetRequesterId()
  result.value = getSDKResult('GetRequesterId', ret, null, false)
}

const getRequesterName = () => {
  let ret = zoomMeetingRecording.GetRequesterName()
  result.value = getSDKResult('GetRequesterName', ret)
}

const start = () => {
  let ret = zoomMeetingRecording.Start()
  result.value = getSDKResult('Start', ret)
}

const deny = (bDenyAll: boolean) => {
  let param = { bDenyAll }
  let ret = zoomMeetingRecording.Deny(param)
  result.value = getSDKResult('Deny', ret, param)
}

const getRequestEnableAndStartSmartRecordingUserId = () => {
  let ret = zoomMeetingRecording.GetRequestEnableAndStartSmartRecordingUserId()
  result.value = getSDKResult('GetRequestEnableAndStartSmartRecordingUserId', ret, null, false)
}

const getRequestEnableAndStartSmartRecordingTipString = () => {
  let ret = zoomMeetingRecording.GetRequestEnableAndStartSmartRecordingTipString()
  result.value = getSDKResult('GetRequestEnableAndStartSmartRecordingTipString', ret)
}

const startCloudRecordingWithoutEnableSmartRecording = () => {
  let ret = zoomMeetingRecording.StartCloudRecordingWithoutEnableSmartRecording()
  result.value = getSDKResult('StartCloudRecordingWithoutEnableSmartRecording', ret)
}

const agreeToEnableAndStartSmartRecording = (bAllMeetings: boolean) => {
  let param = { bAllMeetings }
  let ret = zoomMeetingRecording.AgreeToEnableAndStartSmartRecording(param)
  result.value = getSDKResult('AgreeToEnableAndStartSmartRecording', ret, param)
}

const declineEnableAndStartSmartRecording = (bDenyAll: boolean) => {
  let param = { bDenyAll }
  let ret = zoomMeetingRecording.DeclineEnableAndStartSmartRecording(param)
  result.value = getSDKResult('DeclineEnableAndStartSmartRecording', ret, param)
}

const getSmartRecordingEnableActionTipString = () => {
  let ret = zoomMeetingRecording.GetSmartRecordingEnableActionTipString()
  result.value = getSDKResult('GetSmartRecordingEnableActionTipString', ret)
}

const actionConfirmEnableSmartRecording = (bAllMeetings: boolean) => {
  let param = { bAllMeetings }
  let ret = zoomMeetingRecording.ActionConfirmEnableSmartRecording(param)
  result.value = getSDKResult('ActionConfirmEnableSmartRecording', ret, param)
}

const actionCancelEnableSmartRecording = () => {
  let ret = zoomMeetingRecording.ActionCancelEnableSmartRecording()
  result.value = getSDKResult('ActionCancelEnableSmartRecording', ret)
}

onMounted(() => {
  getParticipantsList()
  window.addEventListener('resize', () => {
    clientHeight.value = (window.innerHeight - 495) + 'px'
  })
})
</script>
<style scoped lang="scss">
.meeting {
  height: v-bind(clientHeight);
}
.result {
  .title {
    width: 340px;
  }
}
</style>