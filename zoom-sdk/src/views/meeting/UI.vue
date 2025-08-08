<template>
  <div class="meeting">
    <el-button type="primary" round plain @click="setInviteButtonClickedCB">SetInviteButtonClickedCB</el-button>
    <el-button type="primary" round plain @click="setonStartShareBtnClickedCB">SetonStartShareBtnClickedCB</el-button>
    <el-button type="primary" round plain @click="setonEndMeetingBtnClickedCB">SetonEndMeetingBtnClickedCB</el-button>
    <el-button type="primary" round plain @click="setonParticipantListBtnClickedCB">SetonParticipantListBtnClickedCB</el-button>
    <el-button type="primary" round plain @click="setonCustomLiveStreamMenuClickedCB">SetonCustomLiveStreamMenuClickedCB</el-button>
    <el-button type="primary" round plain @click="setonZoomInviteDialogFailedCB">SetonZoomInviteDialogFailedCB</el-button>
    <el-button type="primary" round plain @click="setonCCBTNClickedCB">SetonCCBTNClickedCB</el-button>
    <el-button type="primary" round plain @click="setonAudioBTNClickedCB">SetonAudioBTNClickedCB</el-button>
    <el-button type="primary" round plain @click="setonAudioMenuBTNClickedCB">SetonAudioMenuBTNClickedCB</el-button>
    <div class="in-line">
      <el-input v-model.trim="hParent" placeholder="input hParent" />
      <el-input v-model.trim="left" placeholder="input left" />
      <el-input v-model.trim="top" placeholder="input top" />
      <el-input v-model.trim="right" placeholder="input right" />
      <el-input v-model.trim="bottom" placeholder="input bottom" @keyup.enter.native="showChatDlg" />
      <el-button type="primary" round plain @click="showChatDlg">ShowChatDlg</el-button>
    </div>
    <el-button type="primary" round plain @click="hideChatDlg">HideChatDlg</el-button>
    <div class="in-line">
      <el-select v-model="viewtype" placeholder="Select">
        <el-option v-for="(item, key) in ZoomMeetingUIViewType" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="enterFullScreen">EnterFullScreen</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="viewtype" placeholder="Select">
        <el-option v-for="(item, key) in ZoomMeetingUIViewType" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="exitFullScreen">ExitFullScreen</el-button>
    </div>
    <el-button type="primary" round plain @click="switchToVideoWall">SwitchToVideoWall</el-button>
    <el-button type="primary" round plain @click="switchToActiveSpeaker">SwitchToActiveSpeaker</el-button>
    <div class="in-line">
      <el-input v-model.trim="left" placeholder="input left" />
      <el-input v-model.trim="top" placeholder="input top" @keyup.enter.native="moveFloatVideoWnd" />
      <el-button type="primary" round plain @click="moveFloatVideoWnd">MoveFloatVideoWnd</el-button>
    </div>
    <el-checkbox v-model="show_sharingToolbar" label="ShowSharingToolbar" @change="showSharingToolbar"></el-checkbox>
    <el-button type="primary" round plain @click="switchFloatVideoToActiveSpkMod">SwitchFloatVideoToActiveSpkMod</el-button>
    <el-button type="primary" round plain @click="switchFloatVideoToGalleryMod">SwitchFloatVideoToGalleryMod</el-button>
    <div class="in-line">
      <el-select v-model="floatvideotype" placeholder="Select">
        <el-option v-for="(item, key) in ZoomMeetingUIFloatVideoType" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="changeFloatToActiveSpkVideoSize">ChangeFloatToActiveSpkVideoSize</el-button>
    </div>
    <el-checkbox v-model="show_participantsListWnd" label="ShowParticipantsListWnd" @change="showParticipantsListWnd"></el-checkbox>
    <el-checkbox v-model="show_bottomFloatToolbarWnd" label="ShowBottomFloatToolbarWnd" @change="showBottomFloatToolbarWnd"></el-checkbox>
    <el-button type="primary" round plain @click="showJoinAudioDlg">ShowJoinAudioDlg</el-button>
    <el-button type="primary" round plain @click="hideJoinAudioDlg">HideJoinAudioDlg</el-button>
    <el-button type="primary" round plain @click="getWallViewPageInfo">GetWallViewPageInfo</el-button>
    <el-checkbox v-model="show_preOrNextPageVideo" label="ShowPreOrNextPageVideo" @change="showPreOrNextPageVideo"></el-checkbox>
    <el-checkbox v-model="show_sharingFrameWindows" label="ShowSharingFrameWindows" @change="showSharingFrameWindows"></el-checkbox>
    <el-button type="primary" round plain @click="getCurrentSplitScreenModeInfo">GetCurrentSplitScreenModeInfo</el-button>
    <el-checkbox v-model="switch_splitScreenMode" label="SwitchSplitScreenMode" @change="switchSplitScreenMode"></el-checkbox>
    <el-button type="primary" round plain @click="backToMeeting">BackToMeeting</el-button>
    <el-button type="primary" round plain @click="getMeetingUIWnd">GetMeetingUIWnd</el-button>
    <div class="in-line">
      <el-select v-model="mode" placeholder="Select">
        <el-option v-for="(item, key) in SDKMinimizeUIMode" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="switchMinimizeUIMode4FirstScreenMeetingUIWnd">SwitchMinimizeUIMode4FirstScreenMeetingUIWnd</el-button>
      <el-button type="primary" round plain @click="isMinimizeModeOfFirstScreenMeetingUIWnd">IsMinimizeModeOfFirstScreenMeetingUIWnd</el-button>
    </div>
    <div class="in-line">
      <el-input-number v-model="toDisplayShare" :min="0" />
      <el-button type="primary" round plain @click="swapToShowShareViewOrVideo">SwapToShowShareViewOrVideo</el-button>
      <el-button type="primary" round plain @click="isDisplayingShareViewOrVideo">IsDisplayingShareViewOrVideo</el-button>
      <el-button type="primary" round plain @click="canSwapToShowShareViewOrVideo">CanSwapToShowShareViewOrVideo</el-button>
    </div>
    <div class="in-line">
      <el-input v-model.trim="meetingTopic" placeholder="input meeting topic" @keyup.enter.native="setMeetingTopic" />
      <el-button type="primary" round plain @click="setMeetingTopic">SetMeetingTopic</el-button>
    </div>
    <div class="in-line">
      <el-input v-model.trim="crmURL" placeholder="input crm url" @keyup.enter.native="setCustomizedCloudRecordingMgrUrl" />
      <el-button type="primary" round plain @click="setCustomizedCloudRecordingMgrUrl">SetCustomizedCloudRecordingMgrUrl</el-button>
    </div>
    <div class="in-line">
      <el-input v-model.trim="invitationDomain" placeholder="input invitation domain" @keyup.enter.native="setCustomizedInvitationDomain" />
      <el-button type="primary" round plain @click="setCustomizedInvitationDomain">SetCustomizedInvitationDomain</el-button>
    </div>
    <div class="in-line">
      <el-input v-model.trim="pollingURL" placeholder="input polling url" @keyup.enter.native="setCustomizedPollingUrl" />
      <el-checkbox v-model="bCreate" label="bCreate" @change="showParticipantsListWnd"></el-checkbox>
      <el-button type="primary" round plain @click="setCustomizedPollingUrl">SetCustomizedPollingUrl</el-button>
    </div>
  </div>
  <div class="result">Result: {{result}}</div>
  <div class="result"> 
    <div class="result-item"> 
      <span class="title">onInviteBtnClicked <el-button size="small" @click="onInviteBtnClicked_value=0">Clear</el-button></span> 
      <span v-if="onInviteBtnClicked_value">updated {{onInviteBtnClicked_value}}</span> 
    </div> 
    <div class="result-item"> 
      <span class="title">onStartShareBtnClicked <el-button size="small" @click="onStartShareBtnClicked_value=0">Clear</el-button></span> 
      <span v-if="onStartShareBtnClicked_value">updated {{onStartShareBtnClicked_value}}</span> 
    </div> 
    <div class="result-item"> 
      <span class="title">onEndMeetingBtnClicked <el-button size="small" @click="onEndMeetingBtnClicked_value=0">Clear</el-button></span> 
      <span v-if="onEndMeetingBtnClicked_value">updated {{onEndMeetingBtnClicked_value}}</span> 
    </div> 
    <div class="result-item"> 
      <span class="title">onParticipantListBtnClicked <el-button size="small" @click="onParticipantListBtnClicked_value=0">Clear</el-button></span> 
      <span v-if="onParticipantListBtnClicked_value">updated {{onParticipantListBtnClicked_value}}</span> 
    </div> 
    <div class="result-item"> 
      <span class="title">onCustomLiveStreamMenuClicked <el-button size="small" @click="onCustomLiveStreamMenuClicked_value=0">Clear</el-button></span> 
      <span v-if="onCustomLiveStreamMenuClicked_value">updated {{onCustomLiveStreamMenuClicked_value}}</span> 
    </div> 
    <div class="result-item"> 
      <span class="title">onZoomInviteDialogFailed <el-button size="small" @click="onZoomInviteDialogFailed_value=0">Clear</el-button></span> 
      <span v-if="onZoomInviteDialogFailed_value">updated {{onZoomInviteDialogFailed_value}}</span> 
    </div> 
    <div class="result-item"> 
      <span class="title">onCCBTNClicked <el-button size="small" @click="onCCBTNClicked_value=0">Clear</el-button></span> 
      <span v-if="onCCBTNClicked_value">updated {{onCCBTNClicked_value}}</span> 
    </div> 
    <div class="result-item"> 
      <span class="title">onAudioBtnClicked <el-button size="small" @click="onAudioBtnClicked_value=0">Clear</el-button></span> 
      <span v-if="onAudioBtnClicked_value">updated {{onAudioBtnClicked_value}}</span> 
    </div> 
    <div class="result-item"> 
      <span class="title">onAudioMenuBtnClicked <el-button size="small" @click="onAudioMenuBtnClicked_value=0">Clear</el-button></span> 
      <span v-if="onAudioMenuBtnClicked_value">updated {{onAudioMenuBtnClicked_value}}</span> 
    </div> 
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { ZoomMeetingUIViewType, ZoomMeetingUIFloatVideoType, SDKMinimizeUIMode } from '../../../lib/settings.js'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const result = ref()
const onInviteBtnClicked_value = ref(0)
const onStartShareBtnClicked_value = ref(0)
const onEndMeetingBtnClicked_value = ref(0)
const onParticipantListBtnClicked_value = ref(0)
const onCustomLiveStreamMenuClicked_value = ref(0)
const onZoomInviteDialogFailed_value = ref(0)
const onCCBTNClicked_value = ref(0)
const onAudioBtnClicked_value = ref(0)
const onAudioMenuBtnClicked_value = ref(0)

