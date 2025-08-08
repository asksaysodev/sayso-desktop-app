import { onMounted } from 'vue'
import { getSDKResult } from '../util'

export default function (libuvAddon: any, zoomRawData: any) {
  const audioLibuvClientObj = libuvAddon.GetAudioLibuvClientObj()

  const startAudioClient = () => {
    audioLibuvClientObj.StartAudioClient()
  }

  const stopAudioClient = () => {
    audioLibuvClientObj.StopAudioClient()
  }
  
  const onMixedAudioRawDataReceived = (databuf: any, data_format: string) => {
    console.log('onMixedAudioRawDataReceived', databuf, data_format)
  }
  
  const onOneWayAudioRawDataReceived = (databuf: any, data_format: string, user_id: number) => {
    console.log('onOneWayAudioRawDataReceived', databuf, data_format, user_id)
  }
  
  const setRenderAudioRawDataCB = () => {
    audioLibuvClientObj.SetRenderAudioRawDataCB(onMixedAudioRawDataReceived, onOneWayAudioRawDataReceived)
  }
  
  const initAudioRawDataHelper = () => {
    zoomRawData.InitAudioRawDataHelper()
  }
  
  const handleSubscribeAudio = () => {
    zoomRawData.SubscribeAudioRawdata()
  }
  
  const handleUnsubscribeAudio = () => {
    zoomRawData.UnSubscribeAudioRawdata()
  }
  
  onMounted(() => {
    startAudioClient()
    setRenderAudioRawDataCB()
    initAudioRawDataHelper()
  })

  return {
    handleSubscribeAudio,
    handleUnsubscribeAudio,
    stopAudioClient
  }
}