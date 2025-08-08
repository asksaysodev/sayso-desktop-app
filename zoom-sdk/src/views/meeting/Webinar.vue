<template>
  <div class="meeting">
    <el-button type="primary" round plain @click="setOnPromptAttendee2PanelistResultCB">SetOnPromptAttendee2PanelistResultCB</el-button>
    <el-button type="primary" round plain @click="setOnDepromptPanelist2AttendeeResultCB">SetOnDepromptPanelist2AttendeeResultCB</el-button>
    <el-button type="primary" round plain @click="setOnAllowPanelistStartVideoNotificationCB">SetOnAllowPanelistStartVideoNotificationCB</el-button>
    <el-button type="primary" round plain @click="setOnDisallowPanelistStartVideoNotificationCB">SetOnDisallowPanelistStartVideoNotificationCB</el-button>
    <el-button type="primary" round plain @click="setOnSelfAllowTalkNotificationCB">SetOnSelfAllowTalkNotificationCB</el-button>
    <el-button type="primary" round plain @click="setOnSelfDisallowTalkNotificationCB">SetOnSelfDisallowTalkNotificationCB</el-button>
    <el-button type="primary" round plain @click="setOnAllowAttendeeChatNotificationCB">SetOnAllowAttendeeChatNotificationCB</el-button>
    <el-button type="primary" round plain @click="setOnDisallowAttendeeChatNotificationCB">SetOnDisallowAttendeeChatNotificationCB</el-button>
    <el-button type="primary" round plain @click="setOnAllowWebinarReactionStatusChangedCB">SetOnAllowWebinarReactionStatusChangedCB</el-button>
    <el-button type="primary" round plain @click="setOnAllowAttendeeRaiseHandStatusChangedCB">SetOnAllowAttendeeRaiseHandStatusChangedCB</el-button>
    <el-button type="primary" round plain @click="setOnAllowAttendeeViewTheParticipantCountStatusChangedCB">SetOnAllowAttendeeViewTheParticipantCountStatusChangedCB</el-button>
    <el-button type="primary" round plain @click="setOnAttendeeAudioStatusNotificationCB">SetOnAttendeeAudioStatusNotificationCB</el-button>
    <el-button type="primary" round plain @click="setOnAttendeePromoteConfirmResultCB">SetOnAttendeePromoteConfirmResultCB</el-button>
    <div class="in-line">
      <el-select v-model="participants_value" placeholder="Select" @focus="getParticipantsList">
        <el-option v-for="(item, index) in participantsList" :key="index" :label="item.userName" :value="item.userID"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="promptAttendee2Panelist">PromptAttendee2Panelist</el-button>
      <el-button type="primary" round plain @click="depromptPanelist2Attendee">DepromptPanelist2Attendee</el-button>
    </div>
    <div class="in-line">
      <el-button type="primary" round plain @click="isSupportAttendeeTalk">IsSupportAttendeeTalk</el-button>
      <el-select v-model="participants_value" placeholder="Select" @focus="getParticipantsList">
        <el-option v-for="(item, index) in participantsList" :key="index" :label="item.userName" :value="item.userID"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="allowAttendeeTalk">AllowAttendeeTalk</el-button>
      <el-button type="primary" round plain @click="disallowAttendeeTalk">DisallowAttendeeTalk</el-button>
    </div>
    <div class="in-line">
      <el-button type="primary" round plain @click="allowPanelistStartVideo">AllowPanelistStartVideo</el-button>
      <el-button type="primary" round plain @click="disallowPanelistStartVideo">DisallowPanelistStartVideo</el-button>
    </div>
    <div class="in-line">
      <el-button type="primary" round plain @click="isWebinarEmojiReactionSupported">IsWebinarEmojiReactionSupported</el-button>
      <el-button type="primary" round plain @click="allowWebinarEmojiReaction">AllowWebinarEmojiReaction</el-button>
      <el-button type="primary" round plain @click="disallowWebinarEmojiReaction">DisallowWebinarEmojiReaction</el-button>
    </div>
    <div class="in-line">
      <el-button type="primary" round plain @click="allowAttendeeRaiseHand">AllowAttendeeRaiseHand</el-button>
      <el-button type="primary" round plain @click="disallowAttendeeRaiseHand">DisallowAttendeeRaiseHand</el-button>
    </div>
    <div class="in-line">
      <el-button type="primary" round plain @click="allowAttendeeViewTheParticipantCount">AllowAttendeeViewTheParticipantCount</el-button>
      <el-button type="primary" round plain @click="disallowAttendeeViewTheParticipantCount">DisallowAttendeeViewTheParticipantCount</el-button>
      <el-button type="primary" round plain @click="getParticipantCount">GetParticipantCount</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="privilege" placeholder="Select">
        <el-option v-for="(item, key) in SDKPanelistChatPrivilege" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="setPanelistChatPrivilege">SetPanelistChatPrivilege</el-button>
      <el-button type="primary" round plain @click="getPanelistChatPrivilege">GetPanelistChatPrivilege</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="mode" placeholder="Select">
        <el-option v-for="(item, key) in SDKAttendeeViewMode" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="setAttendeeViewMode">SetAttendeeViewMode</el-button>
      <el-button type="primary" round plain @click="getAttendeeViewMode">GetAttendeeViewMode</el-button>
    </div>
    <div class="in-line">
      <el-button type="primary" round plain @click="getWebinarMeetingStatus">GetWebinarMeetingStatus</el-button>
      <el-button type="primary" round plain @click="getWebinarLegalNoticesPrompt">GetWebinarLegalNoticesPrompt</el-button>
      <el-button type="primary" round plain @click="getWebinarLegalNoticesExplained">GetWebinarLegalNoticesExplained</el-button>
    </div>
  </div>
  <div class="result">Result: {{result}}</div>
  <div class="result"> 
    <div class="result-item"> 
      <span class="title">onPromptAttendee2PanelistResult <el-button size="small" @click="onPromptAttendee2PanelistResult_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onPromptAttendee2PanelistResult_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onDepromptPanelist2AttendeeResult <el-button size="small" @click="onDepromptPanelist2AttendeeResult_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onDepromptPanelist2AttendeeResult_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onAllowPanelistStartVideoNotification <el-button size="small" @click="onAllowPanelistStartVideoNotification_value=0">Clear</el-button></span> 
      <span v-if="onAllowPanelistStartVideoNotification_value">updated {{onAllowPanelistStartVideoNotification_value}}</span> 
    </div>
    <div class="result-item"> 
      <span class="title">onDisallowPanelistStartVideoNotification <el-button size="small" @click="onDisallowPanelistStartVideoNotification_value=0">Clear</el-button></span> 
      <span v-if="onDisallowPanelistStartVideoNotification_value">updated {{onDisallowPanelistStartVideoNotification_value}}</span> 
    </div>
    <div class="result-item"> 
      <span class="title">onSelfAllowTalkNotification <el-button size="small" @click="onSelfAllowTalkNotification_value=0">Clear</el-button></span> 
      <span v-if="onSelfAllowTalkNotification_value">updated {{onSelfAllowTalkNotification_value}}</span> 
    </div>
    <div class="result-item"> 
      <span class="title">onSelfDisallowTalkNotification <el-button size="small" @click="onSelfDisallowTalkNotification_value=0">Clear</el-button></span> 
      <span v-if="onSelfDisallowTalkNotification_value">updated {{onSelfDisallowTalkNotification_value}}</span> 
    </div>
    <div class="result-item"> 
      <span class="title">onAllowAttendeeChatNotification <el-button size="small" @click="onAllowAttendeeChatNotification_value=0">Clear</el-button></span> 
      <span v-if="onAllowAttendeeChatNotification_value">updated {{onAllowAttendeeChatNotification_value}}</span> 
    </div>
    <div class="result-item"> 
      <span class="title">onDisallowAttendeeChatNotification <el-button size="small" @click="onDisallowAttendeeChatNotification_value=0">Clear</el-button></span> 
      <span v-if="onDisallowAttendeeChatNotification_value">updated {{onDisallowAttendeeChatNotification_value}}</span> 
    </div>
    <div class="result-item"> 
      <span class="title">onAllowWebinarReactionStatusChanged <el-button size="small" @click="onAllowWebinarReactionStatusChanged_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onAllowWebinarReactionStatusChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onAllowAttendeeRaiseHandStatusChanged <el-button size="small" @click="onAllowAttendeeRaiseHandStatusChanged_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onAllowAttendeeRaiseHandStatusChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onAllowAttendeeViewTheParticipantCountStatusChanged <el-button size="small" @click="onAllowAttendeeViewTheParticipantCountStatusChanged_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onAllowAttendeeViewTheParticipantCountStatusChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onAttendeeAudioStatusNotification <el-button size="small" @click="onAttendeeAudioStatusNotification_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onAttendeeAudioStatusNotification_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { getSDKResult } from '../../assets/ts/util'