const onInviteBtnClicked = () => {
  console.log('onInviteBtnClicked')
  onInviteBtnClicked_value.value++
}
const onStartShareBtnClicked = () => {
  console.log('onStartShareBtnClicked')
  onStartShareBtnClicked_value.value++
}
const onEndMeetingBtnClicked = () => {
  console.log('onEndMeetingBtnClicked')
  onEndMeetingBtnClicked_value.value++
}
const onParticipantListBtnClicked = () => {
  console.log('onParticipantListBtnClicked')
  onParticipantListBtnClicked_value.value++
}
const onCustomLiveStreamMenuClicked = () => {
  console.log('onCustomLiveStreamMenuClicked')
  onCustomLiveStreamMenuClicked_value.value++
}
const onZoomInviteDialogFailed = () => {
  console.log('onZoomInviteDialogFailed')
  onZoomInviteDialogFailed_value.value++
}
const onCCBTNClicked = () => {
  console.log('onCCBTNClicked')
  onCCBTNClicked_value.value++
}
const onAudioBtnClicked = () => {
  console.log('onAudioBtnClicked')
  onAudioBtnClicked_value.value++
}
const onAudioMenuBtnClicked = () => {
  console.log('onAudioMenuBtnClicked')
  onAudioMenuBtnClicked_value.value++
}
const zoomMeetingUI = remote.app.zoomSdkModule.Meeting.GetMeetingUICtrl({onInviteBtnClicked, onStartShareBtnClicked, onEndMeetingBtnClicked, onParticipantListBtnClicked, onCustomLiveStreamMenuClicked, onZoomInviteDialogFailed, onCCBTNClicked, onAudioBtnClicked, onAudioMenuBtnClicked})
const hParent = ref()
const left = ref()
const top = ref()
const right = ref()
const bottom = ref()
const viewtype = ref(0)
const show_sharingToolbar = ref(false)
const floatvideotype = ref(1)
const show_participantsListWnd = ref(false)
const show_bottomFloatToolbarWnd = ref(false)
const show_preOrNextPageVideo = ref(false)
const show_sharingFrameWindows = ref(false)
const switch_splitScreenMode = ref(false)
const mode = ref(0)
const toDisplayShare = ref()
const meetingTopic = ref()
const crmURL = ref()
const invitationDomain = ref()
const pollingURL = ref('')
const bCreate = ref(false)
const clientHeight = ref((window.innerHeight - 465) + 'px')

