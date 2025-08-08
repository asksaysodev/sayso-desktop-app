<template>
  <div class="board">
    <div class="logo">
      <img alt="zoom logo" src="../assets/images/zoom_logo.png">
      <div class="version" v-if="version">Version: {{version}}</div>
    </div>
    <ul>
      <li>
        <div :class="{'disabled':disabled}" @click="startMeeting">
          <svg width="88" height="88" viewBox="0 0 88 88" fill="none" xmlns="http://www.w3.org/2000/svg">
          <path fill-rule="evenodd" clip-rule="evenodd" d="M0 44C0 30.0181 0 23.0271 2.36022 17.5404C5.2921 10.7248 10.7248 5.2921 17.5404 2.36022C23.0271 0 30.0181 0 44 0C57.9819 0 64.9729 0 70.4596 2.36022C77.2752 5.2921 82.7079 10.7248 85.6398 17.5404C88 23.0271 88 30.0181 88 44C88 57.9819 88 64.9729 85.6398 70.4596C82.7079 77.2752 77.2752 82.7079 70.4596 85.6398C64.9729 88 57.9819 88 44 88C30.0181 88 23.0271 88 17.5404 85.6398C10.7248 82.7079 5.2921 77.2752 2.36022 70.4596C0 64.9729 0 57.9819 0 44Z" fill="#F26D21"/>
          <path d="M57.8094 51.5745L61.8778 54.9648C62.8749 55.7957 63.3734 56.2112 63.7752 56.3102C64.5969 56.5127 65.4484 56.1139 65.8188 55.353C66 54.981 66 54.332 66 53.034V35.606C66 34.308 66 33.659 65.8188 33.287C65.4484 32.5261 64.5969 32.1273 63.7752 32.3298C63.3734 32.4288 62.8749 32.8443 61.8778 33.6752L57.8094 37.0655C57.1465 37.6179 56.8151 37.8941 56.5487 38.2099C56.0141 38.8434 55.6565 39.6069 55.512 40.4231C55.44 40.8299 55.44 41.2614 55.44 42.1243V46.5157C55.44 47.3786 55.44 47.8101 55.512 48.2169C55.6565 49.0331 56.0141 49.7966 56.5487 50.4301C56.8151 50.7459 57.1465 51.0221 57.8094 51.5745Z" fill="white"/>
          <path d="M22 36.6534C22 35.0246 22 34.2102 22.317 33.588C22.5958 33.0408 23.0408 32.5958 23.588 32.317C24.2102 32 25.0246 32 26.6534 32H41.4938C45.7593 32 47.8921 32 49.5214 32.8301C50.9545 33.5603 52.1197 34.7255 52.8499 36.1586C53.68 37.7879 53.68 39.9207 53.68 44.1862V51.9866C53.68 53.6154 53.68 54.4298 53.363 55.052C53.0842 55.5992 52.6392 56.0442 52.092 56.323C51.4698 56.64 50.6554 56.64 49.0266 56.64H34.1862C29.9207 56.64 27.7879 56.64 26.1586 55.8099C24.7255 55.0797 23.5603 53.9145 22.8301 52.4814C22 50.8521 22 48.7193 22 44.4538V36.6534Z" fill="white"/>
          </svg>
        </div>  
        <el-dropdown :disabled="disabled">
          <span class="el-dropdown-link">
            Start Meeting<el-icon class="el-icon--right"><arrow-down /></el-icon>
          </span>
          <template #dropdown>
            <el-dropdown-menu>
              <el-dropdown-item>
                <el-checkbox v-model="with_rawdata" label="With Rawdata"></el-checkbox>
              </el-dropdown-item>
            </el-dropdown-menu>
          </template>
        </el-dropdown>
      </li>
      <li>
        <div :class="{'disabled':disabled}" @click="joinMeeting">
          <svg width="88" height="88" viewBox="0 0 88 88" fill="none" xmlns="http://www.w3.org/2000/svg">
          <path fill-rule="evenodd" clip-rule="evenodd" d="M0 44C0 30.0558 0 23.0837 2.34468 17.6084C5.28236 10.7483 10.7483 5.28236 17.6084 2.34468C23.0837 0 30.0558 0 44 0C57.9442 0 64.9163 0 70.3916 2.34468C77.2517 5.28236 82.7176 10.7483 85.6553 17.6084C88 23.0837 88 30.0558 88 44C88 57.9442 88 64.9163 85.6553 70.3916C82.7176 77.2517 77.2517 82.7176 70.3916 85.6553C64.9163 88 57.9442 88 44 88C30.0558 88 23.0837 88 17.6084 85.6553C10.7483 82.7176 5.28236 77.2517 2.34468 70.3916C0 64.9163 0 57.9442 0 44Z" fill="#0E72ED"/>
          <path fill-rule="evenodd" clip-rule="evenodd" d="M26.763 29.8221C26 31.3194 26 33.2796 26 37.2V50.8C26 54.7204 26 56.6806 26.763 58.1779C27.4341 59.4951 28.5049 60.5659 29.8221 61.237C31.3194 62 33.2796 62 37.2 62H50.8C54.7204 62 56.6806 62 58.1779 61.237C59.4951 60.5659 60.5659 59.4951 61.237 58.1779C62 56.6806 62 54.7204 62 50.8V37.2C62 33.2796 62 31.3194 61.237 29.8221C60.5659 28.5049 59.4951 27.4341 58.1779 26.763C56.6806 26 54.7204 26 50.8 26H37.2C33.2796 26 31.3194 26 29.8221 26.763C28.5049 27.4341 27.4341 28.5049 26.763 29.8221ZM45.25 37C45.25 36.3096 44.6904 35.75 44 35.75C43.3096 35.75 42.75 36.3096 42.75 37V41.95C42.75 42.23 42.75 42.37 42.6955 42.477C42.6476 42.5711 42.5711 42.6476 42.477 42.6955C42.37 42.75 42.23 42.75 41.95 42.75H37C36.3096 42.75 35.75 43.3096 35.75 44C35.75 44.6904 36.3096 45.25 37 45.25H41.95C42.23 45.25 42.37 45.25 42.477 45.3045C42.5711 45.3524 42.6476 45.4289 42.6955 45.523C42.75 45.63 42.75 45.77 42.75 46.05V51C42.75 51.6904 43.3096 52.25 44 52.25C44.6904 52.25 45.25 51.6904 45.25 51V46.05C45.25 45.77 45.25 45.63 45.3045 45.523C45.3524 45.4289 45.4289 45.3524 45.523 45.3045C45.63 45.25 45.77 45.25 46.05 45.25H51C51.6904 45.25 52.25 44.6904 52.25 44C52.25 43.3096 51.6904 42.75 51 42.75H46.05C45.77 42.75 45.63 42.75 45.523 42.6955C45.4289 42.6476 45.3524 42.5711 45.3045 42.477C45.25 42.37 45.25 42.23 45.25 41.95V37Z" fill="white"/>
          </svg>
        </div> 
        <el-dropdown :disabled="disabled">
          <span class="el-dropdown-link">
            Join Meeting<el-icon class="el-icon--right"><arrow-down /></el-icon>
          </span>
          <template #dropdown>
            <el-dropdown-menu>
              <el-dropdown-item>
                <el-checkbox v-model="with_rawdata" label="With Rawdata"></el-checkbox>
              </el-dropdown-item>
            </el-dropdown-menu>
          </template>
        </el-dropdown>
      </li>
      <li @click="handleSetting">
        <div>
          <svg width="88" height="88" viewBox="0 0 88 88" fill="none" xmlns="http://www.w3.org/2000/svg">
          <path fill-rule="evenodd" clip-rule="evenodd" d="M0 44C0 30.0558 0 23.0837 2.34468 17.6084C5.28236 10.7483 10.7483 5.28236 17.6084 2.34468C23.0837 0 30.0558 0 44 0C57.9442 0 64.9163 0 70.3916 2.34468C77.2517 5.28236 82.7176 10.7483 85.6553 17.6084C88 23.0837 88 30.0558 88 44C88 57.9442 88 64.9163 85.6553 70.3916C82.7176 77.2517 77.2517 82.7176 70.3916 85.6553C64.9163 88 57.9442 88 44 88C30.0558 88 23.0837 88 17.6084 85.6553C10.7483 82.7176 5.28236 77.2517 2.34468 70.3916C0 64.9163 0 57.9442 0 44Z" fill="#0E72ED"/>
          <path fill-rule="evenodd" clip-rule="evenodd" d="M33.1927 31.8058C32.0161 31.4706 30.7299 31.7829 29.9637 32.7366C28.824 34.155 27.8949 35.7498 27.2239 37.4735C26.7802 38.613 27.1532 39.8824 28.0314 40.7333L28.9017 41.5767C30.27 42.9025 30.27 45.0975 28.9017 46.4233L28.0314 47.2667C27.1532 48.1176 26.7802 49.387 27.2239 50.5265C27.8949 52.2502 28.824 53.845 29.9637 55.2634C30.7299 56.2171 32.0161 56.5294 33.1927 56.1942L34.3522 55.8638C36.1845 55.3417 38.0855 56.4392 38.5495 58.2871L38.844 59.4598C39.1422 60.6472 40.0569 61.6055 41.2672 61.7899C42.1583 61.9257 43.0709 61.9961 44 61.9961C44.929 61.9961 45.8416 61.9257 46.7327 61.7899C47.9431 61.6055 48.8578 60.6472 49.156 59.4598L49.4504 58.2871C49.9145 56.4392 51.8154 55.3417 53.6477 55.8638L54.8073 56.1942C55.9838 56.5294 57.27 56.2171 58.0363 55.2634C59.1759 53.845 60.105 52.2502 60.7761 50.5265C61.2197 49.387 60.8467 48.1176 59.9686 47.2667L59.0982 46.4233C57.7299 45.0975 57.7299 42.9025 59.0982 41.5767L59.9686 40.7333C60.8467 39.8824 61.2197 38.613 60.7761 37.4735C60.105 35.7498 59.1759 34.155 58.0363 32.7366C57.27 31.7829 55.9838 31.4706 54.8073 31.8058L53.6477 32.1362C51.8154 32.6583 49.9145 31.5608 49.4504 29.7129L49.156 28.5402C48.8578 27.3528 47.9431 26.3945 46.7327 26.2101C45.8416 26.0743 44.929 26.0039 44 26.0039C43.0709 26.0039 42.1583 26.0743 41.2672 26.2101C40.0569 26.3945 39.1422 27.3528 38.844 28.5402L38.5495 29.7129C38.0855 31.5608 36.1845 32.6583 34.3522 32.1362L33.1927 31.8058ZM43.9995 50.7485C47.7266 50.7485 50.7481 47.7271 50.7481 44C50.7481 40.2729 47.7266 37.2514 43.9995 37.2514C40.2724 37.2514 37.251 40.2729 37.251 44C37.251 47.7271 40.2724 50.7485 43.9995 50.7485Z" fill="white"/>
          </svg>
        </div> 
        <div>Setting</div>
      </li>
      <li @click="handleMeeting">
        <div>
          <svg width="88" height="88" viewBox="0 0 88 88" fill="none" xmlns="http://www.w3.org/2000/svg">
          <path fill-rule="evenodd" clip-rule="evenodd" d="M0 44C0 30.0558 0 23.0837 2.34468 17.6084C5.28236 10.7483 10.7483 5.28236 17.6084 2.34468C23.0837 0 30.0558 0 44 0C57.9442 0 64.9163 0 70.3916 2.34468C77.2517 5.28236 82.7176 10.7483 85.6553 17.6084C88 23.0837 88 30.0558 88 44C88 57.9442 88 64.9163 85.6553 70.3916C82.7176 77.2517 77.2517 82.7176 70.3916 85.6553C64.9163 88 57.9442 88 44 88C30.0558 88 23.0837 88 17.6084 85.6553C10.7483 82.7176 5.28236 77.2517 2.34468 70.3916C0 64.9163 0 57.9442 0 44Z" fill="#0E72ED"/>
          <path fill-rule="evenodd" clip-rule="evenodd" d="M44 62.3333C54.1252 62.3333 62.3334 54.1252 62.3334 44C62.3334 33.8748 54.1252 25.6667 44 25.6667C33.8748 25.6667 25.6667 33.8748 25.6667 44C25.6667 54.1252 33.8748 62.3333 44 62.3333ZM44 33.4583C44.7594 33.4583 45.375 34.0739 45.375 34.8333V43.4305L49.5556 47.6111C50.0926 48.148 50.0926 49.0186 49.5556 49.5556C49.0186 50.0926 48.148 50.0926 47.6111 49.5556L43.0288 44.9734L43.0188 44.9633C42.7752 44.7152 42.625 44.3751 42.625 44V34.8333C42.625 34.0739 43.2406 33.4583 44 33.4583Z" fill="white"/>
          </svg>
        </div> 
        <div>Meeting</div>
      </li>
    </ul>
    <JoinMeetingLayer :withRawdata="with_rawdata" ref="joinRef" />
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { useRouter } from 'vue-router'
import { useStore } from 'vuex'
import { ZoomMeetingStatus, ZoomSDKError } from '../../lib/settings.js'
import { ArrowDown } from '@element-plus/icons-vue'
import { toastResult } from '../assets/ts/util'
import JoinMeetingLayer from '@/components/JoinMeetingLayer.vue'
const remote = window.require('@electron/remote')
const router = useRouter()
const store = useStore()
let zoomMeeting = store.state.zoomMeeting
const version = ref()
const joinRef = ref()
const with_rawdata = ref(false)
const disabled = ref(false)
let demo_render_window: Window|null, demo_meeting_window: Window|null

