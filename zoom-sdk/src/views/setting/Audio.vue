<template>
  <div class="setting">
    <div class="selecter">
      <span>Select Mic</span>
      <el-select v-model="selectMicId" placeholder="Select" @change="selectMic">
        <el-option v-for="(item, index) in micList" :key="index" :label="item.deviceName" :value="item.deviceId"></el-option>
      </el-select>
    </div>
    <div class="selecter">
      <span>Set MicVol</span>
      <el-slider v-model="micVol" :max="255" @change="setMicVol"></el-slider>
    </div>
    <div class="selecter">
      <span>Set EchoCancellation</span>
      <el-slider v-model="echoCancellationLevel" :max="255" @change="setEchoCancellationLevel"></el-slider>
    </div>
    <div class="selecter">
      <span>Select Speaker</span>
      <el-select v-model="selectSpeakerId" placeholder="Select" @change="selectSpeaker">
        <el-option v-for="(item, index) in speakerList" :key="index" :label="item.deviceName" :value="item.deviceId"></el-option>
      </el-select>
    </div>
    <div class="selecter">
      <span>Set SpeakerVol</span>
      <el-slider v-model="speakerVol" :max="255" @change="setSpeakerVol"></el-slider>
    </div>
    <el-divider></el-divider>
    <el-checkbox v-model="enable_autoJoinAudio" label="EnableAutoJoinAudio" @change="enableAutoJoinAudio"></el-checkbox>
    <el-checkbox v-model="enable_autoAdjustMic" label="EnableAutoAdjustMic" @change="enableAutoAdjustMic"></el-checkbox>
    <el-checkbox v-model="enable_stereoAudio" label="EnableStereoAudio" @change="enableStereoAudio"></el-checkbox>
    <el-checkbox v-model="enable_micOriginalInput" label="EnableMicOriginalInput" @change="enableMicOriginalInput"></el-checkbox>
    <el-checkbox v-model="enable_holdSpaceKeyToSpeak" label="EnableHoldSpaceKeyToSpeak" @change="enableHoldSpaceKeyToSpeak"></el-checkbox>
    <el-checkbox v-model="enable_alwaysMuteMicWhenJoinVoip" label="EnableAlwaysMuteMicWhenJoinVoip" @change="enableAlwaysMuteMicWhenJoinVoip"></el-checkbox>
    <el-checkbox v-model="enable_suppressAudioNotify" label="EnableSuppressAudioNotify" @change="enableSuppressAudioNotify"></el-checkbox>
    <el-button type="primary" round plain @click="setComputerMicDeviceChangedCB">SetComputerMicDeviceChangedCB</el-button>
    <el-button type="primary" round plain @click="setComputerSpeakerDeviceChangedCB">SetComputerSpeakerDeviceChangedCB</el-button>
    <el-button type="primary" round plain @click="setDefaultMicDeviceChangedCB">SetDefaultMicDeviceChangedCB</el-button>
    <el-button type="primary" round plain @click="setDefaultSpeakerDeviceChangedCB">SetDefaultSpeakerDeviceChangedCB</el-button>
    <el-button type="primary" round plain @click="isAutoJoinAudioEnabled">IsAutoJoinAudioEnabled</el-button>
    <el-button type="primary" round plain @click="isAutoAdjustMicEnabled">IsAutoAdjustMicEnabled</el-button>
    <el-button type="primary" round plain @click="isStereoAudioEnable">IsStereoAudioEnable</el-button>
    <el-button type="primary" round plain @click="isMicOriginalInputEnable">IsMicOriginalInputEnable</el-button>
    <el-button type="primary" round plain @click="isHoldSpaceKeyToSpeakEnabled">IsHoldSpaceKeyToSpeakEnabled</el-button>
    <el-button type="primary" round plain @click="isAlwaysMuteMicWhenJoinVoipEnabled">IsAlwaysMuteMicWhenJoinVoipEnabled</el-button>
    <el-button type="primary" round plain @click="isSuppressAudioNotifyEnabled">IsSuppressAudioNotifyEnabled</el-button>
  </div>
  <div class="result">Result: {{result}}</div>
  <div class="result">
    <div class="result-item">
      <span class="title">onComputerMicDeviceChanged <el-button size="small" @click="onComputerMicDeviceChanged_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onComputerMicDeviceChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onComputerSpeakerDeviceChanged <el-button size="small" @click="onComputerSpeakerDeviceChanged_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onComputerSpeakerDeviceChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onDefaultMicDeviceChanged <el-button size="small" @click="onDefaultMicDeviceChanged_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onDefaultMicDeviceChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
    <div class="result-item"> 
      <span class="title">onDefaultSpeakerDeviceChanged <el-button size="small" @click="onDefaultSpeakerDeviceChanged_options=[]">Clear</el-button></span> 
      <div class="callback-list">
        <span v-for="(item, index) in onDefaultSpeakerDeviceChanged_options" :key="index">{{index}} : {{item}}</span>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const enable_autoJoinAudio = ref(false)
