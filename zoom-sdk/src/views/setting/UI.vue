<template>
  <div class="setting">
    <el-checkbox v-model="disable_advancedFeatures4GeneralSetting" label="DisableAdvancedFeatures4GeneralSetting" @change="disableAdvancedFeatures4GeneralSetting"></el-checkbox>
    <el-checkbox v-model="disable_accountSettingTabPage" label="DisableAccountSettingTabPage" @change="disableAccountSettingTabPage"></el-checkbox>
    <el-checkbox v-model="hide_autoCopyInviteLinkCheckBox" label="HideAutoCopyInviteLinkCheckBox" @change="hideAutoCopyInviteLinkCheckBox"></el-checkbox>
    <el-divider></el-divider>
    <div>
      <el-checkbox v-model="showVirtualBackgroundPage" label="ShowVirtualBackgroundPage" :true-value="1" :false-value="0" />
      <el-checkbox v-model="showVideoPage" label="ShowVideoPage" :true-value="1" :false-value="0" />
      <el-checkbox v-model="showStaticsticsPage" label="ShowStaticsticsPage" :true-value="1" :false-value="0" />
      <el-checkbox v-model="showRecordingPage" label="ShowRecordingPage" :true-value="1" :false-value="0" />
      <el-checkbox v-model="showGeneralPage" label="ShowGeneralPage" :true-value="1" :false-value="0" />
      <el-checkbox v-model="showFeedBackPage" label="ShowFeedBackPage" :true-value="1" :false-value="0" />
      <el-checkbox v-model="showAudioPage" label="ShowAudioPage" :true-value="1" :false-value="0" />
      <el-checkbox v-model="showAdvanceFeaturePage" label="ShowAdvanceFeaturePage" :true-value="1" :false-value="0" />
      <el-checkbox v-model="showAccessibilityPage" label="ShowAccessibilityPage" :true-value="1" :false-value="0" />
      <el-button type="primary" round plain @click="confSettingDialogShownTabPage">ConfSettingDialogShownTabPage</el-button>
    </div>
    <el-divider></el-divider>
    <div>
      <el-checkbox v-model="showViewMoreSettingUrl" label="ShowViewMoreSettingUrl" :true-value="1" :false-value="0" />
      <el-checkbox v-model="showSupportCenterUrl" label="ShowSupportCenterUrl" :true-value="1" :false-value="0" />
      <el-checkbox v-model="showLearnMoreSuppressBackgroundNoiseUrl" label="ShowLearnMoreSuppressBackgroundNoiseUrl" :true-value="1" :false-value="0" />
      <el-checkbox v-model="showLearnMoreScreenCaptureModeUrl" label="ShowLearnMoreScreenCaptureModeUrl" :true-value="1" :false-value="0" />
      <el-button type="primary" round plain @click="configToShowUrlLinksInSetting">ConfigToShowUrlLinksInSetting</el-button>
    </div>
  </div>
  <div class="result">Result: {{result}}</div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const zoomSettingUICtrl = remote.app.zoomSdkModule.Setting.GetSettingUICtrl()
const disable_advancedFeatures4GeneralSetting = ref(false)
const disable_accountSettingTabPage = ref(false)
const hide_autoCopyInviteLinkCheckBox = ref(false)
const showVirtualBackgroundPage = ref(1)
const showVideoPage = ref(1)
const showStaticsticsPage = ref(1)
const showRecordingPage = ref(1)
const showGeneralPage = ref(1)
const showFeedBackPage = ref(0)
const showAudioPage = ref(1)
const showAdvanceFeaturePage = ref(0)
const showAccessibilityPage = ref(1)
const showViewMoreSettingUrl = ref(1)
const showSupportCenterUrl = ref(1)
const showLearnMoreSuppressBackgroundNoiseUrl = ref(1)
const showLearnMoreScreenCaptureModeUrl = ref(1)
const result = ref()
const clientHeight = ref((window.innerHeight - 190) + 'px')

const disableAdvancedFeatures4GeneralSetting = () => {
  let param = {bDisable: disable_advancedFeatures4GeneralSetting.value}
  let ret = zoomSettingUICtrl.SettingUI_DisableAdvancedFeatures4GeneralSetting(param)
  result.value = getSDKResult('SettingUI_DisableAdvancedFeatures4GeneralSetting', ret, param)
}

const disableAccountSettingTabPage = () => {
  let param = {bDisable: disable_accountSettingTabPage.value}
  let ret = zoomSettingUICtrl.SettingUI_DisableAccountSettingTabPage(param)
  result.value = getSDKResult('SettingUI_DisableAccountSettingTabPage', ret, param)
}

const hideAutoCopyInviteLinkCheckBox = () => {
  let param = {bHide: hide_autoCopyInviteLinkCheckBox.value}
  let ret = zoomSettingUICtrl.SettingUI_HideAutoCopyInviteLinkCheckBox(param)
  result.value = getSDKResult('SettingUI_HideAutoCopyInviteLinkCheckBox', ret, param)
}

const confSettingDialogShownTabPage = () => {
  let param = {number: `${showVirtualBackgroundPage.value}${showVideoPage.value}${showStaticsticsPage.value}${showRecordingPage.value}${showGeneralPage.value}${showFeedBackPage.value}${showAudioPage.value}${showAdvanceFeaturePage.value}${showAccessibilityPage.value}`}
  let ret = zoomSettingUICtrl.SettingUI_ConfSettingDialogShownTabPage(param)
  result.value = getSDKResult('SettingUI_ConfSettingDialogShownTabPage', ret, param)
}

const configToShowUrlLinksInSetting = () => {
  let param = {showOption: `${showViewMoreSettingUrl.value}${showSupportCenterUrl.value}${showLearnMoreSuppressBackgroundNoiseUrl.value}${showLearnMoreScreenCaptureModeUrl.value}`}
  let ret = zoomSettingUICtrl.SettingUI_ConfigToShowUrlLinksInSetting(param)
  result.value = getSDKResult('SettingUI_ConfigToShowUrlLinksInSetting', ret, param)
}

onMounted(() => {
  clientHeight.value = (window.innerHeight - 190) + 'px'
})
</script>
<style scoped lang="scss">
.setting {
  height: v-bind(clientHeight);
}
</style>
