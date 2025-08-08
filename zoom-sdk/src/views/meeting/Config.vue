<template>
  <div class="meeting">
    <el-button type="primary" round plain @click="setFreeMeetingNeedToUpgradeCB">SetFreeMeetingNeedToUpgradeCB</el-button>
    <el-button type="primary" round plain @click="setFreeMeetingUpgradeToGiftFreeTrialStartCB">SetFreeMeetingUpgradeToGiftFreeTrialStartCB</el-button>
    <el-button type="primary" round plain @click="setFreeMeetingUpgradeToGiftFreeTrialStopCB">SetFreeMeetingUpgradeToGiftFreeTrialStopCB</el-button>
    <el-button type="primary" round plain @click="setFreeMeetingUpgradeToProMeetingCB">SetFreeMeetingUpgradeToProMeetingCB</el-button>
    <el-button type="primary" round plain @click="setInputMeetingPasswordAndScreenNameNotificationCB">SetInputMeetingPasswordAndScreenNameNotificationCB</el-button>
    <el-button type="primary" round plain @click="setAirPlayInstructionWndNotificationCB">SetAirPlayInstructionWndNotificationCB</el-button>
    <el-button type="primary" round plain @click="setonWebinarNeedRegisterNotificationCB">SetonWebinarNeedRegisterNotificationCB</el-button>
    <el-button type="primary" round plain @click="setonEndOtherMeetingToJoinMeetingNotificationCB">SetonEndOtherMeetingToJoinMeetingNotificationCB</el-button>
    <el-button type="primary" round plain @click="setonFreeMeetingRemainTimeCB">SetonFreeMeetingRemainTimeCB</el-button>
    <el-button type="primary" round plain @click="setonFreeMeetingRemainTimeStopCountDownCB">SetonFreeMeetingRemainTimeStopCountDownCB</el-button>
    <el-button type="primary" round plain @click="setonWebinarNeedInputScreenNameCB">SetonWebinarNeedInputScreenNameCB</el-button>
    <el-button type="primary" round plain @click="setonJoinMeetingNeedUserInfoCB">SetonJoinMeetingNeedUserInfoCB</el-button>
    <el-button type="primary" round plain @click="setonUserConfirmToStartArchiveCB">SetonUserConfirmToStartArchiveCB</el-button>
    <el-checkbox v-model="enable_inviteButtonOnMeetingUI" label="EnableInviteButtonOnMeetingUI" @change="enableInviteButtonOnMeetingUI"></el-checkbox>
    <div class="in-line">
      <el-input v-model.trim="left" placeholder="input left" />
      <el-input v-model.trim="top" placeholder="input top" />
      <el-input v-model.trim="hSelfWnd" placeholder="input hSelfWnd" />
      <el-input v-model.trim="hParent" placeholder="input hParent" @keyup.enter.native="setFloatVideoPos" />
      <el-button type="primary" round plain @click="setFloatVideoPos">SetFloatVideoPos</el-button>
    </div>
    <el-checkbox v-model="show_bottomFloatToolbarWndVisibility" label="SetBottomFloatToolbarWndVisibility" @change="setBottomFloatToolbarWndVisibility"></el-checkbox>
    <el-checkbox v-model="show_sharingToolbarVisibility" label="SetSharingToolbarVisibility" @change="setSharingToolbarVisibility"></el-checkbox>
    <div class="in-line">
      <el-select v-model="monitorID" placeholder="Select">
        <el-option v-for="(item, index) in monitorsList" :key="index" :label="item.label" :value="item.value"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="setDirectShareMonitorID">SetDirectShareMonitorID</el-button>
    </div>
    <div class="in-line">
      <el-input v-model.trim="left" placeholder="input left" />
      <el-input v-model.trim="top" placeholder="input top" />
      <el-input v-model.trim="hSelfWnd" placeholder="input hSelfWnd" />
      <el-input v-model.trim="hParent" placeholder="input hParent" @keyup.enter.native="setMeetingUIPos" />
      <el-button type="primary" round plain @click="setMeetingUIPos">SetMeetingUIPos</el-button>
    </div>
    <el-checkbox v-model="disable_waitingForHostDialog" label="DisableWaitingForHostDialog" @change="disableWaitingForHostDialog"></el-checkbox>
    <el-checkbox v-model="disable_popupMeetingWrongPSWDlg" label="DisablePopupMeetingWrongPSWDlg" @change="disablePopupMeetingWrongPSWDlg"></el-checkbox>
    <el-checkbox v-model="enable_autoEndOtherMeetingWhenStartMeeting" label="EnableAutoEndOtherMeetingWhenStartMeeting" @change="enableAutoEndOtherMeetingWhenStartMeeting"></el-checkbox>
    <el-checkbox v-model="enable_lButtonDBClick4SwitchFullScreenMode" label="EnableLButtonDBClick4SwitchFullScreenMode" @change="enableLButtonDBClick4SwitchFullScreenMode"></el-checkbox>
    <el-checkbox v-model="show_floatVideoWndVisibility" label="SetFloatVideoWndVisibility" @change="setFloatVideoWndVisibility"></el-checkbox>
    <div class="in-line">
      <el-input v-model.trim="email" placeholder="input email" />
      <el-input v-model.trim="userName" placeholder="input userName" @keyup.enter.native="prePopulateWebinarRegistrationInfo" />
      <el-button type="primary" round plain @click="prePopulateWebinarRegistrationInfo">PrePopulateWebinarRegistrationInfo</el-button>
    </div>
    <el-button type="primary" round plain @click="reset">Reset</el-button>
    <el-checkbox v-model="enable_autoAdjustSpeakerVolumeWhenJoinAudio" label="EnableAutoAdjustSpeakerVolumeWhenJoinAudio" @change="enableAutoAdjustSpeakerVolumeWhenJoinAudio"></el-checkbox>
    <el-checkbox v-model="enable_autoAdjustMicVolumeWhenJoinAudio" label="EnableAutoAdjustMicVolumeWhenJoinAudio" @change="enableAutoAdjustMicVolumeWhenJoinAudio"></el-checkbox>
    <div class="in-line">
      <el-input v-model.trim="dscpAudio" placeholder="input dscpAudio" />
      <el-input v-model.trim="dscpVideo" placeholder="input dscpVideo" />
      <el-checkbox v-model="bReset" label="Reset"></el-checkbox>
      <el-button type="primary" round plain @click="configDSCP">ConfigDSCP</el-button>
    </div>
    <el-checkbox v-model="enable_lengthLimitationOfMeetingNumber" label="EnableLengthLimitationOfMeetingNumber" @change="enableLengthLimitationOfMeetingNumber"></el-checkbox>
    <el-checkbox v-model="enable_shareIOSDevice" label="EnableShareIOSDevice" @change="enableShareIOSDevice"></el-checkbox>
    <el-checkbox v-model="enable_shareWhiteBoard" label="EnableShareWhiteBoard" @change="enableShareWhiteBoard"></el-checkbox>
    <el-checkbox v-model="disable_multiShare" label="ForceDisableMultiShare" @change="forceDisableMultiShare"></el-checkbox>
    <el-checkbox v-model="enable_zoomDocs" label="EnableZoomDocs" @change="enableZoomDocs"></el-checkbox>
    <div class="in-line">
      <el-input-number v-model="duration" :min="0" />
      <el-button type="primary" round plain @click="setMaxDurationForOnlyHostInMeeting">SetMaxDurationForOnlyHostInMeeting</el-button>
    </div>
    <el-checkbox v-model="enable_localRecordingConvertProgressBarDialog" label="EnableLocalRecordingConvertProgressBarDialog" @change="enableLocalRecordingConvertProgressBarDialog"></el-checkbox>
    <el-checkbox v-model="enable_approveRemoteControlDlg" label="EnableApproveRemoteControlDlg" @change="enableApproveRemoteControlDlg"></el-checkbox>
    <el-checkbox v-model="enable_declineRemoteControlResponseDlg" label="EnableDeclineRemoteControlResponseDlg" @change="enableDeclineRemoteControlResponseDlg"></el-checkbox>
    <el-checkbox v-model="enable_leaveMeetingOptionForHost" label="EnableLeaveMeetingOptionForHost" @change="enableLeaveMeetingOptionForHost"></el-checkbox>
    <el-checkbox v-model="enable_videoButtonOnMeetingUI" label="EnableVideoButtonOnMeetingUI" @change="enableVideoButtonOnMeetingUI"></el-checkbox>
    <el-checkbox v-model="enable_enterAndExitFullScreenButtonOnMeetingUI" label="EnableEnterAndExitFullScreenButtonOnMeetingUI" @change="enableEnterAndExitFullScreenButtonOnMeetingUI"></el-checkbox>
    <el-checkbox v-model="redirect_clickShareBTNEvent" label="RedirectClickShareBTNEvent" @change="redirectClickShareBTNEvent"></el-checkbox>
    <el-checkbox v-model="redirect_clickEndMeetingBTNEvent" label="RedirectClickEndMeetingBTNEvent" @change="redirectClickEndMeetingBTNEvent"></el-checkbox>
    <el-checkbox v-model="redirect_freeMeetingEndingReminderDlg" label="RedirectFreeMeetingEndingReminderDlg" @change="redirectFreeMeetingEndingReminderDlg"></el-checkbox>
    <el-checkbox v-model="redirect_clickCustomLiveStreamMenuEvent" label="RedirectClickCustomLiveStreamMenuEvent" @change="redirectClickCustomLiveStreamMenuEvent"></el-checkbox>
    <el-checkbox v-model="redirect_clickParticipantListBTNEvent" label="RedirectClickParticipantListBTNEvent" @change="redirectClickParticipantListBTNEvent"></el-checkbox>
    <el-checkbox v-model="redirect_clickCCBTNEvent" label="RedirectClickCCBTNEvent" @change="redirectClickCCBTNEvent"></el-checkbox>
    <div class="in-line">
      <el-checkbox v-model="redirectBadNetwork" label="RedirectBadNetwork"></el-checkbox>
      <el-checkbox v-model="redirectWarnHighCPU" label="RedirectWarnHighCPU"></el-checkbox>
      <el-button type="primary" round plain @click="redirectMeetingWarningMsg">RedirectMeetingWarningMsg</el-button>
    </div>
    <el-checkbox v-model="enable_toolTipsShow" label="EnableToolTipsShow" @change="enableToolTipsShow"></el-checkbox>
    <el-checkbox v-model="enable_claimHostFeature" label="EnableClaimHostFeature" @change="enableClaimHostFeature"></el-checkbox>
    <el-checkbox v-model="enable_autoHideJoinAudioDialog" label="EnableAutoHideJoinAudioDialog" @change="enableAutoHideJoinAudioDialog"></el-checkbox>
    <el-checkbox v-model="always_showIconOnTaskBar" label="AlwaysShowIconOnTaskBar" @change="alwaysShowIconOnTaskBar"></el-checkbox>
    <el-checkbox v-model="disable_splitScreenModeUIElements" label="DisableSplitScreenModeUIElements" @change="disableSplitScreenModeUIElements"></el-checkbox>
    <el-checkbox v-model="show_audioUseComputerSoundChkbox" label="SetShowAudioUseComputerSoundChkbox" @change="setShowAudioUseComputerSoundChkbox"></el-checkbox>
    <el-checkbox v-model="show_callInTab" label="SetShowCallInTab" @change="setShowCallInTab"></el-checkbox>
    <el-checkbox v-model="show_callMeTab" label="SetShowCallMeTab" @change="setShowCallMeTab"></el-checkbox>
    <el-checkbox v-model="disable_topMostAttr4SettingDialog" label="DisableTopMostAttr4SettingDialog" @change="disableTopMostAttr4SettingDialog"></el-checkbox>
    <el-checkbox v-model="enable_grabShareWithoutReminder" label="EnableGrabShareWithoutReminder" @change="enableGrabShareWithoutReminder"></el-checkbox>
    <el-checkbox v-model="enable_showShareSwitchMultiToSingleConfirmDlg" label="EnableShowShareSwitchMultiToSingleConfirmDlg" @change="enableShowShareSwitchMultiToSingleConfirmDlg"></el-checkbox>
    <el-checkbox v-model="disable_freeMeetingRemainTimeNotify" label="DisableFreeMeetingRemainTimeNotify" @change="disableFreeMeetingRemainTimeNotify"></el-checkbox>
    <el-checkbox v-model="hide_chatItemOnMeetingUI" label="HideChatItemOnMeetingUI" @change="hideChatItemOnMeetingUI"></el-checkbox>
    <el-checkbox v-model="hide_recordItemOnMeetingUI" label="HideRecordItemOnMeetingUI" @change="hideRecordItemOnMeetingUI"></el-checkbox>
    <el-checkbox v-model="hide_upgradeFreeMeetingButton" label="HideUpgradeFreeMeetingButton" @change="hideUpgradeFreeMeetingButton"></el-checkbox>
    <div class="in-line">
      <el-input-number v-model="tabPage" :min="0" />
      <el-checkbox v-model="show_inviteDlgTabPage" label="ShowInviteDlgTabPage"></el-checkbox>
      <el-button type="primary" round plain @click="setShowInviteDlgTabPage">SetShowInviteDlgTabPage</el-button>
    </div>
    <div class="in-line">
      <el-input-number v-model="tabPage" :min="0" />
      <el-checkbox v-model="show_h323SubTabPage" label="ShowInviteDlgTabPage"></el-checkbox>
      <el-button type="primary" round plain @click="setShowH323SubTabPage">SetShowH323SubTabPage</el-button>
    </div>
    <el-checkbox v-model="hide_switchCameraButton" label="HideSwitchCameraButton" @change="hideSwitchCameraButton"></el-checkbox>
    <el-checkbox v-model="hide_copyUrlOnInviteWindow" label="HideCopyUrlOnInviteWindow" @change="hideCopyUrlOnInviteWindow"></el-checkbox>
    <el-checkbox v-model="hide_keypadButtonOnMeetingWindow" label="HideKeypadButtonOnMeetingWindow" @change="hideKeypadButtonOnMeetingWindow"></el-checkbox>
    <el-checkbox v-model="hide_remoteControlOnMeetingUI" label="HideRemoteControlOnMeetingUI" @change="hideRemoteControlOnMeetingUI"></el-checkbox>
    <el-checkbox v-model="hide_QAOnMeetingUI" label="HideQAOnMeetingUI" @change="hideQAOnMeetingUI"></el-checkbox>
    <el-checkbox v-model="hide_pollOnMeetingUI" label="HidePollOnMeetingUI" @change="hidePollOnMeetingUI"></el-checkbox>
    <el-checkbox v-model="enable_inputMeetingPasswordDlg" label="EnableInputMeetingPasswordDlg" @change="enableInputMeetingPasswordDlg"></el-checkbox>
    <el-checkbox v-model="enable_inputMeetingScreenNameDlg" label="EnableInputMeetingScreenNameDlg" @change="enableInputMeetingScreenNameDlg"></el-checkbox>
    <el-checkbox v-model="redirect_webinarNeedRegister" label="RedirectWebinarNeedRegister" @change="redirectWebinarNeedRegister"></el-checkbox>
    <el-checkbox v-model="redirect_endOtherMeeting" label="RedirectEndOtherMeeting" @change="redirectEndOtherMeeting"></el-checkbox>
    <el-checkbox v-model="enable_forceAutoStartMyVideoWhenJoinMeeting" label="EnableForceAutoStartMyVideoWhenJoinMeeting" @change="enableForceAutoStartMyVideoWhenJoinMeeting"></el-checkbox>
    <el-checkbox v-model="enable_forceAutoStopMyVideoWhenJoinMeeting" label="EnableForceAutoStopMyVideoWhenJoinMeeting" @change="enableForceAutoStopMyVideoWhenJoinMeeting"></el-checkbox>
    <el-checkbox v-model="disable_autoShowSelectJoinAudioDlgWhenJoinMeeting" label="DisableAutoShowSelectJoinAudioDlgWhenJoinMeeting" @change="disableAutoShowSelectJoinAudioDlgWhenJoinMeeting"></el-checkbox>
    <el-checkbox v-model="show_videoOptimizeChkbox" label="SetShowVideoOptimizeChkbox" @change="setShowVideoOptimizeChkbox"></el-checkbox>
    <el-button type="primary" round plain @click="getRequiredInfoType">GetRequiredInfoType</el-button>
    <div class="in-line">
      <el-input v-model.trim="meetingPassword" placeholder="input meeting password" />
      <el-input v-model.trim="screenName" placeholder="input screen name" />
      <el-button type="primary" round plain @click="inputMeetingPasswordAndScreenName">InputMeetingPasswordAndScreenName</el-button>
    </div>
    <div class="in-line">
      <el-input v-model.trim="meetingID" placeholder="input meeting id" />
      <el-input v-model.trim="screenName" placeholder="input screen name" />
      <el-button type="primary" round plain @click="inputMeetingIDAndScreenName">InputMeetingIDAndScreenName</el-button>
    </div>
    <div class="in-line">
      <el-input v-model.trim="screenName" placeholder="input screen name" />
      <el-button type="primary" round plain @click="inputMeetingScreenName">InputMeetingScreenName</el-button>
    </div>
    <el-button type="primary" round plain @click="meetingPasswordAndScreenNameHandler_Cancel">MeetingPasswordAndScreenNameHandler_Cancel</el-button>
    <el-button type="primary" round plain @click="getWebinarNeedRegisterType">GetWebinarNeedRegisterType</el-button>
    <el-button type="primary" round plain @click="getWebinarRegisterUrl">GetWebinarRegisterUrl</el-button>
    <el-button type="primary" round plain @click="releaseRegisterWebinarByUrl">ReleaseRegisterWebinarByUrl</el-button>
    <div class="in-line">
      <el-input v-model.trim="email" placeholder="input email" />
      <el-input v-model.trim="screenName" placeholder="input screen name" />
      <el-button type="primary" round plain @click="inputWebinarRegisterEmailAndScreenName">InputWebinarRegisterEmailAndScreenName</el-button>
    </div>
    <el-button type="primary" round plain @click="cancelRegisterWebinarByEmail">CancelRegisterWebinarByEmail</el-button>
    <el-checkbox v-model="redirect_clickAudioBTNEvent" label="RedirectClickAudioBTNEvent" @change="redirectClickAudioBTNEvent"></el-checkbox>
    <el-checkbox v-model="redirect_clickAudioMenuBTNEvent" label="RedirectClickAudioMenuBTNEvent" @change="redirectClickAudioMenuBTNEvent"></el-checkbox>
    <el-checkbox v-model="enable_audioButtonOnMeetingUI" label="EnableAudioButtonOnMeetingUI" @change="enableAudioButtonOnMeetingUI"></el-checkbox>
    <el-checkbox v-model="disable_showJoinMeetingWnd" label="DisableShowJoinMeetingWnd" @change="disableShowJoinMeetingWnd"></el-checkbox>
    <el-checkbox v-model="disable_toolbarInviteButtonClickOriginAction" label="DisableToolbarInviteButtonClickOriginAction" @change="disableToolbarInviteButtonClickOriginAction" v-if="platform=='darwin'"></el-checkbox>
    <el-checkbox v-model="enable_inviteLinkOnMeetingUI" label="EnableInviteLinkOnMeetingUI" @change="enableInviteLinkOnMeetingUI"></el-checkbox>
    <el-checkbox v-model="redirect_meetingInputUserInfoDialog" label="RedirectMeetingInputUserInfoDialog" @change="redirectMeetingInputUserInfoDialog"></el-checkbox>
    <el-checkbox v-model="redirect_confirmStartArchiveDialog" label="RedirectConfirmStartArchiveDialog" @change="redirectConfirmStartArchiveDialog"></el-checkbox>
  </div>
  <div class="">
    <div class="result">Result: {{result}}</div>
    <div class="result"> 
      <div class="result-item"> 
        <span class="title">onFreeMeetingNeedToUpgrade <el-button size="small" @click="onFreeMeetingNeedToUpgrade_options=[]">Clear</el-button></span> 
        <div class="callback-list">
          <span v-for="(item, index) in onFreeMeetingNeedToUpgrade_options" :key="index">{{index}} : {{item}}</span>
        </div>
      </div> 
      <div class="result-item"> 
        <span class="title">onFreeMeetingUpgradeToGiftFreeTrialStart <el-button size="small" @click="onFreeMeetingUpgradeToGiftFreeTrialStart_value=0">Clear</el-button></span> 
        <span v-if="onFreeMeetingUpgradeToGiftFreeTrialStart_value">updated {{onFreeMeetingUpgradeToGiftFreeTrialStart_value}}</span> 
      </div> 
      <div class="result-item"> 
        <span class="title">onFreeMeetingUpgradeToGiftFreeTrialStop <el-button size="small" @click="onFreeMeetingUpgradeToGiftFreeTrialStop_value=0">Clear</el-button></span> 
        <span v-if="onFreeMeetingUpgradeToGiftFreeTrialStop_value">updated {{onFreeMeetingUpgradeToGiftFreeTrialStop_value}}</span> 
      </div> 
      <div class="result-item"> 
        <span class="title">onFreeMeetingUpgradeToProMeeting <el-button size="small" @click="onFreeMeetingUpgradeToProMeeting_value=0">Clear</el-button></span> 
        <span v-if="onFreeMeetingUpgradeToProMeeting_value">updated {{onFreeMeetingUpgradeToProMeeting_value}}</span> 
      </div> 
      <div class="result-item"> 
        <span class="title">onInputMeetingPasswordAndScreenNameNotification <el-button size="small" @click="onInputMeetingPasswordAndScreenNameNotification_value=0">Clear</el-button></span> 
        <span v-if="onInputMeetingPasswordAndScreenNameNotification_value">updated {{onInputMeetingPasswordAndScreenNameNotification_value}}</span> 
      </div> 
      <div class="result-item"> 
        <span class="title">onAirPlayInstructionWndNotification <el-button size="small" @click="onAirPlayInstructionWndNotification_options=[]">Clear</el-button></span> 
        <div class="callback-list">
          <span v-for="(item, index) in onAirPlayInstructionWndNotification_options" :key="index">{{index}} : {{item}}</span>
        </div>
      </div> 
      <div class="result-item"> 
        <span class="title">onWebinarNeedRegisterNotification <el-button size="small" @click="onWebinarNeedRegisterNotification_value=0">Clear</el-button></span> 
        <span v-if="onWebinarNeedRegisterNotification_value">updated {{onWebinarNeedRegisterNotification_value}}</span> 
      </div> 
      <div class="result-item"> 
        <span class="title">onEndOtherMeetingToJoinMeetingNotification <el-button size="small" @click="onEndOtherMeetingToJoinMeetingNotification_value=0">Clear</el-button></span> 
        <span v-if="onEndOtherMeetingToJoinMeetingNotification_value">updated {{onEndOtherMeetingToJoinMeetingNotification_value}}</span> 
      </div> 
      <div class="result-item"> 
        <span class="title">onFreeMeetingRemainTime <el-button size="small" @click="onFreeMeetingRemainTime_options=[]">Clear</el-button></span> 
        <div class="callback-list">
          <span v-for="(item, index) in onFreeMeetingRemainTime_options" :key="index">{{index}} : {{item}}</span>
        </div>
      </div> 
      <div class="result-item"> 
        <span class="title">onFreeMeetingRemainTimeStopCountDown <el-button size="small" @click="onFreeMeetingRemainTimeStopCountDown_value=0">Clear</el-button></span> 
        <span v-if="onFreeMeetingRemainTimeStopCountDown_value">updated {{onFreeMeetingRemainTimeStopCountDown_value}}</span> 
      </div> 
      <div class="result-item"> 
        <span class="title">onWebinarNeedInputScreenName <el-button size="small" @click="onWebinarNeedInputScreenName_value=0">Clear</el-button></span> 
        <span v-if="onWebinarNeedInputScreenName_value">updated {{onWebinarNeedInputScreenName_value}}</span> 
        <div>
          <el-input v-model.trim="screenName" size="small" placeholder="input screen name" />
          <el-button type="primary" size="small" round plain @click="inputName">InputName</el-button>
          <el-button type="primary" size="small" round plain @click="cancelJoinWebinar">Cancel</el-button>
        </div>
      </div>
      <div class="result-item"> 
        <span class="title">onJoinMeetingNeedUserInfo <el-button size="small" @click="onJoinMeetingNeedUserInfo_value=0">Clear</el-button></span> 
        <span v-if="onJoinMeetingNeedUserInfo_value">updated {{onJoinMeetingNeedUserInfo_value}}</span> 
        <div>
          <el-button type="primary" size="small" round plain @click="getDefaultDisplayName">GetDefaultDisplayName</el-button>
          <el-button type="primary" size="small" round plain @click="canModifyDefaultDisplayName">CanModifyDefaultDisplayName</el-button>
          <el-input v-model.trim="email" size="small" placeholder="input email" />
          <el-button type="primary" size="small" round plain @click="isValidEmail">IsValidEmail</el-button>
          <el-input v-model.trim="userName" size="small" placeholder="input name" />
          <el-button type="primary" size="small" round plain @click="inputUserInfo">InputUserInfo</el-button>
          <el-button type="primary" size="small" round plain @click="cancelInputUserInfo">CancelInputUserInfo</el-button>
        </div>
      </div>
      <div class="result-item"> 
        <span class="title">onUserConfirmToStartArchive <el-button size="small" @click="onUserConfirmToStartArchive_value=0">Clear</el-button></span> 
        <span v-if="onUserConfirmToStartArchive_value">updated {{onUserConfirmToStartArchive_value}}</span> 
        <div>
          <el-button type="primary" size="small" round plain @click="getArchiveConfirmContent">GetArchiveConfirmContent</el-button>
          <el-button type="primary" size="small" round plain @click="joinWithArchive(true)">JoinWithArchive</el-button>
          <el-button type="primary" size="small" round plain @click="joinWithArchive(false)">JoinWithoutArchive</el-button>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const desktopCapturer = remote.desktopCapturer
