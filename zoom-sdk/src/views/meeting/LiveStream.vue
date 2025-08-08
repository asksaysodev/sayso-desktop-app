<template>
  <div class="meeting">
    <el-button type="primary" round plain @click="setOnLiveStreamStatusChangeCB">SetOnLiveStreamStatusChangeCB</el-button>
    <el-button type="primary" round plain @click="setOnRawLiveStreamPrivilegeChangedCB">SetOnRawLiveStreamPrivilegeChangedCB</el-button>
    <el-button type="primary" round plain @click="setOnUserRawLiveStreamPrivilegeChangedCB">SetOnUserRawLiveStreamPrivilegeChangedCB</el-button>
    <el-button type="primary" round plain @click="setOnRawLiveStreamPrivilegeRequestedCB">SetOnRawLiveStreamPrivilegeRequestedCB</el-button>
    <el-button type="primary" round plain @click="setOnUserRawLiveStreamingStatusChangedCB">SetOnUserRawLiveStreamingStatusChangedCB</el-button>
    <el-button type="primary" round plain @click="setOnRawLiveStreamPrivilegeRequestTimeoutCB">SetOnRawLiveStreamPrivilegeRequestTimeoutCB</el-button>
    <el-button type="primary" round plain @click="setOnLiveStreamReminderStatusChangedCB">SetOnLiveStreamReminderStatusChangedCB</el-button>
    <el-button type="primary" round plain @click="setOnLiveStreamReminderStatusChangeFailedCB">SetOnLiveStreamReminderStatusChangeFailedCB</el-button>
    <el-button type="primary" round plain @click="setOnUserThresholdReachedForLiveStreamCB">SetOnUserThresholdReachedForLiveStreamCB</el-button>
    <div class="in-line">
      <el-button type="primary" round plain @click="canStartLiveStream">CanStartLiveStream</el-button>
      <el-button type="primary" round plain @click="getSupportLiveStreamItems">GetSupportLiveStreamItems</el-button>
      <el-select v-model="liveStreamHandle" placeholder="Select">
        <el-option v-for="(item, index) in supportLiveStreamItems" :key="index" :label="item.deviceId" :value="item.deviceId"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="startLiveStream">StartLiveStream</el-button>
      <el-button type="primary" round plain @click="getLiveStreamItemInfo">GetLiveStreamItemInfo</el-button>
      <el-button type="primary" round plain @click="getCurrentLiveStreamItem">GetCurrentLiveStreamItem</el-button>
    </div>
    <div class="in-line">
      <el-input v-model.trim="streamingURL" placeholder="input streamingURL" />
      <el-input v-model.trim="streamingKey" placeholder="input streamingKey" />
      <el-input v-model.trim="broadcastURL" placeholder="input broadcastURL" @keyup.enter.native="startLiveStreamWithSteamingURL" />
      <el-button type="primary" round plain @click="startLiveStreamWithSteamingURL">StartLiveStreamWithSteamingURL</el-button>
    </div>
    <el-button type="primary" round plain @click="stopLiveStream">StopLiveStream</el-button>
    <el-button type="primary" round plain @click="getCurrentLiveStreamStatus">GetCurrentLiveStreamStatus</el-button>
    <div class="in-line">
      <el-button type="primary" round plain @click="stopRawLiveStream">StopRawLiveStream</el-button>
    </div>
    <el-button type="primary" round plain @click="isRawLiveStreamSupported">IsRawLiveStreamSupported</el-button>
    <el-button type="primary" round plain @click="canStartRawLiveStream">CanStartRawLiveStream</el-button>
    <div class="in-line">
      <el-select v-model="participants_value" placeholder="Select" @focus="getParticipantsList">
        <el-option v-for="(item, index) in participantsList" :key="index" :label="item.userName" :value="item.userID"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="removeRawLiveStreamPrivilege">RemoveRawLiveStreamPrivilege</el-button>
    </div>
    <el-button type="primary" round plain @click="getRawLiveStreamingInfoList">GetRawLiveStreamingInfoList</el-button>
    <el-button type="primary" round plain @click="getRawLiveStreamPrivilegeUserList">GetRawLiveStreamPrivilegeUserList</el-button>
    <div class="in-line">
      <el-button type="primary" round plain @click="isLiveStreamReminderEnabled">IsLiveStreamReminderEnabled</el-button>
      <el-button type="primary" round plain @click="canEnableLiveStreamReminder">CanEnableLiveStreamReminder</el-button>
      <el-checkbox v-model="enable_liveStreamReminder" label="EnableLiveStreamReminder" @change="enableLiveStreamReminder"></el-checkbox>
    </div>
  </div>
  <div class="result">Result: {{result}}</div>
  <div class="result"> 
    <div class="result-item">
      <span class="title">onLiveStreamStatusChange <el-button size="small" @click="onLiveStreamStatusChange_options=[]">Clear</el-button></span>
      <div class="callback-list">
        <span v-for="(item, index) in onLiveStreamStatusChange_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item">
      <span class="title">onRawLiveStreamPrivilegeChanged <el-button size="small" @click="onRawLiveStreamPrivilegeChanged_options=[]">Clear</el-button></span>
      <div class="callback-list">
        <span v-for="(item, index) in onRawLiveStreamPrivilegeChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item">
      <span class="title">onUserRawLiveStreamPrivilegeChanged <el-button size="small" @click="onUserRawLiveStreamPrivilegeChanged_options=[]">Clear</el-button></span>
      <div class="callback-list">
        <span v-for="(item, index) in onUserRawLiveStreamPrivilegeChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onRawLiveStreamPrivilegeRequested <el-button size="small" @click="onRawLiveStreamPrivilegeRequested_value=0">Clear</el-button></span> 
      <span v-if="onRawLiveStreamPrivilegeRequested_value">updated {{onRawLiveStreamPrivilegeRequested_value}}</span> 
    </div>
    <div class="result-item">
      <span class="title">onUserRawLiveStreamingStatusChanged <el-button size="small" @click="onUserRawLiveStreamingStatusChanged_options=[]">Clear</el-button></span>
      <div class="callback-list">
        <span v-for="(item, index) in onUserRawLiveStreamingStatusChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onRawLiveStreamPrivilegeRequestTimeout <el-button size="small" @click="onRawLiveStreamPrivilegeRequestTimeout_value=0">Clear</el-button></span> 
      <span v-if="onRawLiveStreamPrivilegeRequestTimeout_value">updated {{onRawLiveStreamPrivilegeRequestTimeout_value}}</span> 
    </div>
    <div class="result-item">
      <span class="title">onLiveStreamReminderStatusChanged <el-button size="small" @click="onLiveStreamReminderStatusChanged_options=[]">Clear</el-button></span>
      <div class="callback-list">
        <span v-for="(item, index) in onLiveStreamReminderStatusChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onLiveStreamReminderStatusChangeFailed <el-button size="small" @click="onLiveStreamReminderStatusChangeFailed_value=0">Clear</el-button></span> 
      <span v-if="onLiveStreamReminderStatusChangeFailed_value">updated {{onLiveStreamReminderStatusChangeFailed_value}}</span> 
    </div>
    <div class="result-item">
      <span class="title">onUserThresholdReachedForLiveStream <el-button size="small" @click="onUserThresholdReachedForLiveStream_options=[]">Clear</el-button></span>
      <div class="callback-list">
        <span v-for="(item, index) in onUserThresholdReachedForLiveStream_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { LiveStreamStatus } from '../../../lib/settings.js'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const result = ref()
