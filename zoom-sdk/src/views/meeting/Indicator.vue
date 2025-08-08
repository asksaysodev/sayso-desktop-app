<template>
  <div class="meeting">
    <el-button type="primary" round plain @click="setOnIndicatorItemReceivedCB">SetOnIndicatorItemReceivedCB</el-button>
    <el-button type="primary" round plain @click="setOnIndicatorItemRemovedCB">SetOnIndicatorItemRemovedCB</el-button>
  </div>
  <div class="result">Result: {{result}}</div>
  <div class="result"> 
    <div class="result-item"> 
      <span class="title">onIndicatorItemReceived <el-button size="small" @click="onIndicatorItemReceived_count=0">Clear</el-button></span> 
      <el-button type="primary" size="small" round plain @click="getIndicatorItemId">GetIndicatorItemId</el-button>
      <el-button type="primary" size="small" round plain @click="getIndicatorName">GetIndicatorName</el-button>
      <el-button type="primary" size="small" round plain @click="getIndicatorIcon">GetIndicatorIcon</el-button>
      <el-input v-model.trim="x" size="small" placeholder="input x" />
      <el-input v-model.trim="y" size="small" placeholder="input y" />
      <el-select v-model="windowId" size="small" placeholder="select" v-if="platform=='darwin'">
        <el-option v-for="(item, index) in windowIdList" :key="index" :label="item" :value="item"></el-option>
      </el-select>
      <el-button type="primary" size="small" round plain @click="showIndicatorPanel">ShowIndicatorPanel</el-button>
      <el-button type="primary" size="small" round plain @click="hideIndicatorPanel">HideIndicatorPanel</el-button>
      <span v-if="onIndicatorItemReceived_count">updated {{onIndicatorItemReceived_count}}</span> 
    </div>
    <div class="result-item"> 
      <span class="title">onIndicatorItemRemoved <el-button size="small" @click="onIndicatorItemRemoved_count=0">Clear</el-button></span> 
      <span v-if="onIndicatorItemRemoved_count">updated {{onIndicatorItemRemoved_count}}</span> 
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onBeforeMount } from 'vue'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const desktopCapturer = remote.desktopCapturer
const platform = ref(process.platform)

const result = ref()
const onIndicatorItemReceived_count = ref(0)
const onIndicatorItemRemoved_count = ref(0)

const onIndicatorItemReceived = () => {
  console.log('onIndicatorItemReceived')
  onIndicatorItemReceived_count.value++
}
const onIndicatorItemRemoved = () => {
  console.log('onIndicatorItemRemoved')
  onIndicatorItemRemoved_count.value++
}
const zoomMeetingIndicator = remote.app.zoomSdkModule.Meeting.GetMeetingIndicator({ onIndicatorItemReceived, onIndicatorItemRemoved })

const setOnIndicatorItemReceivedCB = () => {
  let ret = zoomMeetingIndicator.SetOnIndicatorItemReceivedCB(onIndicatorItemReceived)
  result.value = getSDKResult('SetOnIndicatorItemReceivedCB', ret)
}

const setOnIndicatorItemRemovedCB = () => {
  let ret = zoomMeetingIndicator.SetOnIndicatorItemRemovedCB(onIndicatorItemRemoved)
  result.value = getSDKResult('SetOnIndicatorItemRemovedCB', ret)
}

const getIndicatorItemId = () => {
  let ret = zoomMeetingIndicator.GetIndicatorItemId()
  result.value = getSDKResult('GetIndicatorItemId', ret)
}

const getIndicatorName = () => {
  let ret = zoomMeetingIndicator.GetIndicatorName()
  result.value = getSDKResult('GetIndicatorName', ret)
}

const getIndicatorIcon = () => {
  let ret = zoomMeetingIndicator.GetIndicatorIcon()
  result.value = getSDKResult('GetIndicatorIcon', ret)
}

const x = ref(0)
const y = ref(0)
const windowId = ref<number>(0)

const showIndicatorPanel = () => {
  let param = { x: x.value, y: y.value, windowId: windowId.value }
  let ret = zoomMeetingIndicator.ShowIndicatorPanel(param)
  result.value = getSDKResult('ShowIndicatorPanel', ret, param)
}

const hideIndicatorPanel = () => {
  let ret = zoomMeetingIndicator.HideIndicatorPanel()
  result.value = getSDKResult('HideIndicatorPanel', ret)
}

const windowIdList = ref<number[]>([])
onBeforeMount(() => {
  desktopCapturer.getSources({ types: ['window'] }).then((sources: any) => {
    let tempList = []
    console.log(sources)
    for (let index in sources) {
      const obj = { ...sources[index] }
      const id = obj.id.split(':')[1]
      tempList.push(parseInt(id))
    }
    windowIdList.value = tempList
    windowId.value = tempList.length > 0 ? tempList[0] : 0
  })
})
</script>

<style scoped lang="scss">
.result {
  .title {
    width: 230px;
  }
  .el-input {
    width: 60px;
  }
  .el-select {
    width: 100px;
  }
}
</style>