const result = ref()
const onFreeMeetingNeedToUpgrade_options = ref<any[]>([])
const onFreeMeetingUpgradeToGiftFreeTrialStart_value = ref(0)
const onFreeMeetingUpgradeToGiftFreeTrialStop_value = ref(0)
const onFreeMeetingUpgradeToProMeeting_value = ref(0)
const onInputMeetingPasswordAndScreenNameNotification_value = ref(0)
const onAirPlayInstructionWndNotification_options = ref<any[]>([])
const onWebinarNeedRegisterNotification_value = ref(0)
const onEndOtherMeetingToJoinMeetingNotification_value = ref(0)
const onFreeMeetingRemainTime_options = ref<any[]>([])
const onFreeMeetingRemainTimeStopCountDown_value = ref(0)
const onWebinarNeedInputScreenName_value = ref(0)
const onJoinMeetingNeedUserInfo_value = ref(0)
const onUserConfirmToStartArchive_value = ref(0)
const clientHeight = ref((window.innerHeight - 530) + 'px')

const onFreeMeetingNeedToUpgrade = (type: string, gift_url: string) => {
  console.log('onFreeMeetingNeedToUpgrade', type, gift_url)
  let content = `${type} - ${gift_url}`
  onFreeMeetingNeedToUpgrade_options.value.push(content)
}
const onFreeMeetingUpgradeToGiftFreeTrialStart = () => {
  console.log('onFreeMeetingUpgradeToGiftFreeTrialStart')
  onFreeMeetingUpgradeToGiftFreeTrialStart_value.value++
}
const onFreeMeetingUpgradeToGiftFreeTrialStop = () => {
  console.log('onFreeMeetingUpgradeToGiftFreeTrialStop')
  onFreeMeetingUpgradeToGiftFreeTrialStop_value.value++
}
const onFreeMeetingUpgradeToProMeeting = () => {
  console.log('onFreeMeetingUpgradeToProMeeting')
  onFreeMeetingUpgradeToProMeeting_value.value++
}
const onInputMeetingPasswordAndScreenNameNotification = () => {
  console.log('onInputMeetingPasswordAndScreenNameNotification')
  onInputMeetingPasswordAndScreenNameNotification_value.value++
}
const onAirPlayInstructionWndNotification = (bShow: boolean, airhostName: string) => {
  console.log('onAirPlayInstructionWndNotification', bShow, airhostName)
  let content = `${bShow} - ${airhostName}`
  onAirPlayInstructionWndNotification_options.value.push(content)
}
const onWebinarNeedRegisterNotification = () => {
  console.log('onWebinarNeedRegisterNotification')
  onWebinarNeedRegisterNotification_value.value++
}
const onEndOtherMeetingToJoinMeetingNotification = () => {
  console.log('onEndOtherMeetingToJoinMeetingNotification')
  onEndOtherMeetingToJoinMeetingNotification_value.value++
}
const onFreeMeetingRemainTime = (leftTime: string) => {
  console.log('onFreeMeetingRemainTime', leftTime)
  onFreeMeetingRemainTime_options.value.push(leftTime)
}
const onFreeMeetingRemainTimeStopCountDown = () => {
  console.log('onFreeMeetingRemainTimeStopCountDown')
  onFreeMeetingRemainTimeStopCountDown_value.value++
}
const onWebinarNeedInputScreenName = () => {
  console.log('onWebinarNeedInputScreenName')
  onWebinarNeedInputScreenName_value.value++
}
const onJoinMeetingNeedUserInfo = () => {
  console.log('onJoinMeetingNeedUserInfo')
  onJoinMeetingNeedUserInfo_value.value++
}
const onUserConfirmToStartArchive = () => {
  console.log('onUserConfirmToStartArchive')
  onUserConfirmToStartArchive_value.value++
}
const zoomMeetingConfig = remote.app.zoomSdkModule.Meeting.GetMeetingConfiguration({ onFreeMeetingNeedToUpgrade, onFreeMeetingUpgradeToGiftFreeTrialStart,
  onFreeMeetingUpgradeToGiftFreeTrialStop, onFreeMeetingUpgradeToProMeeting, onInputMeetingPasswordAndScreenNameNotification, onAirPlayInstructionWndNotification, 
  onWebinarNeedRegisterNotification, onEndOtherMeetingToJoinMeetingNotification, onFreeMeetingRemainTime, onFreeMeetingRemainTimeStopCountDown, onWebinarNeedInputScreenName,
  onJoinMeetingNeedUserInfo, onUserConfirmToStartArchive })