const onLiveStreamStatusChange_options = ref<any[]>([])
const onRawLiveStreamPrivilegeChanged_options = ref<any[]>([])
const onUserRawLiveStreamPrivilegeChanged_options = ref<any[]>([])
const onRawLiveStreamPrivilegeRequested_value = ref(0)
const onUserRawLiveStreamingStatusChanged_options = ref<any[]>([])
const onRawLiveStreamPrivilegeRequestTimeout_value = ref(0)
const onLiveStreamReminderStatusChanged_options = ref<string[]>([])
const onLiveStreamReminderStatusChangeFailed_value = ref(0)
const onUserThresholdReachedForLiveStream_options = ref<string[]>([])
const liveStreamHandle = ref('')
const streamingURL = ref('')
const streamingKey = ref('')
const broadcastURL = ref('')
const supportLiveStreamItems = ref<any[]>([])
const participantsList = ref<any[]>([])
const participants_value = ref()
const clientHeight = ref((window.innerHeight - 465) + 'px')

const onLiveStreamStatusChange = (status: any) => {
  console.log(status)
  let statusContent = Object.keys(LiveStreamStatus).find(k => LiveStreamStatus[k] == status)
  onLiveStreamStatusChange_options.value.push(statusContent)
}
const onRawLiveStreamPrivilegeChanged = (hasPrivilege: any) => {
  console.log('onRawLiveStreamPrivilegeChanged', hasPrivilege)
  onRawLiveStreamPrivilegeChanged_options.value.push(`hasPrivilege: ${hasPrivilege}`)
}
const onUserRawLiveStreamPrivilegeChanged = (userid: any, hasPrivilege: any) => {
  console.log('onUserRawLiveStreamPrivilegeChanged', userid, hasPrivilege)
  let content = `userid: ${userid} - hasPrivilege: ${hasPrivilege}`
  onUserRawLiveStreamPrivilegeChanged_options.value.push(content)
}
const onRawLiveStreamPrivilegeRequested = () => {
  console.log('onRawLiveStreamPrivilegeRequested')
  onRawLiveStreamPrivilegeRequested_value.value++
}
const onUserRawLiveStreamingStatusChanged = (livingList: any) => {
  console.log('onUserRawLiveStreamingStatusChanged', livingList)
  onUserRawLiveStreamingStatusChanged_options.value.push(JSON.stringify(livingList))
}
const onRawLiveStreamPrivilegeRequestTimeout = () => {
  console.log('onRawLiveStreamPrivilegeRequestTimeout')
  onRawLiveStreamPrivilegeRequestTimeout_value.value++
}
const onLiveStreamReminderStatusChanged = (enable: boolean) => {
  console.log('onLiveStreamReminderStatusChanged', enable)
  onLiveStreamReminderStatusChanged_options.value.push(`enable: ${enable}`)
}
const onLiveStreamReminderStatusChangeFailed = () => {
  console.log('onLiveStreamReminderStatusChangeFailed')
  onLiveStreamReminderStatusChangeFailed_value.value++
}
const onUserThresholdReachedForLiveStream = (percent: number) => {
  console.log('onUserThresholdReachedForLiveStream', percent)
  onUserThresholdReachedForLiveStream_options.value.push(`percent: ${percent}`)
}

