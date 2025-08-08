<template>
  <div class="meeting">
    <el-button type="primary" round plain @click="startRawArchiving">StartRawArchiving</el-button>
    <el-button type="primary" round plain @click="stopRawArchiving">StopRawArchiving</el-button>
  </div>
  <div class="result">Result: {{result}}</div>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const zoomMeetingRawArchivingCtrl = remote.app.zoomSdkModule.Meeting.GetMeetingRawArchivingCtrl()
const result = ref()

const startRawArchiving = () => {
  let ret = zoomMeetingRawArchivingCtrl.StartRawArchiving()
  result.value = getSDKResult('StartRawArchiving', ret)
}

const stopRawArchiving = () => {
  let ret = zoomMeetingRawArchivingCtrl.StopRawArchiving()
  result.value = getSDKResult('StopRawArchiving', ret)
}
</script>
