<template>
  <div class="meeting">
    <el-button type="primary" round plain @click="setOnAICompanionFeatureTurnOffByParticipantCB">SetOnAICompanionFeatureTurnOffByParticipantCB</el-button>
    <el-button type="primary" round plain @click="setOnAICompanionFeatureSwitchRequestedCB">SetOnAICompanionFeatureSwitchRequestedCB</el-button>
    <el-button type="primary" round plain @click="setOnAICompanionFeatureSwitchRequestResponseCB">SetOnAICompanionFeatureSwitchRequestResponseCB</el-button>
    <el-button type="primary" round plain @click="setOnAICompanionFeatureCanNotBeTurnedOffCB">SetOnAICompanionFeatureCanNotBeTurnedOffCB</el-button>
    <el-button type="primary" round plain @click="setOnSmartSummaryStateNotSupportedCB">SetOnSmartSummaryStateNotSupportedCB</el-button>
    <el-button type="primary" round plain @click="setOnSmartSummaryStateSupportedButDisabledCB">SetOnSmartSummaryStateSupportedButDisabledCB</el-button>
    <el-button type="primary" round plain @click="setOnSmartSummaryStateEnabledButNotStartedCB">SetOnSmartSummaryStateEnabledButNotStartedCB</el-button>
    <el-button type="primary" round plain @click="setOnSmartSummaryStateStartedCB">SetOnSmartSummaryStateStartedCB</el-button>
    <el-button type="primary" round plain @click="setOnFailedToStartSmartSummaryCB">SetOnFailedToStartSmartSummaryCB</el-button>
    <el-button type="primary" round plain @click="setOnSmartSummaryEnableRequestReceivedCB">SetOnSmartSummaryEnableRequestReceivedCB</el-button>
    <el-button type="primary" round plain @click="setOnSmartSummaryStartRequestReceivedCB">SetOnSmartSummaryStartRequestReceivedCB</el-button>
    <el-button type="primary" round plain @click="setOnSmartSummaryEnableActionCallbackCB">SetOnSmartSummaryEnableActionCallbackCB</el-button>
    <el-button type="primary" round plain @click="setOnQueryStateNotSupportedCB">SetOnQueryStateNotSupportedCB</el-button>
    <el-button type="primary" round plain @click="setOnQueryStateSupportedButDisabledCB">SetOnQueryStateSupportedButDisabledCB</el-button>
    <el-button type="primary" round plain @click="setOnQueryStateEnabledButNotStartedCB">SetOnQueryStateEnabledButNotStartedCB</el-button>
    <el-button type="primary" round plain @click="setOnQueryStateStartedCB">SetOnQueryStateStartedCB</el-button>
    <el-button type="primary" round plain @click="setOnQuerySettingChangedCB">SetOnQuerySettingChangedCB</el-button>
    <el-button type="primary" round plain @click="setOnFailedToStartQueryCB">SetOnFailedToStartQueryCB</el-button>
    <el-button type="primary" round plain @click="setOnReceiveRequestToEnableQueryCB">SetOnReceiveRequestToEnableQueryCB</el-button>
    <el-button type="primary" round plain @click="setOnReceiveRequestToStartQueryCB">SetOnReceiveRequestToStartQueryCB</el-button>
    <el-button type="primary" round plain @click="setOnReceiveQueryAnswerCB">SetOnReceiveQueryAnswerCB</el-button>
    <el-button type="primary" round plain @click="setOnQueryEnableActionCallback">SetOnQueryEnableActionCallback</el-button>
    <el-button type="primary" round plain @click="setOnSendQueryPrivilegeChangedCB">SetOnSendQueryPrivilegeChangedCB</el-button>
    <el-button type="primary" round plain @click="setOnFailedToRequestSendQueryCB">SetOnFailedToRequestSendQueryCB</el-button>
    <el-button type="primary" round plain @click="setOnReceiveRequestToSendQueryCB">SetOnReceiveRequestToSendQueryCB</el-button>
    <div class="in-line">
      <el-button type="primary" round plain @click="isTurnoffAllAICompanionsSupported">IsTurnoffAllAICompanionsSupported</el-button>
      <el-button type="primary" round plain @click="isTurnOnAllAICompanionsSupported">IsTurnOnAllAICompanionsSupported</el-button>
      <el-button type="primary" round plain @click="canTurnOffAllAICompanions">CanTurnOffAllAICompanions</el-button>
      <el-button type="primary" round plain @click="canTurnOnAllAICompanions">CanTurnOnAllAICompanions</el-button>
      <el-select v-model="deleteAssets" placeholder="Select" class="select-small">
        <el-option label="true" :value="true" />
        <el-option label="false" :value="false" />
      </el-select>
      <el-button type="primary" round plain @click="turnOffAllAICompanions">TurnOffAllAICompanions</el-button>
      <el-button type="primary" round plain @click="turnOnAllAICompanions">TurnOnAllAICompanions</el-button>
    </div>
    <div class="in-line">
      <el-button type="primary" round plain @click="canRequestTurnoffAllAICompanions">CanRequestTurnoffAllAICompanions</el-button>
      <el-button type="primary" round plain @click="canRequestTurnOnAllAICompanions">CanRequestTurnOnAllAICompanions</el-button>
      <el-button type="primary" round plain @click="requestTurnoffAllAICompanions">RequestTurnoffAllAICompanions</el-button>
      <el-button type="primary" round plain @click="requestTurnOnAllAICompanions">RequestTurnOnAllAICompanions</el-button>
    </div>
    <div class="in-line">
      <el-button type="primary" round plain @click="canChangeQuerySetting">CanChangeQuerySetting</el-button>
      <el-select v-model="setting" placeholder="Select">
        <el-option v-for="(item, key) in MeetingAICompanionQuerySettingOptions" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="changeQuerySettings">ChangeQuerySettings</el-button>
      <el-button type="primary" round plain @click="getSelectedQuerySetting">GetSelectedQuerySetting</el-button>
    </div>
    <div class="in-line">
      <el-button type="primary" round plain @click="isAICompanionQueryLegalNoticeAvailable">IsAICompanionQueryLegalNoticeAvailable</el-button>
      <el-button type="primary" round plain @click="getAICompanionQueryLegalNoticesPrompt">GetAICompanionQueryLegalNoticesPrompt</el-button>
      <el-button type="primary" round plain @click="getAICompanionQueryLegalNoticesExplained">GetAICompanionQueryLegalNoticesExplained</el-button>
    </div>
    <div class="in-line">
      <el-button type="primary" round plain @click="isAICompanionQueryPrivacyLegalNoticeAvailable">IsAICompanionQueryPrivacyLegalNoticeAvailable</el-button>
      <el-button type="primary" round plain @click="getAICompanionQueryPrivacyLegalNoticesPrompt">GetAICompanionQueryPrivacyLegalNoticesPrompt</el-button>
    </div>
  </div>
  <div class="result">Result: {{result}}</div>
  <div class="result"> 
    <div class="result-item"> 
      <span class="title">onAICompanionFeatureTurnOffByParticipant <el-button size="small" @click="onSmartSummaryStateNotSupported_count=0">Clear</el-button></span> 
      <el-button type="primary" size="small" round plain @click="getFeatureList">GetFeatureList</el-button>
      <el-button type="primary" size="small" round plain @click="getAssetsDeletedFeatureList">GetAssetsDeletedFeatureList</el-button>
      <el-button type="primary" size="small" round plain @click="turnOnAgain">TurnOnAgain</el-button>
      <el-button type="primary" size="small" round plain @click="agreeTurnOff">AgreeTurnOff</el-button>
      <span v-if="onAICompanionFeatureTurnOffByParticipant_count">updated {{onAICompanionFeatureTurnOffByParticipant_count}}</span> 
    </div>
    <div class="result-item"> 
      <span class="title">onSmartSummaryStateNotSupported <el-button size="small" @click="onSmartSummaryStateNotSupported_count=0">Clear</el-button></span> 
      <el-button type="primary" size="small" round plain @click="getRequestUserID">GetRequestUserID</el-button>
      <el-button type="primary" size="small" round plain @click="isTurnOn">IsTurnOn</el-button>
      <el-select v-model="deleteAssets" size="small" placeholder="Select" class="select-small">
        <el-option label="true" :value="true" />
        <el-option label="false" :value="false" />
      </el-select>
      <el-button type="primary" size="small" round plain @click="agree">Agree</el-button>
      <el-button type="primary" size="small" round plain @click="decline">Decline</el-button>
      <span v-if="onSmartSummaryStateNotSupported_count">updated {{onSmartSummaryStateNotSupported_count}}</span> 
    </div>
    <div class="result-item"> 
      <span class="title">onAICompanionFeatureSwitchRequestResponse <el-button size="small" @click="onAICompanionFeatureSwitchRequestResponse_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onAICompanionFeatureSwitchRequestResponse_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onAICompanionFeatureCanNotBeTurnedOff <el-button size="small" @click="onAICompanionFeatureCanNotBeTurnedOff_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onAICompanionFeatureCanNotBeTurnedOff_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onSmartSummaryStateSupportedButDisabled <el-button size="small" @click="onSmartSummaryStateSupportedButDisabled_count=0">Clear</el-button></span> 
      <el-button type="primary" size="small" round plain @click="enableSmartSummary">EnableSmartSummary</el-button>
      <el-button type="primary" size="small" round plain @click="isForRequestEnableSmartSummary">IsForRequest</el-button>
      <span v-if="onSmartSummaryStateSupportedButDisabled_count">updated {{onSmartSummaryStateSupportedButDisabled_count}}</span> 
    </div>
    <div class="result-item"> 
      <span class="title">onSmartSummaryStateEnabledButNotStarted <el-button size="small" @click="onSmartSummaryStateEnabledButNotStarted_count=0">Clear</el-button></span> 
      <el-button type="primary" size="small" round plain @click="startSmartSummary">StartSmartSummary</el-button>
      <el-button type="primary" size="small" round plain @click="isForRequestStartSmartSummary">IsForRequest</el-button>
      <span v-if="onSmartSummaryStateEnabledButNotStarted_count">updated {{onSmartSummaryStateEnabledButNotStarted_count}}</span> 
    </div>
    <div class="result-item"> 
      <span class="title">onSmartSummaryStateStarted <el-button size="small" @click="onSmartSummaryStateStarted_count=0">Clear</el-button></span> 
      <el-button type="primary" size="small" round plain @click="stopSmartSummary">StopSmartSummary</el-button>
      <span v-if="onSmartSummaryStateStarted_count">updated {{onSmartSummaryStateStarted_count}}</span> 
    </div>
    <div class="result-item"> 
      <span class="title">onFailedToStartSmartSummary <el-button size="small" @click="onFailedToStartSmartSummary_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onFailedToStartSmartSummary_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onSmartSummaryEnableRequestReceived <el-button size="small" @click="onSmartSummaryEnableRequestReceived_count=0">Clear</el-button></span> 
      <el-button type="primary" size="small" round plain @click="getSenderEnableSmartSummaryUserID">GetSenderUserID</el-button>
      <el-button type="primary" size="small" round plain @click="continueApproveEnableSmartSummary">ContinueApprove</el-button>
      <span v-if="onSmartSummaryEnableRequestReceived_count">updated {{onSmartSummaryEnableRequestReceived_count}}</span> 
    </div>
    <div class="result-item"> 
      <span class="title">onSmartSummaryStartRequestReceived <el-button size="small" @click="onSmartSummaryStartRequestReceived_count=0">Clear</el-button></span> 
      <el-button type="primary" size="small" round plain @click="getSenderStartSmartSummaryUserID">GetSenderUserID</el-button>
      <el-button type="primary" size="small" round plain @click="approveStartSmartSummary">Approve</el-button>
      <el-button type="primary" size="small" round plain @click="declineStartSmartSummary">Decline</el-button>
      <span v-if="onSmartSummaryStartRequestReceived_count">updated {{onSmartSummaryStartRequestReceived_count}}</span> 
    </div>
    <div class="result-item"> 
      <span class="title">onSmartSummaryEnableActionCallback <el-button size="small" @click="onSmartSummaryEnableActionCallback_count=0">Clear</el-button></span> 
      <el-button type="primary" size="small" round plain @click="getEnableSmartSummaryActionTipTitle">GetTipTitle</el-button>
      <el-button type="primary" size="small" round plain @click="getEnableSmartSummaryActionTipString">GetTipString</el-button>
      <el-button type="primary" size="small" round plain @click="confirmEnableSmartSummaryAction">Confirm</el-button>
      <el-button type="primary" size="small" round plain @click="cancelEnableSmartSummaryAction">Cancel</el-button>
      <span v-if="onSmartSummaryEnableActionCallback_count">updated {{onSmartSummaryEnableActionCallback_count}}</span> 
    </div>
    <div class="result-item"> 
      <span class="title">onQueryStateNotSupported <el-button size="small" @click="onQueryStateNotSupported_count=0">Clear</el-button></span> 
      <span v-if="onQueryStateNotSupported_count">updated {{onQueryStateNotSupported_count}}</span> 
    </div>
    <div class="result-item"> 
      <span class="title">onQueryStateSupportedButDisabled <el-button size="small" @click="onQueryStateSupportedButDisabled_count=0">Clear</el-button></span> 
      <el-button type="primary" size="small" round plain @click="enableQuery">EnableQuery</el-button>
      <el-button type="primary" size="small" round plain @click="isForRequestEnableQuery">IsForRequest</el-button>
      <span v-if="onQueryStateSupportedButDisabled_count">updated {{onQueryStateSupportedButDisabled_count}}</span> 
    </div>
    <div class="result-item"> 
      <span class="title">onQueryStateEnabledButNotStarted <el-button size="small" @click="onQueryStateEnabledButNotStarted_count=0">Clear</el-button></span> 
      <el-button type="primary" size="small" round plain @click="startMeetingQuery">StartMeetingQuery</el-button>
      <el-button type="primary" size="small" round plain @click="isForRequestStartMeetingQuery">IsForRequest</el-button>
      <span v-if="onQueryStateEnabledButNotStarted_count">updated {{onQueryStateEnabledButNotStarted_count}}</span> 
    </div>
    <div class="result-item"> 
      <span class="title">onQueryStateStarted <el-button size="small" @click="onQueryStateStarted_count=0">Clear</el-button></span> 
      <el-button type="primary" size="small" round plain @click="getDefaultQueryQuestions">GetDefaultQueryQuestions</el-button>
      <el-input v-model.trim="question" size="small" placeholder="input question" />
      <el-button type="primary" size="small" round plain @click="sendQueryQuestion">SendQueryQuestion</el-button>
      <el-button type="primary" size="small" round plain @click="stopMeetingQuery">StopMeetingQuery</el-button>
      <el-button type="primary" size="small" round plain @click="canSendQuery">CanSendQuery</el-button>
      <el-button type="primary" size="small" round plain @click="requestSendQueryPrivilege">RequestSendQueryPrivilege</el-button>
      <span v-if="onQueryStateStarted_count">updated {{onQueryStateStarted_count}}</span> 
    </div>
    <div class="result-item"> 
      <span class="title">onQuerySettingChanged <el-button size="small" @click="onQuerySettingChanged_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onQuerySettingChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onFailedToStartQuery <el-button size="small" @click="onFailedToStartQuery_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onFailedToStartQuery_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onReceiveRequestToEnableQuery <el-button size="small" @click="onReceiveRequestToEnableQuery_count=0">Clear</el-button></span> 
      <el-button type="primary" size="small" round plain @click="getSenderEnableQueryUserID">GetSenderUserID</el-button>
      <el-button type="primary" size="small" round plain @click="continueApproveEnableQuery">ContinueApprove</el-button>
      <span v-if="onReceiveRequestToEnableQuery_count">updated {{onReceiveRequestToEnableQuery_count}}</span> 
    </div>
    <div class="result-item"> 
      <span class="title">onReceiveRequestToStartQuery <el-button size="small" @click="onReceiveRequestToStartQuery_count=0">Clear</el-button></span> 
      <el-button type="primary" size="small" round plain @click="getSenderStartQueryUserID">GetSenderUserID</el-button>
      <el-button type="primary" size="small" round plain @click="approveStartQuery">Approve</el-button>
      <el-button type="primary" size="small" round plain @click="declineStartQuery">Decline</el-button>
      <span v-if="onReceiveRequestToStartQuery_count">updated {{onReceiveRequestToStartQuery_count}}</span> 
    </div>
    <div class="result-item"> 
      <span class="title">onReceiveQueryAnswer <el-button size="small" @click="onReceiveQueryAnswer_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onReceiveQueryAnswer_options" :key="index">{{index}} : {{item}}</span>
      </div>
      <el-select v-model="feedbackType" size="small" placeholder="select type">
        <el-option v-for="(item, key) in MeetingAICompanionQueryFeedbackType" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-button type="primary" size="small" round plain @click="aiCompanionQueryFeedback">Feedback</el-button>
    </div>
    <div class="result-item"> 
      <span class="title">onQueryEnableActionCallback <el-button size="small" @click="onQueryEnableActionCallback_count=0">Clear</el-button></span> 
      <el-button type="primary" size="small" round plain @click="getEnableQueryActionTipTitle">GetTipTitle</el-button>
      <el-button type="primary" size="small" round plain @click="getEnableQueryActionTipString">GetTipString</el-button>
      <el-button type="primary" size="small" round plain @click="confirmEnableQueryAction">Confirm</el-button>
      <el-button type="primary" size="small" round plain @click="cancelEnableQueryAction">Cancel</el-button>
      <span v-if="onQueryEnableActionCallback_count">updated {{onQueryEnableActionCallback_count}}</span> 
    </div>
    <div class="result-item"> 
      <span class="title">onSendQueryPrivilegeChanged <el-button size="small" @click="onSendQueryPrivilegeChanged_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onSendQueryPrivilegeChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onFailedToRequestSendQuery <el-button size="small" @click="onFailedToRequestSendQuery_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onFailedToRequestSendQuery_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onReceiveRequestToSendQuery <el-button size="small" @click="onReceiveRequestToSendQuery_count=0">Clear</el-button></span> 
      <el-button type="primary" size="small" round plain @click="getSenderQueryUserID">GetSenderUserID</el-button>
      <el-button type="primary" size="small" round plain @click="approveSendQuery">Approve</el-button>
      <el-select v-model="bDeclineAll" size="small" placeholder="Select" class="select-small">
        <el-option label="true" :value="true" />
        <el-option label="false" :value="false" />
      </el-select>
      <el-button type="primary" size="small" round plain @click="declineSendQuery">Decline</el-button>
      <span v-if="onReceiveRequestToSendQuery_count">updated {{onReceiveRequestToSendQuery_count}}</span> 
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import { MeetingAICompanionQuerySettingOptions, MeetingAICompanionQueryFeedbackType } from '../../../lib/settings.js'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const result = ref()
const onAICompanionFeatureTurnOffByParticipant_count = ref(0)
const onAICompanionFeatureSwitchRequested_count = ref(0)
const onAICompanionFeatureSwitchRequestResponse_options = ref<string[]>([])
const onAICompanionFeatureCanNotBeTurnedOff_options = ref<string[]>([])

