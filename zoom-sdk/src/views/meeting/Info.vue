<template>
  <div class="meeting">
    <el-button type="primary" round plain @click="getMeetingNumber">GetMeetingNumber</el-button>
    <el-button type="primary" round plain @click="getMeetingTopic">GetMeetingTopic</el-button>
    <el-button type="primary" round plain @click="getMeetingID">GetMeetingID</el-button>
    <el-button type="primary" round plain @click="getMeetingType">GetMeetingType</el-button>
    <el-button type="primary" round plain @click="getInviteEmailTemplate">GetInviteEmailTemplate</el-button>
    <el-button type="primary" round plain @click="getInviteEmailTitle">GetInviteEmailTitle</el-button>
    <el-button type="primary" round plain @click="getJoinMeetingUrl">GetJoinMeetingUrl</el-button>
    <el-button type="primary" round plain @click="getMeetingHostTag">GetMeetingHostTag</el-button>
  </div>
  <div class="result">Result: {{result}}</div>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import { MeetingType } from '../../../lib/settings.js'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const result = ref()

const zoomMeetingInfo = remote.app.zoomSdkModule.Meeting.GetMeetingInfo()

const getMeetingNumber = () => {
  let ret = zoomMeetingInfo.GetMeetingNumber()
  result.value = getSDKResult('GetMeetingNumber', ret, null, false)
}

const getMeetingTopic = () => {
  let ret = zoomMeetingInfo.GetMeetingTopic()
  result.value = getSDKResult('GetMeetingTopic', ret)
}

const getMeetingID = () => {
  let ret = zoomMeetingInfo.GetMeetingID();
  result.value = getSDKResult('GetMeetingID', ret)
}

const getMeetingType = () => {
  let ret = zoomMeetingInfo.GetMeetingType()
  let desc = Object.keys(MeetingType).find(k => MeetingType[k] === ret)
  result.value = `getMeetingType - ${desc}`
}

const getInviteEmailTemplate = () => {
  let ret = zoomMeetingInfo.GetInviteEmailTemplate();
  result.value = getSDKResult('GetInviteEmailTemplate', ret)
}

const getInviteEmailTitle = () => {
  let ret = zoomMeetingInfo.GetInviteEmailTitle();
  result.value = getSDKResult('GetInviteEmailTitle', ret)
}

const getJoinMeetingUrl = () => {
  let ret = zoomMeetingInfo.GetJoinMeetingUrl()
  result.value = getSDKResult('GetJoinMeetingUrl', ret)
}

const getMeetingHostTag = () => {
  let ret = zoomMeetingInfo.GetMeetingHostTag()
  result.value = getSDKResult('GetMeetingHostTag', ret)
}
</script>