const enable_autoAdjustMic = ref(false)
const enable_stereoAudio = ref(false)
const enable_micOriginalInput = ref(false)
const enable_holdSpaceKeyToSpeak = ref(false)
const enable_alwaysMuteMicWhenJoinVoip = ref(false)
const enable_suppressAudioNotify = ref(false)
const result = ref()
const clientHeight = ref((window.innerHeight - 360) + 'px')
const onComputerMicDeviceChanged_options = ref<any[]>([])
const onComputerSpeakerDeviceChanged_options = ref<any[]>([])
const onDefaultMicDeviceChanged_options = ref<any[]>([])
const onDefaultSpeakerDeviceChanged_options = ref<any[]>([])

const onComputerMicDeviceChanged = (newMicList: any) => {
  console.log(newMicList)
  onComputerMicDeviceChanged_options.value.push(newMicList)
}
const onComputerSpeakerDeviceChanged = (newSpeakerList: any) => {
  console.log(newSpeakerList)
  onComputerSpeakerDeviceChanged_options.value.push(newSpeakerList)
}
const onDefaultMicDeviceChanged = (deviceId: string, deviceName: string) => {
  console.log('onDefaultMicDeviceChanged', deviceId, deviceName)
  onDefaultMicDeviceChanged_options.value = [{deviceId, deviceName}]
}
const onDefaultSpeakerDeviceChanged = (deviceId: string, deviceName: string) => {
  console.log('onDefaultSpeakerDeviceChanged', deviceId, deviceName)
  onDefaultSpeakerDeviceChanged_options.value = [{deviceId, deviceName}]
}
const zoomSettingAudio = remote.app.zoomSdkModule.Setting.GetAudioSetting({onComputerMicDeviceChanged, onComputerSpeakerDeviceChanged, onDefaultMicDeviceChanged, onDefaultSpeakerDeviceChanged})

const micList = ref(zoomSettingAudio.Setting_GetMicList())
const selectMicId = ref()
const speakerList = ref(zoomSettingAudio.Setting_GetSpeakerList())
const selectSpeakerId = ref()
const micVol = ref(zoomSettingAudio.Setting_GetMicVol().MicVol)
console.log('micVol', micVol.value)
const speakerVol = ref(zoomSettingAudio.Setting_GetSpeakerVol().SpeakerVol)
console.log('speakerVol', speakerVol.value)
const echoCancellationLevel = ref(zoomSettingAudio.Setting_GetEchoCancellationLevel())
console.log('echoCancellationLevel', echoCancellationLevel.value)

const selectMic = () => {
  micList.value = zoomSettingAudio.Setting_GetMicList()
  let obj: any = {};
  obj = micList.value.find((item: any)=>{
    return item.deviceId === selectMicId.value;
  });
  let ret = zoomSettingAudio.Setting_SelectMic(obj)
  result.value = getSDKResult('Setting_SelectMic', ret, obj)
}

const selectSpeaker = () => {
  speakerList.value = zoomSettingAudio.Setting_GetSpeakerList()
  let obj: any = {};
  obj = speakerList.value.find((item: any)=>{
    return item.deviceId === selectSpeakerId.value;
  });
  let ret = zoomSettingAudio.Setting_SelectSpeaker(obj)
  result.value = getSDKResult('Setting_SelectSpeaker', ret, obj)
}

const setMicVol = () => {
  let param = {value: micVol.value}
  let ret = zoomSettingAudio.Setting_SetMicVol(param)
  result.value = getSDKResult('Setting_SetMicVol', ret, param)
}

const setEchoCancellationLevel = () => {
  let param = {level: echoCancellationLevel.value}
  let ret = zoomSettingAudio.Setting_SetEchoCancellationLevel(param)
  result.value = getSDKResult('Setting_SetEchoCancellationLevel', ret, param)
}

const setSpeakerVol = () => {
  let param = {value: speakerVol.value}
  let ret = zoomSettingAudio.Setting_SetSpeakerVol(param)
  result.value = getSDKResult('Setting_SetSpeakerVol', ret, param)
}

const enableAutoJoinAudio = () => {
  let param = {zn_bEnable: enable_autoJoinAudio.value}
  let ret = zoomSettingAudio.Setting_EnableAutoJoinAudio(param)
  result.value = getSDKResult('Setting_EnableAutoJoinAudio', ret, param)
}

const enableAutoAdjustMic = () => {
  let param = {zn_bEnable: enable_autoAdjustMic.value}
  let ret = zoomSettingAudio.Setting_EnableAutoAdjustMic(param)
  result.value = getSDKResult('Setting_EnableAutoAdjustMic', ret, param)
}

const enableStereoAudio = () => {
  let param = {bEnable: enable_stereoAudio.value}
  let ret = zoomSettingAudio.Setting_EnableStereoAudio(param)
  result.value = getSDKResult('Setting_EnableStereoAudio', ret, param)
}