import { SDKPanelistChatPrivilege, SDKAttendeeViewMode } from '../../../lib/settings.js'
const remote = window.require('@electron/remote')
const result = ref()
const onPromptAttendee2PanelistResult_options = ref<string[]>([])
const onDepromptPanelist2AttendeeResult_options = ref<string[]>([])
const onAllowPanelistStartVideoNotification_value = ref(0)
const onDisallowPanelistStartVideoNotification_value = ref(0)
const onSelfAllowTalkNotification_value = ref(0)
const onSelfDisallowTalkNotification_value = ref(0)
const onAllowAttendeeChatNotification_value = ref(0)
const onDisallowAttendeeChatNotification_value = ref(0)
const onAllowWebinarReactionStatusChanged_options = ref<string[]>([])
const onAllowAttendeeRaiseHandStatusChanged_options = ref<string[]>([])
const onAllowAttendeeViewTheParticipantCountStatusChanged_options = ref<string[]>([])
const onAttendeeAudioStatusNotification_options = ref<string[]>([])
const onAttendeePromoteConfirmResult_options = ref<string[]>([])
const privilege = ref(1)
const mode = ref(0)

const onPromptAttendee2PanelistResult = (result: number) => {
  console.log('onPromptAttendee2PanelistResult', result)
  onPromptAttendee2PanelistResult_options.value.push(`result: ${result}`)
}
const onDepromptPanelist2AttendeeResult = (result: number) => {
  console.log('onDepromptPanelist2AttendeeResult', result)
  onDepromptPanelist2AttendeeResult_options.value.push(`result: ${result}`)
}
const onAllowPanelistStartVideoNotification = () => {
  console.log('onAllowPanelistStartVideoNotification')
  onAllowPanelistStartVideoNotification_value.value++
}
const onDisallowPanelistStartVideoNotification = () => {
  console.log('onDisallowPanelistStartVideoNotification')
  onDisallowPanelistStartVideoNotification_value.value++
}
const onSelfAllowTalkNotification = () => {
  console.log('onSelfAllowTalkNotification')
  onSelfAllowTalkNotification_value.value++
}
const onSelfDisallowTalkNotification = () => {
  console.log('onSelfDisallowTalkNotification')
  onSelfDisallowTalkNotification_value.value++
}
const onAllowAttendeeChatNotification = () => {
  console.log('onAllowAttendeeChatNotification')
  onAllowAttendeeChatNotification_value.value++
}
const onDisallowAttendeeChatNotification = () => {
  console.log('onDisallowAttendeeChatNotification')
  onDisallowAttendeeChatNotification_value.value++
}
const onAllowWebinarReactionStatusChanged = (can_reaction: boolean) => {
  console.log('onAllowWebinarReactionStatusChanged', can_reaction)
  onAllowWebinarReactionStatusChanged_options.value.push(`can_reaction: ${can_reaction}`)
}
const onAllowAttendeeRaiseHandStatusChanged = (can_raiseHand: boolean) => {
  console.log('onAllowAttendeeRaiseHandStatusChanged', can_raiseHand)
  onAllowAttendeeRaiseHandStatusChanged_options.value.push(`can_raiseHand: ${can_raiseHand}`)
}
const onAllowAttendeeViewTheParticipantCountStatusChanged = (can_viewParticipantCount: boolean) => {
  console.log('onAllowAttendeeViewTheParticipantCountStatusChanged', can_viewParticipantCount)
  onAllowAttendeeViewTheParticipantCountStatusChanged_options.value.push(`can_viewParticipantCount: ${can_viewParticipantCount}`)
}
const onAttendeeAudioStatusNotification = (userid: number, can_talk: boolean, is_muted: boolean) => {
  console.log('onAttendeeAudioStatusNotification', userid, can_talk, is_muted)
  onAttendeeAudioStatusNotification_options.value.push(`userid: ${userid}, can_talk: ${can_talk}, is_muted: ${is_muted}`)
}
const onAttendeePromoteConfirmResult = (agree: boolean, userid: number) => {
  console.log('onAttendeePromoteConfirmResult', agree, userid)
  onAttendeePromoteConfirmResult_options.value.push(`agree: ${agree}, userid: ${userid}`)
}
const zoomMeetingWebinar = remote.app.zoomSdkModule.Meeting.GetMeetingWebinar({ onPromptAttendee2PanelistResult, onDepromptPanelist2AttendeeResult,
  onAllowPanelistStartVideoNotification, onDisallowPanelistStartVideoNotification, onSelfAllowTalkNotification, onSelfDisallowTalkNotification,
  onAllowAttendeeChatNotification, onDisallowAttendeeChatNotification, onAllowWebinarReactionStatusChanged, onAllowAttendeeRaiseHandStatusChanged,
  onAllowAttendeeViewTheParticipantCountStatusChanged, onAttendeeAudioStatusNotification, onAttendeePromoteConfirmResult })