const enable_inviteButtonOnMeetingUI = ref(false)
const left = ref()
const top = ref()
const hSelfWnd = ref()
const hParent = ref()
const show_bottomFloatToolbarWndVisibility = ref(false)
const show_sharingToolbarVisibility = ref(false)
const monitorsList = ref<any[]>([])
const monitorID = ref()
const disable_waitingForHostDialog = ref(false)
const disable_popupMeetingWrongPSWDlg = ref(false)
const enable_autoEndOtherMeetingWhenStartMeeting = ref(false)
const enable_lButtonDBClick4SwitchFullScreenMode = ref(false)
const show_floatVideoWndVisibility = ref(false)
const email = ref('')
const userName = ref('')
const enable_autoAdjustSpeakerVolumeWhenJoinAudio = ref(false)
const enable_autoAdjustMicVolumeWhenJoinAudio = ref(false)
const dscpAudio = ref()
const dscpVideo = ref()
const bReset = ref(false)
const enable_lengthLimitationOfMeetingNumber = ref(false)
const enable_shareIOSDevice = ref(false)
const enable_shareWhiteBoard = ref(false)
const disable_multiShare = ref(false)
const duration = ref(0)
const enable_localRecordingConvertProgressBarDialog = ref(false)
const enable_approveRemoteControlDlg = ref(false)
const enable_declineRemoteControlResponseDlg = ref(false)
const enable_leaveMeetingOptionForHost = ref(false)
const enable_videoButtonOnMeetingUI = ref(false)
const enable_enterAndExitFullScreenButtonOnMeetingUI = ref(false)
const redirect_clickShareBTNEvent = ref(false)
const redirect_clickEndMeetingBTNEvent = ref(false)
const redirect_freeMeetingEndingReminderDlg = ref(false)
const redirect_clickCustomLiveStreamMenuEvent = ref(false)
const redirect_clickParticipantListBTNEvent = ref(false)
const redirect_clickCCBTNEvent = ref(false)
const redirectBadNetwork = ref(false)
const redirectWarnHighCPU = ref(false)
const enable_toolTipsShow = ref(false)
const enable_claimHostFeature = ref(false)
const enable_autoHideJoinAudioDialog = ref(false)
const always_showIconOnTaskBar = ref(false)
const disable_splitScreenModeUIElements = ref(false)
const show_audioUseComputerSoundChkbox = ref(false)
const show_callInTab = ref(false)
const show_callMeTab = ref(false)
const disable_topMostAttr4SettingDialog = ref(false)
const enable_grabShareWithoutReminder = ref(false)
const enable_showShareSwitchMultiToSingleConfirmDlg = ref(false)
const disable_freeMeetingRemainTimeNotify = ref(false)
const hide_chatItemOnMeetingUI = ref(false)
const hide_recordItemOnMeetingUI = ref(false)
const hide_upgradeFreeMeetingButton = ref(false)
const tabPage = ref(0)
const show_inviteDlgTabPage = ref(false)
const show_h323SubTabPage = ref(false)
const hide_switchCameraButton = ref(false)
const hide_copyUrlOnInviteWindow = ref(false)
const hide_keypadButtonOnMeetingWindow = ref(false)
const hide_remoteControlOnMeetingUI = ref(false)
const hide_QAOnMeetingUI = ref(false)
const hide_pollOnMeetingUI = ref(false)
const enable_inputMeetingPasswordDlg = ref(false)
const enable_inputMeetingScreenNameDlg = ref(false)
const redirect_webinarNeedRegister = ref(false)
const redirect_endOtherMeeting = ref(false)
const enable_forceAutoStartMyVideoWhenJoinMeeting = ref(false)
const enable_forceAutoStopMyVideoWhenJoinMeeting = ref(false)
const disable_autoShowSelectJoinAudioDlgWhenJoinMeeting = ref(false)
const show_videoOptimizeChkbox = ref(false)
const meetingPassword = ref('')
const screenName = ref('')
const meetingID = ref('')
const redirect_clickAudioBTNEvent = ref(false)
const redirect_clickAudioMenuBTNEvent = ref(false)
const enable_audioButtonOnMeetingUI = ref(false)
const disable_showJoinMeetingWnd = ref(false)
const platform = ref(process.platform)
const disable_toolbarInviteButtonClickOriginAction = ref(false)
const enable_inviteLinkOnMeetingUI= ref(false)
const redirect_meetingInputUserInfoDialog = ref(false)
const redirect_confirmStartArchiveDialog = ref(false)