const enableMicOriginalInput = () => {
  let param = {bEnable: enable_micOriginalInput.value}
  let ret = zoomSettingAudio.Setting_EnableMicOriginalInput(param)
  result.value = getSDKResult('Setting_EnableMicOriginalInput', ret, param)
}

const enableHoldSpaceKeyToSpeak = () => {
  let param = {bEnable: enable_holdSpaceKeyToSpeak.value}
  let ret = zoomSettingAudio.Setting_EnableHoldSpaceKeyToSpeak(param)
  result.value = getSDKResult('Setting_EnableHoldSpaceKeyToSpeak', ret, param)
}

const enableAlwaysMuteMicWhenJoinVoip = () => {
  let param = {bEnable: enable_alwaysMuteMicWhenJoinVoip.value}
  let ret = zoomSettingAudio.Setting_EnableAlwaysMuteMicWhenJoinVoip(param)
  result.value = getSDKResult('Setting_EnableAlwaysMuteMicWhenJoinVoip', ret, param)
}

const enableSuppressAudioNotify = () => {
  let param = {bEnable: enable_suppressAudioNotify.value}
  let ret = zoomSettingAudio.Setting_EnableSuppressAudioNotify(param)
  result.value = getSDKResult('Setting_EnableSuppressAudioNotify', ret, param)
}

const setComputerMicDeviceChangedCB = () => {
  let ret = zoomSettingAudio.Setting_SetComputerMicDeviceChangedCB(onComputerMicDeviceChanged)
  result.value = getSDKResult('Setting_SetComputerMicDeviceChangedCB', ret)
}

const setComputerSpeakerDeviceChangedCB = () => {
  let ret = zoomSettingAudio.Setting_SetComputerSpeakerDeviceChangedCB(onComputerSpeakerDeviceChanged)
  result.value = getSDKResult('Setting_SetComputerSpeakerDeviceChangedCB', ret)
}

const setDefaultMicDeviceChangedCB = () => {
  let ret = zoomSettingAudio.Setting_SetDefaultMicDeviceChangedCB(onDefaultMicDeviceChanged)
  result.value = getSDKResult('Setting_SetDefaultMicDeviceChangedCB', ret)
}

const setDefaultSpeakerDeviceChangedCB = () => {
  let ret = zoomSettingAudio.Setting_SetDefaultSpeakerDeviceChangedCB(onDefaultSpeakerDeviceChanged)
  result.value = getSDKResult('Setting_SetDefaultSpeakerDeviceChangedCB', ret)
}

const isAutoJoinAudioEnabled = () => {
  let ret = zoomSettingAudio.Checking_IsAutoJoinAudioEnabled()
  result.value = getSDKResult('Checking_IsAutoJoinAudioEnabled', ret)
}

const isAutoAdjustMicEnabled = () => {
  let ret = zoomSettingAudio.Checking_IsAutoAdjustMicEnabled()
  result.value = getSDKResult('Checking_IsAutoAdjustMicEnabled', ret)
}

const isStereoAudioEnable = () => {
  let ret = zoomSettingAudio.Setting_IsStereoAudioEnable()
  result.value = getSDKResult('Setting_IsStereoAudioEnable', ret)
}

const isMicOriginalInputEnable = () => {
  let ret = zoomSettingAudio.Setting_IsMicOriginalInputEnable()
  result.value = getSDKResult('Setting_IsMicOriginalInputEnable', ret)
}

const isHoldSpaceKeyToSpeakEnabled = () => {
  let ret = zoomSettingAudio.Setting_IsHoldSpaceKeyToSpeakEnabled()
  result.value = getSDKResult('Setting_IsHoldSpaceKeyToSpeakEnabled', ret)
}

const isAlwaysMuteMicWhenJoinVoipEnabled = () => {
  let ret = zoomSettingAudio.Setting_IsAlwaysMuteMicWhenJoinVoipEnabled()
  result.value = getSDKResult('Setting_IsAlwaysMuteMicWhenJoinVoipEnabled', ret)
}

const isSuppressAudioNotifyEnabled = () => {
  let ret = zoomSettingAudio.Setting_IsSuppressAudioNotifyEnabled()
  result.value = getSDKResult('Setting_IsSuppressAudioNotifyEnabled', ret)
}

onMounted(() => {
  for (let item of micList.value) {
    if (item.isSelected) {
      selectMicId.value = item.deviceId
      break
    }
  }
  for (let item of speakerList.value) {
    if (item.isSelected) {
      selectSpeakerId.value = item.deviceId
      break
    }
  }
  window.addEventListener('resize', ()=>{
    clientHeight.value = (window.innerHeight - 360) + 'px'
  })
})
</script>
<style scoped lang="scss">
.setting {
  height: v-bind(clientHeight);
}
</style>