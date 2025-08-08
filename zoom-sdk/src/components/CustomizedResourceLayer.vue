<template>
  <div class="resource">
    <el-dialog v-model="dialogVisible" title="Customized Resource" width="540px">
      <el-tabs v-model="activeName" stretch>
        <el-tab-pane label="String Resource" name="CustomizedStringResource">
          <div class="select-customize-type overflow">
            <el-select v-model="customizedStringType">
              <el-option
                v-for="(item, key) in SDKCustomizedStringType"
                :key="item"
                :label="key"
                :value="item"
              >
              </el-option>
            </el-select>
            <el-button class="right" @click="handleAddCustomizedString">Add</el-button>
          </div>
          <el-form ref="customizedFormRef" :model="strForm" :rules="rules" label-width="80px" label-position="left">
            <template v-for="(item, index) in strForm">
              <el-form-item label="StringType">
                <span class="customized-type">{{item.CustomizedStringTypeDesc}}</span>
                <el-button class="right" @click="handleDeleteCustomizedString(index)">Remove</el-button>
              </el-form-item>
              <el-form-item label="String" :prop="index+'.strCustomizedString'"
                :rules="{required: true, message: 'Please input strCustomizedString', trigger: 'change'}">
                <el-input v-model="item.strCustomizedString" placeholder="input strCustomizedString" clearable />
              </el-form-item>
            </template>
          </el-form>
          <el-button type="primary" class="right" @click="submitForm(customizedFormRef)">Submit Customized String</el-button>
        </el-tab-pane>
        <el-tab-pane label="URL Resource" name="CustomizedURLResource">
          <div class="select-customize-type overflow">
            <el-select v-model="customizedURLType">
              <el-option
                v-for="(item, key) in SDKCustomizedURLType"
                :key="item"
                :label="key"
                :value="item"
              >
              </el-option>
            </el-select>
            <el-button class="right" @click="handleAddCustomizedURLType">Add</el-button>
          </div>
          <el-form ref="customizedFormRef" :model="urlForm" :rules="rules" label-width="80px" label-position="left">
            <template v-for="(item, index) in urlForm">
              <el-form-item label="URLType">
                <span class="customized-type">{{item.CustomizedURLTypeDesc}}</span>
                <el-button class="right" @click="handleDeleteCustomizedURL(index)">Remove</el-button>
              </el-form-item>
              <el-form-item label="URL" :prop="index+'.strCustomizeURL'"
                :rules="{required: true, message: 'Please input strCustomizeURL', trigger: 'change'}">
                <el-input v-model.trim="item.strCustomizeURL" placeholder="input strCustomizeURL" clearable />
              </el-form-item>
            </template>
          </el-form>
          <el-button type="primary" class="right" @click="submitForm(customizedFormRef)">Submit Customize URL</el-button>
        </el-tab-pane>
        <el-tab-pane label="Picture Resource" name="CustomizedPictureResource">
          <el-form ref="customizedFormRef" :model="picForm" :rules="rules" label-width="80px" label-position="left">
            <el-form-item label="PNGID" prop="strPNGID">
              <el-input v-model.trim="picForm.strPNGID" placeholder="input strPNGID" clearable />
            </el-form-item>
            <el-form-item label="Path" prop="strPNGPath">
              <el-input v-model.trim="picForm.strPNGPath" placeholder="input strPNGPath" clearable />
            </el-form-item>
          </el-form>
          <el-button type="primary" class="right" @click="submitForm(customizedFormRef)">Submit Customized Picture</el-button>
        </el-tab-pane>
      </el-tabs>
    </el-dialog>
  </div>
</template>

<script setup lang="ts">
import { reactive, ref } from 'vue'
import type { ElForm } from 'element-plus'
import { SDKCustomizedStringType, SDKCustomizedURLType } from '../../lib/settings.js'
import { getResultDesc } from '../assets/ts/util'
const remote = window.require('@electron/remote')
const zoomcustomizedresource = remote.app.zoomSdk.GetCustomizedResource();