const setInviteButtonClickedCB = () => {
  let ret = zoomMeetingUI.MeetingUI_SetInviteButtonClickedCB(onInviteBtnClicked)
  result.value = getSDKResult('MeetingUI_SetInviteButtonClickedCB', ret)
}

const setonStartShareBtnClickedCB = () => {
  let ret = zoomMeetingUI.MeetingUI_SetonStartShareBtnClickedCB(onStartShareBtnClicked)
  result.value = getSDKResult('MeetingUI_SetonStartShareBtnClickedCB', ret)
}

const setonEndMeetingBtnClickedCB = () => {
  let ret = zoomMeetingUI.MeetingUI_SetonEndMeetingBtnClickedCB(onEndMeetingBtnClicked)
  result.value = getSDKResult('MeetingUI_SetonEndMeetingBtnClickedCB', ret)
}

const setonParticipantListBtnClickedCB = () => {
  let ret = zoomMeetingUI.MeetingUI_SetonParticipantListBtnClickedCB(onParticipantListBtnClicked)
  result.value = getSDKResult('MeetingUI_SetonParticipantListBtnClickedCB', ret)
}

const setonCustomLiveStreamMenuClickedCB = () => {
  let ret = zoomMeetingUI.MeetingUI_SetonCustomLiveStreamMenuClickedCB(onCustomLiveStreamMenuClicked)
  result.value = getSDKResult('MeetingUI_SetonCustomLiveStreamMenuClickedCB', ret)
}