const setFreeMeetingNeedToUpgradeCB = () => {
  let ret = zoomMeetingConfig.MeetingConfig_SetFreeMeetingNeedToUpgradeCB(onFreeMeetingNeedToUpgrade)
  result.value = getSDKResult('MeetingConfig_SetFreeMeetingNeedToUpgradeCB', ret)
}

const setFreeMeetingUpgradeToGiftFreeTrialStartCB = () => {
  let ret = zoomMeetingConfig.MeetingConfig_SetFreeMeetingUpgradeToGiftFreeTrialStartCB(onFreeMeetingUpgradeToGiftFreeTrialStart)
  result.value = getSDKResult('MeetingConfig_SetFreeMeetingUpgradeToGiftFreeTrialStartCB', ret)
}

const setFreeMeetingUpgradeToGiftFreeTrialStopCB = () => {
  let ret = zoomMeetingConfig.MeetingConfig_SetFreeMeetingUpgradeToGiftFreeTrialStopCB(onFreeMeetingUpgradeToGiftFreeTrialStop)
  result.value = getSDKResult('MeetingConfig_SetFreeMeetingUpgradeToGiftFreeTrialStopCB', ret)
}

const setFreeMeetingUpgradeToProMeetingCB = () => {
  let ret = zoomMeetingConfig.MeetingConfig_SetFreeMeetingUpgradeToProMeetingCB(onFreeMeetingUpgradeToProMeeting)
  result.value = getSDKResult('MeetingConfig_SetFreeMeetingUpgradeToProMeetingCB', ret)
}

