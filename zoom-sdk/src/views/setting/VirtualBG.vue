<template>
  <div class="setting">
    <el-button type="primary" round plain @click="setOnVBImageDidDownloadedCB">SetOnVBImageDidDownloadedCB</el-button>
    <el-button type="primary" round plain @click="setOnGreenVBDidUpdateWithReplaceColorCB">SetOnGreenVBDidUpdateWithReplaceColorCB</el-button>
    <el-button type="primary" round plain @click="setOnSelectedVBImageChangedCB">SetOnSelectedVBImageChangedCB</el-button>
    <el-button type="primary" round plain @click="setOnVBVideoUploadedResultCB">SetOnVBVideoUploadedResultCB</el-button>
    <el-button type="primary" round plain @click="isSupportVirtualBG">IsSupportVirtualBG</el-button>
    <el-button type="primary" round plain @click="isDeviceSupportSmartVirtualBG">IsDeviceSupportSmartVirtualBG</el-button>
    <el-button type="primary" round plain @click="isSupportVirtualBackgroundVideo">IsSupportVirtualBackgroundVideo</el-button>
    <el-button type="primary" round plain @click="isDeviceSupportSmartVirtualBackgroundVideo">IsDeviceSupportSmartVirtualBackgroundVideo</el-button>
    <el-button type="primary" round plain @click="isDeviceSupportGreenVirtualBackgroundVideo">IsDeviceSupportGreenVirtualBackgroundVideo</el-button>
    <div class="in-line">
      <el-checkbox v-model="setUsingGreenScreen_bUse" label="bUse" @change="setUsingGreenScreen"></el-checkbox>
      <el-button type="primary" round plain @click="setUsingGreenScreen">SetUsingGreenScreen</el-button>
      <el-button type="primary" round plain @click="isUsingGreenScreenOn">IsUsingGreenScreenOn</el-button>
    </div>
    <el-button type="primary" round plain @click="isAllowToAddNewVBItem">IsAllowToAddNewVBItem</el-button>
    <el-button type="primary" round plain @click="isAllowToRemoveVBItem">IsAllowToRemoveVBItem</el-button>
    <div class="in-line">
      <el-input v-model="addBGImage_filePath" placeholder="input filePath" />
      <el-button type="primary" round plain @click="addBGImage">AddBGImage</el-button>
    </div>
    <div class="in-line">
      <el-button type="primary" round plain @click="getBGImageList">GetBGImageList</el-button>
      <el-select v-model="bgImageIndex" placeholder="Select item">
        <el-option v-for="(item, key) in bgImageList" :key="key" :label="item.imageName" :value="Number(item.imageHandle)" />
      </el-select>
      <el-button type="primary" round plain @click="useBGImage">UseBGImage</el-button>
      <el-button type="primary" round plain @click="removeBGImage">RemoveBGImage</el-button>
    </div>
    <el-button type="primary" round plain @click="getBGReplaceColor">GetBGReplaceColor</el-button>
    <el-button type="primary" round plain @click="beginSelectReplaceVBColor">BeginSelectReplaceVBColor</el-button>
    <div class="in-line">
      <el-input v-model="addBGVideo_filePath" placeholder="input filePath" />
      <el-button type="primary" round plain @click="addBGVideo">AddBGVideo</el-button>
    </div>
    <div class="in-line">
      <el-button type="primary" round plain @click="getBGVideoList">GetBGVideoList</el-button>
      <el-select v-model="bgVideoIndex" placeholder="Select item">
        <el-option v-for="(item, key) in bgVideoList" :key="key" :label="item.imageName" :value="Number(item.imageHandle)" />
      </el-select>
      <el-button type="primary" round plain @click="useBGVideo">UseBGVideo</el-button>
      <el-button type="primary" round plain @click="removeBGVideo">RemoveBGVideo</el-button>
    </div>
    <div class="result">Result: {{ result }}</div>
    <div class="result">
      <div class="result-item">
        <span class="title">onVBImageDidDownloaded <el-button size="small" @click="onVBImageDidDownloaded_value = 0">Clear</el-button></span>
        <span v-if="onVBImageDidDownloaded_value">updated {{ onVBImageDidDownloaded_value }}</span>
      </div>
      <div class="result-item">
        <span class="title">onGreenVBDidUpdateWithReplaceColor <el-button size="small" @click="onGreenVBDidUpdateWithReplaceColor_options = []">Clear</el-button></span>
        <div class="callback-list">
          <span v-for="(item, index) in onGreenVBDidUpdateWithReplaceColor_options" :key="index">{{ index }} : {{ item }}</span>
        </div>
      </div>
      <div class="result-item">
        <span class="title">onSelectedVBImageChanged <el-button size="small" @click="onSelectedVBImageChanged_value = 0">Clear</el-button></span>
        <span v-if="onSelectedVBImageChanged_value">updated {{ onSelectedVBImageChanged_value }}</span>
      </div>
      <div class="result-item">
        <span class="title">onVBVideoUploadedResult <el-button size="small" @click="onVBVideoUploadedResult_options = []">Clear</el-button></span>
        <div class="callback-list">
          <span v-for="(item, index) in onVBVideoUploadedResult_options" :key="index">{{ index }} : {{ item }}</span>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')