const onSmartSummaryStateNotSupported_count = ref(0)
const onSmartSummaryStateSupportedButDisabled_count = ref(0)
const onSmartSummaryStateEnabledButNotStarted_count = ref(0)
const onSmartSummaryStateStarted_count = ref(0)
const onFailedToStartSmartSummary_options = ref<string[]>([])
const onSmartSummaryEnableRequestReceived_count = ref(0)
const onSmartSummaryStartRequestReceived_count = ref(0)
const onSmartSummaryEnableActionCallback_count = ref(0)

const onQueryStateNotSupported_count = ref(0)
const onQueryStateSupportedButDisabled_count = ref(0)
const onQueryStateEnabledButNotStarted_count = ref(0)
const onQueryStateStarted_count = ref(0)
const onQuerySettingChanged_options = ref<string[]>([])
const onFailedToStartQuery_options = ref<string[]>([])
const onReceiveRequestToEnableQuery_count = ref(0)
const onReceiveRequestToStartQuery_count = ref(0)
const onReceiveQueryAnswer_options = ref<string[]>([])
const onQueryEnableActionCallback_count = ref(0)
const onSendQueryPrivilegeChanged_options = ref<string[]>([])
const onFailedToRequestSendQuery_options = ref<string[]>([])
const onReceiveRequestToSendQuery_count = ref(0)

