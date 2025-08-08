<template>
  <div class="login">
    <el-tabs v-model="activeName" stretch @tab-click="handleClick">
      <el-tab-pane label="SSO Login" name="sso">
        <el-form ref="ssoFormRef" :model="ssoForm" :rules="rules" label-width="130px" label-position="left">
          <el-form-item label="PrefixOfVanityUrl" prop="prefixOfVanityUrl">
            <el-input v-model.trim="ssoForm.prefixOfVanityUrl" placeholder="input prefixOfVanityUrl" clearable />
          </el-form-item>
          <el-form-item label="UriProtocol" prop="uriProtocol">
            <el-input v-model.trim="ssoForm.uriProtocol" placeholder="input uriProtocol" clearable />
          </el-form-item>
          <el-form-item>
            <el-button @click="generateSSOLoginWebURL">Generate SSO Login URL</el-button>
            <el-button type="primary" @click="submitForm(ssoFormRef, 'SSOLoginWithWebUriProtocol')">SSO Login With UriProtocol</el-button>
          </el-form-item>
        </el-form>
      </el-tab-pane>
      <el-tab-pane label="RestApi Without Login" name="withoutLogin">
        <el-form ref="restApiLoginFormRef" :model="startForm" :rules="rules" label-width="100px" label-position="left">
          <el-form-item label="MeetingNum" prop="meetingnum">
            <el-input v-model.trim="startForm.meetingnum" placeholder="input meeting number" clearable />
          </el-form-item>
          <el-form-item label="ZAK" prop="zoomaccesstoken">
            <el-input v-model.trim="startForm.zoomaccesstoken" placeholder="input zoomaccesstoken" clearable />
          </el-form-item>
          <el-form-item label="Username" prop="username">
            <el-input v-model.trim="startForm.username" placeholder="input username" clearable />
          </el-form-item>
          <el-form-item>
            <el-dropdown>
              <span class="el-dropdown-link">
                <el-button type="primary" @click="submitForm(restApiLoginFormRef, 'StartMeetingWithOutLogin')">
                  Start Meeting WithOut Login
                  <el-icon class="el-icon--right"><arrow-down /></el-icon>
                </el-button>
              </span>
              <template #dropdown>
                <el-dropdown-menu>
                  <el-dropdown-item>
                    <el-checkbox v-model="with_rawdata" label="With Rawdata"></el-checkbox>
                  </el-dropdown-item>
                </el-dropdown-menu>
              </template>
            </el-dropdown>
          </el-form-item>
        </el-form>
      </el-tab-pane>
      <el-tab-pane label="Only Join" name="join">
        <el-form ref="onlyJoinRef" :model="joinForm" :rules="rules" label-width="100px" label-position="left">
          <el-form-item label="MeetingNum" prop="meetingnum">
            <el-input v-model.trim="joinForm.meetingnum" placeholder="input meeting number" clearable />
          </el-form-item>
          <el-form-item label="Username" prop="username">
            <el-input v-model.trim="joinForm.username" placeholder="input username" clearable />
          </el-form-item>
          <el-form-item label="Password">
            <el-input v-model.trim="joinForm.psw" placeholder="input password" clearable />
          </el-form-item>
          <el-form-item label="OnBehalfToken">
            <el-input v-model.trim="joinForm.onBehalfToken" :autosize="{ minRows: 2 }" type="textarea"
              placeholder="input onBehalfToken" clearable />
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
          <el-form-item>
            <el-dropdown>
              <span class="el-dropdown-link">
                <el-button type="primary" @click="submitForm(onlyJoinRef, 'JoinMeetingWithoutLogin')">
                  Join Meeting Without Login
                  <el-icon class="el-icon--right"><arrow-down /></el-icon>
                </el-button>
              </span>
              <template #dropdown>
                <el-dropdown-menu>
                  <el-dropdown-item>
                    <el-checkbox v-model="with_rawdata" label="With Rawdata"></el-checkbox>
                  </el-dropdown-item>
                </el-dropdown-menu>
              </template>
            </el-dropdown>
          </el-form-item>
        </el-form>
      </el-tab-pane>
      <el-tab-pane label="Join With URL" name="WithURL">
        <el-form ref="urlJoinRef" :model="urlForm" :rules="rules" label-width="80px" label-position="left">
          <el-form-item label="URL" prop="protocol_action">
            <el-input v-model.trim="urlForm.protocol_action" :autosize="{ minRows: 2 }" type="textarea"
             placeholder="input protocol_action" clearable />
          </el-form-item>
          <el-form-item>
            <el-dropdown>
              <span class="el-dropdown-link">
                <el-button type="primary" @click="submitForm(urlJoinRef, 'HandleZoomWebUriProtocolAction')">
                  Join Meeting With URL
                  <el-icon class="el-icon--right"><arrow-down /></el-icon>
                </el-button>
              </span>
              <template #dropdown>
                <el-dropdown-menu>
                  <el-dropdown-item>
                    <el-checkbox v-model="with_rawdata" label="With Rawdata"></el-checkbox>
                  </el-dropdown-item>
                </el-dropdown-menu>
              </template>
            </el-dropdown>
          </el-form-item>
        </el-form>
      </el-tab-pane>
    </el-tabs>
  </div>
