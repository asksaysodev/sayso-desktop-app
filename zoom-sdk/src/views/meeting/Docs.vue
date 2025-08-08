<template>
  <div class="meeting">
    <el-button type="primary" round plain @click="setOnDocsSharingSourceInfoChangedCB">SetOnDocsSharingSourceInfoChangedCB</el-button>
    <el-button type="primary" round plain @click="setOnDocsPermissionChangedCB">SetOnDocsPermissionChangedCB</el-button>
    <el-button type="primary" round plain @click="isSupportDocs">IsSupportDocs</el-button>
    <el-button type="primary" round plain @click="canStartShareDocs">CanStartShareDocs</el-button>
    <el-button type="primary" round plain @click="canSetDocsOption">CanSetDocsOption</el-button>
    <div class="in-line">
      <el-select v-model="setDocsShareOption_option" placeholder="Select option">
        <el-option v-for="(item, key) in SDKDocsShareOption" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="setDocsShareOption">SetDocsShareOption</el-button>
      <el-button type="primary" round plain @click="getDocsShareOption">GetDocsShareOption</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="setDocsCreateOption_option" placeholder="Select option">
        <el-option v-for="(item, key) in SDKDocsCreateOption" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="setDocsCreateOption">SetDocsCreateOption</el-button>
      <el-button type="primary" round plain @click="getDocsCreateOption">GetDocsCreateOption</el-button>
    </div>
    <el-button type="primary" round plain @click="isOtherSharingDocs">IsOtherSharingDocs</el-button>
    <el-button type="primary" round plain @click="isSharingDocsOut">IsSharingDocsOut</el-button>
    <div class="in-line">
      <el-select v-model="setOwnerWindow_windowId" placeholder="Select">
        <el-option v-for="(item, index) in windowIdList" :key="index" :label="item.label" :value="item.value"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="setOwnerWindow">SetOwnerWindow</el-button>
      <el-input-number v-model="setDocsViewPos_x" :min="-1" />
      <el-input-number v-model="setDocsViewPos_y" :min="-1" />
      <el-button type="primary" round plain @click="showDocsDashboard">ShowDocsDashboard</el-button>
    </div>
    <div class="in-line">
      <el-input-number v-model="setDocDashboardSize_width" :min="-1" />
      <el-input-number v-model="setDocDashboardSize_height" :min="-1" />
      <el-button type="primary" round plain @click="setDocDashboardSize">SetDocDashboardSize</el-button>
    </div>
    <div class="in-line">
      <el-input-number v-model="setDocsViewPos_x" :min="-1" />
      <el-input-number v-model="setDocsViewPos_y" :min="-1" />
      <el-button type="primary" round plain @click="setDocsViewPos">SetDocsViewPos</el-button>
    </div>
    <div class="in-line">
      <el-input-number v-model="setDocsViewSize_width" :min="-1" />
      <el-input-number v-model="setDocsViewSize_height" :min="-1" />
      <el-button type="primary" round plain @click="setDocsViewSize">SetDocsViewSize</el-button>
    </div>
    <div class="in-line">
      <el-button type="primary" round plain @click="getViewableDocsSharingUserList">GetViewableDocsSharingUserList</el-button>
      <el-select v-model="getDocsSharingSourceInfoList_userID" placeholder="Select option">
        <el-option v-for="(item, key) in docsSharingUserList" :key="item" :label="item" :value="item"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="getDocsSharingSourceInfoList">GetDocsSharingSourceInfoList</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="showDocsViewWindow_sourceID" placeholder="Select option">
        <el-option v-for="(item, key) in sourceInfoList" :key="key" :label="item.shareSourceID" :value="item.shareSourceID"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="showDocsViewWindow">ShowDocsViewWindow</el-button>
      <el-button type="primary" round plain @click="closeDocsViewWindow">CloseDocsViewWindow</el-button>
    </div>
    <div class="result">Result: {{ result }}</div>
    <div class="result">
      <div class="result-item">
        <span class="title">onDocsSharingSourceInfoChanged <el-button size="small" @click="onDocsSharingSourceInfoChanged_options = []">Clear</el-button></span>
        <div class="callback-list">
          <span v-for="(item, index) in onDocsSharingSourceInfoChanged_options" :key="index">{{ index }} : {{ item }}</span>
        </div>
      </div>
        <div class="result-item">
        <span class="title">onDocsPermissionChanged <el-button size="small" @click="onDocsPermissionChanged_options = []">Clear</el-button></span>
        <div class="callback-list">
          <span v-for="(item, index) in onDocsPermissionChanged_options" :key="index">{{ index }} : {{ item }}</span>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onBeforeMount } from 'vue'