const meetingstatuscb = (meetingStatus: number, result: number) => {
  console.log('meetingstatuscb', meetingStatus, result)
  if (demo_render_window && !demo_render_window.closed) {
    demo_render_window.postMessage({ meetingStatus, result }, '*')
  }
  if (demo_meeting_window && !demo_meeting_window.closed) {
    demo_meeting_window.postMessage({ meetingStatus, result }, '*')
  }
  if (meetingStatus == ZoomMeetingStatus.MEETING_STATUS_INMEETING) {
    disabled.value = true
  } else {
    disabled.value = false
    if (meetingStatus == ZoomMeetingStatus.MEETING_STATUS_DISCONNECTING && demo_render_window && !demo_render_window.closed) {
      demo_render_window.close()
    }
  }
}

const startMeeting = () => {
  if (disabled.value) {
    return
  }
  let ret = zoomMeeting.StartMeeting({withRawdata: with_rawdata.value})
  console.log('StartMeeting', ret)
  if (with_rawdata.value) {
    if (ret != ZoomSDKError.SDKERR_SUCCESS) {
      toastResult('please call SetPipeName interface to set pipe name')
      return
    }
    let routeData = router.resolve({ 
      path: '/rawdata'
    })
    demo_render_window = window.open(routeData.href, 'Rawdata', 'width=1000,height=900,nodeIntegration=yes,contextIsolation=no')
  }
} 

