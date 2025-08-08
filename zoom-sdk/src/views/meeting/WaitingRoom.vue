<template>
  <div class="meeting">
    <el-button type="primary" round plain @click="setOnWaitingRoomUserJoinCB">SetOnWaitingRoomUserJoinCB</el-button>
    <el-button type="primary" round plain @click="setOnWaitingRoomUserLeftCB">SetOnWaitingRoomUserLeftCB</el-button>
    <el-button type="primary" round plain @click="setOnWaitingRoomPresetAudioStatusChangedCB">SetOnWaitingRoomPresetAudioStatusChangedCB</el-button>
    <el-button type="primary" round plain @click="setOnWaitingRoomPresetVideoStatusChangedCB">SetOnWaitingRoomPresetVideoStatusChangedCB</el-button>
    <el-button type="primary" round plain @click="setOnCustomWaitingRoomDataUpdatedCB">SetOnCustomWaitingRoomDataUpdatedCB</el-button>
    <el-button type="primary" round plain @click="setOnWaitingRoomUserNameChangedCB">SetOnWaitingRoomUserNameChangedCB</el-button>
    <el-button type="primary" round plain @click="setOnWaitingRoomEntranceEnabledCB">SetOnWaitingRoomEntranceEnabledCB</el-button>
    <div class="in-line">
      <el-button type="primary" round plain @click="isSupportWaitingRoom">IsSupportWaitingRoom</el-button>
      <el-button type="primary" round plain @click="isWaitingRoomOnEntryFlagOn">IsWaitingRoomOnEntryFlagOn</el-button>
      <el-button type="primary" round plain @click="admitAllToMeeting">AdmitAllToMeeting</el-button>
    </div>
    <el-checkbox v-model="enable_waitingRoomOnEntry" label="EnableWaitingRoomOnEntry" @change="enableWaitingRoomOnEntry"></el-checkbox>
    <div class="in-line">
      <el-select v-model="waitingRoom_value" placeholder="Select" @focus="getWaitingRoomList" @change="getWaitingRoomUserInfoByID">
        <el-option v-for="(item, index) in waitingRoomList" :key="index" :label="item.userName" :value="item.userID"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="admitToMeeting">AdmitToMeeting</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="participants_value" placeholder="Select" @focus="getParticipantsList">
        <el-option v-for="(item, index) in participantsList" :key="index" :label="item.userName" :value="item.userID"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="putInWaitingRoom">PutInWaitingRoom</el-button>
    </div>
    <div class="in-line">
      <el-button type="primary" round plain @click="isAudioEnabledInWaitingRoom">IsAudioEnabledInWaitingRoom</el-button>
      <el-button type="primary" round plain @click="isVideoEnabledInWaitingRoom">IsVideoEnabledInWaitingRoom</el-button>
      <el-button type="primary" round plain @click="requestCustomWaitingRoomData">RequestCustomWaitingRoomData</el-button>
    </div>
    <div class="in-line">
      <el-button type="primary" round plain @click="canRenameUser">CanRenameUser</el-button>
      <el-select v-model="waitingRoom_value" placeholder="Select" @focus="getWaitingRoomList">
        <el-option v-for="(item, index) in waitingRoomList" :key="index" :label="item.userName" :value="item.userID"></el-option>
      </el-select>
      <el-input v-model="newName" placeholder="input new name" />
      <el-button type="primary" round plain @click="renameUser">RenameUser</el-button>
    </div>
    <div class="in-line">
      <el-button type="primary" round plain @click="canExpelUser">CanExpelUser</el-button>
      <el-select v-model="waitingRoom_value" placeholder="Select" @focus="getWaitingRoomList">
        <el-option v-for="(item, index) in waitingRoomList" :key="index" :label="item.userName" :value="item.userID"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="expelUser">ExpelUser</el-button>
    </div>
  </div>
  <div class="result">Result: {{result}}</div>
  <div class="result"> 
    <div class="result-item"> 
      <span class="title">onWaitingRoomUserJoin <el-button size="small" @click="onWaitingRoomUserJoin_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onWaitingRoomUserJoin_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onWaitingRoomUserLeft <el-button size="small" @click="onWaitingRoomUserLeft_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onWaitingRoomUserLeft_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onWaitingRoomPresetAudioStatusChanged <el-button size="small" @click="onWaitingRoomPresetAudioStatusChanged_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onWaitingRoomPresetAudioStatusChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onWaitingRoomPresetVideoStatusChanged <el-button size="small" @click="onWaitingRoomPresetVideoStatusChanged_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onWaitingRoomPresetVideoStatusChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onCustomWaitingRoomDataUpdated <el-button size="small" @click="onCustomWaitingRoomDataUpdated_options=[]">Clear</el-button></span> 
      <el-button type="primary" size="small" round plain @click="retry">Retry</el-button>
      <el-button type="primary" size="small" round plain @click="ignore">Ignore</el-button>
      <div class="callback-list">
        <span v-for="(item, index) in onCustomWaitingRoomDataUpdated_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onWaitingRoomUserNameChanged <el-button size="small" @click="onWaitingRoomUserNameChanged_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onWaitingRoomUserNameChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onWaitingRoomEntranceEnabled <el-button size="small" @click="onWaitingRoomEntranceEnabled_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onWaitingRoomEntranceEnabled_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const result = ref()
