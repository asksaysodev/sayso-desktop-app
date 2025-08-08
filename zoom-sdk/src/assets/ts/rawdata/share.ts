import { onMounted } from 'vue'
import { ZoomSDKRawDataType } from '../../../../lib/settings.js'
import { getSDKResult } from '../util'

export default function (libuvAddon: any, zoomRawData: any) {
  const shareLibuvClientObj = libuvAddon.GetShareLibuvClientObj()
  const shareRecvHandle = 4

  const startShareClient = () => {
    shareLibuvClientObj.StartShareClient()
  }

  const stopShareClient = () => {
    shareLibuvClientObj.StopShareClient()
  }
  
  const handleSubscribeShare = (resolution: number, subscribeId: number) => {
    let resolutionObj = {
      resolution,
      recv_handle: shareRecvHandle
    }
    zoomRawData.SetRawDataResolution(resolutionObj)
    let obj = {
      subscribeId,
      rawdataType: ZoomSDKRawDataType.RAW_DATA_TYPE_SHARE,
      recv_handle: shareRecvHandle
    }
    zoomRawData.Subscribe(obj)
  }

  const handleUnSubscribeShare = () => {
    let obj = {
      recv_handle: shareRecvHandle
    }
    zoomRawData.UnSubscribe(obj)
  }

  const changeShareRawDataResolution = (resolution: number) => {
    let obj = {
      resolution,
      recv_handle: shareRecvHandle
    }
    zoomRawData.SetRawDataResolution(obj)
  }
  
  onMounted(() => {
    startShareClient()
  })

  return {
    shareLibuvClientObj,
    shareRecvHandle, 
    handleSubscribeShare,
    handleUnSubscribeShare,
    changeShareRawDataResolution,
    stopShareClient
  }
}