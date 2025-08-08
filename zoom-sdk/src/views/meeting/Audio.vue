<template>
  <div class="meeting">
    <el-button type="primary" round plain @click="setMeetingAudioStatusCB">SetMeetingAudioStatusCB</el-button>
    <el-button type="primary" round plain @click="setUserActiveAudioChangeCB">SetUserActiveAudioChangeCB</el-button>
    <el-button type="primary" round plain @click="setOnMuteOnEntryStatusChangeCB">SetOnMuteOnEntryStatusChangeCB</el-button>
    <el-button type="primary" round plain @click="joinVoip">JoinVoip</el-button>
    <el-button type="primary" round plain @click="leaveVoip">LeaveVoip</el-button>
    <div class="in-line">
      <el-select v-model="participants_value" placeholder="Select" @focus="getParticipantsList">
        <el-option v-for="(item, index) in participantsList" :key="index" :label="item.userName" :value="item.userID"></el-option>
      </el-select>
      <el-checkbox v-model="allow_unmutebyself" label="Allowunmutebyself" @change="muteAudio"></el-checkbox>
      <el-button type="primary" round plain @click="muteAudio">MuteAudio</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="participants_value" placeholder="Select" @focus="getParticipantsList">
        <el-option v-for="(item, index) in participantsList" :key="index" :label="item.userName" :value="item.userID"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="unMuteAudio">UnMuteAudio</el-button>
    </div>
    <div class="in-line">
      <el-checkbox v-model="enable_muteOnEntry" label="EnableHardwareEncode" @change="enableMuteOnEntry"></el-checkbox>
      <el-checkbox v-model="allow_unmutebyself" label="AllowUnmuteBySelf" @change="enableMuteOnEntry"></el-checkbox>
      <el-button type="primary" round plain @click="enableMuteOnEntry">EnableMuteOnEntry</el-button>
      <el-button type="primary" round plain @click="canEnableMuteOnEntry">CanEnableMuteOnEntry</el-button>
      <el-button type="primary" round plain @click="isMuteOnEntryEnabled">IsMuteOnEntryEnabled</el-button>
    </div>
    <el-checkbox v-model="enable_playChimeWhenEnterOrExit" label="EnablePlayChimeWhenEnterOrExit" @change="enablePlayChimeWhenEnterOrExit"></el-checkbox>
    <div class="in-line">
      <el-checkbox v-model="enable_playMeetingAudio" label="EnablePlayMeetingAudio" @change="enablePlayMeetingAudio"></el-checkbox>
      <el-button type="primary" round plain @click="isPlayMeetingAudioEnabled">IsPlayMeetingAudioEnabled</el-button>
    </div>
  </div>
  <div class="result">Result: {{result}}</div>
  <div class="result"> 
    <div class="result-item"> 
      <span class="title">onUserAudioStatusChange <el-button size="small" @click="onUserAudioStatusChange_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onUserAudioStatusChange_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onUserActiveAudioChange <el-button size="small" @click="onUserActiveAudioChange_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onUserActiveAudioChange_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onMuteOnEntryStatusChange <el-button size="small" @click="onMuteOnEntryStatusChange_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onMuteOnEntryStatusChange_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { ZoomMeetingAudioStatus } from '../../../lib/settings.js'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const result = ref()
const onUserAudioStatusChange_options = ref<any[]>([])
const onUserActiveAudioChange_options = ref<any[]>([])
const onMuteOnEntryStatusChange_options = ref<string[]>([])

const onUserAudioStatusChange = (audioStatusList: any) => {
  console.log('onUserAudioStatusChange', audioStatusList)
  for (let item of audioStatusList) {
    let content = `${item.userid} - ${Object.keys(ZoomMeetingAudioStatus).find(k => ZoomMeetingAudioStatus[k] == item.audioStauts)}`
    onUserAudioStatusChange_options.value.push(content)
  }
}
const onUserActiveAudioChange = (lstActiveAudio: any) => {
  console.log('onUserActiveAudioChange', lstActiveAudio)
  onUserActiveAudioChange_options.value = lstActiveAudio
}
const onMuteOnEntryStatusChange = (bEnabled: boolean) => {
  console.log('onMuteOnEntryStatusChange', bEnabled)
  onMuteOnEntryStatusChange_options.value.push(`bEnabled: ${bEnabled}`)
}
const zoomMeetingAudio = remote.app.zoomSdkModule.Meeting.GetMeetingAudio({ onUserAudioStatusChange, onUserActiveAudioChange, onMuteOnEntryStatusChange })
const zoomMeetingParticipants = remote.app.zoomSdkModule.Meeting.GetMeetingParticipantsCtrl()
const participantsList = ref<any[]>([])
const participants_value = ref()
const allow_unmutebyself = ref(true)

