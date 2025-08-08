<template>
  <div class="meeting">
    <el-button type="primary" round plain @click="setMeetingUserJoinCB">SetMeetingUserJoinCB</el-button>
    <el-button type="primary" round plain @click="setMeetingUserLeftCB">SetMeetingUserLeftCB</el-button>
    <el-button type="primary" round plain @click="setMeetingHostChangeCB">SetMeetingHostChangeCB</el-button>
    <el-button type="primary" round plain @click="setOnAllowParticipantsRequestCloudRecordingCB">SetOnAllowParticipantsRequestCloudRecordingCB</el-button>
    <el-button type="primary" round plain @click="setOnBotAuthorizerRelationChangedCB">SetOnBotAuthorizerRelationChangedCB</el-button>
    <div class="in-line">
      <el-select v-model="participants_value" placeholder="Select" @focus="getParticipantsList">
        <el-option v-for="(item, index) in participantsList" :key="index" :label="item.userName" :value="item.userID"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="getUserInfoByUserID">GetUserInfoByUserID</el-button>
      <el-button type="primary" round plain @click="getBotAuthorizedUserInfoByUserID">GetBotAuthorizedUserInfoByUserID</el-button>
      <el-button type="primary" round plain @click="getAuthorizedBotListByUserID">GetAuthorizedBotListByUserID</el-button>
    </div>
    <div class="in-line">
      <el-button type="primary" round plain @click="canEnableParticipantRequestCloudRecording">CanEnableParticipantRequestCloudRecording</el-button>
      <el-checkbox v-model="allow_participantsToRequestCloudRecording" label="Allow" @change="allowParticipantsToRequestCloudRecording"></el-checkbox>
      <el-button type="primary" round plain @click="allowParticipantsToRequestCloudRecording">AllowParticipantsToRequestCloudRecording</el-button>
      <el-button type="primary" round plain @click="isParticipantRequestCloudRecordingAllowed">IsParticipantRequestCloudRecordingAllowed</el-button>
    </div>
  </div>
  <el-table :data="participantsList" stripe style="width: 100%">
    <el-table-column prop="userName" label="userName" :show-overflow-tooltip="true" />
    <el-table-column prop="userID" label="userID" />
    <el-table-column prop="isHost" label="isHost" />
    <el-table-column prop="isMySelf" label="isMySelf" />
    <el-table-column prop="isVideoOn" label="isVideoOn" />
    <el-table-column prop="isAudioMuted" label="isAudioMuted" />
  </el-table>
  <div class="result">Result: {{result}}</div>
  <div class="result"> 
    <div class="result-item"> 
      <span class="title">meetinguserjoincb <el-button size="small" @click="meetinguserjoincb_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in meetinguserjoincb_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">meetinguserleftcb <el-button size="small" @click="meetinguserleftcb_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in meetinguserleftcb_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">meetinghostchangecb <el-button size="small" @click="meetinghostchangecb_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in meetinghostchangecb_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onAllowParticipantsRequestCloudRecording <el-button size="small" @click="onAllowParticipantsRequestCloudRecording_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onAllowParticipantsRequestCloudRecording_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onBotAuthorizerRelationChanged <el-button size="small" @click="onBotAuthorizerRelationChanged_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onBotAuthorizerRelationChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const result = ref()
const meetinguserjoincb_options = ref<any[]>([])
const meetinguserleftcb_options = ref<any[]>([])
const meetinghostchangecb_options = ref<any[]>([])
const onAllowParticipantsRequestCloudRecording_options = ref<any[]>([])
const onBotAuthorizerRelationChanged_options = ref<any[]>([])

const meetinguserjoincb = (strUserList: any, lstUserID: number) => {
  console.log('meetinguserjoincb', strUserList, lstUserID)
  meetinguserjoincb_options.value = strUserList
}

const meetinguserleftcb = (strUserList: any, lstUserID: number) => {
  console.log('meetinguserleftcb', strUserList, lstUserID)
  meetinguserleftcb_options.value = strUserList
}

const meetinghostchangecb = (userid: number) => {
  console.log('meetinghostchangecb', userid)
  meetinghostchangecb_options.value.push(`userid: ${userid}`)
}