const setonZoomInviteDialogFailedCB = () => {
  let ret = zoomMeetingUI.MeetingUI_SetonZoomInviteDialogFailedCB(onZoomInviteDialogFailed)
  result.value = getSDKResult('MeetingUI_SetonZoomInviteDialogFailedCB', ret)
}

const setonCCBTNClickedCB = () => {
  let ret = zoomMeetingUI.MeetingUI_SetonCCBTNClickedCB(onCCBTNClicked)
  result.value = getSDKResult('MeetingUI_SetonCCBTNClickedCB', ret)
}

const setonAudioBTNClickedCB = () => {
  let ret = zoomMeetingUI.MeetingUI_SetonAudioBTNClickedCB(onAudioBtnClicked)
  result.value = getSDKResult('MeetingUI_SetonAudioBTNClickedCB', ret)
}

const setonAudioMenuBTNClickedCB = () => {
  let ret = zoomMeetingUI.MeetingUI_SetonAudioMenuBTNClickedCB(onAudioMenuBtnClicked)
  result.value = getSDKResult('MeetingUI_SetonAudioMenuBTNClickedCB', ret)
}

const showChatDlg = () => {
  let param = {
    hParent: hParent.value,
    left: left.value,
    top: top.value,
    right: right.value,
    bottom: bottom.value
  }
  let ret = zoomMeetingUI.MeetingUI_ShowChatDlg(param)
  result.value = getSDKResult('MeetingUI_ShowChatDlg', ret, param)
}

const hideChatDlg = () => {
  let ret = zoomMeetingUI.MeetingUI_HideChatDlg()
  result.value = getSDKResult('MeetingUI_HideChatDlg', ret)
}

const enterFullScreen = () => {
  let param = {viewtype: viewtype.value}
  let ret = zoomMeetingUI.MeetingUI_EnterFullScreen(param)
  result.value = getSDKResult('MeetingUI_EnterFullScreen', ret, param)
}