const onAICompanionFeatureTurnOffByParticipant = () => {
  console.log('onAICompanionFeatureTurnOffByParticipant')
  onAICompanionFeatureTurnOffByParticipant_count.value++
}
const onAICompanionFeatureSwitchRequested = () => {
  console.log('onAICompanionFeatureSwitchRequested')
  onAICompanionFeatureSwitchRequested_count.value++
}
const onAICompanionFeatureSwitchRequestResponse = (bTimeout: boolean, bAgree: boolean, bTurnOn: boolean) => {
  console.log('onAICompanionFeatureSwitchRequestResponse', bTimeout, bAgree, bTurnOn)
  onAICompanionFeatureSwitchRequestResponse_options.value.push(`bTimeout: ${bTimeout}, bAgree: ${bAgree}, bTurnOn: ${bTurnOn}`)
}
const onAICompanionFeatureCanNotBeTurnedOff = (features: any) => {
  console.log('onAICompanionFeatureCanNotBeTurnedOff', features)
  onAICompanionFeatureCanNotBeTurnedOff_options.value.push(`features: ${JSON.stringify(features)}`)
}
const onSmartSummaryStateNotSupported = () => {
  console.log('onSmartSummaryStateNotSupported')
  onSmartSummaryStateNotSupported_count.value++
}
const onSmartSummaryStateSupportedButDisabled = () => {
  console.log('onSmartSummaryStateSupportedButDisabled')
  onSmartSummaryStateSupportedButDisabled_count.value++
}
const onSmartSummaryStateEnabledButNotStarted = () => {
  console.log('onSmartSummaryStateEnabledButNotStarted')
  onSmartSummaryStateEnabledButNotStarted_count.value++
}
const onSmartSummaryStateStarted = () => {
  console.log('onSmartSummaryStateStarted')
  onSmartSummaryStateStarted_count.value++
}
const onFailedToStartSmartSummary = (timeout: boolean) => {
  console.log('onFailedToStartSmartSummary', timeout)
  onFailedToStartSmartSummary_options.value.push(`timeout: ${timeout}`)
}
const onSmartSummaryEnableRequestReceived = () => {
  console.log('onSmartSummaryEnableRequestReceived')
  onSmartSummaryEnableRequestReceived_count.value++
}
const onSmartSummaryStartRequestReceived = () => {
  console.log('onSmartSummaryStartRequestReceived')
  onSmartSummaryStartRequestReceived_count.value++
}
const onSmartSummaryEnableActionCallback = () => {
  console.log('onSmartSummaryEnableActionCallback')
  onSmartSummaryEnableActionCallback_count.value++
}
const onQueryStateNotSupported = () => {
  console.log('onQueryStateNotSupported')
  onQueryStateNotSupported_count.value++
}
const onQueryStateSupportedButDisabled = () => {
  console.log('onQueryStateSupportedButDisabled')
  onQueryStateSupportedButDisabled_count.value++
}
const onQueryStateEnabledButNotStarted = () => {
  console.log('onQueryStateEnabledButNotStarted')
  onQueryStateEnabledButNotStarted_count.value++
}
const onQueryStateStarted = () => {
  console.log('onQueryStateStarted')
  onQueryStateStarted_count.value++
}
const onQuerySettingChanged = (eSetting: boolean) => {
  console.log('onQuerySettingChanged', eSetting)
  onQuerySettingChanged_options.value.push(`eSetting: ${eSetting}`)
}
const onFailedToStartQuery = (bTimeout: boolean) => {
  console.log('onFailedToStartQuery', bTimeout)
  onFailedToStartQuery_options.value.push(`bTimeout: ${bTimeout}`)
}
const onReceiveRequestToEnableQuery = () => {
  console.log('onReceiveRequestToEnableQuery')
  onReceiveRequestToEnableQuery_count.value++
}
const onReceiveRequestToStartQuery = () => {
  console.log('onReceiveRequestToStartQuery')
  onReceiveRequestToStartQuery_count.value++
}
const onReceiveQueryAnswer = (queryItem: any) => {
  console.log('onReceiveQueryAnswer', queryItem)
  onReceiveQueryAnswer_options.value.push(JSON.stringify(queryItem))
}
const onQueryEnableActionCallback = () => {
  console.log('onQueryEnableActionCallback')
  onQueryEnableActionCallback_count.value++
}
const onSendQueryPrivilegeChanged = (canSendQuery: boolean) => {
  console.log('onSendQueryPrivilegeChanged', canSendQuery)
  onSendQueryPrivilegeChanged_options.value.push(`canSendQuery: ${canSendQuery}`)
}
const onFailedToRequestSendQuery = (bTimeout: boolean) => {
  console.log('onFailedToRequestSendQuery', bTimeout)
  onFailedToRequestSendQuery_options.value.push(`bTimeout: ${bTimeout}`)
}
const onReceiveRequestToSendQuery = () => {
  console.log('onReceiveRequestToSendQuery')
  onReceiveRequestToSendQuery_count.value++
}
const zoomMeetingAICompanion = remote.app.zoomSdkModule.Meeting.GetMeetingAICompanion({ onAICompanionFeatureTurnOffByParticipant, onAICompanionFeatureSwitchRequested, 
  onAICompanionFeatureSwitchRequestResponse, onAICompanionFeatureCanNotBeTurnedOff, onSmartSummaryStateNotSupported, onSmartSummaryStateSupportedButDisabled, onSmartSummaryStateEnabledButNotStarted,
  onSmartSummaryStateStarted, onFailedToStartSmartSummary, onSmartSummaryEnableRequestReceived, onSmartSummaryStartRequestReceived, onSmartSummaryEnableActionCallback,
  onQueryStateNotSupported, onQueryStateSupportedButDisabled, onQueryStateEnabledButNotStarted, onQueryStateStarted, 
  onQuerySettingChanged, onFailedToStartQuery, onReceiveRequestToEnableQuery, onReceiveRequestToStartQuery, onReceiveQueryAnswer, onQueryEnableActionCallback,
  onSendQueryPrivilegeChanged, onFailedToRequestSendQuery, onReceiveRequestToSendQuery })

