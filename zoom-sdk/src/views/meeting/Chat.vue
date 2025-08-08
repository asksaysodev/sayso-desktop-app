<template>
  <div class="meeting">
    <el-button type="primary" round plain @click="setOnChatMsgNotificationCB">SetOnChatMsgNotificationCB</el-button>
    <el-button type="primary" round plain @click="setOnChatStatusChangedNotificationCB">SetOnChatStatusChangedNotificationCB</el-button>
    <el-button type="primary" round plain @click="setOnChatMsgDeleteNotificationCB">SetOnChatMsgDeleteNotificationCB</el-button>
    <el-button type="primary" round plain @click="setOnChatMessageEditNotificationCB">SetOnChatMessageEditNotificationCB</el-button>
    <el-button type="primary" round plain @click="setOnShareMeetingChatStatusChangedCB">SetOnShareMeetingChatStatusChangedCB</el-button>
    <el-button type="primary" round plain @click="setOnFileSendStartCB">SetOnFileSendStartCB</el-button>
    <el-button type="primary" round plain @click="setOnFileReceivedCB">SetOnFileReceivedCB</el-button>
    <el-button type="primary" round plain @click="setOnFileTransferProgressCB">SetOnFileTransferProgressCB</el-button>
    <el-button type="primary" round plain @click="getChatStatus">GetChatStatus</el-button>
    <div class="in-line">
      <el-select v-model="privilege" placeholder="Select">
        <el-option v-for="(item, key) in SDKChatPrivilege" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="setParticipantsChatPrivilege">SetParticipantsChatPrivilege</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="participants_value" placeholder="Select" @focus="getParticipantsList">
        <el-option v-for="(item, index) in participantsList" :key="index" :label="item.userName" :value="item.userID"></el-option>
      </el-select>
      <el-select v-model="chatMsgtType" placeholder="Select">
        <el-option v-for="(item, key) in SDKChatMessageType" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-input v-model.trim="chatContent" placeholder="input chat content" />
      <el-input v-model.trim="threadId" placeholder="input thread id" />
      <el-input v-model.number="positionStart" placeholder="input position start" />
      <el-input v-model.number="positionEnd" placeholder="input position end" />
      <template v-if="platform!='darwin'">
        <el-checkbox v-model="bClearStyles" label="ClearStyles"></el-checkbox>
        <el-checkbox v-model="bClear" label="Clear"></el-checkbox>
      </template>
      <el-button type="primary" round plain @click="setChatMessageBuilder">SetChatMessageBuilder</el-button>
      <el-button type="primary" round plain @click="sendLastBuildChatMsgTo">SendLastBuildChatMsgTo</el-button>
    </div>
    <div class="in-line">
      <el-button type="primary" round plain @click="isMeetingChatLegalNoticeAvailable">IsMeetingChatLegalNoticeAvailable</el-button>
      <el-button type="primary" round plain @click="getChatLegalNoticesPrompt">GetChatLegalNoticesPrompt</el-button>
      <el-button type="primary" round plain @click="getChatLegalNoticesExplained">GetChatLegalNoticesExplained</el-button>
    </div>
    <div class="in-line">
      <el-button type="primary" round plain @click="isShareMeetingChatLegalNoticeAvailable">IsShareMeetingChatLegalNoticeAvailable</el-button>
      <el-button type="primary" round plain @click="getShareMeetingChatStartedLegalNoticeContent">GetShareMeetingChatStartedLegalNoticeContent</el-button>
      <el-button type="primary" round plain @click="getShareMeetingChatStoppedLegalNoticeContent">GetShareMeetingChatStoppedLegalNoticeContent</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="msgID" placeholder="Select msgID" @focus="getAllChatMessageID">
        <el-option v-for="(item, index) in chatMessageIDList" :key="index" :label="item.msgID" :value="item.msgID"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="getChatMessageById">GetChatMessageById</el-button>
      <el-button type="primary" round plain @click="isChatMessageCanBeDeleted">IsChatMessageCanBeDeleted</el-button>
      <el-button type="primary" round plain @click="deleteChatMessage">DeleteChatMessage</el-button>
    </div>
    <div class="in-line">
      <el-button type="primary" round plain @click="isFileTransferEnabled">IsFileTransferEnabled</el-button>
      <el-button type="primary" round plain @click="getTransferFileTypeAllowList">GetTransferFileTypeAllowList</el-button>
      <el-button type="primary" round plain @click="getMaxTransferFileSizeBytes">GetMaxTransferFileSizeBytes</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="participants_value" placeholder="Select" @focus="getParticipantsList">
        <el-option v-for="(item, index) in participantsList" :key="index" :label="item.userName" :value="item.userID"></el-option>
      </el-select>
      <el-input v-model="filePath" placeholder="input file path" />
      <el-button type="primary" round plain @click="transferFile">TransferFile</el-button>
    </div>
    <div class="in-line">
      <el-input v-model="filePath" placeholder="input file path" />
      <el-button type="primary" round plain @click="transferFileToAll">TransferFileToAll</el-button>
    </div>
  </div>
  <div class="result">Result: {{result}}</div>
  <div class="result"> 
    <div class="result-item"> 
      <span class="title">onChatMsgNotification <el-button size="small" @click="onChatMsgNotification_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onChatMsgNotification_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onChatStatusChangedNotification <el-button size="small" @click="onChatStatusChangedNotification_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onChatStatusChangedNotification_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onChatMsgDeleteNotification <el-button size="small" @click="onChatMsgDeleteNotification_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onChatMsgDeleteNotification_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onChatMessageEditNotification <el-button size="small" @click="onChatMessageEditNotification_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onChatMessageEditNotification_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onShareMeetingChatStatusChanged <el-button size="small" @click="onShareMeetingChatStatusChanged_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onShareMeetingChatStatusChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onFileSendStart <el-button size="small" @click="onFileSendStart_options=[]">Clear</el-button></span> 
      <el-button type="primary" size="small" round plain @click="getReceiver">GetReceiver</el-button>
      <el-button type="primary" size="small" round plain @click="cancelSend">CancelSend</el-button>
      <div class="callback-list">
        <span v-for="(item, index) in onFileSendStart_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onFileReceived <el-button size="small" @click="onFileReceived_options=[]">Clear</el-button></span> 
      <el-button type="primary" size="small" round plain @click="getSender">GetSender</el-button>
      <el-button type="primary" size="small" round plain @click="cancelReceive">CancelReceive</el-button>
      <el-input v-model.trim="path" size="small" placeholder="input path" />
      <el-button type="primary" size="small" round plain @click="startReceive">StartReceive</el-button>
      <div class="callback-list">
        <span v-for="(item, index) in onFileReceived_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onFileTransferProgress <el-button size="small" @click="onFileTransferProgress_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onFileTransferProgress_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { SDKChatPrivilege, SDKChatMessageType, SDKChatMessageDeleteType } from '../../../lib/settings.js'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const platform = remote.getGlobal('platform')
