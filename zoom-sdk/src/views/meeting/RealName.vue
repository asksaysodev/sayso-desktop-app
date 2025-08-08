<template>
  <div class="meeting">
    <el-button type="primary" round plain @click="setNeedRealNameAuthMeetingNotificationCB">SetNeedRealNameAuthMeetingNotificationCB</el-button>
    <el-button type="primary" round plain @click="setRetrieveSMSVerificationCodeResultNotificationCB">SetRetrieveSMSVerificationCodeResultNotificationCB</el-button>
    <el-button type="primary" round plain @click="setVerifySMSVerificationCodeResultNotificationCB">SetVerifySMSVerificationCodeResultNotificationCB</el-button>
    <el-button type="primary" round plain @click="getResendSMSVerificationCodeHandler">GetResendSMSVerificationCodeHandler</el-button>
    <el-checkbox v-model="enable" label="EnableZoomAuthRealNameMeetingUIShown" @change="enableZoomAuthRealNameMeetingUIShown"></el-checkbox>
    <div class="in-line">
      <el-select v-model="country_code" placeholder="Select" @focus="getSupportPhoneNumberCountryList">
        <el-option v-for="(item, index) in supportPhoneNumberCountryList" :key="index" :label="item.country_code" :value="item.country_code"></el-option>
      </el-select>
      <el-input v-model.trim="phone_number" placeholder="input phone number" @keyup.enter.native="retrieve" />
      <el-button type="primary" round plain @click="retrieve">Retrieve</el-button>
    </div>
    <el-button type="primary" round plain @click="retrieveCancelAndLeaveMeeting">Retrieve_CancelAndLeaveMeeting</el-button>
    <el-button type="primary" round plain @click="getReVerifySMSVerificationCodeHandler">GetReVerifySMSVerificationCodeHandler</el-button>
    <div class="in-line">
      <el-select v-model="country_code" placeholder="Select" @focus="getSupportPhoneNumberCountryList">
        <el-option v-for="(item, index) in supportPhoneNumberCountryList" :key="index" :label="item.country_code" :value="item.country_code"></el-option>
      </el-select>
      <el-input v-model.trim="phone_number" placeholder="input phone number" />
      <el-input v-model.trim="verification_code" placeholder="input verification code" @keyup.enter.native="verify" />
      <el-button type="primary" round plain @click="verify">Verify</el-button>
    </div>
    <el-button type="primary" round plain @click="verifyCancelAndLeaveMeeting">Verify_CancelAndLeaveMeeting</el-button>
    <div class="in-line">
      <el-select v-model="country_code" placeholder="Select" @focus="getSupportPhoneNumberCountryList">
        <el-option v-for="(item, index) in supportPhoneNumberCountryList" :key="index" :label="item.country_code" :value="item.country_code"></el-option>
      </el-select>
      <el-input v-model.trim="phone_number" placeholder="input phone number" @keyup.enter.native="setDefaultCellPhoneInfo" />
      <el-button type="primary" round plain @click="setDefaultCellPhoneInfo">SetDefaultCellPhoneInfo</el-button>
    </div>
  </div>
  <div class="result">Result: {{result}}</div>
  <div class="result"> 
    <div class="result-item"> 
      <span class="title">onNeedRealNameAuthMeetingNotification <el-button size="small" @click="onNeedRealNameAuthMeetingNotification_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onNeedRealNameAuthMeetingNotification_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onRetrieveSMSVerificationCodeResultNotification <el-button size="small" @click="onRetrieveSMSVerificationCodeResultNotification_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onRetrieveSMSVerificationCodeResultNotification_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onVerifySMSVerificationCodeResultNotification <el-button size="small" @click="onVerifySMSVerificationCodeResultNotification_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onVerifySMSVerificationCodeResultNotification_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const result = ref()
const onNeedRealNameAuthMeetingNotification_options = ref<any[]>([])
const onRetrieveSMSVerificationCodeResultNotification_options = ref<any[]>([])
const onVerifySMSVerificationCodeResultNotification_options = ref<any[]>([])

