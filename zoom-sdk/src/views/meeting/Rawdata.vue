<template>
  <div class="meeting">
    <div class="controller">
      <el-divider>Video</el-divider>
      <div class="in-line">
        <span>User List</span>
        <el-select v-model="active_user" placeholder="Select" @focus="getParticipantsList">
          <el-option v-for="(item, index) in userList" :key="item.value" :label="item.label" :value="item.value"></el-option>
        </el-select>
      </div>
      <div class="in-line">
        <span>Resolution</span>
        <el-select v-model="video_resolution" placeholder="Select">
          <el-option v-for="(item, index) in resolution_options" :key="index" :label="item.label" :value="item.value"></el-option>
        </el-select>
      </div>
      <div class="in-line">
        <span>Recever Handle</span>
        <el-select v-model="subVideo_recever_handle" placeholder="Select">
          <el-option v-for="(item, index) in 4" :key="index" :label="`recv_handle_${index}`" :value="index"></el-option>
        </el-select>
      </div>
      <el-button type="primary" round plain :disabled="userList.length==0" @click="handleSubscribeVideo(video_resolution, subVideo_recever_handle, active_user)">Subscribe Video</el-button>
      <div class="in-line">
        <span>Recever Handle</span>
        <el-select v-model="unSubVideo_recever_handle" placeholder="Select">
          <el-option v-for="(item, index) in 4" :key="index" :label="`recv_handle_${index}`" :value="index"></el-option>
        </el-select>
      </div>
      <el-button type="primary" round plain :disabled="userList.length==0" @click="handleUnSubscribeVideo(unSubVideo_recever_handle)">UnSubscribe Video</el-button>
      <el-divider>Share</el-divider>
      <div class="in-line">
        <span>Sharing List</span>
        <el-select v-model="active_share" placeholder="Select">
          <el-option v-for="(item, index) in shareUserList" :key="index" :label="item.label" :value="item.value"></el-option>
        </el-select>
      </div>
      <div class="in-line">
        <span>Resolution</span>
        <el-select v-model="share_resolution" placeholder="Select" @change="changeShareRawDataResolution(share_resolution)">
          <el-option v-for="(item, index) in resolution_options" :key="index" :label="item.label" :value="item.value"></el-option>
        </el-select>
      </div>
      <el-button type="primary" round plain :disabled="shareUserList.length==0" @click="handleSubscribeShare(share_resolution, active_share)">Subscribe Share</el-button>
      <el-button type="primary" round plain :disabled="shareUserList.length==0" @click="handleUnSubscribeShare">UnSubscribe Share</el-button>
      <el-divider>Audio</el-divider>
      <el-button type="primary" round plain @click="handleSubscribeAudio">Subscribe Audio</el-button>
      <el-button type="primary" round plain @click="handleUnsubscribeAudio">UnSubscribe Audio</el-button>
    </div>
    <div class="render">
      <el-divider>Share</el-divider>
      <canvas width="605" height="341" id="canvasShare"></canvas>
      <el-divider>Video</el-divider>
      <div class="video-render">
        <div class="item" v-for="(item, index) in 4" :key="index">
          <canvas width="300" height="168.75" :id="`canvasVideo${index}`"></canvas>
          <div class="tag" v-if="subscribedVideoList[index]">{{subscribedVideoList[index].userName}}</div>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onBeforeMount, onMounted, onBeforeUnmount } from 'vue'
import { ZoomMeetingStatus, ShareStatus, RawDataStatus } from '../../../lib/settings.js'
import CanvasRender from '../../assets/ts/CanvasRender.js'
import pipeClient from '../../assets/ts/rawdata/pipe'
import rawDataUser from '../../assets/ts/rawdata/user'
import rawDataAudio from '../../assets/ts/rawdata/audio'
import rawDataVideo from '../../assets/ts/rawdata/video'
import rawDataShare from '../../assets/ts/rawdata/share'
const remote = window.require('@electron/remote')
const zoomMeeting = remote.app.zoomSdkModule.Meeting
const zoomRawData = remote.app.zoomSdk.RawData()
const zoomMeetingVideo = remote.app.zoomSdkModule.Meeting.GetMeetingVideo()
const zoomMeetingShare = remote.app.zoomSdkModule.Meeting.GetMeetingShare()
const zoomMeetingParticipants = remote.app.zoomSdkModule.Meeting.GetMeetingParticipantsCtrl()
const { platform, arch } = remote.app
let nativeSdkPath, libuvAddon: any
try {
  switch (`${platform}:${arch}`) {
    case 'win32:ia32':
      nativeSdkPath = 'sdk/win32'
      break;
    case 'win32:x64':
      nativeSdkPath = 'sdk/win64'
      break;
    case 'darwin:x64': 
    case 'darwin:arm64':
      nativeSdkPath = 'sdk/mac'
      break;
  }
  require(`../../../${nativeSdkPath}/zoomsdk_napi_util.node`)
  libuvAddon = require(`../../../${nativeSdkPath}/zoomsdk_render.node`).exports()
} catch (error) {
  console.log(error)
}