const zoomMeetingParticipants = remote.app.zoomSdkModule.Meeting.GetMeetingParticipantsCtrl()
const participantsList = ref<any[]>([])
const participants_value = ref()

const setOnPromptAttendee2PanelistResultCB = () => {
  let ret = zoomMeetingWebinar.SetOnPromptAttendee2PanelistResultCB(onPromptAttendee2PanelistResult)
  result.value = getSDKResult('SetOnPromptAttendee2PanelistResultCB', ret)
}

const setOnDepromptPanelist2AttendeeResultCB = () => {
  let ret = zoomMeetingWebinar.SetOnDepromptPanelist2AttendeeResultCB(onDepromptPanelist2AttendeeResult)
  result.value = getSDKResult('SetOnDepromptPanelist2AttendeeResultCB', ret)
}

const setOnAllowPanelistStartVideoNotificationCB = () => {
  let ret = zoomMeetingWebinar.SetOnAllowPanelistStartVideoNotificationCB(onAllowPanelistStartVideoNotification)
  result.value = getSDKResult('SetOnAllowPanelistStartVideoNotificationCB', ret)
}

const setOnDisallowPanelistStartVideoNotificationCB = () => {
  let ret = zoomMeetingWebinar.SetOnDisallowPanelistStartVideoNotificationCB(onDisallowPanelistStartVideoNotification)
  result.value = getSDKResult('SetOnDisallowPanelistStartVideoNotificationCB', ret)
}

