import { ref, onMounted } from 'vue'
import { ZoomSDKRawDataType, SDKRawDataError } from '../../../../lib/settings.js'
import { getSDKResult } from '../util'
const remote = window.require('@electron/remote')
const zoomMeetingParticipants = remote.app.zoomSdkModule.Meeting.GetMeetingParticipantsCtrl()

export default function (libuvAddon: any, zoomRawData: any) {
  const videoLibuvClientObj = libuvAddon.GetVideoLibuvClientObj()
  const subscribedVideoList = ref<any[]>([])
  
  const startVideoClient = () => {
    videoLibuvClientObj.StartVideoClient()
  }

  const stopVideoClient = () => {
    videoLibuvClientObj.StopVideoClient()
  }

  const startPreviewDevice = () => {
    let obj = {
      subscribeId: 0,
      rawdataType: ZoomSDKRawDataType.RAW_DATA_TYPE_VIDEO,
      recv_handle: 3
    }
    zoomRawData.Subscribe(obj)
  }
  
  const handleSubscribeVideo = (resolution: number, recv_handle: number, subscribeId: number) => {
    let obj = {resolution, recv_handle}
    let ret = zoomRawData.SetRawDataResolution(obj)
    let subscribeObj = {
      subscribeId,
      rawdataType: ZoomSDKRawDataType.RAW_DATA_TYPE_VIDEO,
      recv_handle: recv_handle
    }
    ret = zoomRawData.Subscribe(subscribeObj)
    if (ret == SDKRawDataError.SDKRawDataError_SUCCESS || ret == SDKRawDataError.SDKRawDataError_NO_VIDEO_DATA) {
      subscribedVideoList.value[recv_handle] = zoomMeetingParticipants.GetUserInfoByUserID(subscribeId)
    }
  }

  const handleUnSubscribeVideo = (recv_handle: number) => {
    let obj = {recv_handle}
    let ret = zoomRawData.UnSubscribe(obj);
    if (ret == SDKRawDataError.SDKRawDataError_SUCCESS) {
      subscribedVideoList.value[recv_handle] = null
    }
  }

  const changeVideoRawDataResolution = (resolution: number, recv_handle: number) => {
    let obj = {resolution, recv_handle}
    zoomRawData.SetRawDataResolution(obj)
  }
  
  onMounted(() => {
    startVideoClient()
  })

  return {
    videoLibuvClientObj,
    subscribedVideoList, 
    startPreviewDevice, 
    handleSubscribeVideo,
    handleUnSubscribeVideo,
    changeVideoRawDataResolution,
    stopVideoClient
  }
}