const setInputMeetingPasswordAndScreenNameNotificationCB = () => {
  let ret = zoomMeetingConfig.MeetingConfig_SetInputMeetingPasswordAndScreenNameNotificationCB(onInputMeetingPasswordAndScreenNameNotification)
  result.value = getSDKResult('MeetingConfig_SetInputMeetingPasswordAndScreenNameNotificationCB', ret)
}

const setAirPlayInstructionWndNotificationCB = () => {
  let ret = zoomMeetingConfig.MeetingConfig_SetAirPlayInstructionWndNotificationCB(onAirPlayInstructionWndNotification)
  result.value = getSDKResult('MeetingConfig_SetAirPlayInstructionWndNotificationCB', ret)
}

const setonWebinarNeedRegisterNotificationCB = () => {
  let ret = zoomMeetingConfig.MeetingConfig_SetonWebinarNeedRegisterNotificationCB(onWebinarNeedRegisterNotification)
  result.value = getSDKResult('MeetingConfig_SetonWebinarNeedRegisterNotificationCB', ret)
}

const setonEndOtherMeetingToJoinMeetingNotificationCB = () => {
  let ret = zoomMeetingConfig.MeetingConfig_SetonEndOtherMeetingToJoinMeetingNotificationCB(onEndOtherMeetingToJoinMeetingNotification)
  result.value = getSDKResult('MeetingConfig_SetonEndOtherMeetingToJoinMeetingNotificationCB', ret)
}

const setonFreeMeetingRemainTimeCB = () => {
  let ret = zoomMeetingConfig.MeetingConfig_SetonFreeMeetingRemainTimeCB(onFreeMeetingRemainTime)
  result.value = getSDKResult('MeetingConfig_SetonFreeMeetingRemainTimeCB', ret)
}

const setonFreeMeetingRemainTimeStopCountDownCB = () => {
  let ret = zoomMeetingConfig.MeetingConfig_SetonFreeMeetingRemainTimeStopCountDownCB(onFreeMeetingRemainTimeStopCountDown)
  result.value = getSDKResult('MeetingConfig_SetonFreeMeetingRemainTimeStopCountDownCB', ret)
}

const setonWebinarNeedInputScreenNameCB = () => {
  let ret = zoomMeetingConfig.MeetingConfig_SetonWebinarNeedInputScreenNameCB(onWebinarNeedInputScreenName)
  result.value = getSDKResult('MeetingConfig_SetonWebinarNeedInputScreenNameCB', ret)
}

const setonJoinMeetingNeedUserInfoCB = () => {
  let ret = zoomMeetingConfig.MeetingConfig_SetonJoinMeetingNeedUserInfoCB(onJoinMeetingNeedUserInfo)
  result.value = getSDKResult('MeetingConfig_SetonJoinMeetingNeedUserInfoCB', ret)
}

const setonUserConfirmToStartArchiveCB = () => {
  let ret = zoomMeetingConfig.MeetingConfig_SetonUserConfirmToStartArchiveCB(onUserConfirmToStartArchive)
  result.value = getSDKResult('MeetingConfig_SetonUserConfirmToStartArchiveCB', ret)
}

const enableInviteButtonOnMeetingUI = () => {
  let param = {bEnable: enable_inviteButtonOnMeetingUI.value}
  let ret = zoomMeetingConfig.MeetingConfig_EnableInviteButtonOnMeetingUI(param)
  result.value = getSDKResult('MeetingConfig_EnableInviteButtonOnMeetingUI', ret, param)
}

const setFloatVideoPos = () => {
  let param = {
    left: left.value,
    top: top.value,
    hSelfWnd: hSelfWnd.value,
    hParent: hParent.value
  }
  let ret = zoomMeetingConfig.MeetingConfig_SetFloatVideoPos(param)
  result.value = getSDKResult('MeetingConfig_SetFloatVideoPos', ret, param)
}

const setBottomFloatToolbarWndVisibility = () => {
  let param = {
    bShow: show_bottomFloatToolbarWndVisibility.value
  }
  let ret = zoomMeetingConfig.MeetingConfig_SetBottomFloatToolbarWndVisibility(param)
  result.value = getSDKResult('MeetingConfig_SetBottomFloatToolbarWndVisibility', ret, param)
}

const setSharingToolbarVisibility = () => {
  let param = {
    bShow: show_sharingToolbarVisibility.value
  }
  let ret = zoomMeetingConfig.MeetingConfig_SetSharingToolbarVisibility(param)
  result.value = getSDKResult('MeetingConfig_SetSharingToolbarVisibility', ret, param)
}

const setDirectShareMonitorID = () => {
  let param = {monitorID: monitorID.value}
  let ret = zoomMeetingConfig.MeetingConfig_SetDirectShareMonitorID(param)
  result.value = getSDKResult('MeetingConfig_SetDirectShareMonitorID', ret, param)
}

const setMeetingUIPos = () => {
  let param = {
    left: left.value,
    top: top.value,
    hSelfWnd: hSelfWnd.value,
    hParent: hParent.value
  }
  let ret = zoomMeetingConfig.MeetingConfig_SetMeetingUIPos(param)
  result.value = getSDKResult('MeetingConfig_SetMeetingUIPos', ret, param)
}

const disableWaitingForHostDialog = () => {
  let param = {bDisable: disable_waitingForHostDialog.value}
  let ret = zoomMeetingConfig.MeetingConfig_DisableWaitingForHostDialog(param)
  result.value = getSDKResult('MeetingConfig_DisableWaitingForHostDialog', ret, param)
}

const disablePopupMeetingWrongPSWDlg = () => {
  let param = {bDisable: disable_popupMeetingWrongPSWDlg.value}
  let ret = zoomMeetingConfig.MeetingConfig_DisablePopupMeetingWrongPSWDlg(param)
  result.value = getSDKResult('MeetingConfig_DisablePopupMeetingWrongPSWDlg', ret, param)
}

const enableAutoEndOtherMeetingWhenStartMeeting = () => {
  let param = {bEnable: enable_autoEndOtherMeetingWhenStartMeeting.value}
  let ret = zoomMeetingConfig.MeetingConfig_EnableAutoEndOtherMeetingWhenStartMeeting(param)
  result.value = getSDKResult('MeetingConfig_EnableAutoEndOtherMeetingWhenStartMeeting', ret, param)
}

const enableLButtonDBClick4SwitchFullScreenMode = () => {
  let param = {bEnable: enable_lButtonDBClick4SwitchFullScreenMode.value}
  let ret = zoomMeetingConfig.MeetingConfig_EnableLButtonDBClick4SwitchFullScreenMode(param)
  result.value = getSDKResult('MeetingConfig_EnableLButtonDBClick4SwitchFullScreenMode', ret, param)
}

const setFloatVideoWndVisibility = () => {
  let param = {bShow: show_floatVideoWndVisibility.value}
  let ret = zoomMeetingConfig.MeetingConfig_SetFloatVideoWndVisibility(param)
  result.value = getSDKResult('MeetingConfig_SetFloatVideoWndVisibility', ret, param)
}

const prePopulateWebinarRegistrationInfo = () => {
  let param = {email: email.value, userName: userName.value}
  let ret = zoomMeetingConfig.MeetingConfig_PrePopulateWebinarRegistrationInfo(param)
  result.value = getSDKResult('MeetingConfig_PrePopulateWebinarRegistrationInfo', ret, param)
}

const reset = () => {
  let ret = zoomMeetingConfig.MeetingConfig_Reset()
  result.value = getSDKResult('MeetingConfig_Reset', ret)
}

const enableAutoAdjustSpeakerVolumeWhenJoinAudio = () => {
  let param = {bEnable: enable_autoAdjustSpeakerVolumeWhenJoinAudio.value}
  let ret = zoomMeetingConfig.MeetingConfig_EnableAutoAdjustSpeakerVolumeWhenJoinAudio(param)
  result.value = getSDKResult('MeetingConfig_EnableAutoAdjustSpeakerVolumeWhenJoinAudio', ret, param)
}