const setOnSelfAllowTalkNotificationCB = () => {
  let ret = zoomMeetingWebinar.SetOnSelfAllowTalkNotificationCB(onSelfAllowTalkNotification)
  result.value = getSDKResult('SetOnSelfAllowTalkNotificationCB', ret)
}

const setOnSelfDisallowTalkNotificationCB = () => {
  let ret = zoomMeetingWebinar.SetOnSelfDisallowTalkNotificationCB(onSelfDisallowTalkNotification)
  result.value = getSDKResult('SetOnSelfDisallowTalkNotificationCB', ret)
}

const setOnAllowAttendeeChatNotificationCB = () => {
  let ret = zoomMeetingWebinar.SetOnAllowAttendeeChatNotificationCB(onAllowAttendeeChatNotification)
  result.value = getSDKResult('SetOnAllowAttendeeChatNotificationCB', ret)
}

const setOnDisallowAttendeeChatNotificationCB = () => {
  let ret = zoomMeetingWebinar.SetOnDisallowAttendeeChatNotificationCB(onDisallowAttendeeChatNotification)
  result.value = getSDKResult('setOnDisallowAttendeeChatNotificationCB', ret)
}

const setOnAllowWebinarReactionStatusChangedCB = () => {
  let ret = zoomMeetingWebinar.SetOnAllowWebinarReactionStatusChangedCB(onAllowWebinarReactionStatusChanged)
  result.value = getSDKResult('setOnAllowWebinarReactionStatusChangedCB', ret)
}