const exitFullScreen = () => {
  let param = {viewtype: viewtype.value}
  let ret = zoomMeetingUI.MeetingUI_ExitFullScreen(param)
  result.value = getSDKResult('MeetingUI_ExitFullScreen', ret, param)
}

const switchToVideoWall = () => {
  let ret = zoomMeetingUI.MeetingUI_SwitchToVideoWall()
  result.value = getSDKResult('MeetingUI_SwitchToVideoWall', ret)
}

const switchToActiveSpeaker = () => {
  let ret = zoomMeetingUI.MeetingUI_SwitchToActiveSpeaker()
  result.value = getSDKResult('MeetingUI_SwitchToActiveSpeaker', ret)
}

const moveFloatVideoWnd = () => {
  let param = {
    left: left.value,
    top: top.value
  }
  let ret = zoomMeetingUI.MeetingUI_MoveFloatVideoWnd(param)
  result.value = getSDKResult('MeetingUI_MoveFloatVideoWnd', ret, param)
}

const showSharingToolbar = () => {
  let param = {
    show: show_sharingToolbar.value
  }
  let ret = zoomMeetingUI.MeetingUI_ShowSharingToolbar(param)
  result.value = getSDKResult('MeetingUI_ShowSharingToolbar', ret, param)
}

const switchFloatVideoToActiveSpkMod = () => {
  let ret = zoomMeetingUI.MeetingUI_SwitchFloatVideoToActiveSpkMod()
  result.value = getSDKResult('MeetingUI_SwitchFloatVideoToActiveSpkMod', ret)
}

const switchFloatVideoToGalleryMod = () => {
  let ret = zoomMeetingUI.MeetingUI_SwitchFloatVideoToGalleryMod()
  result.value = getSDKResult('MeetingUI_SwitchFloatVideoToGalleryMod', ret)
}

const changeFloatToActiveSpkVideoSize = () => {
  let param = {
    floatvideotype: floatvideotype.value
  }
  let ret = zoomMeetingUI.MeetingUI_ChangeFloatToActiveSpkVideoSize(param)
  result.value = getSDKResult('MeetingUI_ChangeFloatToActiveSpkVideoSize', ret, param)
}

const showParticipantsListWnd = () => {
  let param = {
    show: show_participantsListWnd.value
  }
  let ret = zoomMeetingUI.MeetingUI_ShowParticipantsListWnd(param)
  result.value = getSDKResult('MeetingUI_ShowParticipantsListWnd', ret, param)
}

const showBottomFloatToolbarWnd = () => {
  let param = {
    show: show_bottomFloatToolbarWnd.value
  }
  let ret = zoomMeetingUI.MeetingUI_ShowBottomFloatToolbarWnd(param)
  result.value = getSDKResult('MeetingUI_ShowBottomFloatToolbarWnd', ret, param)
}

const showJoinAudioDlg = () => {
  let ret = zoomMeetingUI.MeetingUI_ShowJoinAudioDlg()
  result.value = getSDKResult('MeetingUI_ShowJoinAudioDlg', ret)
}

const hideJoinAudioDlg = () => {
  let ret = zoomMeetingUI.MeetingUI_HideJoinAudioDlg()
  result.value = getSDKResult('MeetingUI_HideJoinAudioDlg', ret)
}

const getWallViewPageInfo = () => {
  let ret = zoomMeetingUI.MeetingUI_GetWallViewPageInfo()
  result.value = getSDKResult('MeetingUI_GetWallViewPageInfo', ret)
}

const showPreOrNextPageVideo = () => {
  let param = {
    show: show_preOrNextPageVideo.value
  }
  let ret = zoomMeetingUI.MeetingUI_ShowPreOrNextPageVideo(param)
  result.value = getSDKResult('MeetingUI_ShowPreOrNextPageVideo', ret, param)
}

const showSharingFrameWindows = () => {
  let param = {
    show: show_sharingFrameWindows.value
  }
  let ret = zoomMeetingUI.MeetingUI_ShowSharingFrameWindows(param)
  result.value = getSDKResult('MeetingUI_ShowSharingFrameWindows', ret, param)
}