const result = ref('')
const clientHeight = ref((window.innerHeight - 130) + 'px')

const onVBImageDidDownloaded_value = ref<number>(0)
const onVBImageDidDownloaded = () => {
  console.log('onVBImageDidDownloaded')
  onVBImageDidDownloaded_value.value++
}

const onGreenVBDidUpdateWithReplaceColor_options = ref<string[]>([])
const onGreenVBDidUpdateWithReplaceColor = (selectedColor: any) => {
  console.log('onGreenVBDidUpdateWithReplaceColor', `selectedColor: ${selectedColor}`)
  onGreenVBDidUpdateWithReplaceColor_options.value.push(`selectedColor: ${selectedColor}`)
}

const onSelectedVBImageChanged_value = ref<number>(0)
const onSelectedVBImageChanged = () => {
  console.log('onSelectedVBImageChanged')
  onSelectedVBImageChanged_value.value++
}

const onVBVideoUploadedResult_options = ref<string[]>([])
const onVBVideoUploadedResult = (success: boolean, error: number) => {
  console.log('onVBVideoUploadedResult', `success: ${success}, error: ${error}`)
  onVBVideoUploadedResult_options.value.push(`success: ${success}, error: ${error}`)
}

const zoomSettingVirtualBG = remote.app.zoomSdkModule.Setting.GetVirtualBGSetting({ onVBImageDidDownloaded, onGreenVBDidUpdateWithReplaceColor, onSelectedVBImageChanged, onVBVideoUploadedResult })

const setOnVBImageDidDownloadedCB = () => {
  const ret = zoomSettingVirtualBG.SetOnVBImageDidDownloadedCB(onVBImageDidDownloaded)
  result.value = getSDKResult('SetOnVBImageDidDownloadedCB', ret)
}

const setOnGreenVBDidUpdateWithReplaceColorCB = () => {
  const ret = zoomSettingVirtualBG.SetOnGreenVBDidUpdateWithReplaceColorCB(onGreenVBDidUpdateWithReplaceColor)
  result.value = getSDKResult('SetOnGreenVBDidUpdateWithReplaceColorCB', ret)
}

const setOnSelectedVBImageChangedCB = () => {
  const ret = zoomSettingVirtualBG.SetOnSelectedVBImageChangedCB(onSelectedVBImageChanged)
  result.value = getSDKResult('SetOnSelectedVBImageChangedCB', ret)
}

const setOnVBVideoUploadedResultCB = () => {
  const ret = zoomSettingVirtualBG.SetOnVBVideoUploadedResultCB(onVBVideoUploadedResult)
  result.value = getSDKResult('SetOnVBVideoUploadedResultCB', ret)
}

const isSupportVirtualBG = () => {
  const ret = zoomSettingVirtualBG.IsSupportVirtualBG()
  console.log('IsSupportVirtualBG', result.value)
  result.value = getSDKResult('IsSupportVirtualBG', ret)
}

const isDeviceSupportSmartVirtualBG = () => {
  const ret = zoomSettingVirtualBG.IsDeviceSupportSmartVirtualBG()
  console.log('IsDeviceSupportSmartVirtualBG', result.value)
  result.value = getSDKResult('IsDeviceSupportSmartVirtualBG', ret)
}

const isSupportVirtualBackgroundVideo = () => {
  const ret = zoomSettingVirtualBG.IsSupportVirtualBackgroundVideo()
  console.log('IsSupportVirtualBackgroundVideo', result.value)
  result.value = getSDKResult('IsSupportVirtualBackgroundVideo', ret)
}

const isDeviceSupportSmartVirtualBackgroundVideo = () => {
  const ret = zoomSettingVirtualBG.IsDeviceSupportSmartVirtualBackgroundVideo()
  console.log('IsDeviceSupportSmartVirtualBackgroundVideo', result.value)
  result.value = getSDKResult('IsDeviceSupportSmartVirtualBackgroundVideo', ret)
}

const isDeviceSupportGreenVirtualBackgroundVideo = () => {
  const ret = zoomSettingVirtualBG.IsDeviceSupportGreenVirtualBackgroundVideo()
  console.log('IsDeviceSupportGreenVirtualBackgroundVideo', result.value)
  result.value = getSDKResult('IsDeviceSupportGreenVirtualBackgroundVideo', ret)
}