const setOnAICompanionFeatureTurnOffByParticipantCB = () => {
  let ret = zoomMeetingAICompanion.SetOnAICompanionFeatureTurnOffByParticipantCB(onAICompanionFeatureTurnOffByParticipant)
  result.value = getSDKResult('SetOnAICompanionFeatureTurnOffByParticipantCB', ret)
}

const setOnAICompanionFeatureSwitchRequestedCB = () => {
  let ret = zoomMeetingAICompanion.SetOnAICompanionFeatureSwitchRequestedCB(onAICompanionFeatureSwitchRequested)
  result.value = getSDKResult('SetOnAICompanionFeatureSwitchRequestedCB', ret)
}

const setOnAICompanionFeatureSwitchRequestResponseCB = () => {
  let ret = zoomMeetingAICompanion.SetOnAICompanionFeatureSwitchRequestResponseCB(onAICompanionFeatureSwitchRequestResponse)
  result.value = getSDKResult('SetOnAICompanionFeatureSwitchRequestResponseCB', ret)
}

const setOnAICompanionFeatureCanNotBeTurnedOffCB = () => {
  let ret = zoomMeetingAICompanion.SetOnAICompanionFeatureCanNotBeTurnedOffCB(onAICompanionFeatureCanNotBeTurnedOff)
  result.value = getSDKResult('SetOnAICompanionFeatureCanNotBeTurnedOffCB', ret)
}