import { getSDKResult } from '../../assets/ts/util'
import { SDKDocsStatus, SDKDocsShareOption, SDKDocsCreateOption } from '../../../lib/settings.js'
const remote = window.require('@electron/remote')
const desktopCapturer = remote.desktopCapturer

const result = ref('')

const onDocsSharingSourceInfoChanged_options = ref<string[]>([])
const onDocsSharingSourceInfoChanged = (info: any) => {
  let info_str = JSON.stringify(info)
  console.log('onDocsSharingSourceInfoChanged', `info: ${info_str}`)
  onDocsSharingSourceInfoChanged_options.value.push(`info: ${info_str}`)
}

const onDocsPermissionChanged_options = ref<string[]>([])
const onDocsPermissionChanged = (createOption: any, option: any) => {
  console.log('onDocsPermissionChanged', `createOption: ${createOption}, option: ${option}`)
  onDocsPermissionChanged_options.value.push(`createOption: ${createOption}, option: ${option}`)
}

const zoomMeetingDocs = remote.app.zoomSdkModule.Meeting.GetMeetingDocs({ onDocsSharingSourceInfoChanged, onDocsPermissionChanged })

const setOnDocsSharingSourceInfoChangedCB = () => {
  const ret = zoomMeetingDocs.SetOnDocsSharingSourceInfoChangedCB(onDocsSharingSourceInfoChanged)
  result.value = getSDKResult('SetOnDocsSharingSourceInfoChangedCB', ret)
}

const setOnDocsPermissionChangedCB = () => {
  const ret = zoomMeetingDocs.SetOnDocsPermissionChangedCB(onDocsPermissionChanged)
  result.value = getSDKResult('SetOnDocsPermissionChangedCB', ret)
}

const isSupportDocs = () => {
  const ret = zoomMeetingDocs.IsSupportDocs()
  console.log('IsSupportDocs', result.value)
  result.value = getSDKResult('IsSupportDocs', ret)
}

const canStartShareDocs = () => {
  const ret = zoomMeetingDocs.CanStartShareDocs()
  console.log('CanStartShareDocs', result.value)
  result.value = getSDKResult('CanStartShareDocs', ret)
}

const canSetDocsOption = () => {
  const ret = zoomMeetingDocs.CanSetDocsOption()
  console.log('CanSetDocsOption', result.value)
  result.value = getSDKResult('CanSetDocsOption', ret)
}

const setDocsShareOption_option = ref(SDKDocsShareOption.SDKDocsShareOption_HostShare)
const setDocsShareOption = () => {
  const param = {
    option: setDocsShareOption_option.value
  }
  const ret = zoomMeetingDocs.SetDocsShareOption(param)
  console.log('SetDocsShareOption', result.value, param)
  result.value = getSDKResult('SetDocsShareOption', ret, param)
}

const getDocsShareOption = () => {
  const ret = zoomMeetingDocs.GetDocsShareOption()
  console.log('GetDocsShareOption', result.value)
  result.value = getSDKResult('GetDocsShareOption', ret)
}

const setDocsCreateOption_option = ref(SDKDocsCreateOption.SDKDocsCreateOption_HostOnly)
const setDocsCreateOption = () => {
  const param = {
    option: setDocsCreateOption_option.value
  }
  const ret = zoomMeetingDocs.SetDocsCreateOption(param)
  console.log('SetDocsCreateOption', result.value, param)
  result.value = getSDKResult('SetDocsCreateOption', ret, param)
}

const getDocsCreateOption = () => {
  const ret = zoomMeetingDocs.GetDocsCreateOption()
  console.log('GetDocsCreateOption', result.value)
  result.value = getSDKResult('GetDocsCreateOption', ret)
}

const isOtherSharingDocs = () => {
  const ret = zoomMeetingDocs.IsOtherSharingDocs()
  console.log('IsOtherSharingDocs', result.value)
  result.value = getSDKResult('IsOtherSharingDocs', ret)
}