const isUsingGreenScreenOn = () => {
  const ret = zoomSettingVirtualBG.IsUsingGreenScreenOn()
  console.log('IsUsingGreenScreenOn', result.value)
  result.value = getSDKResult('IsUsingGreenScreenOn', ret)
}

const setUsingGreenScreen_bUse = ref(false)
const setUsingGreenScreen = () => {
  const param = {
    bUse: setUsingGreenScreen_bUse.value
  }
  const ret = zoomSettingVirtualBG.SetUsingGreenScreen(param)
  console.log('SetUsingGreenScreen', result.value, param)
  result.value = getSDKResult('SetUsingGreenScreen', ret, param)
}

const isAllowToAddNewVBItem = () => {
  const ret = zoomSettingVirtualBG.IsAllowToAddNewVBItem()
  console.log('IsAllowToAddNewVBItem', result.value)
  result.value = getSDKResult('IsAllowToAddNewVBItem', ret)
}

const isAllowToRemoveVBItem = () => {
  const ret = zoomSettingVirtualBG.IsAllowToRemoveVBItem()
  console.log('IsAllowToRemoveVBItem', result.value)
  result.value = getSDKResult('IsAllowToRemoveVBItem', ret)
}

const addBGImage_filePath = ref('')
const addBGImage = () => {
  const param = {
    filePath: addBGImage_filePath.value
  }
  const ret = zoomSettingVirtualBG.AddBGImage(param)
  console.log('AddBGImage', result.value, param)
  result.value = getSDKResult('AddBGImage', ret, param)
}

const bgImageList = ref<any>([])
const bgImageIndex = ref<number>()

const getBGImageList = () => {
  const ret = zoomSettingVirtualBG.GetBGImageList()
  console.log('GetBGImageList', result.value)
  bgImageList.value = ret
  result.value = getSDKResult('GetBGImageList', ret)
}

const removeBGImage = () => {
  const param = {
    imageHandle: bgImageIndex.value
  }
  const ret = zoomSettingVirtualBG.RemoveBGImage(param)
  console.log('RemoveBGImage', result.value, param)
  result.value = getSDKResult('RemoveBGImage', ret, param)
}

const useBGImage = () => {
  const param = {
    imageHandle: bgImageIndex.value
  }
  const ret = zoomSettingVirtualBG.UseBGImage(param)
  console.log('UseBGImage', result.value, param)
  result.value = getSDKResult('UseBGImage', ret, param)
}

const getBGReplaceColor = () => {
  const ret = zoomSettingVirtualBG.GetBGReplaceColor()
  console.log('GetBGReplaceColor', result.value)
  result.value = getSDKResult('GetBGReplaceColor', ret)
}

const beginSelectReplaceVBColor = () => {
  const ret = zoomSettingVirtualBG.BeginSelectReplaceVBColor()
  console.log('BeginSelectReplaceVBColor', result.value)
  result.value = getSDKResult('BeginSelectReplaceVBColor', ret)
}

const addBGVideo_filePath = ref('')
const addBGVideo = () => {
  const param = {
    filePath: addBGVideo_filePath.value
  }
  const ret = zoomSettingVirtualBG.AddBGVideo(param)
  console.log('AddBGVideo', result.value, param)
  result.value = getSDKResult('AddBGVideo', ret, param)
}

const removeBGVideo = () => {
  const param = {
    imageHandle: bgVideoIndex.value
  }
  const ret = zoomSettingVirtualBG.RemoveBGVideo(param)
  console.log('RemoveBGVideo', result.value, param)
  result.value = getSDKResult('RemoveBGVideo', ret, param)
}

const bgVideoList = ref<any>([])
const bgVideoIndex = ref<number>()

const getBGVideoList = () => {
  const ret = zoomSettingVirtualBG.GetBGVideoList()
  console.log('GetBGVideoList', result.value)
  bgVideoList.value = ret
  result.value = getSDKResult('GetBGVideoList', ret)
}

const useBGVideo = () => {
  const param = {
    imageHandle: bgVideoIndex.value
  }
  const ret = zoomSettingVirtualBG.UseBGVideo(param)
  console.log('UseBGVideo', result.value, param)
  result.value = getSDKResult('UseBGVideo', ret, param)
}

onMounted(() => {
  window.addEventListener('resize', ()=>{
    clientHeight.value = (window.innerHeight - 130) + 'px'
  })
})
</script>

<style scoped lang="scss">
.setting {
  height: v-bind(clientHeight);
  .result {
    .title {
      width: 320px;
    }
  }
}
</style>