const setOnSmartSummaryStateNotSupportedCB = () => {
  let ret = zoomMeetingAICompanion.SetOnSmartSummaryStateNotSupportedCB(onSmartSummaryStateNotSupported)
  result.value = getSDKResult('SetOnSmartSummaryStateNotSupportedCB', ret)
}

const setOnSmartSummaryStateSupportedButDisabledCB = () => {
  let ret = zoomMeetingAICompanion.SetOnSmartSummaryStateSupportedButDisabledCB(onSmartSummaryStateSupportedButDisabled)
  result.value = getSDKResult('SetOnSmartSummaryStateSupportedButDisabledCB', ret)
}

const setOnSmartSummaryStateEnabledButNotStartedCB = () => {
  let ret = zoomMeetingAICompanion.SetOnSmartSummaryStateEnabledButNotStartedCB(onSmartSummaryStateEnabledButNotStarted)
  result.value = getSDKResult('SetOnSmartSummaryStateEnabledButNotStartedCB', ret)
}

const setOnSmartSummaryStateStartedCB = () => {
  let ret = zoomMeetingAICompanion.SetOnSmartSummaryStateStartedCB(onSmartSummaryStateStarted)
  result.value = getSDKResult('SetOnSmartSummaryStateStartedCB', ret)
}

const setOnFailedToStartSmartSummaryCB = () => {
  let ret = zoomMeetingAICompanion.SetOnFailedToStartSmartSummaryCB(onFailedToStartSmartSummary)
  result.value = getSDKResult('SetOnFailedToStartSmartSummaryCB', ret)
}

const setOnSmartSummaryEnableRequestReceivedCB = () => {
  let ret = zoomMeetingAICompanion.SetOnSmartSummaryEnableRequestReceivedCB(onSmartSummaryEnableRequestReceived)
  result.value = getSDKResult('SetOnSmartSummaryEnableRequestReceivedCB', ret)
}

