<template>
  <div class="setting">
    <el-checkbox v-model="enable_dualScreenMode" label="EnableDualScreenMode" @change="enableDualScreenMode"></el-checkbox>
    <el-checkbox v-model="enable_turnOffAeroModeInSharing" label="TurnOffAeroModeInSharing" @change="turnOffAeroModeInSharing"></el-checkbox>
    <el-checkbox v-model="enable_autoFullScreenVideoWhenJoinMeeting" label="EnableAutoFullScreenVideoWhenJoinMeeting" @change="enableAutoFullScreenVideoWhenJoinMeeting"></el-checkbox>
    <el-checkbox v-model="enable_splitScreenMode" label="EnableSplitScreenMode" @change="enableSplitScreenMode"></el-checkbox>
    <el-checkbox v-model="enable_displayReminderWindowWhenExit" label="EnableDisplayReminderWindowWhenExit" @change="enableDisplayReminderWindowWhenExit"></el-checkbox>
    <el-checkbox v-model="enable_showMyMeetingElapseTime" label="EnableShowMyMeetingElapseTime" @change="enableShowMyMeetingElapseTime"></el-checkbox>
    <el-button type="primary" round plain @click="isDualScreenModeEnabled">IsDualScreenModeEnabled</el-button>
    <el-button type="primary" round plain @click="isAeroModeInSharingTurnOff">IsAeroModeInSharingTurnOff</el-button>
    <el-button type="primary" round plain @click="isAutoFullScreenVideoWhenJoinMeetingEnabled">IsAutoFullScreenVideoWhenJoinMeetingEnabled</el-button>
    <el-button type="primary" round plain @click="isSplitScreenModeEnabled">IsSplitScreenModeEnabled</el-button>
    <el-button type="primary" round plain @click="isDisplayReminderWindowWhenExitEnabled">IsDisplayReminderWindowWhenExitEnabled</el-button>
    <el-button type="primary" round plain @click="isShowMyMeetingElapseTimeEnabled">IsShowMyMeetingElapseTimeEnabled</el-button>
  </div>
  <div class="result">Result: {{result}}</div>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const zoomSettingGeneral = remote.app.zoomSdkModule.Setting.GetGeneralSetting()
const enable_dualScreenMode = ref(false)
const enable_turnOffAeroModeInSharing = ref(false)
const enable_autoFullScreenVideoWhenJoinMeeting = ref(false)
const enable_splitScreenMode = ref(false)
const enable_displayReminderWindowWhenExit = ref(false)
const enable_showMyMeetingElapseTime = ref(false)
const result = ref()

const enableDualScreenMode = () => {
  let param = {zn_bEnable: enable_dualScreenMode.value}
  let ret = zoomSettingGeneral.Setting_EnableDualScreenMode(param)
  result.value = getSDKResult('Setting_EnableDualScreenMode', ret, param)
}

const turnOffAeroModeInSharing = () => {
  let param = {zn_bEnable: enable_turnOffAeroModeInSharing.value}
  let ret = zoomSettingGeneral.Setting_TurnOffAeroModeInSharing(param)
  result.value = getSDKResult('Setting_TurnOffAeroModeInSharing', ret, param)
}

const enableAutoFullScreenVideoWhenJoinMeeting = () => {
  let param = {zn_bEnable: enable_autoFullScreenVideoWhenJoinMeeting.value}
  let ret = zoomSettingGeneral.Setting_EnableAutoFullScreenVideoWhenJoinMeeting(param)
  result.value = getSDKResult('Setting_EnableAutoFullScreenVideoWhenJoinMeeting', ret, param)
}

const enableSplitScreenMode = () => {
  let param = {zn_bEnable: enable_splitScreenMode.value}
  let ret = zoomSettingGeneral.Setting_EnableSplitScreenMode(param)
  result.value = getSDKResult('Setting_EnableSplitScreenMode', ret, param)
}

const enableDisplayReminderWindowWhenExit = () => {
  let param = {bEnable: enable_displayReminderWindowWhenExit.value}
  let ret = zoomSettingGeneral.Setting_EnableDisplayReminderWindowWhenExit(param)
  result.value = getSDKResult('Setting_EnableDisplayReminderWindowWhenExit', ret, param)
}

const enableShowMyMeetingElapseTime = () => {
  let param = {bEnable: enable_showMyMeetingElapseTime.value}
  let ret = zoomSettingGeneral.Setting_EnableShowMyMeetingElapseTime(param)
  result.value = getSDKResult('Setting_EnableShowMyMeetingElapseTime', ret, param)
}

const isDualScreenModeEnabled = () => {
  let ret = zoomSettingGeneral.Checking_IsDualScreenModeEnabled()
  result.value = getSDKResult('Checking_IsDualScreenModeEnabled', ret)
}

const isAeroModeInSharingTurnOff = () => {
  let ret = zoomSettingGeneral.Checking_IsAeroModeInSharingTurnOff()
  result.value = getSDKResult('Checking_IsAeroModeInSharingTurnOff', ret)
}

const isAutoFullScreenVideoWhenJoinMeetingEnabled = () => {
  let ret = zoomSettingGeneral.Checking_IsAutoFullScreenVideoWhenJoinMeetingEnabled()
  result.value = getSDKResult('Checking_IsAutoFullScreenVideoWhenJoinMeetingEnabled', ret)
}

function isSplitScreenModeEnabled() {
  let ret = zoomSettingGeneral.Checking_IsSplitScreenModeEnabled()
  result.value = getSDKResult('Checking_IsSplitScreenModeEnabled', ret)
}

const isDisplayReminderWindowWhenExitEnabled = () => {
  let ret = zoomSettingGeneral.Setting_IsDisplayReminderWindowWhenExitEnabled()
  result.value = getSDKResult('Setting_IsDisplayReminderWindowWhenExitEnabled', ret)
}

const isShowMyMeetingElapseTimeEnabled = () => {
  let ret = zoomSettingGeneral.Setting_IsShowMyMeetingElapseTimeEnabled()
  result.value = getSDKResult('Setting_IsShowMyMeetingElapseTimeEnabled', ret)
}
</script>