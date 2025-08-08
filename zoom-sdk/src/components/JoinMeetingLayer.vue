<template>
  <div class="join">
    <el-dialog v-model="dialogVisible" title="Join Meeting" width="450px">
      <el-form ref="joinFormRef" :model="joinForm" :rules="rules" label-width="110px">
        <el-form-item label="Meetingnum" prop="meetingnum">
          <el-input v-model.trim="joinForm.meetingnum" placeholder="input meetingnum" clearable />
        </el-form-item>
        <el-form-item label="Username" prop="username">
          <el-input v-model.trim="joinForm.username" placeholder="input username" clearable />
        </el-form-item>
        <el-form-item label="Password">
          <el-input v-model.trim="joinForm.psw" placeholder="input password" clearable />
        </el-form-item>
        <el-form-item label="Video Off">
          <el-switch v-model="joinForm.isvideooff"></el-switch>
        </el-form-item>
        <el-form-item label="Audio Off">
          <el-switch v-model="joinForm.isaudiooff"></el-switch>
        </el-form-item>
        <el-form-item label="Direct Share">
          <el-switch v-model="joinForm.isdirectsharedesktop"></el-switch>
        </el-form-item>
        <div class="overflow">
          <el-button type="primary" class="right" @click="submitForm(joinFormRef)">Join</el-button>
        </div>
      </el-form>
    </el-dialog>
  </div>
</template>

<script setup lang="ts">
import { reactive, ref } from 'vue'
import { useRouter } from 'vue-router'
import type { ElForm } from 'element-plus'
import { getResultDesc, toastResult } from '../assets/ts/util'
const props = defineProps({
  withRawdata: {
    type: Boolean
  }
})
const router = useRouter()
const remote = window.require('@electron/remote')
const zoomMeeting = remote.app.zoomSdkModule.Meeting
const dialogVisible = ref(false)
const joinFormRef = ref<InstanceType<typeof ElForm>>()
const joinForm = reactive({
  meetingnum: '',
  username: '',
  psw: '',
  isvideooff: false,
  isaudiooff: false,
  isdirectsharedesktop: false
})
const rules = reactive({
  meetingnum: [
    {
      required: true,
      message: 'Please input meetingnum',
      trigger: 'change'
    }
  ],
  username: [
    {
      required: true,
      message: 'Please input username',
      trigger: 'change'
    }
  ]
})

const submitForm = (formEl: InstanceType<typeof ElForm> | undefined) => {
  if (!formEl) return
  formEl.validate((valid) => {
    if (valid) {
      let ret = getResultDesc('JoinMeeting', zoomMeeting.JoinMeeting({...joinForm, ...{withRawdata: props.withRawdata}}), true)
      dialogVisible.value = false
      if (props.withRawdata) {
        if (ret != 'SDKERR_SUCCESS') {
          toastResult('please call SetPipeName interface to set pipe name')
          return
        }
        let routeData = router.resolve({ 
          path: '/rawdata'
        }); 
        window.open(routeData.href, 'Rawdata', 'width=1000,height=900,nodeIntegration=yes,contextIsolation=no')
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