const dialogVisible = ref(false)
const activeName = ref('CustomizedStringResource')
const customizedStringType = ref(SDKCustomizedStringType.SDK_Customized_Title_App)
const customizedURLType = ref(SDKCustomizedURLType.SDKCustomizedURL_SUPPORTURL)
const customizedFormRef = ref<InstanceType<typeof ElForm>>()
const strForm: any = reactive([{
  CustomizedStringType: SDKCustomizedStringType.SDK_Customized_Title_App,
  CustomizedStringTypeDesc: 'SDK_Customized_Title_App',
  strCustomizedString: 'zoom electron sdk demo'
}])
const urlForm: any = reactive([{
  CustomizedURLType: SDKCustomizedURLType.SDKCustomizedURL_SUPPORTURL,
  CustomizedURLTypeDesc: 'SDKCustomizedURL_SUPPORTURL',
  strCustomizeURL: 'https://zoom.us/'
}])
const picForm = reactive({
  strPNGID: 'ZOOMAPPICON.PNG',
  strPNGPath: 'D:\\emoticons.png'
})
const rules = reactive({
  strPNGID: [
    {
      required: true,
      message: 'Please input strPNGID',
      trigger: 'change'
    }
  ],
  strPNGPath: [
    {
      required: true,
      message: 'Please input strPNGPath',
      trigger: 'change'
    }
  ]
})

const handleAddCustomizedString = () => {
  for (let item of strForm) {
    if (item.CustomizedStringType == customizedStringType.value) {
      return
    }
    if (!item.strCustomizedString) {
      return
    }
  }
  let CustomizedStringTypeDesc = Object.keys(SDKCustomizedStringType).find(k => SDKCustomizedStringType[k] == customizedStringType.value)
  let obj = {
    CustomizedStringType: customizedStringType.value,
    CustomizedStringTypeDesc,
    strCustomizedString: ''
  }
  strForm.push(obj)
}

const handleAddCustomizedURLType = () => {
  for (let item of urlForm) {
    if (item.CustomizedURLType == customizedURLType.value) {
      return
    }
    if (!item.strCustomizeURL) {
      return
    }
  }
  let CustomizedURLTypeDesc = Object.keys(SDKCustomizedURLType).find(k => SDKCustomizedURLType[k] == customizedURLType.value)
  let obj = {
    CustomizedURLType: customizedURLType.value,
    CustomizedURLTypeDesc,
    strCustomizeURL: ''
  }
  urlForm.push(obj)
}

const handleDeleteCustomizedString = (index: number) => {
  strForm.splice(index, 1)
}

const handleDeleteCustomizedURL = (index: number) => {
  urlForm.splice(index, 1)
}

const submitForm = (formEl: InstanceType<typeof ElForm> | undefined) => {
  if (!formEl) return
  formEl.validate((valid) => {
    if (valid) {
      let result
      switch (activeName.value) {
        case 'CustomizedStringResource':
          for (let item of strForm) {
            result = zoomcustomizedresource.Resource_AddCustomizedStringResource(item);
            getResultDesc(activeName.value, result, true)
          }
          break;
        case 'CustomizedURLResource':
          for (let item of urlForm) {
            result = zoomcustomizedresource.Resource_AddCustomizedURLResource(item);
            getResultDesc(activeName.value, result, true)
          }
        case 'CustomizedPictureResource':
          result = zoomcustomizedresource.Resource_AddCustomizedPictureResource(picForm);
          getResultDesc(activeName.value, result, true)
          break;
      }
    } else {
      console.log('error submit!')
      return false
    }
  })
}

defineExpose({
  dialogVisible
})
</script>

<style scoped lang="scss">
.resource {
  .el-select {
    width: 86.5%;
  }
  .el-form-item {
    .customized-type {
      font-size: 12px;
    }
    &:not(:last-child) {
      margin-bottom: 12px;
    }
  }
  .select-customize-type {
    margin-bottom: 10px;
  }
}
</style>
<style lang="scss">
.resource {
  .el-form-item {
    .el-form-item__content {
      justify-content: space-between;
    }
  }
}
</style>