pipeClient(libuvAddon, platform)
let { active_user, userList, getParticipantsList } = rawDataUser()
let { videoLibuvClientObj, subscribedVideoList, startPreviewDevice, handleSubscribeVideo, handleUnSubscribeVideo, changeVideoRawDataResolution, stopVideoClient } = rawDataVideo(libuvAddon, zoomRawData)
let { shareLibuvClientObj, shareRecvHandle, handleSubscribeShare, handleUnSubscribeShare, changeShareRawDataResolution, stopShareClient } = rawDataShare(libuvAddon, zoomRawData)
let { handleSubscribeAudio, handleUnsubscribeAudio, stopAudioClient } = rawDataAudio(libuvAddon, zoomRawData)

const resolution_options = ref([{label: '90P', value: 0}, {label: '180P', value: 1}, {label: '360P', value: 2}, {label: '720P', value: 3}])
const video_resolution = ref(0)
const subVideo_recever_handle = ref(0)
const unSubVideo_recever_handle = ref(0)
const active_share = ref()
const shareUserList = ref<any[]>([])
const share_resolution = ref(3)
let rawdataStatusObj: any = {}
let videoRenderList: any = [null, null, null, null], canvasShare, shareRender: any
let croppingParams = {
  top: '0',
  left: '0',
  width: '1280',
  height: '720',
  rotation: '0'
}

const meetingstatuscb = (meetingStatus: string, result: string) => {
  let statusContent = Object.keys(ZoomMeetingStatus).find(k => ZoomMeetingStatus[k] == meetingStatus)
  console.log('meetingstatuscb', meetingStatus, result, statusContent)
  if (statusContent == 'MEETING_STATUS_INMEETING') {
    getParticipantsList()
    handleUnSubscribeVideo(3)
  } else if (statusContent == 'MEETING_STATUS_DISCONNECTING') {
    stopVideoClient()
    stopShareClient()
    stopAudioClient()
    remote.getCurrentWindow().close()
  }
}

const onRawDataStatusChanged = (status: number, recv_handle: number) => {
  console.log('onRawDataStatusChanged', status, recv_handle)
  rawdataStatusObj[recv_handle] = status
  if (status == RawDataStatus.RawData_Off) {
    if (recv_handle == shareRecvHandle) {
      shareRender.clear()
    } else {
      videoRenderList[recv_handle].clear()
    }
  }
}

const onUserJoin = (userId_list: any) => {
  console.log('onUserJoin', userId_list)
  for (let i = 0; i < userId_list.length; i++) {
    let userObj = zoomMeetingParticipants.GetUserInfoByUserID(userId_list[i].userid)
    console.log('onUserJoin', userObj)
    if (userObj.userID) {
      let obj = {
        value: userObj.userID,
        label: `${userObj.userName} - ${userObj.isVideoOn ? 'video on': 'video off'}`
      }
      userList.value.unshift(obj)
    }
  }
}

const onUserLeft = (userId_list: any) => {
  console.log('onUserLeft', userId_list);
  let tempList = [...userList.value]
  for (let index = 0; index < userId_list.length; index++) {
    for (let t_index = 0; t_index < tempList.length; t_index++) {
      if (tempList[t_index].value == userId_list[index].userid) {
        tempList[t_index] = null
        break
      }
    }
    userList.value = tempList.filter((item: object) => item)
    if (userId_list[index].userid == active_user.value) {
      active_user.value = userList.value[0]
    }
    for (let sub_index = 0; sub_index < subscribedVideoList.value.length; sub_index++) {
      if (subscribedVideoList.value[sub_index]&&subscribedVideoList.value[sub_index].userID == userId_list[index].userid) {
        handleUnSubscribeVideo(sub_index)
        videoRenderList[sub_index].clear()
      }
    }
  }
}

const onUserVideoStatusChange = (videoStatus: any) => {
  console.log('onUserVideoStatusChange', videoStatus)
  for (let k = 0; k < userList.value.length; k++) {
    if (userList.value[k].value == videoStatus.userId) {
      let userName = userList.value[k].label.split(' - ')[0]
      let newVideoStatus = videoStatus.videoStatus == 0 ? 'video on' : 'video off'
      userList.value[k].label = `${userName} - ${newVideoStatus}`
    }
  }
}

const onRenderVideoRawDataReceived = (format: string, recv_handle: number, databuf: any) => {
  let formatList = format.split(';');
  croppingParams.width = formatList[2];
  croppingParams.height = formatList[3];
  croppingParams.rotation = formatList[4];
  let data = new Uint8Array(databuf);
  if (rawdataStatusObj[recv_handle] == RawDataStatus.RawData_On) {
    videoRenderList[recv_handle].drawNextOuptutPictureGL(croppingParams.width, croppingParams.height, croppingParams, data, croppingParams.rotation, 1);
  }
}

