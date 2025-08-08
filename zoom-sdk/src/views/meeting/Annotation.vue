<template>
  <div class="meeting">
    <el-button type="primary" round plain @click="isAnnotationDisable">IsAnnotationDisable</el-button>
    <div class="in-line">
      <el-select v-model="viewtype" placeholder="Select">
        <el-option v-for="(item, key) in SDKViewType" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-input v-model.trim="left" placeholder="input left" />
      <el-input v-model.trim="top" placeholder="input top" />
      <el-button type="primary" round plain @click="startAnnotation">StartAnnotation</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="viewtype" placeholder="Select">
        <el-option v-for="(item, key) in SDKViewType" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="stopAnnotation">StopAnnotation</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="viewtype" placeholder="Select">
        <el-option v-for="(item, key) in SDKViewType" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-select v-model="toolType" placeholder="Select">
        <el-option v-for="(item, key) in ZoomAnnotationToolType" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="setTool">SetTool</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="viewtype" placeholder="Select">
        <el-option v-for="(item, key) in SDKViewType" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-select v-model="clearType" placeholder="Select">
        <el-option v-for="(item, key) in ZoomAnnotationClearType" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="clear">Clear</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="viewtype" placeholder="Select">
        <el-option v-for="(item, key) in SDKViewType" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-input v-model.trim="color" placeholder="input color" />
      <el-button type="primary" round plain @click="setColor">SetColor</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="viewtype" placeholder="Select">
        <el-option v-for="(item, key) in SDKViewType" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-input-number v-model="lineWidth" :min="0" />
      <el-button type="primary" round plain @click="setLineWidth">SetLineWidth</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="viewtype" placeholder="Select">
        <el-option v-for="(item, key) in SDKViewType" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="undo">Undo</el-button>
    </div>
    <div class="in-line">
      <el-select v-model="viewtype" placeholder="Select">
        <el-option v-for="(item, key) in SDKViewType" :key="item" :label="key" :value="item"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="redo">Redo</el-button>
    </div>
    <el-button type="primary" round plain @click="isAnnotationLegalNoticeAvailable">IsAnnotationLegalNoticeAvailable</el-button>
    <el-button type="primary" round plain @click="getAnnotationLegalNoticesPrompt">GetAnnotationLegalNoticesPrompt</el-button>
    <el-button type="primary" round plain @click="getAnnotationLegalNoticesExplained">GetAnnotationLegalNoticesExplained</el-button>
  </div>
  <div class="result">Result: {{result}}</div>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import { SDKViewType, ZoomAnnotationToolType, ZoomAnnotationClearType } from '../../../lib/settings.js'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const result = ref()

const zoomMeetingAnnotation = remote.app.zoomSdkModule.Meeting.GetAnnotationCtrl()
const viewtype = ref(0)
const left = ref()
const top = ref()
const toolType = ref(0)
const clearType = ref(0)
const color = ref()
const lineWidth = ref(1)

const isAnnotationDisable = () => {
  let ret = zoomMeetingAnnotation.Annotation_IsAnnotationDisable()
  result.value = getSDKResult('Annotation_IsAnnotationDisable', ret)
}

const startAnnotation = () => {
  let param = {
    viewtype: viewtype.value,
    left: left.value,
    top: top.value
  }
  let ret = zoomMeetingAnnotation.Annotation_StartAnnotation(param)
  result.value = getSDKResult('Annotation_StartAnnotation', ret, param)
}

const stopAnnotation = () => {
  let param = {
    viewtype: viewtype.value,
  }
  let ret = zoomMeetingAnnotation.Annotation_StopAnnotation(param)
  result.value = getSDKResult('Annotation_StopAnnotation', ret, param)
}

const setTool = () => {
  let param = {
    viewtype: viewtype.value,
    toolType: toolType.value
  }
  let ret = zoomMeetingAnnotation.Annotation_SetTool(param)
  result.value = getSDKResult('Annotation_SetTool', ret, param)
}

const clear = () => {
  let param = {
    viewtype: viewtype.value,
    clearType: clearType.value
  }
  let ret = zoomMeetingAnnotation.Annotation_Clear(param)
  result.value = getSDKResult('Annotation_Clear', ret, param)
}

const setColor = () => {
  let param = {
    viewtype: viewtype.value,
    color: color.value
  }
  let ret = zoomMeetingAnnotation.Annotation_SetColor(param)
  result.value = getSDKResult('Annotation_SetColor', ret, param)
}

const setLineWidth = () => {
  let param = {
    viewtype: viewtype.value,
    lineWidth: lineWidth.value
  }
  let ret = zoomMeetingAnnotation.Annotation_SetLineWidth(param)
  result.value = getSDKResult('Annotation_SetLineWidth', ret, param)
}

const undo = () => {
  let param = {
    viewtype: viewtype.value,
  }
  let ret = zoomMeetingAnnotation.Annotation_Undo(param)
  result.value = getSDKResult('Annotation_Undo', ret, param)
}

const redo = () => {
  let param = {
    viewtype: viewtype.value,
  }
  let ret = zoomMeetingAnnotation.Annotation_Redo(param)
  result.value = getSDKResult('Annotation_Redo', ret, param)
}

const isAnnotationLegalNoticeAvailable = () => {
  let ret = zoomMeetingAnnotation.Annotation_IsAnnotationLegalNoticeAvailable();
  result.value = getSDKResult('Annotation_IsAnnotationLegalNoticeAvailable', ret)
}

const getAnnotationLegalNoticesPrompt = () => {
  let ret = zoomMeetingAnnotation.Annotation_GetAnnotationLegalNoticesPrompt();
  result.value = getSDKResult('Annotation_GetAnnotationLegalNoticesPrompt', ret)
}

const getAnnotationLegalNoticesExplained = () => {
  let ret = zoomMeetingAnnotation.Annotation_GetAnnotationLegalNoticesExplained();
  result.value = getSDKResult('Annotation_GetAnnotationLegalNoticesExplained', ret)
}
</script>