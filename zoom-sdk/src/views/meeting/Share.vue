<template>
  <div class="meeting">
    <el-button type="primary" round plain @click="setOnSharingStatusCB">SetOnSharingStatusCB</el-button>
    <el-button type="primary" round plain @click="setOnFailedToStartShareCB">SetOnFailedToStartShareCB</el-button>
    <el-button type="primary" round plain @click="setOnOptimizingShareForVideoClipStatusChangedCB">SetOnOptimizingShareForVideoClipStatusChangedCB</el-button>
    <div class="in-line">
      <el-select v-model="monitorID" placeholder="Select">
        <el-option v-for="(item, index) in monitorsList" :key="index" :label="item.label" :value="item.value"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="startMonitorShare">StartMonitorShare</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="hShare_app" placeholder="Select">
        <el-option v-for="(item, index) in appsList" :key="index" :label="item.label" :value="item.value"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="startAppShare">StartAppShare</el-button>
    </div>
    <el-button type="primary" round plain @click="stopShare">StopShare</el-button>
    <el-button type="primary" round plain @click="isWhiteboardLegalNoticeAvailable">IsWhiteboardLegalNoticeAvailable</el-button>
    <el-button type="primary" round plain @click="getWhiteboardLegalNoticesPrompt">GetWhiteboardLegalNoticesPrompt</el-button>
    <el-button type="primary" round plain @click="getWhiteboardLegalNoticesExplained">GetWhiteboardLegalNoticesExplained</el-button>
    <div class="in-line">
      <el-select v-model="audio_share_mode" placeholder="Select">
         <el-option v-for="(value, key) in AudioShareMode" :key="key" :label="key" :value="value"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="setAudioShareMode">SetAudioShareMode</el-button>
      <el-button type="primary" round plain @click="getAudioShareMode">GetAudioShareMode</el-button>
    </div>
  </div>
  <div class="result">Result: {{result}}</div>
  <div class="result"> 
    <div class="result-item">
      <span class="title">onSharingStatus <el-button size="small" @click="onSharingStatus_options=[]">Clear</el-button></span>  
      <div class="callback-list">
        <span v-for="(item, index) in onSharingStatus_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onFailedToStartShare <el-button size="small" @click="onFailedToStartShare_value=0">Clear</el-button></span> 
      <span v-if="onFailedToStartShare_value">updated {{onFailedToStartShare_value}}</span> 
    </div>
    <div class="result-item">
      <span class="title">onOptimizingShareForVideoClipStatusChanged <el-button size="small" @click="onOptimizingShareForVideoClipStatusChanged_options=[]">Clear</el-button></span>  
      <div class="callback-list">
        <span v-for="(item, index) in onOptimizingShareForVideoClipStatusChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { ShareStatus, AudioShareMode } from '../../../lib/settings.js'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const desktopCapturer = remote.desktopCapturer
const { name } = require('../../../package.json')
const result = ref()
const onSharingStatus_options = ref<any[]>([])
const onFailedToStartShare_value = ref(0)
const onOptimizingShareForVideoClipStatusChanged_options = ref<any[]>([])

const onSharingStatus = (shareInfo: any) => {
  console.log('onSharingStatus', shareInfo)
  shareInfo.windowID = Number(shareInfo.windowID)
  let statusContent = `${JSON.stringify(shareInfo)} - ${Object.keys(ShareStatus).find(k => ShareStatus[k] == shareInfo.status)}`
  onSharingStatus_options.value.push(statusContent)
}

const onFailedToStartShare = () => {
  console.log('onFailedToStartShare')
  onFailedToStartShare_value.value++
}

const onOptimizingShareForVideoClipStatusChanged = (shareInfo: any) => {
  console.log('onOptimizingShareForVideoClipStatusChanged', shareInfo)
  shareInfo.windowID = Number(shareInfo.windowID)
  let statusContent = `${JSON.stringify(shareInfo)} - ${Object.keys(ShareStatus).find(k => ShareStatus[k] == shareInfo.status)}`
  onOptimizingShareForVideoClipStatusChanged_options.value.push(statusContent)
}

const zoomMeetingShare = remote.app.zoomSdkModule.Meeting.GetMeetingShare({ onSharingStatus, onFailedToStartShare, onOptimizingShareForVideoClipStatusChanged })
const monitorsList = ref<any[]>([])
const appsList = ref<any[]>([])
const monitorID = ref()
const hShare_app = ref()
const audio_share_mode = ref(0)