const setOnAllowAttendeeRaiseHandStatusChangedCB = () => {
  let ret = zoomMeetingWebinar.SetOnAllowAttendeeRaiseHandStatusChangedCB(onAllowAttendeeRaiseHandStatusChanged)
  result.value = getSDKResult('setOnAllowAttendeeRaiseHandStatusChangedCB', ret)
}

const setOnAllowAttendeeViewTheParticipantCountStatusChangedCB = () => {
  let ret = zoomMeetingWebinar.SetOnAllowAttendeeViewTheParticipantCountStatusChangedCB(onAllowAttendeeViewTheParticipantCountStatusChanged)
  result.value = getSDKResult('setOnAllowAttendeeViewTheParticipantCountStatusChangedCB', ret)
}

const setOnAttendeeAudioStatusNotificationCB = () => {
  let ret = zoomMeetingWebinar.SetOnAttendeeAudioStatusNotificationCB(onAttendeeAudioStatusNotification)
  result.value = getSDKResult('setOnAttendeeAudioStatusNotificationCB', ret)
}

const setOnAttendeePromoteConfirmResultCB = () => {
  let ret = zoomMeetingWebinar.SetOnAttendeePromoteConfirmResultCB(onAttendeePromoteConfirmResult)
  result.value = getSDKResult('setOnAttendeePromoteConfirmResultCB', ret)
}