const setOnSmartSummaryStartRequestReceivedCB = () => {
  let ret = zoomMeetingAICompanion.SetOnSmartSummaryStartRequestReceivedCB(onSmartSummaryStartRequestReceived)
  result.value = getSDKResult('SetOnSmartSummaryStartRequestReceivedCB', ret)
}

const setOnSmartSummaryEnableActionCallbackCB = () => {
  let ret = zoomMeetingAICompanion.SetOnSmartSummaryEnableActionCallbackCB(onSmartSummaryEnableActionCallback)
  result.value = getSDKResult('SetOnSmartSummaryEnableActionCallbackCB', ret)
}

const setOnQueryStateNotSupportedCB = () => {
  let ret = zoomMeetingAICompanion.SetOnQueryStateNotSupportedCB(onQueryStateNotSupported)
  result.value = getSDKResult('SetOnQueryStateNotSupportedCB', ret)
}

const setOnQueryStateSupportedButDisabledCB = () => {
  let ret = zoomMeetingAICompanion.SetOnQueryStateSupportedButDisabledCB(onQueryStateSupportedButDisabled)
  result.value = getSDKResult('SetOnQueryStateSupportedButDisabledCB', ret)
}

const setOnQueryStateEnabledButNotStartedCB = () => {
  let ret = zoomMeetingAICompanion.SetOnQueryStateEnabledButNotStartedCB(onQueryStateEnabledButNotStarted)
  result.value = getSDKResult('SetOnQueryStateEnabledButNotStartedCB', ret)
}

const setOnQueryStateStartedCB = () => {
  let ret = zoomMeetingAICompanion.SetOnQueryStateStartedCB(onQueryStateStarted)
  result.value = getSDKResult('SetOnQueryStateStartedCB', ret)
}

const setOnQuerySettingChangedCB = () => {
  let ret = zoomMeetingAICompanion.SetOnQuerySettingChangedCB(onQuerySettingChanged)
  result.value = getSDKResult('SetOnQuerySettingChangedCB', ret)
}

const setOnFailedToStartQueryCB = () => {
  let ret = zoomMeetingAICompanion.SetOnFailedToStartQueryCB(onFailedToStartQuery)
  result.value = getSDKResult('SetOnFailedToStartQueryCB', ret)
}

const setOnReceiveRequestToEnableQueryCB = () => {
  let ret = zoomMeetingAICompanion.SetOnReceiveRequestToEnableQueryCB(onReceiveRequestToEnableQuery)
  result.value = getSDKResult('SetOnReceiveRequestToEnableQueryCB', ret)
}

const setOnReceiveRequestToStartQueryCB = () => {
  let ret = zoomMeetingAICompanion.SetOnReceiveRequestToStartQueryCB(onReceiveRequestToStartQuery)
  result.value = getSDKResult('SetOnReceiveRequestToStartQueryCB', ret)
}

const setOnReceiveQueryAnswerCB = () => {
  let ret = zoomMeetingAICompanion.SetOnReceiveQueryAnswerCB(onReceiveQueryAnswer)
  result.value = getSDKResult('SetOnReceiveQueryAnswerCB', ret)
}

const setOnQueryEnableActionCallback = () => {
  let ret = zoomMeetingAICompanion.SetOnQueryEnableActionCallback(onQueryEnableActionCallback)
  result.value = getSDKResult('SetOnQueryEnableActionCallback', ret)
}

const setOnSendQueryPrivilegeChangedCB = () => {
  let ret = zoomMeetingAICompanion.SetOnSendQueryPrivilegeChangedCB(onSendQueryPrivilegeChanged)
  result.value = getSDKResult('SetOnSendQueryPrivilegeChangedCB', ret)
}

const setOnFailedToRequestSendQueryCB = () => {
  let ret = zoomMeetingAICompanion.SetOnFailedToRequestSendQueryCB(onFailedToRequestSendQuery)
  result.value = getSDKResult('SetOnFailedToRequestSendQueryCB', ret)
}

const setOnReceiveRequestToSendQueryCB = () => {
  let ret = zoomMeetingAICompanion.SetOnReceiveRequestToSendQueryCB(onReceiveRequestToSendQuery)
  result.value = getSDKResult('SetOnReceiveRequestToSendQueryCB', ret)
}

const isTurnoffAllAICompanionsSupported = () => {
  let ret = zoomMeetingAICompanion.IsTurnoffAllAICompanionsSupported()
  result.value = getSDKResult('IsTurnoffAllAICompanionsSupported', ret)
}

const isTurnOnAllAICompanionsSupported = () => {
  let ret = zoomMeetingAICompanion.IsTurnOnAllAICompanionsSupported()
  result.value = getSDKResult('IsTurnOnAllAICompanionsSupported', ret)
}

const canTurnOffAllAICompanions = () => {
  let ret = zoomMeetingAICompanion.CanTurnOffAllAICompanions()
  result.value = getSDKResult('CanTurnOffAllAICompanions', ret)
}

const canTurnOnAllAICompanions = () => {
  let ret = zoomMeetingAICompanion.CanTurnOnAllAICompanions()
  result.value = getSDKResult('CanTurnOnAllAICompanions', ret)
}

const deleteAssets = ref(false)
const turnOffAllAICompanions = () => {
  let param = { deleteAssets: deleteAssets.value }
  let ret = zoomMeetingAICompanion.TurnOffAllAICompanions(param)
  result.value = getSDKResult('TurnOffAllAICompanions', ret)
}

const turnOnAllAICompanions = () => {
  let ret = zoomMeetingAICompanion.TurnOnAllAICompanions()
  result.value = getSDKResult('TurnOnAllAICompanions', ret)
}

const canRequestTurnoffAllAICompanions = () => {
  let ret = zoomMeetingAICompanion.CanRequestTurnoffAllAICompanions()
  result.value = getSDKResult('CanRequestTurnoffAllAICompanions', ret)
}

