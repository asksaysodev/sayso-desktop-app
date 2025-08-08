<template>
  <div class="meeting">
    <el-button type="primary" round plain @click="setDirectShareStatusUpdateCB">SetDirectShareStatusUpdateCB</el-button>
    <el-button type="primary" round plain @click="canStartDirectShare">CanStartDirectShare</el-button>
    <el-button type="primary" round plain @click="isDirectShareInProgress">IsDirectShareInProgress</el-button>
    <el-button type="primary" round plain @click="startDirectShare">StartDirectShare</el-button>
    <el-button type="primary" round plain @click="stopDirectShare">StopDirectShare</el-button>
    <el-button type="primary" round plain @click="cancel">Cancel</el-button>
    <div class="in-line">
      <el-input v-model.trim="meetingNumber" placeholder="input meetingNumber" @keyup.enter.native="tryWithMeetingNumber" />
      <el-button type="primary" round plain @click="tryWithMeetingNumber">TryWithMeetingNumber</el-button>
    </div>
    <div class="in-line">
      <el-input v-model.trim="pairingCode" placeholder="input pairingCode" @keyup.enter.native="tryWithPairingCode" />
      <el-button type="primary" round plain @click="tryWithPairingCode">TryWithPairingCode</el-button>
    </div>
  </div>
  <div class="result">Result: {{result}}</div>
  <div class="result"> 
    <span class="title">OnDirectShareStatusUpdate <el-button size="small" @click="OnDirectShareStatusUpdate_options=[]">Clear</el-button></span>
    <div class="callback-list">
      <span v-for="(item, index) in OnDirectShareStatusUpdate_options" :key="index">{{index}} : {{item}}</span>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import { DirectShareStatus } from '../../../lib/settings.js'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const meetingNumber = ref()
const pairingCode = ref()
const result = ref()
const OnDirectShareStatusUpdate_options = ref<any[]>([])

const OnDirectShareStatusUpdate = (status: any) => {
  console.log(status.DirectShareStatus)
  let statusContent = Object.keys(DirectShareStatus).find(k => DirectShareStatus[k] == status.DirectShareStatus)
  OnDirectShareStatusUpdate_options.value.push(statusContent)
}
const zoomDirectShare = remote.app.zoomSdkModule.Auth.GetDirectShare({OnDirectShareStatusUpdate})

const setDirectShareStatusUpdateCB = () => {
  let ret = zoomDirectShare.SetDirectShareStatusUpdateCB(OnDirectShareStatusUpdate)
  result.value = getSDKResult('SetDirectShareStatusUpdateCB', ret)
}

const canStartDirectShare = () => {
  let ret = zoomDirectShare.CanStartDirectShare()
  result.value = getSDKResult('CanStartDirectShare', ret)
}

const isDirectShareInProgress = () => {
  let ret = zoomDirectShare.IsDirectShareInProgress()
  result.value = getSDKResult('IsDirectShareInProgress', ret)
}

const startDirectShare = () => {
  let ret = zoomDirectShare.StartDirectShare();
  result.value = getSDKResult('StartDirectShare', ret)
}

const stopDirectShare = () => {
  let ret = zoomDirectShare.StopDirectShare()
  result.value = getSDKResult('StopDirectShare', ret)
}

const tryWithMeetingNumber = () => {
  let param = {meetingNumber: meetingNumber.value}
  let ret = zoomDirectShare.TryWithMeetingNumber(param);
  result.value = getSDKResult('StopDirectShare', ret, param)
}

const tryWithPairingCode = () => {
  let param = {pairingCode: pairingCode.value}
  let ret = zoomDirectShare.TryWithPairingCode(param);
  result.value = getSDKResult('TryWithPairingCode', ret, param)
}

const cancel = () => {
  let ret = zoomDirectShare.Cancel()
  result.value = getSDKResult('Cancel', ret)
}
</script>