const GetMeetingLiveStreamCtrl = remote.app.zoomSdkModule.Meeting.GetMeetingLiveStreamCtrl({
  onLiveStreamStatusChange,
  onRawLiveStreamPrivilegeChanged,
  onUserRawLiveStreamPrivilegeChanged,
  onRawLiveStreamPrivilegeRequested,
  onUserRawLiveStreamingStatusChanged,
  onRawLiveStreamPrivilegeRequestTimeout,
  onLiveStreamReminderStatusChanged,
  onLiveStreamReminderStatusChangeFailed,
  onUserThresholdReachedForLiveStream
})
const zoomMeetingParticipants = remote.app.zoomSdkModule.Meeting.GetMeetingParticipantsCtrl()

const setOnLiveStreamStatusChangeCB = () => {
  let ret = GetMeetingLiveStreamCtrl.SetOnLiveStreamStatusChangeCB(onLiveStreamStatusChange)
  result.value = getSDKResult('SetOnLiveStreamStatusChangeCB', ret)
}

const setOnRawLiveStreamPrivilegeChangedCB = () => {
  let ret = GetMeetingLiveStreamCtrl.SetOnRawLiveStreamPrivilegeChangedCB(onRawLiveStreamPrivilegeChanged)
  result.value = getSDKResult('SetOnRawLiveStreamPrivilegeChangedCB', ret)
}