const canRequestTurnOnAllAICompanions = () => {
  let ret = zoomMeetingAICompanion.CanRequestTurnOnAllAICompanions()
  result.value = getSDKResult('CanRequestTurnOnAllAICompanions', ret)
}

const requestTurnoffAllAICompanions = () => {
  let ret = zoomMeetingAICompanion.RequestTurnoffAllAICompanions()
  result.value = getSDKResult('RequestTurnoffAllAICompanions', ret)
}

const requestTurnOnAllAICompanions = () => {
  let ret = zoomMeetingAICompanion.RequestTurnOnAllAICompanions()
  result.value = getSDKResult('RequestTurnOnAllAICompanions', ret)
}

const getFeatureList = () => {
  let ret = zoomMeetingAICompanion.GetFeatureList()
  result.value = getSDKResult('GetFeatureList', ret)
}

const getAssetsDeletedFeatureList = () => {
  let ret = zoomMeetingAICompanion.GetAssetsDeletedFeatureList()
  result.value = getSDKResult('GetAssetsDeletedFeatureList', ret)
}

const turnOnAgain = () => {
  let ret = zoomMeetingAICompanion.TurnOnAgain()
  result.value = getSDKResult('TurnOnAgain', ret)
}

const agreeTurnOff = () => {
  let ret = zoomMeetingAICompanion.AgreeTurnOff()
  result.value = getSDKResult('AgreeTurnOff', ret)
}

const getRequestUserID = () => {
  let ret = zoomMeetingAICompanion.GetRequestUserID()
  result.value = getSDKResult('GetRequestUserID', ret, null, false)
}

const isTurnOn = () => {
  let ret = zoomMeetingAICompanion.IsTurnOn()
  result.value = getSDKResult('IsTurnOn', ret)
}

const agree = () => {
  let param = { deleteAssets: deleteAssets.value }
  let ret = zoomMeetingAICompanion.Agree(param)
  result.value = getSDKResult('Agree', ret)
}

const decline = () => {
  let ret = zoomMeetingAICompanion.Decline()
  result.value = getSDKResult('Decline', ret)
}

const canChangeQuerySetting = () => {
  let ret = zoomMeetingAICompanion.CanChangeQuerySetting()
  result.value = getSDKResult('CanChangeQuerySetting', ret)
}

const setting = ref(MeetingAICompanionQuerySettingOptions.MeetingAICompanionQuerySettingOptions_None)
const changeQuerySettings = () => {
  let param = { setting: setting.value }
  let ret = zoomMeetingAICompanion.ChangeQuerySettings(param)
  result.value = getSDKResult('ChangeQuerySettings', ret, param)
}

const getSelectedQuerySetting = () => {
  let ret = zoomMeetingAICompanion.GetSelectedQuerySetting()
  result.value = getSDKResult('GetSelectedQuerySetting', ret, null, false)
}

const isAICompanionQueryLegalNoticeAvailable = () => {
  let ret = zoomMeetingAICompanion.IsAICompanionQueryLegalNoticeAvailable()
  result.value = getSDKResult('IsAICompanionQueryLegalNoticeAvailable', ret)
}

const getAICompanionQueryLegalNoticesPrompt = () => {
  let ret = zoomMeetingAICompanion.GetAICompanionQueryLegalNoticesPrompt()
  result.value = getSDKResult('GetAICompanionQueryLegalNoticesPrompt', ret)
}

const getAICompanionQueryLegalNoticesExplained = () => {
  let ret = zoomMeetingAICompanion.GetAICompanionQueryLegalNoticesExplained()
  result.value = getSDKResult('GetAICompanionQueryLegalNoticesExplained', ret)
}

const isAICompanionQueryPrivacyLegalNoticeAvailable = () => {
  let ret = zoomMeetingAICompanion.IsAICompanionQueryPrivacyLegalNoticeAvailable()
  result.value = getSDKResult('IsAICompanionQueryPrivacyLegalNoticeAvailable', ret)
}

const getAICompanionQueryPrivacyLegalNoticesPrompt = () => {
  let ret = zoomMeetingAICompanion.GetAICompanionQueryPrivacyLegalNoticesPrompt()
  result.value = getSDKResult('GetAICompanionQueryPrivacyLegalNoticesPrompt', ret)
}

const enableSmartSummary = () => {
  let ret = zoomMeetingAICompanion.EnableSmartSummary()
  result.value = getSDKResult('EnableSmartSummary', ret)
}

const isForRequestEnableSmartSummary = () => {
  let ret = zoomMeetingAICompanion.IsForRequestEnableSmartSummary()
  result.value = getSDKResult('IsForRequestEnableSmartSummary', ret)
}

const startSmartSummary = () => {
  let ret = zoomMeetingAICompanion.StartSmartSummary()
  result.value = getSDKResult('StartSmartSummary', ret)
}

const isForRequestStartSmartSummary = () => {
  let ret = zoomMeetingAICompanion.IsForRequestStartSmartSummary()
  result.value = getSDKResult('IsForRequestStartSmartSummary', ret)
}

const stopSmartSummary = () => {
  let ret = zoomMeetingAICompanion.StopSmartSummary()
  result.value = getSDKResult('StopSmartSummary', ret)
}

const getSenderEnableSmartSummaryUserID = () => {
  let ret = zoomMeetingAICompanion.GetSenderEnableSmartSummaryUserID()
  result.value = getSDKResult('GetSenderEnableSmartSummaryUserID', ret, null, false)
}

const continueApproveEnableSmartSummary = () => {
  let ret = zoomMeetingAICompanion.ContinueApproveEnableSmartSummary()
  result.value = getSDKResult('ContinueApproveEnableSmartSummary', ret)
}

const getSenderStartSmartSummaryUserID = () => {
  let ret = zoomMeetingAICompanion.GetSenderStartSmartSummaryUserID()
  result.value = getSDKResult('GetSenderStartSmartSummaryUserID', ret, null, false)
}

const approveStartSmartSummary = () => {
  let ret = zoomMeetingAICompanion.ApproveStartSmartSummary()
  result.value = getSDKResult('ApproveStartSmartSummary', ret)
}