const result = ref()
const onChatMsgNotification_options = ref<any[]>([])
const onChatStatusChangedNotification_options = ref<any[]>([])
const onChatMsgDeleteNotification_options = ref<any[]>([])
const onChatMessageEditNotification_options = ref<any[]>([])
const onShareMeetingChatStatusChanged_options = ref<any[]>([])
const onFileSendStart_options = ref<any[]>([])
const onFileReceived_options = ref<any[]>([])
const onFileTransferProgress_options = ref<any[]>([])

const onChatMsgNotification = (chatMsg: any, content: string) => {
  console.log('onChatMsgNotification', chatMsg, content)
  let desc = `${JSON.stringify(chatMsg)}-${content}`
  onChatMsgNotification_options.value.push(desc)
}
const onChatStatusChangedNotification = (status: any) => {
  console.log('onChatStatusChangedNotification', status)
  let content = JSON.stringify(status)
  onChatStatusChangedNotification_options.value.push(content)
}
const onChatMsgDeleteNotification = (msgID: string, deleteBy: number) => {
  console.log('onChatMsgDeleteNotification', msgID, deleteBy)
  let content = `msgID: ${msgID}, deleteBy: ${Object.keys(SDKChatMessageDeleteType).find(k => SDKChatMessageDeleteType[k] == deleteBy)}`
  onChatMsgDeleteNotification_options.value.push(content)
}
const onChatMessageEditNotification = (chatMsg: any) => {
  console.log('onChatMessageEditNotification', chatMsg)
  let desc = `${JSON.stringify(chatMsg)}`
  onChatMessageEditNotification_options.value.push(desc)
}
const onShareMeetingChatStatusChanged = (isStart: boolean) => {
  console.log('onShareMeetingChatStatusChanged', isStart)
  onShareMeetingChatStatusChanged_options.value.push(`isStart: ${isStart}`)
}
const onFileSendStart = (status: any) => {
  console.log('onFileSendStart', status)
  let content = JSON.stringify(status)
  onFileSendStart_options.value.push(content)
}
const onFileReceived = (status: any) => {
  console.log('onFileReceived', status)
  let content = JSON.stringify(status)
  onFileReceived_options.value.push(content)
}
const onFileTransferProgress = (status: any) => {
  console.log('onFileTransferProgress', status)
  let content = JSON.stringify(status)
  onFileTransferProgress_options.value.push(content)
}
const zoomMeetingChat = remote.app.zoomSdkModule.Meeting.GetMeetingChat({ onChatMsgNotification, onChatStatusChangedNotification,
  onChatMsgDeleteNotification, onChatMessageEditNotification, onShareMeetingChatStatusChanged, onFileSendStart, onFileReceived, onFileTransferProgress })
