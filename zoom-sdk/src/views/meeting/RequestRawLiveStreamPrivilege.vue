<template>
  <div class="meeting">
    <el-button type="primary" round plain @click="getRequestId">GetRequestId</el-button>
    <el-button type="primary" round plain @click="getRequesterId">GetRequesterId</el-button>
    <el-button type="primary" round plain @click="getRequesterName">GetRequesterName</el-button>
    <el-button type="primary" round plain @click="getBroadcastUrl">GetBroadcastUrl</el-button>
    <el-button type="primary" round plain @click="grantRawLiveStreamPrivilege">GrantRawLiveStreamPrivilege</el-button>
    <el-button type="primary" round plain @click="denyRawLiveStreamPrivilege">DenyRawLiveStreamPrivilege</el-button>
  </div>
  <div class="result">Result: {{result}}</div>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const zoomMeetingRequestRawLiveStreamPrivilegeHandler = remote.app.zoomSdkModule.Meeting.GetMeetingRequestRawLiveStreamPrivilegeHandler()
const result = ref()

const getRequestId = () => {
  let ret = zoomMeetingRequestRawLiveStreamPrivilegeHandler.GetRequestId()
  result.value = getSDKResult('GetRequestId', ret, null, false)
}

const getRequesterId = () => {
  let ret = zoomMeetingRequestRawLiveStreamPrivilegeHandler.GetRequesterId();
  result.value = getSDKResult('GetRequesterId', ret, null, false)
}

const getRequesterName = () => {
  let ret = zoomMeetingRequestRawLiveStreamPrivilegeHandler.GetRequesterName();
  result.value = getSDKResult('GetRequesterName', ret)
}

const getBroadcastUrl = () => {
  let ret = zoomMeetingRequestRawLiveStreamPrivilegeHandler.GetBroadcastUrl();
  result.value = getSDKResult('GetBroadcastUrl', ret)
}

const grantRawLiveStreamPrivilege = () => {
  let ret = zoomMeetingRequestRawLiveStreamPrivilegeHandler.GrantRawLiveStreamPrivilege();
  result.value = getSDKResult('GrantRawLiveStreamPrivilege', ret)
}

const denyRawLiveStreamPrivilege = () => {
  let ret = zoomMeetingRequestRawLiveStreamPrivilegeHandler.DenyRawLiveStreamPrivilege();
  result.value = getSDKResult('DenyRawLiveStreamPrivilege', ret)
}
</script>