const setOnSharingStatusCB = () => {
  let ret = zoomMeetingShare.MeetingShare_SetOnSharingStatusCB(onSharingStatus)
  result.value = getSDKResult('MeetingShare_SetOnSharingStatusCB', ret)
}

const setOnFailedToStartShareCB = () => {
  let ret = zoomMeetingShare.MeetingShare_SetOnFailedToStartShareCB(onFailedToStartShare)
  result.value = getSDKResult('MeetingShare_SetOnFailedToStartShareCB', ret)
}

const setOnOptimizingShareForVideoClipStatusChangedCB = () => {
  let ret = zoomMeetingShare.MeetingShare_SetOnOptimizingShareForVideoClipStatusChangedCB(onOptimizingShareForVideoClipStatusChanged)
  result.value = getSDKResult('MeetingShare_SetOnOptimizingShareForVideoClipStatusChangedCB', ret)
}

const startMonitorShare = () => {
  let param = {
    zn_monitorID: monitorID.value
  }
  let ret = zoomMeetingShare.MeetingShare_StartMonitorShare(param)
  result.value = getSDKResult('MeetingShare_StartMonitorShare', ret, param)
}

const startAppShare = () => {
  let param = {
    zn_hShare_app: hShare_app.value
  }
  let ret = zoomMeetingShare.MeetingShare_StartAppShare(param)
  result.value = getSDKResult('MeetingShare_StartAppShare', ret, param)
}

const stopShare = () => {
  let ret = zoomMeetingShare.MeetingShare_StopShare()
  result.value = getSDKResult('MeetingShare_StopShare', ret)
}

const isWhiteboardLegalNoticeAvailable = () => {
  let ret = zoomMeetingShare.MeetingShare_IsWhiteboardLegalNoticeAvailable();
  result.value = getSDKResult('MeetingShare_IsWhiteboardLegalNoticeAvailable', ret)
}

const getWhiteboardLegalNoticesPrompt = () => {
  let ret = zoomMeetingShare.MeetingShare_GetWhiteboardLegalNoticesPrompt();
  result.value = getSDKResult('MeetingShare_GetWhiteboardLegalNoticesPrompt', ret)
}

const getWhiteboardLegalNoticesExplained = () => {
  let ret = zoomMeetingShare.MeetingShare_GetWhiteboardLegalNoticesExplained();
  result.value = getSDKResult('MeetingShare_GetWhiteboardLegalNoticesExplained', ret)
}

const setAudioShareMode = () => {
  let param = {
    mode: audio_share_mode.value
  }
  let ret = zoomMeetingShare.MeetingShare_SetAudioShareMode(param)
  result.value = getSDKResult('MeetingShare_SetAudioShareMode', ret, param)
}

const getAudioShareMode = () => {
  let ret = zoomMeetingShare.MeetingShare_GetAudioShareMode();
  result.value = getSDKResult('MeetingShare_GetAudioShareMode', ret)
}

onMounted(() => {
  desktopCapturer.getSources({ types: ['screen'] }).then(async (sources: any) => {
		console.log(sources)
		for (let i in sources) {
			let obj = {...sources[i]}
			let shareMonitorID: string = obj.id.split(':')[1]
			if (window.navigator.userAgent.indexOf('Windows') > -1) {
				shareMonitorID = `\\\\.\\DISPLAY${Number(obj.id.split(':')[1]) + 1}`
			}
			monitorsList.value.push({value: shareMonitorID, label: `${obj.name} - ${obj.id}`})
		}
    if (monitorsList.value.length > 0) {
      monitorID.value = monitorsList.value[0].value
    }
		console.log('monitorsList', monitorsList.value);
	});
	desktopCapturer.getSources({ types: ['window'] }).then(async (sources: any) => {
		console.log(sources)
		for (let i in sources) {
			let obj = {...sources[i]}
      if (obj.name != name) {
        let shareAppHandle = obj.id.split(':')[1]
        if (window.navigator.userAgent.indexOf('Windows') > -1) {
          shareAppHandle = Number(obj.id.split(':')[1]).toString(16)
        }
        appsList.value.push({value: shareAppHandle, label: obj.name})
      }
		}
		console.log('appsList', appsList.value);
    if (appsList.value.length > 0) {
      hShare_app.value = appsList.value[0].value
    }
	})
})
</script>