</template>

<script setup lang="ts">
import { reactive, ref, onMounted } from 'vue'
import { useRouter } from 'vue-router'
import { useStore } from 'vuex'
import type { ElForm, TabsPaneContext  } from 'element-plus'
import { ArrowDown } from '@element-plus/icons-vue'
import { getResultDesc } from '../assets/ts/util'
const { shell } = window.require('electron')
const remote = window.require('@electron/remote')
const router = useRouter()
const store = useStore()
let zoomAuth = store.state.zoomAuth
let zoomMeeting = store.state.zoomMeeting
const activeName = ref('sso')
const ssoFormRef = ref<InstanceType<typeof ElForm>>()
const restApiLoginFormRef = ref<InstanceType<typeof ElForm>>()
const onlyJoinRef = ref<InstanceType<typeof ElForm>>()
const urlJoinRef = ref<InstanceType<typeof ElForm>>()
const ssoForm = reactive({
  prefixOfVanityUrl: '',
  uriProtocol: ''
})
const startForm = reactive({
  meetingnum: '',
  zoomaccesstoken: '',
  username: ''
})
const joinForm = reactive({
  meetingnum: '',
  username: '',
  psw: '',
  onBehalfToken: '',
  isvideooff: false,
  isaudiooff: false,
  isdirectsharedesktop: false
})
const with_rawdata = ref(false)
const urlForm = reactive({
  protocol_action: ''
})
const rules = reactive({
  prefixOfVanityUrl: [
    {
      required: true,
      message: 'Please input prefixOfVanityUrl',
      trigger: 'change'
    }
  ],
  uriProtocol: [
    {
      required: true,
      message: 'Please input uriProtocol',
      trigger: 'change'
    }
  ],
  meetingnum: [
    {
      required: true,
      message: 'Please input meetingnum',
      trigger: 'change'
    }
  ],
  zoomaccesstoken: [
    {
      required: true,
      message: 'Please input zoomaccesstoken',
      trigger: 'change'
    }
  ],
  username: [
    {
      required: true,
      message: 'Please input username',
      trigger: 'change'
    }
  ],
  protocol_action: [
    {
      required: true,
      message: 'Please input protocol_action',
      trigger: 'change'
    }
  ]
})
let demo_render_window: Window|null

const generateSSOLoginWebURL = () => {
  let url = zoomAuth.GenerateSSOLoginWebURL(ssoForm)
  console.log(url)
  shell.openExternal(url)
}

const handleClick = (tab: TabsPaneContext, event: Event) => {
  if (tab.props.label == 'RestApi Without Login' || tab.props.label == 'Only Join') {
    setPipeName()
  }
}

const setPipeName = () => {
  if (!remote.app.pipeParams) {
    let pipeParams = {
      videoPipeName: 'videoPipeDemo',
      sharePipeName: 'sharePipeDemo',
      audioPipeName: 'audioPipeDemo',
      maxReadLength: 1920 * 1080 * 3 / 2
    }
    zoomMeeting.SetPipeName(pipeParams)
  }
}

const openRawdataWindow = () => {
  if (with_rawdata.value) {
    let routeData = router.resolve({ 
      path: '/rawdata'
    })
    demo_render_window = window.open(routeData.href, 'Rawdata', 'width=1000,height=900,nodeIntegration=yes,contextIsolation=no')
  }
}

const submitForm = (formEl: InstanceType<typeof ElForm> | undefined, type: string | undefined) => {
  if (!formEl) return
  formEl.validate((valid) => {
    if (valid) {
      let ret
      switch (type) {
        case 'SSOLoginWithWebUriProtocol':
          getResultDesc(type, zoomAuth.SSOLoginWithWebUriProtocol(ssoForm), true)
          break;
        case 'StartMeetingWithOutLogin':
          ret = getResultDesc('StartMeetingWithOutLogin', zoomMeeting.StartMeetingWithOutLogin({...startForm, ...{withRawdata: with_rawdata.value}}), true)
          if (ret == 'SDKERR_SUCCESS') {
            router.push({ path: '/board' })
          }
          openRawdataWindow()
          break;
        case 'JoinMeetingWithoutLogin':
          ret = getResultDesc('JoinMeetingWithoutLogin', zoomMeeting.JoinMeetingWithoutLogin({...joinForm, ...{withRawdata: with_rawdata.value}}), true)
          if (ret == 'SDKERR_SUCCESS') {
            router.push({ path: '/board' })
          }
          openRawdataWindow()
          break;
        case 'HandleZoomWebUriProtocolAction':
          ret = getResultDesc('HandleZoomWebUriProtocolAction', zoomMeeting.HandleZoomWebUriProtocolAction(urlForm), true)
          if (ret == 'SDKERR_SUCCESS') {
            router.push({ path: '/board' })
          }
          openRawdataWindow()
          break;
      }
    } else {
      console.log('error submit!')
      return false
    }
  })
}

onMounted(() => {
  if (remote.app.zoomSdkModule) {
    zoomAuth = remote.app.zoomSdkModule.Auth
    zoomMeeting = remote.app.zoomSdkModule.Meeting
  }
})
</script>

<style scoped lang="scss">
.login {
  max-width: 550px;
  margin: 10% auto;
}
</style>