const onWaitingRoomUserJoin_options = ref<any[]>([])
const onWaitingRoomUserLeft_options = ref<any[]>([])
const onWaitingRoomPresetAudioStatusChanged_options = ref<any[]>([])
const onWaitingRoomPresetVideoStatusChanged_options = ref<any[]>([])
const onCustomWaitingRoomDataUpdated_options = ref<any[]>([])
const onWaitingRoomUserNameChanged_options = ref<any[]>([])
const onWaitingRoomEntranceEnabled_options = ref<any[]>([])

const onWaitingRoomUserJoin = (userID: number) => {
  console.log('onWaitingRoomUserJoin', userID)
  onWaitingRoomUserJoin_options.value.push(`userID: ${userID}`)
}
const onWaitingRoomUserLeft = (userID: number) => {
  console.log('onWaitingRoomUserLeft', userID)
  onWaitingRoomUserLeft_options.value.push(`userID: ${userID}`)
}
const onWaitingRoomPresetAudioStatusChanged = (bAudioCanTurnOn: boolean) => {
  console.log('onWaitingRoomPresetAudioStatusChanged', bAudioCanTurnOn)
  onWaitingRoomPresetAudioStatusChanged_options.value.push(`bAudioCanTurnOn: ${bAudioCanTurnOn}`)
}
const onWaitingRoomPresetVideoStatusChanged = (bVideoCanTurnOn: boolean) => {
  console.log('onWaitingRoomPresetVideoStatusChanged', bVideoCanTurnOn)
  onWaitingRoomPresetVideoStatusChanged_options.value.push(`bVideoCanTurnOn: ${bVideoCanTurnOn}`)
}
const onCustomWaitingRoomDataUpdated = (bData: any) => {
  console.log('onCustomWaitingRoomDataUpdated', bData, bData.status)
  let content = JSON.stringify({...bData})
  onCustomWaitingRoomDataUpdated_options.value.push(content)
}
const onWaitingRoomUserNameChanged = (userID: number, userName: string) => {
  console.log('onWaitingRoomUserNameChanged', userID, userName)
  let content = `userID: ${userID}, userName: ${userName}`
  onWaitingRoomUserNameChanged_options.value.push(content)
}
const onWaitingRoomEntranceEnabled = (bIsEnabled: boolean) => {
  console.log('onWaitingRoomEntranceEnabled', bIsEnabled)
  onWaitingRoomEntranceEnabled_options.value.push(`bIsEnabled: ${bIsEnabled}`)
}
const zoomMeetingWaitingRoom = remote.app.zoomSdkModule.Meeting.GetMeetingWaitingRoom({ onWaitingRoomUserJoin, onWaitingRoomUserLeft, onWaitingRoomPresetAudioStatusChanged,
  onWaitingRoomPresetVideoStatusChanged, onCustomWaitingRoomDataUpdated, onWaitingRoomUserNameChanged, onWaitingRoomEntranceEnabled })
const zoomMeetingParticipants = remote.app.zoomSdkModule.Meeting.GetMeetingParticipantsCtrl()
const participantsList = ref<any[]>([])
const participants_value = ref()
const waitingRoomList = ref<any[]>([])
const waitingRoom_value = ref()
const enable_waitingRoomOnEntry = ref(false)
const newName = ref('')

const setOnWaitingRoomUserJoinCB = () => {
  let ret = zoomMeetingWaitingRoom.SetOnWaitingRoomUserJoinCB(onWaitingRoomUserJoin)
  result.value = getSDKResult('SetOnWaitingRoomUserJoinCB', ret)
}

const setOnWaitingRoomUserLeftCB = () => {
  let ret = zoomMeetingWaitingRoom.SetOnWaitingRoomUserLeftCB(onWaitingRoomUserLeft)
  result.value = getSDKResult('SetOnWaitingRoomUserLeftCB', ret)
}

const setOnWaitingRoomPresetAudioStatusChangedCB = () => {
  let ret = zoomMeetingWaitingRoom.SetOnWaitingRoomPresetAudioStatusChangedCB(onWaitingRoomPresetAudioStatusChanged)
  result.value = getSDKResult('SetOnWaitingRoomPresetAudioStatusChangedCB', ret)
}

const setOnWaitingRoomPresetVideoStatusChangedCB = () => {
  let ret = zoomMeetingWaitingRoom.SetOnWaitingRoomPresetVideoStatusChangedCB(onWaitingRoomPresetVideoStatusChanged)
  result.value = getSDKResult('SetOnWaitingRoomPresetVideoStatusChangedCB', ret)
}

const setOnCustomWaitingRoomDataUpdatedCB = () => {
  let ret = zoomMeetingWaitingRoom.SetOnCustomWaitingRoomDataUpdatedCB(onCustomWaitingRoomDataUpdated)
  result.value = getSDKResult('SetOnCustomWaitingRoomDataUpdatedCB', ret)
}