const enableAutoAdjustMicVolumeWhenJoinAudio = () => {
  let param = {bEnable: enable_autoAdjustMicVolumeWhenJoinAudio.value}
  let ret = zoomMeetingConfig.MeetingConfig_EnableAutoAdjustMicVolumeWhenJoinAudio(param)
  result.value = getSDKResult('MeetingConfig_EnableAutoAdjustMicVolumeWhenJoinAudio', ret, param)
}

const configDSCP = () => {
  let param = {dscpAudio: dscpAudio.value, dscpVideo: dscpVideo.value, bReset: bReset.value}
  let ret = zoomMeetingConfig.MeetingConfig_ConfigDSCP(param)
  result.value = getSDKResult('MeetingConfig_ConfigDSCP', ret, param)
}

const enableLengthLimitationOfMeetingNumber = () => {
  let param = {bEnable: enable_lengthLimitationOfMeetingNumber.value}
  let ret = zoomMeetingConfig.MeetingConfig_EnableLengthLimitationOfMeetingNumber(param)
  result.value = getSDKResult('MeetingConfig_EnableLengthLimitationOfMeetingNumber', ret, param)
}

const enableShareIOSDevice = () => {
  let param = {bEnable: enable_shareIOSDevice.value}
  let ret = zoomMeetingConfig.MeetingConfig_EnableShareIOSDevice(param)
  result.value = getSDKResult('MeetingConfig_EnableShareIOSDevice', ret, param)
}

const enableShareWhiteBoard = () => {
  let param = {bEnable: enable_shareWhiteBoard.value}
  let ret = zoomMeetingConfig.MeetingConfig_EnableShareWhiteBoard(param)
  result.value = getSDKResult('MeetingConfig_EnableShareWhiteBoard', ret, param)
}

const forceDisableMultiShare = () => {
  let param = {bEnable: disable_multiShare.value}
  let ret = zoomMeetingConfig.MeetingConfig_ForceDisableMultiShare(param)
  result.value = getSDKResult('MeetingConfig_ForceDisableMultiShare', ret, param)
}

const enable_zoomDocs = ref(false)
const enableZoomDocs = () => {
  let param = { bEnable: enable_zoomDocs.value }
  let ret = zoomMeetingConfig.MeetingConfig_EnableZoomDocs(param)
  result.value = getSDKResult('MeetingConfig_EnableZoomDocs', ret, param)
}

const setMaxDurationForOnlyHostInMeeting = () => {
  let param = {nDuration: duration.value}
  let ret = zoomMeetingConfig.MeetingConfig_SetMaxDurationForOnlyHostInMeeting(param)
  result.value = getSDKResult('MeetingConfig_SetMaxDurationForOnlyHostInMeeting', ret, param)
}

const enableLocalRecordingConvertProgressBarDialog = () => {
  let param = {bShow: enable_localRecordingConvertProgressBarDialog.value}
  let ret = zoomMeetingConfig.MeetingConfig_EnableLocalRecordingConvertProgressBarDialog(param)
  result.value = getSDKResult('MeetingConfig_EnableLocalRecordingConvertProgressBarDialog', ret, param)
}

const enableApproveRemoteControlDlg = () => {
  let param = {bEnable: enable_approveRemoteControlDlg.value}
  let ret = zoomMeetingConfig.MeetingConfig_EnableApproveRemoteControlDlg(param)
  result.value = getSDKResult('MeetingConfig_EnableApproveRemoteControlDlg', ret, param)
}

const enableDeclineRemoteControlResponseDlg = () => {
  let param = {bEnable: enable_declineRemoteControlResponseDlg.value}
  let ret = zoomMeetingConfig.MeetingConfig_EnableDeclineRemoteControlResponseDlg(param)
  result.value = getSDKResult('MeetingConfig_EnableDeclineRemoteControlResponseDlg', ret, param)
}

const enableLeaveMeetingOptionForHost = () => {
  let param = {bEnable: enable_leaveMeetingOptionForHost.value}
  let ret = zoomMeetingConfig.MeetingConfig_EnableLeaveMeetingOptionForHost(param)
  result.value = getSDKResult('MeetingConfig_EnableLeaveMeetingOptionForHost', ret, param)
}

const enableVideoButtonOnMeetingUI = () => {
  let param = {bEnable: enable_videoButtonOnMeetingUI.value}
  let ret = zoomMeetingConfig.MeetingConfig_EnableVideoButtonOnMeetingUI(param)
  result.value = getSDKResult('MeetingConfig_EnableVideoButtonOnMeetingUI', ret, param)
}

const enableEnterAndExitFullScreenButtonOnMeetingUI = () => {
  let param = {bEnable: enable_enterAndExitFullScreenButtonOnMeetingUI.value}
  let ret = zoomMeetingConfig.MeetingConfig_EnableEnterAndExitFullScreenButtonOnMeetingUI(param)
  result.value = getSDKResult('MeetingConfig_EnableEnterAndExitFullScreenButtonOnMeetingUI', ret, param)
}

const redirectClickShareBTNEvent = () => {
  let param = {bEnable: redirect_clickShareBTNEvent.value}
  let ret = zoomMeetingConfig.MeetingConfig_RedirectClickShareBTNEvent(param)
  result.value = getSDKResult('MeetingConfig_RedirectClickShareBTNEvent', ret, param)
}

const redirectClickEndMeetingBTNEvent = () => {
  let param = {bEnable: redirect_clickEndMeetingBTNEvent.value}
  let ret = zoomMeetingConfig.MeetingConfig_RedirectClickEndMeetingBTNEvent(param)
  result.value = getSDKResult('MeetingConfig_RedirectClickEndMeetingBTNEvent', ret, param)
}

const redirectFreeMeetingEndingReminderDlg = () => {
  let param = {bEnable: redirect_freeMeetingEndingReminderDlg.value}
  let ret = zoomMeetingConfig.MeetingConfig_RedirectFreeMeetingEndingReminderDlg(param)
  result.value = getSDKResult('MeetingConfig_RedirectFreeMeetingEndingReminderDlg', ret, param)
}

const redirectClickCustomLiveStreamMenuEvent = () => {
  let param = {bEnable: redirect_clickCustomLiveStreamMenuEvent.value}
  let ret = zoomMeetingConfig.MeetingConfig_RedirectClickCustomLiveStreamMenuEvent(param)
  result.value = getSDKResult('MeetingConfig_RedirectClickCustomLiveStreamMenuEvent', ret, param)
}

const redirectClickParticipantListBTNEvent = () => {
  let param = {bEnable: redirect_clickParticipantListBTNEvent.value}
  let ret = zoomMeetingConfig.MeetingConfig_RedirectClickParticipantListBTNEvent(param)
  result.value = getSDKResult('MeetingConfig_RedirectClickParticipantListBTNEvent', ret, param)
}

const redirectClickCCBTNEvent = () => {
  let param = {bEnable: redirect_clickCCBTNEvent.value}
  let ret = zoomMeetingConfig.MeetingConfig_RedirectClickCCBTNEvent(param)
  result.value = getSDKResult('MeetingConfig_RedirectClickCCBTNEvent', ret, param)
}

const redirectMeetingWarningMsg = () => {
  let param = {bRedirectBadNetwork: redirectBadNetwork.value, bRedirectWarnHighCPU: redirectWarnHighCPU.value}
  let ret = zoomMeetingConfig.MeetingConfig_RedirectMeetingWarningMsg(param)
  result.value = getSDKResult('MeetingConfig_RedirectMeetingWarningMsg', ret, param)
}

const enableToolTipsShow = () => {
  let param = {bEnable: enable_toolTipsShow.value}
  let ret = zoomMeetingConfig.MeetingConfig_EnableToolTipsShow(param)
  result.value = getSDKResult('MeetingConfig_EnableToolTipsShow', ret, param)
}

const enableClaimHostFeature = () => {
  let param = {bEnable: enable_claimHostFeature.value}
  let ret = zoomMeetingConfig.MeetingConfig_EnableClaimHostFeature(param)
  result.value = getSDKResult('MeetingConfig_EnableClaimHostFeature', ret, param)
}

const enableAutoHideJoinAudioDialog = () => {
  let param = {bEnable: enable_autoHideJoinAudioDialog.value}
  let ret = zoomMeetingConfig.MeetingConfig_EnableAutoHideJoinAudioDialog(param)
  result.value = getSDKResult('MeetingConfig_EnableAutoHideJoinAudioDialog', ret, param)
}

const alwaysShowIconOnTaskBar = () => {
  let param = {bEnable: always_showIconOnTaskBar.value}
  let ret = zoomMeetingConfig.MeetingConfig_AlwaysShowIconOnTaskBar(param)
  result.value = getSDKResult('MeetingConfig_AlwaysShowIconOnTaskBar', ret, param)
}

const disableSplitScreenModeUIElements = () => {
  let param = {bEnable: disable_splitScreenModeUIElements.value}
  let ret = zoomMeetingConfig.MeetingConfig_DisableSplitScreenModeUIElements(param)
  result.value = getSDKResult('MeetingConfig_DisableSplitScreenModeUIElements', ret, param)
}