const setOnUserRawLiveStreamPrivilegeChangedCB = () => {
  let ret = GetMeetingLiveStreamCtrl.SetOnUserRawLiveStreamPrivilegeChangedCB(onUserRawLiveStreamPrivilegeChanged)
  result.value = getSDKResult('SetOnUserRawLiveStreamPrivilegeChangedCB', ret)
}

const setOnRawLiveStreamPrivilegeRequestedCB = () => {
  let ret = GetMeetingLiveStreamCtrl.SetOnRawLiveStreamPrivilegeRequestedCB(onRawLiveStreamPrivilegeRequested)
  result.value = getSDKResult('SetOnRawLiveStreamPrivilegeRequestedCB', ret)
}

const setOnUserRawLiveStreamingStatusChangedCB = () => {
  let ret = GetMeetingLiveStreamCtrl.SetOnUserRawLiveStreamingStatusChangedCB(onUserRawLiveStreamingStatusChanged)
  result.value = getSDKResult('SetOnUserRawLiveStreamingStatusChangedCB', ret)
}

const setOnRawLiveStreamPrivilegeRequestTimeoutCB = () => {
  let ret = GetMeetingLiveStreamCtrl.SetOnRawLiveStreamPrivilegeRequestTimeoutCB(onRawLiveStreamPrivilegeRequestTimeout)
  result.value = getSDKResult('SetOnRawLiveStreamPrivilegeRequestTimeoutCB', ret)
}

const setOnLiveStreamReminderStatusChangedCB = () => {
  let ret = GetMeetingLiveStreamCtrl.SetOnLiveStreamReminderStatusChangedCB(onLiveStreamReminderStatusChanged)
  result.value = getSDKResult('SetOnLiveStreamReminderStatusChangedCB', ret)
}

const setOnLiveStreamReminderStatusChangeFailedCB = () => {
  let ret = GetMeetingLiveStreamCtrl.SetOnLiveStreamReminderStatusChangeFailedCB(onLiveStreamReminderStatusChangeFailed)
  result.value = getSDKResult('SetOnLiveStreamReminderStatusChangeFailedCB', ret)
}

const setOnUserThresholdReachedForLiveStreamCB = () => {
  let ret = GetMeetingLiveStreamCtrl.SetOnUserThresholdReachedForLiveStreamCB(onUserThresholdReachedForLiveStream)
  result.value = getSDKResult('SetOnUserThresholdReachedForLiveStreamCB', ret)
}

const canStartLiveStream = () => {
  let ret = GetMeetingLiveStreamCtrl.CanStartLiveStream()
  result.value = getSDKResult('CanStartDirectShare', ret)
}

const startLiveStream = () => {
  let param = {liveStreamHandle: liveStreamHandle.value}
  let ret = GetMeetingLiveStreamCtrl.StartLiveStream(param);
  result.value = getSDKResult('StartLiveStream', ret, param)
}

const startLiveStreamWithSteamingURL = () => {
  let param = {
    streamingURL: streamingURL.value,
    streamingKey: streamingKey.value,
    broadcastURL: broadcastURL.value
  }
  let ret = GetMeetingLiveStreamCtrl.StartLiveStreamWithSteamingURL(param);
  result.value = getSDKResult('StartLiveStreamWithSteamingURL', ret, param)
}

const stopLiveStream = () => {
  let ret = GetMeetingLiveStreamCtrl.StopLiveStream()
  result.value = getSDKResult('StopLiveStream', ret)
}

