<template>
  <div class="meeting">
    <div class="in-line">
      <span>H323 Address</span>
      <el-select v-model="h323_address_value" placeholder="Select">
        <el-option v-for="(item, index) in h323Address" :key="index" :label="item.H323Address" :value="item.H323Address"></el-option>
      </el-select>
    </div>
    <div class="in-line">
      <el-select v-model="h323_device_value" placeholder="Select">
        <el-option v-for="(item, index) in h323DeviceList" :key="index" :label="item.H323DeviceName" :value="item.H323DeviceIP"></el-option>
      </el-select>
      <el-button type="primary" round plain @click="callOutH323">CallOutH323</el-button>
    </div>
    <el-button type="primary" round plain @click="setH323CallOutStatusCB">SetH323CallOutStatusCB</el-button>
    <el-button type="primary" round plain @click="cancelCallOutH323">CancelCallOutH323</el-button>
    <el-button type="primary" round plain @click="getH323Password">GetH323Password</el-button>
  </div>
  <div class="result">Result: {{result}}</div>
  <div class="result"> 
    <span class="title">onH323CalloutStatusNotify <el-button size="small" @click="onH323CalloutStatusNotify_options=[]">Clear</el-button></span>  
    <div class="callback-list">
      <span v-for="(item, index) in onH323CalloutStatusNotify_options" :key="index">{{index}} : {{item}}</span>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { ZoomH323CalloutStatus } from '../../../lib/settings.js'
import { getSDKResult } from '../../assets/ts/util'
const remote = window.require('@electron/remote')
const result = ref()
const onH323CalloutStatusNotify_options = ref<any[]>([])

const onH323CalloutStatusNotify = (status: any) => {
  console.log('onH323CalloutStatusNotify', status)
  let statusContent = Object.keys(ZoomH323CalloutStatus).find(k => ZoomH323CalloutStatus[k] == status)
  onH323CalloutStatusNotify_options.value.push(statusContent)
}
const zoomMeetingH323 = remote.app.zoomSdkModule.Meeting.GetMeetingH323({onH323CalloutStatusNotify})
const h323Address = ref(zoomMeetingH323.H323_GetH323Address())
const h323DeviceList = ref(zoomMeetingH323.H323_GetCalloutH323DeviceList())
const h323_address_value = ref()
const h323_device_value = ref()

const setH323CallOutStatusCB = () => {
  let ret = zoomMeetingH323.H323_SetH323CallOutStatusCB(onH323CalloutStatusNotify)
  result.value = getSDKResult('H323_SetH323CallOutStatusCB', ret)
}

const callOutH323 = () => {
  let obj: any = {};
  obj = h323DeviceList.value.find((item: any)=>{
    return item.H323DeviceIP === h323_device_value.value;
  });
  let param = {
    deviceName: obj.H323DeviceName,
    deviceIP: obj.H323DeviceIP,
    deviceE164num: obj.H323DeviceE164Name,
    type: obj.H323DeviceType
  }
  let ret = zoomMeetingH323.H323_CallOutH323(param)
  result.value = getSDKResult('H323_CallOutH323', ret, param)
}

const cancelCallOutH323 = () => {
  let ret = zoomMeetingH323.H323_CancelCallOutH323()
  result.value = getSDKResult('H323_CancelCallOutH323', ret)
}

const getH323Password = () => {
  let ret = zoomMeetingH323.H323_GetH323Password();
  result.value = getSDKResult('H323_GetH323Password', ret)
}

onMounted(() => {
  if (h323Address.value.length > 0) {
    h323_address_value.value = h323Address.value[0].H323Address
  }
  if (h323DeviceList.value.length > 0) {
    h323_device_value.value = h323DeviceList.value[0].H323DeviceIP
  }
})
</script>