const declineStartSmartSummary = () => {
  let ret = zoomMeetingAICompanion.DeclineStartSmartSummary()
  result.value = getSDKResult('DeclineStartSmartSummary', ret)
}

const getEnableSmartSummaryActionTipTitle = () => {
  let ret = zoomMeetingAICompanion.GetEnableSmartSummaryActionTipTitle()
  result.value = getSDKResult('GetEnableSmartSummaryActionTipTitle', ret)
}

const getEnableSmartSummaryActionTipString = () => {
  let ret = zoomMeetingAICompanion.GetEnableSmartSummaryActionTipString()
  result.value = getSDKResult('GetEnableSmartSummaryActionTipString', ret)
}

const confirmEnableSmartSummaryAction = () => {
  let ret = zoomMeetingAICompanion.ConfirmEnableSmartSummaryAction()
  result.value = getSDKResult('ConfirmEnableSmartSummaryAction', ret)
}

const cancelEnableSmartSummaryAction = () => {
  let ret = zoomMeetingAICompanion.CancelEnableSmartSummaryAction()
  result.value = getSDKResult('CancelEnableSmartSummaryAction', ret)
}

const enableQuery = () => {
  let ret = zoomMeetingAICompanion.EnableQuery()
  result.value = getSDKResult('EnableQuery', ret)
}

const isForRequestEnableQuery = () => {
  let ret = zoomMeetingAICompanion.IsForRequestEnableQuery()
  result.value = getSDKResult('IsForRequestEnableQuery', ret)
}

const startMeetingQuery = () => {
  let ret = zoomMeetingAICompanion.StartMeetingQuery()
  result.value = getSDKResult('StartMeetingQuery', ret)
}

const isForRequestStartMeetingQuery = () => {
  let ret = zoomMeetingAICompanion.IsForRequestStartMeetingQuery()
  result.value = getSDKResult('IsForRequestStartMeetingQuery', ret)
}

const getDefaultQueryQuestions = () => {
  let ret = zoomMeetingAICompanion.GetDefaultQueryQuestions()
  result.value = getSDKResult('GetDefaultQueryQuestions', ret)
}

const question = ref('')
const sendQueryQuestion = () => {
  let param = { question: question.value }
  let ret = zoomMeetingAICompanion.SendQueryQuestion(param)
  result.value = getSDKResult('SendQueryQuestion', ret, param)
}

const stopMeetingQuery = () => {
  let ret = zoomMeetingAICompanion.StopMeetingQuery()
  result.value = getSDKResult('StopMeetingQuery', ret)
}

const canSendQuery = () => {
  let ret = zoomMeetingAICompanion.CanSendQuery()
  result.value = getSDKResult('CanSendQuery', ret)
}

const requestSendQueryPrivilege = () => {
  let ret = zoomMeetingAICompanion.RequestSendQueryPrivilege()
  result.value = getSDKResult('RequestSendQueryPrivilege', ret)
}

const getSenderEnableQueryUserID = () => {
  let ret = zoomMeetingAICompanion.GetSenderEnableQueryUserID()
  result.value = getSDKResult('GetSenderEnableQueryUserID', ret, null, false)
}

const continueApproveEnableQuery = () => {
  let ret = zoomMeetingAICompanion.ContinueApproveEnableQuery()
  result.value = getSDKResult('ContinueApproveEnableQuery', ret)
}

const getSenderStartQueryUserID = () => {
  let ret = zoomMeetingAICompanion.GetSenderStartQueryUserID()
  result.value = getSDKResult('GetSenderStartQueryUserID', ret, null, false)
}

const approveStartQuery = () => {
  let ret = zoomMeetingAICompanion.ApproveStartQuery()
  result.value = getSDKResult('ApproveStartQuery', ret)
}

const declineStartQuery = () => {
  let ret = zoomMeetingAICompanion.DeclineStartQuery()
  result.value = getSDKResult('DeclineStartQuery', ret)
}

const feedbackType = ref(MeetingAICompanionQueryFeedbackType.MeetingAICompanionQueryFeedbackType_None)
const aiCompanionQueryFeedback = () => {
  let param = { feedbackType: feedbackType.value }
  let ret = zoomMeetingAICompanion.AICompanionQueryFeedback(param)
  result.value = getSDKResult('AICompanionQueryFeedback', ret, param)
}

const getEnableQueryActionTipTitle = () => {
  let ret = zoomMeetingAICompanion.GetEnableQueryActionTipTitle()
  result.value = getSDKResult('GetEnableQueryActionTipTitle', ret)
}

const getEnableQueryActionTipString = () => {
  let ret = zoomMeetingAICompanion.GetEnableQueryActionTipString()
  result.value = getSDKResult('GetEnableQueryActionTipString', ret)
}

const confirmEnableQueryAction = () => {
  let ret = zoomMeetingAICompanion.ConfirmEnableQueryAction()
  result.value = getSDKResult('ConfirmEnableQueryAction', ret)
}

const cancelEnableQueryAction = () => {
  let ret = zoomMeetingAICompanion.CancelEnableQueryAction()
  result.value = getSDKResult('CancelEnableQueryAction', ret)
}

const getSenderQueryUserID = () => {
  let ret = zoomMeetingAICompanion.GetSenderQueryUserID()
  result.value = getSDKResult('GetSenderQueryUserID', ret, null, false)
}

const approveSendQuery = () => {
  let ret = zoomMeetingAICompanion.ApproveSendQuery()
  result.value = getSDKResult('ApproveSendQuery', ret)
}

const bDeclineAll = ref(false)
const declineSendQuery = () => {
  let param = { bDeclineAll: bDeclineAll.value }
  let ret = zoomMeetingAICompanion.DeclineSendQuery(param)
  result.value = getSDKResult('DeclineSendQuery', ret)
}
</script>

<style scoped lang="scss">
.result {
  .title {
    width: 365px;
  }
  .el-select {
    width: 300px;
  }
}
.select-small {
  width: 90px !important;
}
</style>