const setOnWaitingRoomUserNameChangedCB = () => {
  let ret = zoomMeetingWaitingRoom.SetOnWaitingRoomUserNameChangedCB(onWaitingRoomUserNameChanged)
  result.value = getSDKResult('SetOnWaitingRoomUserNameChangedCB', ret)
}

const setOnWaitingRoomEntranceEnabledCB = () => {
  let ret = zoomMeetingWaitingRoom.SetOnWaitingRoomEntranceEnabledCB(onWaitingRoomEntranceEnabled)
  result.value = getSDKResult('SetOnWaitingRoomEntranceEnabledCB', ret)
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

const isSupportWaitingRoom = () => {
  let ret = zoomMeetingWaitingRoom.IsSupportWaitingRoom()
  result.value = getSDKResult('IsSupportWaitingRoom', ret)
}

const isWaitingRoomOnEntryFlagOn = () => {
  let ret = zoomMeetingWaitingRoom.IsWaitingRoomOnEntryFlagOn()
  result.value = getSDKResult('IsWaitingRoomOnEntryFlagOn', ret)
}

const enableWaitingRoomOnEntry = () => {
  let param = { bEnable: enable_waitingRoomOnEntry.value }
  let ret = zoomMeetingWaitingRoom.EnableWaitingRoomOnEntry(param)
  result.value = getSDKResult('EnableWaitingRoomOnEntry', ret, param)
}

const getWaitingRoomList = () => {
  let list = zoomMeetingWaitingRoom.GetWaitingRoomList(), obj
  result.value = getSDKResult('GetWaitingRoomList', list)
  waitingRoomList.value = []
  for (let item of list) {
    let param = { 
      userid: item.userid
    }
    obj = zoomMeetingWaitingRoom.GetWaitingRoomUserInfoByID(param)
    result.value = getSDKResult('GetWaitingRoomUserInfoByID', obj)
    waitingRoomList.value.push(obj)
  }
  if (waitingRoomList.value.length == 0) {
    waitingRoom_value.value = ''
  } else {
    waitingRoom_value.value = obj.userID
  }
}

const getWaitingRoomUserInfoByID = () => {
  let param = { 
    userid: waitingRoom_value.value
  }
  let ret = zoomMeetingWaitingRoom.GetWaitingRoomUserInfoByID(param)
  result.value = getSDKResult('GetWaitingRoomUserInfoByID', ret, param)
}

const admitAllToMeeting = () => {
  let ret = zoomMeetingWaitingRoom.AdmitAllToMeeting()
  result.value = getSDKResult('AdmitAllToMeeting', ret)
}

const admitToMeeting = () => {
  let param = { 
    userid: waitingRoom_value.value
  }
  let ret = zoomMeetingWaitingRoom.AdmitToMeeting(param)
  result.value = getSDKResult('AdmitToMeeting', ret, param)
}

const putInWaitingRoom = () => {
  let param = { 
    userid: participants_value.value
  }
  let ret = zoomMeetingWaitingRoom.PutInWaitingRoom(param)
  result.value = getSDKResult('PutInWaitingRoom', ret)
}

const isAudioEnabledInWaitingRoom = () => {
  let ret = zoomMeetingWaitingRoom.IsAudioEnabledInWaitingRoom()
  result.value = getSDKResult('IsAudioEnabledInWaitingRoom', ret)
}

const isVideoEnabledInWaitingRoom = () => {
  let ret = zoomMeetingWaitingRoom.IsVideoEnabledInWaitingRoom()
  result.value = getSDKResult('IsVideoEnabledInWaitingRoom', ret)
}

const requestCustomWaitingRoomData = () => {
  let ret = zoomMeetingWaitingRoom.RequestCustomWaitingRoomData()
  result.value = getSDKResult('RequestCustomWaitingRoomData', ret)
}

const canRenameUser = () => {
  let ret = zoomMeetingWaitingRoom.CanRenameUser()
  result.value = getSDKResult('CanRenameUser', ret)
}

const renameUser = () => {
  let param = { 
    userid: waitingRoom_value.value,
    newName: newName.value
  }
  let ret = zoomMeetingWaitingRoom.RenameUser(param)
  result.value = getSDKResult('RenameUser', ret, param)
}

const canExpelUser = () => {
  let ret = zoomMeetingWaitingRoom.CanExpelUser()
  result.value = getSDKResult('CanExpelUser', ret)
}

const expelUser = () => {
  let param = { 
    userid: waitingRoom_value.value
  }
  let ret = zoomMeetingWaitingRoom.ExpelUser(param)
  result.value = getSDKResult('ExpelUser', ret, param)
}

const retry = () => {
  let ret = zoomMeetingWaitingRoom.Retry()
  result.value = getSDKResult('Retry', ret)
}

const ignore = () => {
  let ret = zoomMeetingWaitingRoom.Ignore()
  result.value = getSDKResult('Ignore', ret)
}

onMounted(() => {
  getWaitingRoomList()
})
</script>
<style scoped lang="scss">
.result {
  .title {
    width: 340px;
  }
}
</style>