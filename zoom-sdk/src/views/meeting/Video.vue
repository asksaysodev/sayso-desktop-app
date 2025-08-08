<template>
  <div class="meeting">
    <el-button type="primary" round plain @click="setMeetingVideoStatusCB">SetMeetingVideoStatusCB</el-button>
    <el-button type="primary" round plain @click="setActiveSpeakerVideoUserChangedCB">SetActiveSpeakerVideoUserChangedCB</el-button>
    <el-button type="primary" round plain @click="setActiveVideoUserChangedCB">SetActiveVideoUserChangedCB</el-button>
    <div class="in-line">
      <el-select v-model="participants_value" placeholder="Select" @focus="getParticipantsList">
        <el-option v-for="(item, index) in participantsList" :key="index" :label="item.userName" :value="item.userID"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="muteVideo">MuteVideo</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="participants_value" placeholder="Select" @focus="getParticipantsList">
        <el-option v-for="(item, index) in participantsList" :key="index" :label="item.userName" :value="item.userID"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="unMuteVideo">UnMuteVideo</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="participants_value" placeholder="Select" @focus="getParticipantsList">
        <el-option v-for="(item, index) in participantsList" :key="index" :label="item.userName" :value="item.userID"></el-option>
      </el-select>
      <el-checkbox v-model="pin" label="Pin" @change="pinVideo"></el-checkbox>
      <el-checkbox v-model="firstView" label="FirstView" @change="pinVideo"></el-checkbox>
      <el-button type="primary" round plain @click="pinVideo">PinVideo</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="participants_value" placeholder="Select" @focus="getParticipantsList">
        <el-option v-for="(item, index) in participantsList" :key="index" :label="item.userName" :value="item.userID"></el-option>
      </el-select>
      <el-checkbox v-model="spotlight" label="Spotlight" @change="spotlightVideo"></el-checkbox>
      <el-button type="primary" round plain @click="spotlightVideo">SpotlightVideo</el-button>
    </div>
    <el-checkbox v-model="hide" label="HideOrShowNoVideoUserOnVideoWall" @change="hideOrShowNoVideoUserOnVideoWall"></el-checkbox>
    <div class="in-line">
      <el-select v-model="videoPreference_mode" placeholder="Select">
        <el-option v-for="(item, key) in SDKVideoPreferenceMode" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-input-number v-model="minimumFrameRate" :min="0" :max="30" />
      <el-input-number v-model="maximumFrameRate" :min="0" :max="30" />
      <el-button type="primary" round plain @click="setVideoQualityPreference">SetVideoQualityPreference</el-button>
    </div>
  </div>
  <div class="result">Result: {{result}}</div>
  <div class="result"> 
    <div class="result-item"> 
      <span class="title">onUserVideoStatusChange <el-button size="small" @click="onUserVideoStatusChange_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onUserVideoStatusChange_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onActiveSpeakerVideoUserChanged <el-button size="small" @click="onActiveSpeakerVideoUserChanged_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onActiveSpeakerVideoUserChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onActiveVideoUserChanged <el-button size="small" @click="onActiveVideoUserChanged_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onActiveVideoUserChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { ZoomMeetingVideoStatus, SDKVideoPreferenceMode } from '../../../lib/settings.js'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const result = ref()
const onUserVideoStatusChange_options = ref<any[]>([])
const onActiveSpeakerVideoUserChanged_options = ref<any[]>([])
const onActiveVideoUserChanged_options = ref<any[]>([])

const onUserVideoStatusChange = (status: any) => {
  console.log('onUserVideoStatusChange', status)
  let content = `${status.userId} - ${Object.keys(ZoomMeetingVideoStatus).find(k => ZoomMeetingVideoStatus[k] == status.videoStatus)}`
  onUserVideoStatusChange_options.value.push(content)
}
const onActiveSpeakerVideoUserChanged = (userId: number) => {
  console.log('onActiveSpeakerVideoUserChanged', userId)
  onActiveSpeakerVideoUserChanged_options.value.push(userId)
}
const onActiveVideoUserChanged = (userId: number) => {
  console.log('onActiveVideoUserChanged', userId)
  onActiveVideoUserChanged_options.value.push(userId)
}
const zoomMeetingVideo = remote.app.zoomSdkModule.Meeting.GetMeetingVideo({onUserVideoStatusChange, onActiveSpeakerVideoUserChanged, onActiveVideoUserChanged})
const zoomMeetingParticipants = remote.app.zoomSdkModule.Meeting.GetMeetingParticipantsCtrl()
const participantsList = ref<any[]>([])
const participants_value = ref()
const pin = ref(true)
const firstView = ref(true)
const spotlight = ref(true)
const hide = ref(true)

const setMeetingVideoStatusCB = () => {
  let ret = zoomMeetingVideo.MeetingVideo_SetMeetingVideoStatusCB(onUserVideoStatusChange)
  result.value = getSDKResult('MeetingVideo_SetMeetingVideoStatusCB', ret)
}

const setActiveSpeakerVideoUserChangedCB = () => {
  let ret = zoomMeetingVideo.MeetingVideo_SetActiveSpeakerVideoUserChangedCB(onActiveSpeakerVideoUserChanged)
  result.value = getSDKResult('MeetingVideo_SetActiveSpeakerVideoUserChangedCB', ret)
}

const setActiveVideoUserChangedCB = () => {
  let ret = zoomMeetingVideo.MeetingVideo_SetActiveVideoUserChangedCB(onActiveVideoUserChanged)
  result.value = getSDKResult('MeetingVideo_SetActiveVideoUserChangedCB', ret)
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

const muteVideo = () => {
  let param = {userid: participants_value.value}
  let ret = zoomMeetingVideo.MeetingVideo_MuteVideo(param)
  result.value = getSDKResult('MeetingVideo_MuteVideo', ret, param)
}

const unMuteVideo = () => {
  let param = {userid: participants_value.value}
  let ret = zoomMeetingVideo.MeetingVideo_UnMuteVideo(param)
  result.value = getSDKResult('MeetingVideo_UnMuteVideo', ret, param)
}

const pinVideo = () => {
  let param = {userid: participants_value.value, bPin: pin.value, bFirstView: firstView.value}
  let ret = zoomMeetingVideo.MeetingVideo_PinVideo(param)
  result.value = getSDKResult('MeetingVideo_PinVideo', ret, param)
}

const spotlightVideo = () => {
  let param = {userid: participants_value.value, bSpotlight: spotlight.value}
  let ret = zoomMeetingVideo.MeetingVideo_SpotlightVideo(param)
  result.value = getSDKResult('MeetingVideo_SpotlightVideo', ret, param)
}

const hideOrShowNoVideoUserOnVideoWall = () => {
  let param = {bHide: hide.value}
  let ret = zoomMeetingVideo.MeetingVideo_HideOrShowNoVideoUserOnVideoWall(param)
  result.value = getSDKResult('MeetingVideo_HideOrShowNoVideoUserOnVideoWall', ret, param)
}

const videoPreference_mode = ref(SDKVideoPreferenceMode.SDKVideoPreferenceMode_Balance)
const minimumFrameRate = ref(0)
const maximumFrameRate = ref(30)
const setVideoQualityPreference = () => {
  let param = {
    mode: videoPreference_mode.value,
    minimumFrameRate: minimumFrameRate.value,
    maximumFrameRate: maximumFrameRate.value
  }
  let ret = zoomMeetingVideo.MeetingVideo_SetVideoQualityPreference(param)
  result.value = getSDKResult('MeetingVideo_SetVideoQualityPreference', ret, param)
}

onMounted(() => {
  getParticipantsList()
})
</script>