const joinMeeting = () => {
  if (disabled.value) {
    return
  }
  joinRef.value.dialogVisible=true
}

const handleSetting = () => {
  let routeData = router.resolve({ 
    path: '/setting/general'
  })
  window.open(routeData.href, 'Setting', 'width=700,height=900,nodeIntegration=yes,contextIsolation=no')
}

const handleMeeting = () => {
  let routeData = router.resolve({ 
    path: '/meeting/meeting'
  })
  demo_meeting_window = window.open(routeData.href, 'Meeting', 'width=1600,height=900,nodeIntegration=yes,contextIsolation=no')
}

onMounted(() => {
  if (remote.app.zoomSdkModule) {
    zoomMeeting = remote.app.zoomSdkModule.Meeting
  }
  let pipeParams = {
    videoPipeName: 'videoPipeDemo',
    sharePipeName: 'sharePipeDemo',
    audioPipeName: 'audioPipeDemo',
    maxReadLength: 1920 * 1080 * 3 / 2
  }
  zoomMeeting.SetPipeName(pipeParams)
  zoomMeeting.SetMeetingStatusCB(meetingstatuscb)
  version.value = remote.app.zoomSdk.GetZoomSDKVersion()
})
</script>

<style scoped lang="scss">
.board {
  ul {
    list-style: none;
    width: 320px;
    position: absolute;
    top: 50%;
    left: 50%;
    transform: translate(-50%, -40%);
    margin: 0;
    padding: 0;
    li {
      width: 130px;
      height: 126px;
      margin: 15px;
      font-size: 15px;
      color: #606266;
      text-align: center;
      float: left;
      cursor: pointer;
      .disabled {
        cursor: not-allowed;
      }
      .el-dropdown-link {
        font-size: 15px;
      }
      &>div:nth-child(2) {
        margin-top: 3px;
      }
    } 
  }
}
</style>
