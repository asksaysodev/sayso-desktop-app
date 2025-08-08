<template>
  <div class="setting">
    <el-form :model="form" label-position="left" class="rule-form" label-width="90px">
      <el-form-item label="HParent">
        <el-input v-model="form.hParent" placeholder="require hexadecimal" />
      </el-form-item>
      <el-form-item label="Top">
        <el-input-number v-model="form.top" />
      </el-form-item>
      <el-form-item label="Left">
        <el-input-number v-model="form.left" />
      </el-form-item>
      <el-form-item label="HSettingWnd">
        <el-input v-model="form.hSettingWnd" placeholder="require hexadecimal" />
      </el-form-item>
      <el-form-item label="Show">
        <el-switch v-model="form.bShow"></el-switch>
      </el-form-item>
      <div>
        <el-button type="primary" round plain @click="showTheSettingDlg">ShowTheSettingDlg</el-button>
        <el-button type="primary" round plain @click="hideSettingDlg">HideSettingDlg</el-button>
      </div>
    </el-form>
  </div>
  <div class="result">Result: {{result}}</div>
</template>

<script setup lang="ts">
import { reactive, ref } from 'vue'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const zoomSetting = remote.app.zoomSdkModule.Setting
const form = reactive({
  hParent: '',
  top: 0,
  left: 0,
  hSettingWnd: '',
  bShow: true
})
const result = ref()

const showTheSettingDlg = () => {
  let ret = zoomSetting.SettingUI_ShowTheSettingDlg(form)
  result.value = getSDKResult('SettingUI_ShowTheSettingDlg', ret, form)
}

const hideSettingDlg = () => {
  let ret = zoomSetting.SettingUI_HideSettingDlg();
  result.value = getSDKResult('SettingUI_ShowTheSettingDlg', ret)
}
</script>

<style scoped lang="scss">
.el-input {
  max-width: 150px;
}
</style>