const setShowAudioUseComputerSoundChkbox = () => {
  let param = {bEnable: show_audioUseComputerSoundChkbox.value}
  let ret = zoomMeetingConfig.MeetingConfig_SetShowAudioUseComputerSoundChkbox(param)
  result.value = getSDKResult('MeetingConfig_SetShowAudioUseComputerSoundChkbox', ret, param)
}

const setShowCallInTab = () => {
  let param = {bEnable: show_callInTab.value}
  let ret = zoomMeetingConfig.MeetingConfig_SetShowCallInTab(param)
  result.value = getSDKResult('MeetingConfig_SetShowCallInTab', ret, param)
}

const setShowCallMeTab = () => {
  let param = {bEnable: show_callMeTab.value}
  let ret = zoomMeetingConfig.MeetingConfig_SetShowCallMeTab(param)
  result.value = getSDKResult('MeetingConfig_SetShowCallMeTab', ret, param)
}

const disableTopMostAttr4SettingDialog = () => {
  let param = {bEnable: disable_topMostAttr4SettingDialog.value}
  let ret = zoomMeetingConfig.MeetingConfig_DisableTopMostAttr4SettingDialog(param)
  result.value = getSDKResult('MeetingConfig_DisableTopMostAttr4SettingDialog', ret, param)
}

const enableGrabShareWithoutReminder = () => {
  let param = {bEnable: enable_grabShareWithoutReminder.value}
  let ret = zoomMeetingConfig.MeetingConfig_EnableGrabShareWithoutReminder(param)
  result.value = getSDKResult('MeetingConfig_EnableGrabShareWithoutReminder', ret, param)
}

const enableShowShareSwitchMultiToSingleConfirmDlg = () => {
  let param = {bEnable: enable_showShareSwitchMultiToSingleConfirmDlg.value}
  let ret = zoomMeetingConfig.MeetingConfig_EnableShowShareSwitchMultiToSingleConfirmDlg(param)
  result.value = getSDKResult('MeetingConfig_EnableShowShareSwitchMultiToSingleConfirmDlg', ret, param)
}

const disableFreeMeetingRemainTimeNotify = () => {
  let param = {bEnable: disable_freeMeetingRemainTimeNotify.value}
  let ret = zoomMeetingConfig.MeetingConfig_DisableFreeMeetingRemainTimeNotify(param)
  result.value = getSDKResult('MeetingConfig_DisableFreeMeetingRemainTimeNotify', ret, param)
}

const hideChatItemOnMeetingUI = () => {
  let param = {bEnable: hide_chatItemOnMeetingUI.value}
  let ret = zoomMeetingConfig.MeetingConfig_HideChatItemOnMeetingUI(param)
  result.value = getSDKResult('MeetingConfig_HideChatItemOnMeetingUI', ret, param)
}

const hideRecordItemOnMeetingUI = () => {
  let param = {bEnable: hide_recordItemOnMeetingUI.value}
  let ret = zoomMeetingConfig.MeetingConfig_HideRecordItemOnMeetingUI(param)
  result.value = getSDKResult('MeetingConfig_HideRecordItemOnMeetingUI', ret, param)
}

const hideUpgradeFreeMeetingButton = () => {
  let param = {bEnable: hide_upgradeFreeMeetingButton.value}
  let ret = zoomMeetingConfig.MeetingConfig_HideUpgradeFreeMeetingButton(param)
  result.value = getSDKResult('MeetingConfig_HideUpgradeFreeMeetingButton', ret, param)
}

const setShowInviteDlgTabPage = () => {
  let param = {tabPage: tabPage.value, bShow: show_inviteDlgTabPage.value}
  let ret = zoomMeetingConfig.MeetingConfig_SetShowInviteDlgTabPage(param)
  result.value = getSDKResult('MeetingConfig_SetShowInviteDlgTabPage', ret, param)
}

const setShowH323SubTabPage = () => {
  let param = {tabPage: tabPage.value, bShow: show_h323SubTabPage.value}
  let ret = zoomMeetingConfig.MeetingConfig_SetShowH323SubTabPage(param)
  result.value = getSDKResult('MeetingConfig_SetShowH323SubTabPage', ret, param)
}

const hideSwitchCameraButton = () => {
  let param = {bEnable: hide_switchCameraButton.value}
  let ret = zoomMeetingConfig.MeetingConfig_HideSwitchCameraButton(param)
  result.value = getSDKResult('MeetingConfig_HideSwitchCameraButton', ret, param)
}

const hideCopyUrlOnInviteWindow = () => {
  let param = {bEnable: hide_copyUrlOnInviteWindow.value}
  let ret = zoomMeetingConfig.MeetingConfig_HideCopyUrlOnInviteWindow(param)
  result.value = getSDKResult('MeetingConfig_HideCopyUrlOnInviteWindow', ret, param)
}

const hideKeypadButtonOnMeetingWindow = () => {
  let param = {bEnable: hide_keypadButtonOnMeetingWindow.value}
  let ret = zoomMeetingConfig.MeetingConfig_HideKeypadButtonOnMeetingWindow(param)
  result.value = getSDKResult('MeetingConfig_HideKeypadButtonOnMeetingWindow', ret, param)
}

const hideRemoteControlOnMeetingUI = () => {
  let param = {bEnable: hide_remoteControlOnMeetingUI.value}
  let ret = zoomMeetingConfig.MeetingConfig_HideRemoteControlOnMeetingUI(param)
  result.value = getSDKResult('MeetingConfig_HideRemoteControlOnMeetingUI', ret, param)
}

const hideQAOnMeetingUI = () => {
  let param = {bEnable: hide_QAOnMeetingUI.value}
  let ret = zoomMeetingConfig.MeetingConfig_HideQAOnMeetingUI(param)
  result.value = getSDKResult('MeetingConfig_HideQAOnMeetingUI', ret, param)
}

const hidePollOnMeetingUI = () => {
  let param = {bEnable: hide_pollOnMeetingUI.value}
  let ret = zoomMeetingConfig.MeetingConfig_HidePollOnMeetingUI(param)
  result.value = getSDKResult('MeetingConfig_HidePollOnMeetingUI', ret, param)
}

const enableInputMeetingPasswordDlg = () => {
  let param = {bEnable: enable_inputMeetingPasswordDlg.value}
  let ret = zoomMeetingConfig.MeetingConfig_EnableInputMeetingPasswordDlg(param)
  result.value = getSDKResult('MeetingConfig_EnableInputMeetingPasswordDlg', ret, param)
}

const enableInputMeetingScreenNameDlg = () => {
  let param = {bEnable: enable_inputMeetingScreenNameDlg.value}
  let ret = zoomMeetingConfig.MeetingConfig_EnableInputMeetingScreenNameDlg(param)
  result.value = getSDKResult('MeetingConfig_EnableInputMeetingScreenNameDlg', ret, param)
}

const redirectWebinarNeedRegister = () => {
  let param = {bEnable: redirect_webinarNeedRegister.value}
  let ret = zoomMeetingConfig.MeetingConfig_RedirectWebinarNeedRegister(param)
  result.value = getSDKResult('MeetingConfig_RedirectWebinarNeedRegister', ret, param)
}

const redirectEndOtherMeeting = () => {
  let param = {bEnable: redirect_endOtherMeeting.value}
  let ret = zoomMeetingConfig.MeetingConfig_RedirectEndOtherMeeting(param)
  result.value = getSDKResult('MeetingConfig_RedirectEndOtherMeeting', ret, param)
}

const enableForceAutoStartMyVideoWhenJoinMeeting = () => {
  let param = {bEnable: enable_forceAutoStartMyVideoWhenJoinMeeting.value}
  let ret = zoomMeetingConfig.MeetingConfig_EnableForceAutoStartMyVideoWhenJoinMeeting(param)
  result.value = getSDKResult('MeetingConfig_EnableForceAutoStartMyVideoWhenJoinMeeting', ret, param)
}

const enableForceAutoStopMyVideoWhenJoinMeeting = () => {
  let param = {bEnable: enable_forceAutoStopMyVideoWhenJoinMeeting.value}
  let ret = zoomMeetingConfig.MeetingConfig_EnableForceAutoStopMyVideoWhenJoinMeeting(param)
  result.value = getSDKResult('MeetingConfig_EnableForceAutoStopMyVideoWhenJoinMeeting', ret, param)
}

const disableAutoShowSelectJoinAudioDlgWhenJoinMeeting = () => {
  let param = {bEnable: disable_autoShowSelectJoinAudioDlgWhenJoinMeeting.value}
  let ret = zoomMeetingConfig.MeetingConfig_DisableAutoShowSelectJoinAudioDlgWhenJoinMeeting(param)
  result.value = getSDKResult('MeetingConfig_DisableAutoShowSelectJoinAudioDlgWhenJoinMeeting', ret, param)
}

const setShowVideoOptimizeChkbox = () => {
  let param = {bShow: show_videoOptimizeChkbox.value}
  let ret = zoomMeetingConfig.MeetingConfig_SetShowVideoOptimizeChkbox(param)
  result.value = getSDKResult('MeetingConfig_SetShowVideoOptimizeChkbox', ret, param)
}