const zoomMeetingParticipants = remote.app.zoomSdkModule.Meeting.GetMeetingParticipantsCtrl()
const participantsList = ref<any[]>([])
const participants_value = ref()
const privilege = ref(1)
const chatMsgtType = ref(1)
const chatContent = ref('')
const threadId = ref('')
const positionStart = ref()
const positionEnd = ref()
const bClearStyles = ref(false)
const bClear = ref(false)
const chatMessageIDList = ref<any[]>([])
const msgID = ref('')
const filePath = ref('')

const setOnChatMsgNotificationCB = () => {
  let ret = zoomMeetingChat.SetOnChatMsgNotificationCB(onChatMsgNotification)
  result.value = getSDKResult('SetOnChatMsgNotificationCB', ret)
}

const setOnChatStatusChangedNotificationCB = () => {
  let ret = zoomMeetingChat.SetOnChatStatusChangedNotificationCB(onChatStatusChangedNotification)
  result.value = getSDKResult('SetOnChatStatusChangedNotificationCB', ret)
}

const setOnChatMsgDeleteNotificationCB = () => {
  let ret = zoomMeetingChat.SetOnChatMsgDeleteNotificationCB(onChatMsgDeleteNotification)
  result.value = getSDKResult('SetOnChatMsgDeleteNotificationCB', ret)
}

const setOnChatMessageEditNotificationCB = () => {
  let ret = zoomMeetingChat.SetOnChatMessageEditNotificationCB(onChatMessageEditNotification)
  result.value = getSDKResult('SetOnChatMessageEditNotificationCB', ret)
}

const setOnShareMeetingChatStatusChangedCB = () => {
  let ret = zoomMeetingChat.SetOnShareMeetingChatStatusChangedCB(onShareMeetingChatStatusChanged)
  result.value = getSDKResult('SetOnShareMeetingChatStatusChangedCB', ret)
}

const setOnFileSendStartCB = () => {
  let ret = zoomMeetingChat.SetOnFileSendStartCB(onFileSendStart)
  result.value = getSDKResult('SetOnFileSendStartCB', ret)
}

const setOnFileReceivedCB = () => {
  let ret = zoomMeetingChat.SetOnFileReceivedCB(onFileReceived)
  result.value = getSDKResult('SetOnFileReceivedCB', ret)
}

