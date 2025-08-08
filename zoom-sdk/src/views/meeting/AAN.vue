<template>
  <div class="meeting">
    <el-form :model="form" class="rule-form" label-width="115px">
      <el-form-item label="X">
        <el-input-number v-model="form.x" :min="0" />
      </el-form-item>
      <el-form-item label="Y">
        <el-input-number v-model="form.y" :min="0" />
      </el-form-item>
      <el-form-item label="Window ID">
        <el-select v-model="form.windowID" placeholder="select">
          <el-option v-for="(item, index) in windowIdList" :key="index" :label="item" :value="item"></el-option>
        </el-select>
      </el-form-item>
      <el-form-item>
        <el-button type="primary" round plain @click="showAANPanel">ShowAANPanel</el-button>
        <el-button type="primary" round plain @click="hideAANPanel">HideAANPanel</el-button>
      </el-form-item>
    </el-form>
  </div>
  <div class="result">Result: {{result}}</div>
</template>

<script setup lang="ts">
import { reactive, ref, onBeforeMount } from 'vue'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const zoomMeetingAAN = remote.app.zoomSdkModule.Meeting.GetMeetingAAN()
const desktopCapturer = remote.desktopCapturer

const form = reactive({
  x: 0,
  y: 0,
  windowID: 0
})
const result = ref()

const showAANPanel = () => {
  let ret = zoomMeetingAAN.ShowAANPanel(form)
  result.value = getSDKResult('ShowAANPanel', ret, form)
}

const hideAANPanel = () => {
  let ret = zoomMeetingAAN.HideAANPanel();
  result.value = getSDKResult('HideAANPanel', ret)
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
    form.windowID = tempList.length > 0 ? tempList[0] : 0
  })
})
</script>

<style scoped lang="scss">
.el-select {
  width: 150px;
}
</style>