const onAllowParticipantsRequestCloudRecording = (bAllow: boolean) => {
  console.log('onAllowParticipantsRequestCloudRecording', bAllow)
  onAllowParticipantsRequestCloudRecording_options.value.push(`bAllow: ${bAllow}`)
}

const onBotAuthorizerRelationChanged = (authorizeUserID: number) => {
  console.log('onBotAuthorizerRelationChanged', authorizeUserID)
  onBotAuthorizerRelationChanged_options.value.push(`authorizeUserID: ${authorizeUserID}`)
}

const zoomMeetingParticipants = remote.app.zoomSdkModule.Meeting.GetMeetingParticipantsCtrl({ meetinguserjoincb, meetinguserleftcb, meetinghostchangecb,
  onAllowParticipantsRequestCloudRecording, onBotAuthorizerRelationChanged })
const participantsList = ref<any[]>([])
const participants_value = ref()
const allow_participantsToRequestCloudRecording = ref(false)

const setMeetingUserJoinCB = () => {
  let ret = zoomMeetingParticipants.SetMeetingUserJoinCB(meetinguserjoincb)
  result.value = getSDKResult('SetMeetingUserJoinCB', ret)
}

const setMeetingUserLeftCB = () => {
  let ret = zoomMeetingParticipants.SetMeetingUserLeftCB(meetinguserleftcb)
  result.value = getSDKResult('SetMeetingUserLeftCB', ret)
}

const setMeetingHostChangeCB = () => {
  let ret = zoomMeetingParticipants.SetMeetingHostChangeCB(meetinghostchangecb)
  result.value = getSDKResult('SetMeetingHostChangeCB', ret)
}

const setOnAllowParticipantsRequestCloudRecordingCB = () => {
  let ret = zoomMeetingParticipants.SetOnAllowParticipantsRequestCloudRecordingCB(onAllowParticipantsRequestCloudRecording)
  result.value = getSDKResult('SetOnAllowParticipantsRequestCloudRecordingCB', ret)
}

const setOnBotAuthorizerRelationChangedCB = () => {
  let ret = zoomMeetingParticipants.SetOnBotAuthorizerRelationChangedCB(onBotAuthorizerRelationChanged)
  result.value = getSDKResult('SetOnBotAuthorizerRelationChangedCB', ret)
}

const getParticipantsList = () => {
  let list = zoomMeetingParticipants.GetParticipantsList()
  console.log(list)
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

const getUserInfoByUserID = () => {
  let param = participants_value.value
  let ret = zoomMeetingParticipants.GetUserInfoByUserID(param)
  result.value = getSDKResult('GetUserInfoByUserID', ret, param)
}

const getBotAuthorizedUserInfoByUserID = () => {
  let param = {
    userid: participants_value.value
  }
  let ret = zoomMeetingParticipants.GetBotAuthorizedUserInfoByUserID(param)
  result.value = getSDKResult('GetBotAuthorizedUserInfoByUserID', ret, param)
}

const getAuthorizedBotListByUserID = () => {
  let param = {
    userid: participants_value.value
  }
  let ret = zoomMeetingParticipants.GetAuthorizedBotListByUserID(param)
  result.value = getSDKResult('GetAuthorizedBotListByUserID', ret, param)
}

const canEnableParticipantRequestCloudRecording = () => {
  let ret = zoomMeetingParticipants.CanEnableParticipantRequestCloudRecording()
  result.value = getSDKResult('CanEnableParticipantRequestCloudRecording', ret)
}

const allowParticipantsToRequestCloudRecording = () => {
  let param = { bAllow: allow_participantsToRequestCloudRecording.value }
  let ret = zoomMeetingParticipants.AllowParticipantsToRequestCloudRecording(param)
  result.value = getSDKResult('AllowParticipantsToRequestCloudRecording', ret, param)
}

const isParticipantRequestCloudRecordingAllowed = () => {
  let ret = zoomMeetingParticipants.IsParticipantRequestCloudRecordingAllowed()
  result.value = getSDKResult('IsParticipantRequestCloudRecordingAllowed', ret)
}

onMounted(() => {
  getParticipantsList()
})
</script>
<style scoped lang="scss">
.result {
  .title {
    width: 410px;
  }
}
</style>