const setMeetingAudioStatusCB = () => {
  let ret = zoomMeetingAudio.MeetingAudio_SetMeetingAudioStatusCB(onUserAudioStatusChange)
  result.value = getSDKResult('MeetingAudio_SetMeetingAudioStatusCB', ret)
}

const setUserActiveAudioChangeCB = () => {
  let ret = zoomMeetingAudio.MeetingAudio_SetUserActiveAudioChangeCB(onUserActiveAudioChange)
  result.value = getSDKResult('MeetingAudio_SetUserActiveAudioChangeCB', ret)
}

const setOnMuteOnEntryStatusChangeCB = () => {
  let ret = zoomMeetingAudio.MeetingAudio_SetOnMuteOnEntryStatusChangeCB(onMuteOnEntryStatusChange)
  result.value = getSDKResult('MeetingAudio_SetOnMuteOnEntryStatusChangeCB', ret)
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

const muteAudio = () => {
  let param = { userid: participants_value.value, allowunmutebyself: allow_unmutebyself.value }
  let ret = zoomMeetingAudio.MeetingAudio_MuteAudio(param)
  result.value = getSDKResult('MeetingAudio_MuteAudio', ret, param)
}

const unMuteAudio = () => {
  let param = {userid: participants_value.value}
  let ret = zoomMeetingAudio.MeetingAudio_UnMuteAudio(param)
  result.value = getSDKResult('MeetingAudio_UnMuteAudio', ret, param)
}

const joinVoip = () => {
  let ret = zoomMeetingAudio.MeetingAudio_JoinVoip()
  result.value = getSDKResult('MeetingAudio_JoinVoip', ret)
}

const leaveVoip = () => {
  let ret = zoomMeetingAudio.MeetingAudio_LeaveVoip()
  result.value = getSDKResult('MeetingAudio_LeaveVoip', ret)
}

const canEnableMuteOnEntry = () => {
  let ret = zoomMeetingAudio.MeetingAudio_CanEnableMuteOnEntry()
  result.value = getSDKResult('MeetingAudio_CanEnableMuteOnEntry', ret)
}

const isMuteOnEntryEnabled = () => {
  let ret = zoomMeetingAudio.MeetingAudio_IsMuteOnEntryEnabled()
  result.value = getSDKResult('MeetingAudio_IsMuteOnEntryEnabled', ret)
}

const enable_muteOnEntry = ref(false)
const enableMuteOnEntry = () => {
  let param = { bEnable: enable_muteOnEntry.value, allowUnmuteBySelf: allow_unmutebyself.value }
  let ret = zoomMeetingAudio.MeetingAudio_EnableMuteOnEntry(param)
  result.value = getSDKResult('MeetingAudio_EnableMuteOnEntry', ret, param)
}

const enable_playChimeWhenEnterOrExit = ref(true)
const enablePlayChimeWhenEnterOrExit = () => {
  let param = { bEnable: enable_playChimeWhenEnterOrExit.value }
  let ret = zoomMeetingAudio.MeetingAudio_EnablePlayChimeWhenEnterOrExit(param)
  result.value = getSDKResult('MeetingAudio_EnablePlayChimeWhenEnterOrExit', ret, param)
}

const enable_playMeetingAudio = ref(true)
const enablePlayMeetingAudio = () => {
  let param = { bEnable: enable_playMeetingAudio.value }
  let ret = zoomMeetingAudio.MeetingAudio_EnablePlayMeetingAudio(param)
  result.value = getSDKResult('MeetingAudio_EnablePlayMeetingAudio', ret, param)
}

const isPlayMeetingAudioEnabled = () => {
  let ret = zoomMeetingAudio.MeetingAudio_IsPlayMeetingAudioEnabled()
  result.value = getSDKResult('MeetingAudio_IsPlayMeetingAudioEnabled', ret)
}

onMounted(() => {
  getParticipantsList()
})
</script>