const onNeedRealNameAuthMeetingNotification = (support_country_list: any, privacy_url: string) => {
  console.log('onNeedRealNameAuthMeetingNotification', support_country_list, privacy_url)
  let content = `${JSON.stringify(support_country_list)}-${privacy_url}`
  onNeedRealNameAuthMeetingNotification_options.value.push(content)
}
const onRetrieveSMSVerificationCodeResultNotification = (result: string) => {
  console.log('onRetrieveSMSVerificationCodeResultNotification', result)
  onRetrieveSMSVerificationCodeResultNotification_options.value.push(result)
}
const onVerifySMSVerificationCodeResultNotification = (result: string) => {
  console.log('onVerifySMSVerificationCodeResultNotification', result)
  onVerifySMSVerificationCodeResultNotification_options.value.push(result)
}
const zoomSMS = remote.app.zoomSdk.SMSHelper({onNeedRealNameAuthMeetingNotification, onRetrieveSMSVerificationCodeResultNotification, onVerifySMSVerificationCodeResultNotification})
const supportPhoneNumberCountryList = ref<any[]>([])
const country_code = ref()
const enable = ref(true)
const phone_number = ref('')
const verification_code = ref('')

const setNeedRealNameAuthMeetingNotificationCB = () => {
  let ret = zoomSMS.SetNeedRealNameAuthMeetingNotificationCB(onNeedRealNameAuthMeetingNotification)
  result.value = getSDKResult('SetNeedRealNameAuthMeetingNotificationCB', ret)
}

const setRetrieveSMSVerificationCodeResultNotificationCB = () => {
  let ret = zoomSMS.SetRetrieveSMSVerificationCodeResultNotificationCB(onRetrieveSMSVerificationCodeResultNotification)
  result.value = getSDKResult('SetRetrieveSMSVerificationCodeResultNotificationCB', ret)
}

const setVerifySMSVerificationCodeResultNotificationCB = () => {
  let ret = zoomSMS.SetVerifySMSVerificationCodeResultNotificationCB(onVerifySMSVerificationCodeResultNotification)
  result.value = getSDKResult('SetVerifySMSVerificationCodeResultNotificationCB', ret)
}

const getSupportPhoneNumberCountryList = () => {
  supportPhoneNumberCountryList.value = zoomSMS.GetSupportPhoneNumberCountryList()
  result.value = getSDKResult('GetSupportPhoneNumberCountryList', supportPhoneNumberCountryList.value)
  if (supportPhoneNumberCountryList.value.length > 0) {
    country_code.value = supportPhoneNumberCountryList.value[0].country_code
  }
}

const enableZoomAuthRealNameMeetingUIShown = () => {
  let param = {b_enable: enable.value}
  let ret = zoomSMS.EnableZoomAuthRealNameMeetingUIShown(param)
  result.value = getSDKResult('EnableZoomAuthRealNameMeetingUIShown', ret, param)
}

const getResendSMSVerificationCodeHandler = () => {
  let ret = zoomSMS.GetResendSMSVerificationCodeHandler()
  result.value = getSDKResult('GetResendSMSVerificationCodeHandler', ret)
}

const retrieve = () => {
  let param = {country_code: country_code.value, phone_number: phone_number.value}
  let ret = zoomSMS.Retrieve(param)
  result.value = getSDKResult('Retrieve', ret, param)
}

const retrieveCancelAndLeaveMeeting = () => {
  let ret = zoomSMS.Retrieve_CancelAndLeaveMeeting()
  result.value = getSDKResult('Retrieve_CancelAndLeaveMeeting', ret)
}

const getReVerifySMSVerificationCodeHandler = () => {
  let ret = zoomSMS.GetReVerifySMSVerificationCodeHandler()
  result.value = getSDKResult('GetReVerifySMSVerificationCodeHandler', ret)
}

const verify = () => {
  let param = {country_code: country_code.value, phone_number: phone_number.value, verification_code: verification_code.value}
  let ret = zoomSMS.Verify(param)
  result.value = getSDKResult('Verify', ret, param)
}

const verifyCancelAndLeaveMeeting = () => {
  let ret = zoomSMS.Verify_CancelAndLeaveMeeting()
  result.value = getSDKResult('Verify_CancelAndLeaveMeeting', ret)
}

const setDefaultCellPhoneInfo = () => {
  let param = {country_code: country_code.value, phone_number: phone_number.value}
  let ret = zoomSMS.SetDefaultCellPhoneInfo(param)
  result.value = getSDKResult('SetDefaultCellPhoneInfo', ret, param)
}

onMounted(() => {
  getSupportPhoneNumberCountryList()
})
</script>

<style scoped lang="scss">
.result {
  .title {
    width: 370px;
  }
}
</style>