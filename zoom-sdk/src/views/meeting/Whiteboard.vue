<template>
  <div class="meeting">
    <el-button type="primary" round plain @click="setOnWhiteboardStatusChangedCB">SetOnWhiteboardStatusChangedCB</el-button>
    <el-button type="primary" round plain @click="setOnWhiteboardSettingsChangedCB">SetOnWhiteboardSettingsChangedCB</el-button>
    <div class="in-line">
      <el-select v-model="windowId" placeholder="Select">
        <el-option v-for="(item, index) in windowIdList" :key="index" :label="item.label" :value="item.value"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="setOwnerWindow">SetOwnerWindow</el-button>
      <el-button type="primary" round plain @click="isSupportWhiteBoard">IsSupportWhiteBoard</el-button>
      <el-button type="primary" round plain @click="canStartShareWhiteboard">CanStartShareWhiteboard</el-button>
    </div>
    <div class="in-line">
      <el-input-number v-model="x" :min="-1" />
      <el-input-number v-model="y" :min="-1" />
      <el-button type="primary" round plain @click="showDashboardView">ShowDashboardView</el-button>
      <el-input-number v-model="width" :min="-1" />
      <el-input-number v-model="height" :min="-1" />
      <el-button type="primary" round plain @click="setDashboardViewSize">SetDashboardViewSize</el-button>
    </div>
    <div class="in-line">
      <el-input-number v-model="x" :min="-1" />
      <el-input-number v-model="y" :min="-1" />
      <el-button type="primary" round plain @click="setWhiteboardViewPos">SetWhiteboardViewPos</el-button>
      <el-input-number v-model="width" :min="-1" />
      <el-input-number v-model="height" :min="-1" />
      <el-button type="primary" round plain @click="setWhiteboardViewSize">SetWhiteboardViewSize</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="shareOption" placeholder="Select">
        <el-option v-for="(item, key) in SDKWhiteboardShareOption" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="setWhiteboardShareOption">SetWhiteboardShareOption</el-button>
      <el-button type="primary" round plain @click="getWhiteboardShareOption">GetWhiteboardShareOption</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="createOption" placeholder="Select">
        <el-option v-for="(item, key) in SDKWhiteboardCreateOption" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="setWhiteboardCreateOption">SetWhiteboardCreateOption</el-button>
      <el-button type="primary" round plain @click="getWhiteboardCreateOption">GetWhiteboardCreateOption</el-button>
    </div>
    <el-checkbox v-model="enable_participantsCreateWithoutHost" label="EnableParticipantsCreateWithoutHost" @change="enableParticipantsCreateWithoutHost"></el-checkbox>
    <el-button type="primary" round plain @click="isParticipantsCreateWithoutHostEnabled">IsParticipantsCreateWithoutHostEnabled</el-button>
    <el-button type="primary" round plain @click="isOtherSharingWhiteboard">IsOtherSharingWhiteboard</el-button>
    <el-button type="primary" round plain @click="isSharingWhiteboardOut">IsSharingWhiteboardOut</el-button>
  </div>
  <div class="result">Result: {{result}}</div>
  <div class="result"> 
    <div class="result-item"> 
      <span class="title">onWhiteboardStatusChanged <el-button size="small" @click="onWhiteboardStatusChanged_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onWhiteboardStatusChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onWhiteboardSettingsChanged <el-button size="small" @click="onWhiteboardSettingsChanged_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onWhiteboardSettingsChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onBeforeMount } from 'vue'
import { getSDKResult } from '../../assets/ts/util'
import { SDKWhiteboardShareOption, SDKWhiteboardCreateOption } from '../../../lib/settings.js'
const remote = window.require('@electron/remote')
const desktopCapturer = remote.desktopCapturer

const result = ref()
const onWhiteboardStatusChanged_options = ref<string[]>([])
const onWhiteboardSettingsChanged_options = ref<string[]>([])

const onWhiteboardStatusChanged = (status: number) => {
  console.log('onWhiteboardStatusChanged', status)
  onWhiteboardStatusChanged_options.value.push(`status: ${status}`)
}
const onWhiteboardSettingsChanged = (shareOption: number, createOption: number, bEnable: boolean) => {
  console.log('onWhiteboardSettingsChanged', shareOption, createOption, bEnable)
  onWhiteboardSettingsChanged_options.value.push(`shareOption: ${shareOption} createOption: ${createOption} bEnable: ${bEnable}`)
}
const zoomMeetingWhiteboard = remote.app.zoomSdkModule.Meeting.GetMeetingWhiteboard({ onWhiteboardStatusChanged, onWhiteboardSettingsChanged })

const setOnWhiteboardStatusChangedCB = () => {
  let ret = zoomMeetingWhiteboard.SetOnWhiteboardStatusChangedCB(onWhiteboardStatusChanged)
  result.value = getSDKResult('SetOnWhiteboardStatusChangedCB', ret)
}