const getRequiredInfoType = () => {
  let ret = zoomMeetingConfig.MeetingConfig_GetRequiredInfoType()
  result.value = getSDKResult('MeetingConfig_GetRequiredInfoType', ret)
}

const inputMeetingPasswordAndScreenName = () => {
  let param = {meeting_Password: meetingPassword.value, screenName: screenName.value}
  let ret = zoomMeetingConfig.MeetingConfig_InputMeetingPasswordAndScreenName(param)
  result.value = getSDKResult('MeetingConfig_InputMeetingPasswordAndScreenName', ret, param)
}

const inputMeetingIDAndScreenName = () => {
  let param = {meetingID: meetingID.value, screenName: screenName.value}
  let ret = zoomMeetingConfig.MeetingConfig_InputMeetingIDAndScreenName(param)
  result.value = getSDKResult('MeetingConfig_InputMeetingIDAndScreenName', ret, param)
}

const inputMeetingScreenName = () => {
  let param = {screenName: screenName.value}
  let ret = zoomMeetingConfig.MeetingConfig_InputMeetingScreenName(param)
  result.value = getSDKResult('MeetingConfig_InputMeetingScreenName', ret, param)
}

const meetingPasswordAndScreenNameHandler_Cancel = () => {
  let ret = zoomMeetingConfig.MeetingConfig_MeetingPasswordAndScreenNameHandler_Cancel()
  result.value = getSDKResult('MeetingConfig_MeetingPasswordAndScreenNameHandler_Cancel', ret)
}

const getWebinarNeedRegisterType = () => {
  let ret = zoomMeetingConfig.MeetingConfig_GetWebinarNeedRegisterType()
  result.value = getSDKResult('MeetingConfig_GetWebinarNeedRegisterType', ret)
}

const getWebinarRegisterUrl = () => {
  let ret = zoomMeetingConfig.MeetingConfig_GetWebinarRegisterUrl()
  result.value = getSDKResult('MeetingConfig_GetWebinarRegisterUrl', ret)
}

const releaseRegisterWebinarByUrl = () => {
  let ret = zoomMeetingConfig.MeetingConfig_ReleaseRegisterWebinarByUrl()
  result.value = getSDKResult('MeetingConfig_ReleaseRegisterWebinarByUrl', ret)
}

const inputWebinarRegisterEmailAndScreenName = () => {
  let param = {email: email.value, screenName: screenName.value}
  let ret = zoomMeetingConfig.MeetingConfig_InputWebinarRegisterEmailAndScreenName(param)
  result.value = getSDKResult('MeetingConfig_InputWebinarRegisterEmailAndScreenName', ret, param)
}

const cancelRegisterWebinarByEmail = () => {
  let ret = zoomMeetingConfig.MeetingConfig_CancelRegisterWebinarByEmail()
  result.value = getSDKResult('MeetingConfig_CancelRegisterWebinarByEmail', ret)
}

const redirectClickAudioBTNEvent = () => {
  let param = {bRedirect: redirect_clickAudioBTNEvent.value}
  let ret = zoomMeetingConfig.MeetingConfig_RedirectClickAudioBTNEvent(param)
  result.value = getSDKResult('MeetingConfig_RedirectClickAudioBTNEvent', ret, param)
}

const redirectClickAudioMenuBTNEvent = () => {
  let param = {bRedirect: redirect_clickAudioMenuBTNEvent.value}
  let ret = zoomMeetingConfig.MeetingConfig_RedirectClickAudioMenuBTNEvent(param)
  result.value = getSDKResult('MeetingConfig_RedirectClickAudioMenuBTNEvent', ret, param)
}

const enableAudioButtonOnMeetingUI = () => {
  let param = {bEnable: enable_audioButtonOnMeetingUI.value}
  let ret = zoomMeetingConfig.MeetingConfig_EnableAudioButtonOnMeetingUI(param)
  result.value = getSDKResult('MeetingConfig_EnableAudioButtonOnMeetingUI', ret, param)
}

const disableShowJoinMeetingWnd = () => {
  let param = {bDisable: disable_showJoinMeetingWnd.value}
  let ret = zoomMeetingConfig.MeetingConfig_DisableShowJoinMeetingWnd(param)
  result.value = getSDKResult('MeetingConfig_DisableShowJoinMeetingWnd', ret, param)
}

const disableToolbarInviteButtonClickOriginAction = () => {
  let param = {bDisable: disable_toolbarInviteButtonClickOriginAction.value}
  let ret = zoomMeetingConfig.MeetingConfig_DisableToolbarInviteButtonClickOriginAction(param)
  result.value = getSDKResult('MeetingConfig_DisableToolbarInviteButtonClickOriginAction', ret, param)
}

const enableInviteLinkOnMeetingUI = () => {
  let param = {bDisable: enable_inviteLinkOnMeetingUI.value}
  let ret = zoomMeetingConfig.MeetingConfig_EnableInviteLinkOnMeetingUI(param)
  result.value = getSDKResult('MeetingConfig_EnableInviteLinkOnMeetingUI', ret, param)
}

const redirectMeetingInputUserInfoDialog = () => {
  let param = { bRedirect: redirect_meetingInputUserInfoDialog.value }
  let ret = zoomMeetingConfig.MeetingConfig_RedirectMeetingInputUserInfoDialog(param)
  result.value = getSDKResult('MeetingConfig_RedirectMeetingInputUserInfoDialog', ret, param)
}

const redirectConfirmStartArchiveDialog = () => {
  let param = { bRedirect: redirect_confirmStartArchiveDialog.value }
  let ret = zoomMeetingConfig.MeetingConfig_RedirectConfirmStartArchiveDialog(param)
  result.value = getSDKResult('MeetingConfig_RedirectConfirmStartArchiveDialog', ret, param)
}

const inputName = () => {
  let param = { screenName: screenName.value }
  let ret = zoomMeetingConfig.MeetingConfig_InputScreenName(param)
  result.value = getSDKResult('MeetingConfig_InputScreenName', ret, param)
}

const cancelJoinWebinar = () => {
  let ret = zoomMeetingConfig.MeetingConfig_CancelJoinWebinar()
  result.value = getSDKResult('MeetingConfig_CancelJoinWebinar', ret)
}

const getDefaultDisplayName = () => {
  let ret = zoomMeetingConfig.MeetingConfig_GetDefaultDisplayName()
  result.value = getSDKResult('MeetingConfig_GetDefaultDisplayName', ret)
}

const canModifyDefaultDisplayName = () => {
  let ret = zoomMeetingConfig.MeetingConfig_CanModifyDefaultDisplayName()
  result.value = getSDKResult('MeetingConfig_CanModifyDefaultDisplayName', ret)
}

const isValidEmail = () => {
  let param = { email: email.value }
  let ret = zoomMeetingConfig.MeetingConfig_IsValidEmail(param)
  result.value = getSDKResult('MeetingConfig_IsValidEmail', ret, param)
}

const inputUserInfo = () => {
  let param = { name: userName.value, email: email.value }
  let ret = zoomMeetingConfig.MeetingConfig_InputUserInfo(param)
  result.value = getSDKResult('MeetingConfig_InputUserInfo', ret, param)
}

const cancelInputUserInfo = () => {
  let ret = zoomMeetingConfig.MeetingConfig_CancelInputUserInfo()
  result.value = getSDKResult('MeetingConfig_CancelInputUserInfo', ret)
}

const getArchiveConfirmContent = () => {
  let ret = zoomMeetingConfig.MeetingConfig_GetArchiveConfirmContent()
  result.value = getSDKResult('MeetingConfig_GetArchiveConfirmContent', ret)
}

const joinWithArchive = (bStartArchive: boolean) => {
  let param = { bStartArchive }
  let ret = zoomMeetingConfig.MeetingConfig_JoinWithArchive(param)
  result.value = getSDKResult('MeetingConfig_JoinWithArchive', ret, param)
}

onMounted(() => {
  desktopCapturer.getSources({ types: ['screen'] }).then(async (sources: any) => {
		console.log(sources);
		for (let i in sources) {
			let obj = {...sources[i]};
			let shareMonitorID: string = obj.id.split(':')[1];
			if (window.navigator.userAgent.indexOf('Windows') > -1) {
				shareMonitorID = `\\\\.\\DISPLAY${Number(obj.id.split(':')[1]) + 1}`
			}
			monitorsList.value.push({value: shareMonitorID, label: `${obj.name} - ${obj.id}`})
		}
    if (monitorsList.value.length > 0) {
      monitorID.value = monitorsList.value[0].value
    }
		console.log('monitorsList', monitorsList.value);
	})
  window.addEventListener('resize', ()=>{
    clientHeight.value = (window.innerHeight - 530) + 'px'
  })
})
</script>
<style scoped lang="scss">
.meeting {
  height: v-bind(clientHeight);
}
.result {
  .title {
    width: 400px;
  }
}
</style>