const getCurrentSplitScreenModeInfo = () => {
  let ret = zoomMeetingUI.MeetingUI_GetCurrentSplitScreenModeInfo()
  result.value = getSDKResult('MeetingUI_GetCurrentSplitScreenModeInfo', ret)
}

const switchSplitScreenMode = () => {
  let param = {
    isSwitch: switch_splitScreenMode.value
  }
  let ret = zoomMeetingUI.MeetingUI_SwitchSplitScreenMode(param)
  result.value = getSDKResult('MeetingUI_SwitchSplitScreenMode', ret, param)
}

const backToMeeting = () => {
  let ret = zoomMeetingUI.MeetingUI_BackToMeeting()
  result.value = getSDKResult('MeetingUI_BackToMeeting', ret)
}

const getMeetingUIWnd = () => {
  let ret = zoomMeetingUI.MeetingUI_GetMeetingUIWnd()
  result.value = getSDKResult('MeetingUI_GetMeetingUIWnd', ret)
}

const switchMinimizeUIMode4FirstScreenMeetingUIWnd = () => {
  let param = {
    mode: mode.value
  }
  let ret = zoomMeetingUI.MeetingUI_SwitchMinimizeUIMode4FirstScreenMeetingUIWnd(param)
  result.value = getSDKResult('MeetingUI_SwitchMinimizeUIMode4FirstScreenMeetingUIWnd', ret, param)
}

const isMinimizeModeOfFirstScreenMeetingUIWnd = () => {
  let ret = zoomMeetingUI.MeetingUI_IsMinimizeModeOfFirstScreenMeetingUIWnd()
  result.value = getSDKResult('MeetingUI_IsMinimizeModeOfFirstScreenMeetingUIWnd', ret)
}

const swapToShowShareViewOrVideo = () => {
  let param = {
    bToDisplayShare: toDisplayShare.value
  }
  let ret = zoomMeetingUI.MeetingUI_SwapToShowShareViewOrVideo(param)
  result.value = getSDKResult('MeetingUI_SwapToShowShareViewOrVideo', ret, param)
}

const isDisplayingShareViewOrVideo = () => {
  let ret = zoomMeetingUI.MeetingUI_IsDisplayingShareViewOrVideo()
  result.value = getSDKResult('MeetingUI_IsDisplayingShareViewOrVideo', ret)
}

const canSwapToShowShareViewOrVideo = () => {
  let ret = zoomMeetingUI.MeetingUI_CanSwapToShowShareViewOrVideo()
  result.value = getSDKResult('MeetingUI_CanSwapToShowShareViewOrVideo', ret)
}

const setMeetingTopic = () => {
  let param = {
    meetingTopic: meetingTopic.value
  }
  let ret = zoomMeetingUI.MeetingUI_SetMeetingTopic(param)
  result.value = getSDKResult('MeetingUI_SetMeetingTopic', ret, param)
}

const setCustomizedCloudRecordingMgrUrl = () => {
  let param = {
    crmURL: crmURL.value
  }
  let ret = zoomMeetingUI.MeetingUI_SetCustomizedCloudRecordingMgrUrl(param)
  result.value = getSDKResult('MeetingUI_SetCustomizedCloudRecordingMgrUrl', ret, param)
}

const setCustomizedInvitationDomain = () => {
  let param = {
    invitationDomain: invitationDomain.value
  }
  let ret = zoomMeetingUI.MeetingUI_SetCustomizedInvitationDomain(param)
  result.value = getSDKResult('MeetingUI_SetCustomizedInvitationDomain', ret, param)
}

const setCustomizedPollingUrl = () => {
  let param = {
    pollingURL: pollingURL.value,
    bCreate: bCreate.value
  }
  let ret = zoomMeetingUI.MeetingUI_SetCustomizedPollingUrl(param)
  result.value = getSDKResult('MeetingUI_SetCustomizedPollingUrl', ret, param)
}

onMounted(() => {
  window.addEventListener('resize', ()=>{
    clientHeight.value = (window.innerHeight - 465) + 'px'
  })
})
</script>
<style scoped lang="scss">
  .meeting {
    height: v-bind(clientHeight);
  }
</style>