const setOnFileTransferProgressCB = () => {
  let ret = zoomMeetingChat.SetOnFileTransferProgressCB(onFileTransferProgress)
  result.value = getSDKResult('SetOnFileTransferProgressCB', ret)
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

const getChatStatus = () => {
  let ret = zoomMeetingChat.GetChatStatus()
  result.value = getSDKResult('GetChatStatus', ret)
}

const setParticipantsChatPrivilege = () => {
  let param = { privilege: privilege.value }
  let ret = zoomMeetingChat.SetParticipantsChatPrivilege(param)
  result.value = getSDKResult('SetParticipantsChatPrivilege', ret, param)
}

const isMeetingChatLegalNoticeAvailable = () => {
  let ret = zoomMeetingChat.IsMeetingChatLegalNoticeAvailable()
  result.value = getSDKResult('IsMeetingChatLegalNoticeAvailable', ret)
}

const getChatLegalNoticesPrompt = () => {
  let ret = zoomMeetingChat.GetChatLegalNoticesPrompt()
  result.value = getSDKResult('GetChatLegalNoticesPrompt', ret)
}

const getChatLegalNoticesExplained = () => {
  let ret = zoomMeetingChat.GetChatLegalNoticesExplained()
  result.value = getSDKResult('GetChatLegalNoticesExplained', ret)
}

const isShareMeetingChatLegalNoticeAvailable = () => {
  let ret = zoomMeetingChat.IsShareMeetingChatLegalNoticeAvailable()
  result.value = getSDKResult('IsShareMeetingChatLegalNoticeAvailable', ret)
}

const getShareMeetingChatStartedLegalNoticeContent = () => {
  let ret = zoomMeetingChat.GetShareMeetingChatStartedLegalNoticeContent()
  result.value = getSDKResult('GetShareMeetingChatStartedLegalNoticeContent', ret)
}

const getShareMeetingChatStoppedLegalNoticeContent = () => {
  let ret = zoomMeetingChat.GetShareMeetingChatStoppedLegalNoticeContent()
  result.value = getSDKResult('GetShareMeetingChatStoppedLegalNoticeContent', ret)
}

const getAllChatMessageID = () => {
  let list = zoomMeetingChat.GetAllChatMessageID()
  result.value = getSDKResult('GetAllChatMessageID', list)
  chatMessageIDList.value = list
  if (chatMessageIDList.value.length == 0) {
    msgID.value = ''
  } else {
    msgID.value = list[0].msgID
  }
}

const isChatMessageCanBeDeleted = () => {
  let param = { msgID: msgID.value }
  let ret = zoomMeetingChat.IsChatMessageCanBeDeleted(param)
  result.value = getSDKResult('IsChatMessageCanBeDeleted', ret, param)
}

const deleteChatMessage = () => {
  let param = { msgID: msgID.value }
  let ret = zoomMeetingChat.DeleteChatMessage(param)
  result.value = getSDKResult('DeleteChatMessage', ret, param)
}

const getChatMessageById = () => {
  let param = { msgID: msgID.value }
  let ret = zoomMeetingChat.GetChatMessageById(param)
  result.value = getSDKResult('GetChatMessageById', ret, param)
}

const setChatMessageBuilder = () => {
  let param = { 
    receiver: participants_value.value,
    type: chatMsgtType.value,
    content: chatContent.value,
    threadId: threadId.value,
    positionStart: positionStart.value,
    positionEnd: positionEnd.value,
    bClearStyles: bClearStyles.value,
    bClear: bClear.value
  }
  let ret = zoomMeetingChat.SetChatMessageBuilder(param)
  result.value = getSDKResult('SetChatMessageBuilder', ret, param)
}

const sendLastBuildChatMsgTo = () => {
  let ret = zoomMeetingChat.SendLastBuildChatMsgTo()
  result.value = getSDKResult('SendLastBuildChatMsgTo', ret)
}

const isFileTransferEnabled = () => {
  let ret = zoomMeetingChat.IsFileTransferEnabled()
  result.value = getSDKResult('IsFileTransferEnabled', ret)
}

const getTransferFileTypeAllowList = () => {
  let ret = zoomMeetingChat.GetTransferFileTypeAllowList()
  result.value = getSDKResult('GetTransferFileTypeAllowList', ret)
}

const getMaxTransferFileSizeBytes = () => {
  let ret = zoomMeetingChat.GetMaxTransferFileSizeBytes()
  result.value = getSDKResult('GetMaxTransferFileSizeBytes', ret, null, false)
}

const transferFile = () => {
  let param = { 
    userid: participants_value.value,
    filePath: filePath.value
  }
  let ret = zoomMeetingChat.TransferFile(param)
  result.value = getSDKResult('TransferFile', ret, param)
}

const transferFileToAll = () => {
  let param = { filePath: filePath.value }
  let ret = zoomMeetingChat.TransferFileToAll(param)
  result.value = getSDKResult('TransferFileToAll', ret, param)
}

const getReceiver = () => {
  let ret = zoomMeetingChat.GetReceiver()
  result.value = getSDKResult('GetReceiver', ret, null, false)
}

const cancelSend = () => {
  let ret = zoomMeetingChat.CancelSend()
  result.value = getSDKResult('CancelSend', ret)
}

const getSender = () => {
  let ret = zoomMeetingChat.GetSender()
  result.value = getSDKResult('GetSender', ret, null, false)
}

const cancelReceive = () => {
  let ret = zoomMeetingChat.CancelReceive()
  result.value = getSDKResult('CancelReceive', ret)
}

const path = ref('')
const startReceive = () => {
  let param = { path: path.value }
  let ret = zoomMeetingChat.StartReceive(param)
  result.value = getSDKResult('StartReceive', ret, param)
}

onMounted(() => {
  getParticipantsList()
})
</script>