const getParticipantsList = () => {
  let list = zoomMeetingParticipants.GetParticipantsList(), obj
  result.value = getSDKResult('GetParticipantsList', list)
  participantsList.value = []
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

const promptAttendee2Panelist = () => {
  let param = { 
    userid: participants_value.value
  }
  let ret = zoomMeetingWebinar.PromptAttendee2Panelist(param)
  result.value = getSDKResult('PromptAttendee2Panelist', ret, param)
}

const depromptPanelist2Attendee = () => {
  let param = { 
    userid: participants_value.value
  }
  let ret = zoomMeetingWebinar.DepromptPanelist2Attendee(param)
  result.value = getSDKResult('DepromptPanelist2Attendee', ret, param)
}

const isSupportAttendeeTalk = () => {
  let ret = zoomMeetingWebinar.IsSupportAttendeeTalk()
  result.value = getSDKResult('IsSupportAttendeeTalk', ret)
}

const allowAttendeeTalk = () => {
  let param = { 
    userid: participants_value.value
  }
  let ret = zoomMeetingWebinar.AllowAttendeeTalk(param)
  result.value = getSDKResult('AllowAttendeeTalk', ret, param)
}

const disallowAttendeeTalk = () => {
  let param = { 
    userid: participants_value.value
  }
  let ret = zoomMeetingWebinar.DisallowAttendeeTalk(param)
  result.value = getSDKResult('DisallowAttendeeTalk', ret, param)
}

const allowPanelistStartVideo = () => {
  let ret = zoomMeetingWebinar.AllowPanelistStartVideo()
  result.value = getSDKResult('AllowPanelistStartVideo', ret)
}

const disallowPanelistStartVideo = () => {
  let ret = zoomMeetingWebinar.DisallowPanelistStartVideo()
  result.value = getSDKResult('DisallowPanelistStartVideo', ret)
}

const isWebinarEmojiReactionSupported = () => {
  let ret = zoomMeetingWebinar.IsWebinarEmojiReactionSupported()
  result.value = getSDKResult('IsWebinarEmojiReactionSupported', ret)
}

const allowWebinarEmojiReaction = () => {
  let ret = zoomMeetingWebinar.AllowWebinarEmojiReaction()
  result.value = getSDKResult('AllowWebinarEmojiReaction', ret)
}

const disallowWebinarEmojiReaction = () => {
  let ret = zoomMeetingWebinar.DisallowWebinarEmojiReaction()
  result.value = getSDKResult('DisallowWebinarEmojiReaction', ret)
}

const allowAttendeeRaiseHand = () => {
  let ret = zoomMeetingWebinar.AllowAttendeeRaiseHand()
  result.value = getSDKResult('AllowAttendeeRaiseHand', ret)
}

const disallowAttendeeRaiseHand = () => {
  let ret = zoomMeetingWebinar.DisallowAttendeeRaiseHand()
  result.value = getSDKResult('DisallowAttendeeRaiseHand', ret)
}

const allowAttendeeViewTheParticipantCount = () => {
  let ret = zoomMeetingWebinar.AllowAttendeeViewTheParticipantCount()
  result.value = getSDKResult('AllowAttendeeViewTheParticipantCount', ret)
}

const disallowAttendeeViewTheParticipantCount = () => {
  let ret = zoomMeetingWebinar.DisallowAttendeeViewTheParticipantCount()
  result.value = getSDKResult('DisallowAttendeeViewTheParticipantCount', ret)
}

const getParticipantCount = () => {
  let ret = zoomMeetingWebinar.GetParticipantCount()
  result.value = getSDKResult('GetParticipantCount', ret, null, false)
}

const getWebinarMeetingStatus = () => {
  let ret = zoomMeetingWebinar.GetWebinarMeetingStatus()
  result.value = getSDKResult('GetWebinarMeetingStatus', ret)
}

const setPanelistChatPrivilege = () => {
  let param = { 
    privilege: privilege.value
  }
  let ret = zoomMeetingWebinar.SetPanelistChatPrivilege(param)
  result.value = getSDKResult('SetPanelistChatPrivilege', ret, param)
}

const getPanelistChatPrivilege = () => {
  let ret = zoomMeetingWebinar.GetPanelistChatPrivilege()
  console.log('GetPanelistChatPrivilege', ret)
  let privilege = Object.keys(SDKPanelistChatPrivilege).find(k => SDKPanelistChatPrivilege[k] == ret.privilege)
  result.value = getSDKResult('GetPanelistChatPrivilege', privilege)
}

const setAttendeeViewMode = () => {
  let param = { 
    mode: mode.value
  }
  let ret = zoomMeetingWebinar.SetAttendeeViewMode(param)
  result.value = getSDKResult('SetAttendeeViewMode', ret, param)
}

const getAttendeeViewMode = () => {
  let ret = zoomMeetingWebinar.GetAttendeeViewMode()
  let privilege = Object.keys(SDKAttendeeViewMode).find(k => SDKAttendeeViewMode[k] == ret)
  result.value = getSDKResult('GetAttendeeViewMode', privilege)
}

const getWebinarLegalNoticesPrompt = () => {
  let ret = zoomMeetingWebinar.GetWebinarLegalNoticesPrompt()
  result.value = getSDKResult('GetWebinarLegalNoticesPrompt', ret)
}

const getWebinarLegalNoticesExplained = () => {
  let ret = zoomMeetingWebinar.GetWebinarLegalNoticesExplained()
  result.value = getSDKResult('GetWebinarLegalNoticesExplained', ret)
}

onMounted(() => {
  getParticipantsList()
})
</script>

<style scoped lang="scss">
.result {
  .title {
    width: 430px;
  }
}
</style>