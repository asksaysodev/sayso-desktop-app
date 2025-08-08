<template>
  <div>
    <div class="logo">
      <img alt="zoom logo" src="../assets/images/zoom_logo.png">
    </div>
    <el-form ref="ruleFormRef" :model="ruleForm" :rules="rules" size="large" class="domain-form" label-width="115px"
      label-position="left" v-loading="loading" element-loading-background="rgba(0, 0, 0, 0.8)">
      <el-form-item label="Domain" prop="domain">
        <el-input v-model.trim="ruleForm.domain" placeholder="input domain" clearable />
      </el-form-item>
      <el-form-item label="Language">
        <el-select v-model="ruleForm.langid">
          <el-option v-for="(item, key) in LanguageOptions" :key="item" :label="key" :value="item"></el-option>
        </el-select>
      </el-form-item>
      <el-form-item label="Enable Log">
        <el-switch v-model="ruleForm.enableLog"></el-switch>
      </el-form-item>
      <el-form-item label="Log File Size" v-if="ruleForm.enableLog">
        <el-input-number v-model="ruleForm.logfilesize" :min="1" :max="50" />
      </el-form-item>
      <el-form-item label="Generate Dump" v-if="platform=='win32'">
        <el-switch v-model="ruleForm.enableGenerateDump"></el-switch>
      </el-form-item>
      <div class="btn-group">
        <el-button plain @click="resourceRef.dialogVisible=!resourceRef.dialogVisible" v-if="platform=='win32'">Set Resource</el-button>
        <el-button type="primary" @click="submitForm(ruleFormRef)">Init SDK</el-button>
      </div>
    </el-form>
    <CustomizedResourceLayer ref="resourceRef" v-if="platform=='win32'" />
    <TokenLayer :zoomAuth="zoomAuth" ref="tokenRef" />
  </div>
</template>

<script setup lang="ts">
import { reactive, ref } from 'vue'
import { useRouter } from 'vue-router'
import { useStore } from 'vuex'
import type { ElForm } from 'element-plus'
import CustomizedResourceLayer from '@/components/CustomizedResourceLayer.vue'
import TokenLayer from '@/components/TokenLayer.vue'
import { ZoomSDK_LANGUAGE_ID, ZoomAuthResult, ZoomLoginStatus } from '../../lib/settings.js'
import { getResultDesc } from '../assets/ts/util'
const { ipcRenderer } = window.require('electron')
const remote = window.require('@electron/remote')
const zoomSdk = remote.app.zoomSdk
const platform = remote.getGlobal('platform')
const router = useRouter()
const store = useStore()
const zoomAuth = ref({})
const loading = ref(false)
const resourceRef = ref()
const supportDarkMode = ref(false)
const tokenRef = ref()
const { LANGUAGE_Unknown, ...LanguageOptions } = ZoomSDK_LANGUAGE_ID
LanguageOptions.LANGUAGE_Follow_As_System = 0

const ruleFormRef = ref<InstanceType<typeof ElForm>>()
const ruleForm = reactive({
  domain: 'https://www.zoom.us',
  langid: ZoomSDK_LANGUAGE_ID.LANGUAGE_English,
  enableLog: true,
  enableGenerateDump: false,
  logfilesize: 5
})
const rules = reactive({
  domain: [
    {
      required: true,
      message: 'Please input domain',
      trigger: 'change'
    }
  ]
})

const submitForm = (formEl: InstanceType<typeof ElForm> | undefined) => {
  if (!formEl) return
  formEl.validate((valid) => {
    if (valid) {
      let ret = getResultDesc('InitSDK', zoomSdk.InitSDK(ruleForm), true)
      if (ret == 'SDKERR_SUCCESS') {
        zoomAuth.value = zoomSdk.GetAuth({authcb, onLoginReturnWithReason})
        tokenRef.value.dialogVisible = true
      }
    } else {
      console.log('error submit!')
      return false
    }
  })
}

const authcb = (status: number) => {
  console.log('authcb', status)
  if (status == ZoomAuthResult.AUTHRET_SUCCESS) {
    ipcRenderer.send('auth-success')
    store.commit('setZoomAuth', zoomAuth.value)
    store.commit('setZoomMeeting', zoomSdk.GetMeeting())
    router.push({ path: '/login' })
  } else {
    tokenRef.value.dialogVisible = true
  }
}

const onLoginReturnWithReason = (loginStatus: number, loginFailReason: number) => {
  console.log('onLoginReturnWithReason', loginStatus, loginFailReason)
  loading.value = false
  switch (loginStatus) {
    case ZoomLoginStatus.LOGIN_SUCCESS:
      router.push({ path: '/board' })
      break;
    case ZoomLoginStatus.LOGIN_PROCESSING:
      loading.value = true
      break;
    case ZoomLoginStatus.LOGIN_IDLE: 
    case ZoomLoginStatus.LOGIN_FAILED:
      break;
    default:
      break;
  }
}
</script>

<style scoped lang="scss">
.domain-form {
  max-width: 520px;
  position: absolute;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -40%);
  .el-select {
    width: 100%;
  }
}
</style>
