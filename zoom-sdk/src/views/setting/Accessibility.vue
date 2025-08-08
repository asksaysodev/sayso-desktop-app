<template>
  <div class="setting">
    <el-checkbox v-model="enable" label="EnableAlwaysShowMeetingControls" @change="enableAlwaysShowMeetingControls"></el-checkbox>
    <el-button type="primary" round plain @click="isAlwaysShowMeetingControlsEnable">IsAlwaysShowMeetingControlsEnable</el-button>
  </div>
  <div class="result">Result: {{result}}</div>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const zoomSettingAccessibilityCtrl = remote.app.zoomSdkModule.Setting.GetSettingAccessibilityCtrl()
const enable = ref(false)
const result = ref()

const enableAlwaysShowMeetingControls = () => {
  let param = {bEnable: enable.value}
  let ret = zoomSettingAccessibilityCtrl.Setting_EnableAlwaysShowMeetingControls(param)
  result.value = getSDKResult('Setting_EnableDualScreenMode', ret, param)
}

const isAlwaysShowMeetingControlsEnable = () => {
  let ret = zoomSettingAccessibilityCtrl.Setting_IsAlwaysShowMeetingControlsEnable()
  result.value = getSDKResult('Setting_IsAlwaysShowMeetingControlsEnable', ret) 
}
</script>