const onSharingStatus = (shareInfo: any) => {
  console.log('onSharingStatus', shareInfo);
  if (shareInfo.status == ShareStatus.Sharing_Other_Share_Begin) {
    let userObj = zoomMeetingParticipants.GetUserInfoByUserID(shareInfo.userid);
    let obj = {
      value: shareInfo.shareSourceID,
      label: `${userObj.userName} - ${shareInfo.shareSourceID}`
    }
    shareUserList.value.unshift(obj)
    if (!active_share.value) {
      active_share.value = obj.value
    }
  } else if (shareInfo.status == ShareStatus.Sharing_Other_Share_End) {
    let tempList = [...shareUserList.value]
    for (let i = 0; i < tempList.length; i++) {
      if (shareInfo.shareSourceID == tempList[i].value) {
        tempList[i] = null
        break;
      }
    }
    shareUserList.value = tempList.filter((item: object) => item)
    if (shareInfo.shareSourceID == active_share.value) {
      handleUnSubscribeShare()
      active_share.value = shareUserList.value.length > 0 ? shareUserList.value[0]: ''
      shareRender.clear()
    }
  }
}

const onRenderShareRawDataReceived = (format: string, recv_handle: number, databuf: any) => {
  let formatList = format.split(';');
  croppingParams.width = formatList[2];
  croppingParams.height = formatList[3];
  croppingParams.rotation = formatList[4];
  let data = new Uint8Array(databuf);
  if (rawdataStatusObj[recv_handle] == RawDataStatus.RawData_On) {
    shareRender.drawNextOuptutPictureGL(croppingParams.width, croppingParams.height, croppingParams, data, croppingParams.rotation, 1);
  }
}

const handleMsg = (event: any) => {
  if (event.type == 'message') {
    let { meetingStatus, result } = event.data
    if (Number.isInteger(meetingStatus) || Number.isInteger(result)) {
      meetingstatuscb(meetingStatus, result)
    }
  }
}

const beforeUnload = () => {
  for (let i = 0; i <= 4; i++) {
    let obj = {
      recv_handle: i
    }
    zoomRawData.UnSubscribe(obj)
    zoomRawData.DestroyRenderer(obj)
  }
  zoomMeeting.LeaveMeeting()
}

onBeforeMount(() => {
  for (let i = 0; i <= 4; i++) {
    let obj = {
      recv_handle: i
    }
    zoomRawData.CreateRenderer(obj);
  }
  zoomMeetingParticipants.SetMeetingUserJoinCB(onUserJoin)
  zoomMeetingParticipants.SetMeetingUserLeftCB(onUserLeft)
  let ret = zoomMeetingVideo.MeetingVideo_SetMeetingVideoStatusCB(onUserVideoStatusChange);
  console.log('MeetingVideo_SetMeetingVideoStatusCB', ret);
  ret = zoomRawData.SetonRawDataStatusChangedCB(onRawDataStatusChanged)
  console.log('SetonRawDataStatusChangedCB', ret)
  ret = videoLibuvClientObj.SetRenderVideoRawDataCB(onRenderVideoRawDataReceived);
  console.log('SetRenderVideoRawDataCB', ret)
  ret = zoomMeetingShare.MeetingShare_SetOnSharingStatusCB(onSharingStatus)
  console.log('MeetingShare_SetOnSharingStatusCB', ret)
  ret = shareLibuvClientObj.SetRenderShareRawDataCB(onRenderShareRawDataReceived)
  console.log('SetRenderShareRawDataCB', ret)
})

onMounted(() => {
  for (let i = 0; i < 4; i++) {
    let canvasVideoItem = document.querySelector(`#canvasVideo${i}`)
    videoRenderList[i] = new CanvasRender(canvasVideoItem)
    videoRenderList[i].clear()
  }
  console.log(videoRenderList)
  canvasShare = document.querySelector('#canvasShare')
  shareRender = new CanvasRender(canvasShare)
  shareRender.clear()
  startPreviewDevice()
  window.addEventListener('message', msg => handleMsg(msg))
  window.addEventListener('beforeunload', beforeUnload)
})

onBeforeUnmount(() => {
  window.removeEventListener('message', handleMsg)
  window.removeEventListener('beforeunload', beforeUnload)
})
</script>
<style scoped lang="scss">
.meeting {
  display: flex;
  justify-content:center;
  padding: 10px 20px 20px;
  .controller {
    width: 310px;
    margin-right: 30px;
    .in-line {
      flex-wrap: nowrap;
      span {
        width: 130px;
      }
    }
  }
  .render {
    .video-render {
      width: 630px;
      text-align: justify;
      .item {
        display: inline-block;
        width: 300px;
        margin: 5px 3px 0 0;
        position: relative;
        .tag {
          position: absolute;
          bottom: 5px;
          left: 0;
          padding: 2px 6px;
          font-size: 12px;
          color: #fff;
          font-weight: 100;
          background: rgba(0, 0, 0, 0.4);
        }
      }
    }
  }
}
</style>
