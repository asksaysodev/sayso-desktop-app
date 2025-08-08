import { onMounted } from 'vue'
const remote = window.require('@electron/remote')
const messages = require('../../../../lib/electron_sdk_pb.js');

export default function (libuvAddon: any, platform: string) {
  let pipeParams = remote.app.pipeParams
  
  const setPipeClientInitParam = () => {
    if (libuvAddon) {
      try {
        let SetPipeInitParams = new messages.SetPipeInitParams();
        SetPipeInitParams.setVideopipename(pipeParams.videoPipeName);
        SetPipeInitParams.setSharepipename(pipeParams.sharePipeName);
        SetPipeInitParams.setAudiopipename(pipeParams.audioPipeName);
        SetPipeInitParams.setMaxreadlength(pipeParams.maxReadLength);
        let bytes = SetPipeInitParams.serializeBinary();
        let result = libuvAddon.SetPipeClientInitParam(bytes);
        console.log('SetPipeClientInitParam', result, SetPipeInitParams)
        return result
      } catch (error) {
        return error
      }
    }
  }
  
  onMounted(() => {
    setPipeClientInitParam()
  })
}