const isSharingDocsOut = () => {
  const ret = zoomMeetingDocs.IsSharingDocsOut()
  console.log('IsSharingDocsOut', result.value)
  result.value = getSDKResult('IsSharingDocsOut', ret)
}

const setOwnerWindow_windowId = ref<string>('')
const setOwnerWindow = () => {
  const param = {
    windowId: setOwnerWindow_windowId.value
  }
  const ret = zoomMeetingDocs.SetOwnerWindow(param)
  console.log('SetOwnerWindow', result.value, param)
  result.value = getSDKResult('SetOwnerWindow', ret, param)
}

const showDocsDashboard = () => {
  const param = {
    x: setDocsViewPos_x.value,
    y: setDocsViewPos_y.value
  }
  const ret = zoomMeetingDocs.ShowDocsDashboard(param)
  console.log('ShowDocsDashboard', result.value, param)
  result.value = getSDKResult('ShowDocsDashboard', ret, param)
}

const setDocDashboardSize_width = ref(0)
const setDocDashboardSize_height = ref(0)
const setDocDashboardSize = () => {
  const param = {
    width: setDocDashboardSize_width.value,
    height: setDocDashboardSize_height.value
  }
  const ret = zoomMeetingDocs.SetDocDashboardSize(param)
  console.log('SetDocDashboardSize', result.value, param)
  result.value = getSDKResult('SetDocDashboardSize', ret, param)
}

const setDocsViewPos_x = ref(0)
const setDocsViewPos_y = ref(0)
const setDocsViewPos = () => {
  const param = {
    x: setDocsViewPos_x.value,
    y: setDocsViewPos_y.value
  }
  const ret = zoomMeetingDocs.SetDocsViewPos(param)
  console.log('SetDocsViewPos', result.value, param)
  result.value = getSDKResult('SetDocsViewPos', ret, param)
}

const setDocsViewSize_width = ref(0)
const setDocsViewSize_height = ref(0)
const setDocsViewSize = () => {
  const param = {
    width: setDocsViewSize_width.value,
    height: setDocsViewSize_height.value
  }
  const ret = zoomMeetingDocs.SetDocsViewSize(param)
  console.log('SetDocsViewSize', result.value, param)
  result.value = getSDKResult('SetDocsViewSize', ret, param)
}

const showDocsViewWindow_sourceID = ref()
const showDocsViewWindow = () => {
  const param = {
    sourceID: Number(showDocsViewWindow_sourceID.value)
  }
  const ret = zoomMeetingDocs.ShowDocsViewWindow(param)
  console.log('ShowDocsViewWindow', result.value, param)
  result.value = getSDKResult('ShowDocsViewWindow', ret, param)
}

const closeDocsViewWindow = () => {
  const ret = zoomMeetingDocs.CloseDocsViewWindow()
  console.log('CloseDocsViewWindow', result.value)
  result.value = getSDKResult('CloseDocsViewWindow', ret)
}

const sourceInfoList = ref<any[]>([])
const getDocsSharingSourceInfoList_userID = ref()
const getDocsSharingSourceInfoList = () => {
  const param = {
    userID: getDocsSharingSourceInfoList_userID.value
  }
  const ret = zoomMeetingDocs.GetDocsSharingSourceInfoList(param)
  sourceInfoList.value = ret
  console.log('GetDocsSharingSourceInfoList', result.value, param)
  result.value = getSDKResult('GetDocsSharingSourceInfoList', ret, param)
}

const docsSharingUserList = ref<number[]>([])
const getViewableDocsSharingUserList = () => {
  const ret = zoomMeetingDocs.GetViewableDocsSharingUserList()
  docsSharingUserList.value = ret
  console.log('GetViewableDocsSharingUserList', result.value)
  result.value = getSDKResult('GetViewableDocsSharingUserList', ret)
}

const windowIdList = ref<any[]>([])
onBeforeMount(() => {
  desktopCapturer.getSources({ types: ['window'] }).then(async (sources: any) => {
    console.log(sources)
    let tempList = []
    for (let i in sources) {
      let obj = {...sources[i]}
      let shareAppHandle = obj.id.split(':')[1]
      if (window.navigator.userAgent.indexOf('Windows') > -1) {
        shareAppHandle = Number(obj.id.split(':')[1]).toString(16)
      }
      tempList.push({value: shareAppHandle, label: obj.name})
    }
    windowIdList.value = tempList
  })
})
</script>