const getSupportLiveStreamItems = () => {
  let tempList = GetMeetingLiveStreamCtrl.GetSupportLiveStreamItems()
  console.log('getSupportLiveStreamItems', tempList)
  for (let item of tempList) {
    if (typeof item.deviceId == 'bigint') {
      let tempId = BigInt(item.deviceId)
      item.deviceId = Number(tempId)
    }
  }
  supportLiveStreamItems.value = tempList
  result.value = JSON.stringify(tempList)
}

const getCurrentLiveStreamItem = () => {
  let ret = GetMeetingLiveStreamCtrl.GetCurrentLiveStreamItem()
  console.log('getCurrentLiveStreamItem', ret)
  result.value = JSON.stringify(ret)
}

const getCurrentLiveStreamStatus = () => {
  let ret = GetMeetingLiveStreamCtrl.GetCurrentLiveStreamStatus()
  result.value = getSDKResult('GetCurrentLiveStreamStatus', ret, null, false)
}

const stopRawLiveStream = () => {
  let ret = GetMeetingLiveStreamCtrl.StopRawLiveStream()
  result.value = getSDKResult('StopRawLiveStream', ret)
}

const getLiveStreamItemInfo = () => {
  let param = {
    liveStreamHandle: liveStreamHandle.value
  }
  let ret = GetMeetingLiveStreamCtrl.GetLiveStreamItemInfo(param);
  result.value = getSDKResult('GetLiveStreamItemInfo', ret, param)
}

const isRawLiveStreamSupported = () => {
  let ret = GetMeetingLiveStreamCtrl.IsRawLiveStreamSupported()
  result.value = getSDKResult('IsRawLiveStreamSupported', ret)
}

const canStartRawLiveStream = () => {
  let ret = GetMeetingLiveStreamCtrl.CanStartRawLiveStream()
  result.value = getSDKResult('CanStartRawLiveStream', ret)
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

const removeRawLiveStreamPrivilege = () => {
  let param = {
    userID: participants_value.value
  }
  let ret = GetMeetingLiveStreamCtrl.RemoveRawLiveStreamPrivilege(param);
  result.value = getSDKResult('RemoveRawLiveStreamPrivilege', ret, param)
}

const getRawLiveStreamingInfoList = () => {
  let ret = GetMeetingLiveStreamCtrl.GetRawLiveStreamingInfoList()
  result.value = getSDKResult('GetRawLiveStreamingInfoList', ret)
}

const getRawLiveStreamPrivilegeUserList = () => {
  let ret = GetMeetingLiveStreamCtrl.GetRawLiveStreamPrivilegeUserList()
  result.value = getSDKResult('GetRawLiveStreamPrivilegeUserList', ret)
}

const isLiveStreamReminderEnabled = () => {
  let ret = GetMeetingLiveStreamCtrl.IsLiveStreamReminderEnabled()
  result.value = getSDKResult('IsLiveStreamReminderEnabled', ret)
}

const canEnableLiveStreamReminder = () => {
  let ret = GetMeetingLiveStreamCtrl.CanEnableLiveStreamReminder()
  result.value = getSDKResult('CanEnableLiveStreamReminder', ret)
}

const enable_liveStreamReminder = ref(false)
const enableLiveStreamReminder = () => {
  let param = {
    enable: enable_liveStreamReminder.value
  }
  let ret = GetMeetingLiveStreamCtrl.EnableLiveStreamReminder(param);
  result.value = getSDKResult('EnableLiveStreamReminder', ret, param)
}

onMounted(() => {
  window.addEventListener('resize', ()=>{
    clientHeight.value = (window.innerHeight - 465) + 'px'
  })
})
</script>

<style scoped lang="scss">
  .meeting {
    height: v-bind(clientHeight);
  }
  .result {
	  .title {
		  width: 330px;
	  }
	}
</style>