const setOnWhiteboardSettingsChangedCB = () => {
  let ret = zoomMeetingWhiteboard.SetOnWhiteboardSettingsChangedCB(onWhiteboardSettingsChanged)
  result.value = getSDKResult('SetOnWhiteboardSettingsChangedCB', ret)
}

const windowId = ref<string>('')
const setOwnerWindow = () => {
  let param = { windowId: windowId.value }
  let ret = zoomMeetingWhiteboard.SetOwnerWindow(param)
  result.value = getSDKResult('SetOwnerWindow', ret, param)
}

const isSupportWhiteBoard = () => {
  let ret = zoomMeetingWhiteboard.IsSupportWhiteBoard()
  result.value = getSDKResult('IsSupportWhiteBoard', ret)
}

const canStartShareWhiteboard = () => {
  let ret = zoomMeetingWhiteboard.CanStartShareWhiteboard()
  result.value = getSDKResult('CanStartShareWhiteboard', ret)
}

const x = ref(-1)
const y = ref(-1)
const showDashboardView = () => {
  let param = { x: x.value, y: y.value }
  let ret = zoomMeetingWhiteboard.ShowDashboardView(param)
  result.value = getSDKResult('ShowDashboardView', ret)
}

const width = ref(-1)
const height = ref(-1)
const setDashboardViewSize = () => {
  let param = { width: width.value, height: height.value }
  let ret = zoomMeetingWhiteboard.SetDashboardViewSize(param)
  result.value = getSDKResult('SetDashboardViewSize', ret)
}

const setWhiteboardViewPos = () => {
  let param = { x: x.value, y: y.value }
  let ret = zoomMeetingWhiteboard.SetWhiteboardViewPos(param)
  result.value = getSDKResult('SetWhiteboardViewPos', ret)
}

const setWhiteboardViewSize = () => {
  let param = { width: width.value, height: height.value }
  let ret = zoomMeetingWhiteboard.SetWhiteboardViewSize(param)
  result.value = getSDKResult('SetWhiteboardViewSize', ret)
}

const shareOption = ref(SDKWhiteboardShareOption.SDKWhiteboardShareOption_HostShare)
const setWhiteboardShareOption = () => {
  let param = { 
    option: shareOption.value
  }
  let ret = zoomMeetingWhiteboard.SetWhiteboardShareOption(param)
  result.value = getSDKResult('SetWhiteboardShareOption', ret, param)
}

const getWhiteboardShareOption = () => {
  let ret = zoomMeetingWhiteboard.GetWhiteboardShareOption()
  console.log('GetWhiteboardShareOption', ret)
  let option = Object.keys(SDKWhiteboardShareOption).find(k => SDKWhiteboardShareOption[k] == ret.option)
  result.value = getSDKResult('GetWhiteboardShareOption', option)
}

const createOption = ref(SDKWhiteboardCreateOption.SDKWhiteboardCreateOption_HostOnly)
const setWhiteboardCreateOption = () => {
  let param = { 
    option: createOption.value
  }
  let ret = zoomMeetingWhiteboard.SetWhiteboardCreateOption(param)
  result.value = getSDKResult('SetWhiteboardCreateOption', ret, param)
}

const getWhiteboardCreateOption = () => {
  let ret = zoomMeetingWhiteboard.GetWhiteboardCreateOption()
  console.log('GetWhiteboardCreateOption', ret)
  let option = Object.keys(SDKWhiteboardCreateOption).find(k => SDKWhiteboardCreateOption[k] == ret.option)
  result.value = getSDKResult('GetWhiteboardCreateOption', option)
}

const enable_participantsCreateWithoutHost = ref(false)
const enableParticipantsCreateWithoutHost = () => {
  let param = { 
    bEnable: enable_participantsCreateWithoutHost.value
  }
  let ret = zoomMeetingWhiteboard.EnableParticipantsCreateWithoutHost(param)
  result.value = getSDKResult('EnableParticipantsCreateWithoutHost', ret, param)
}

const isParticipantsCreateWithoutHostEnabled = () => {
  let ret = zoomMeetingWhiteboard.IsParticipantsCreateWithoutHostEnabled()
  result.value = getSDKResult('IsParticipantsCreateWithoutHostEnabled', ret)
}

const isOtherSharingWhiteboard = () => {
  let ret = zoomMeetingWhiteboard.IsOtherSharingWhiteboard()
  result.value = getSDKResult('IsOtherSharingWhiteboard', ret)
}

const isSharingWhiteboardOut = () => {
  let ret = zoomMeetingWhiteboard.IsSharingWhiteboardOut()
  result.value = getSDKResult('IsSharingWhiteboardOut', ret)
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

<style scoped lang="scss">
.result {
  .title {
    width: 260px;
  }
  .el-input {
    width: 60px;
  }
  .el-select {
    width: 100px;
  }
}
</style>