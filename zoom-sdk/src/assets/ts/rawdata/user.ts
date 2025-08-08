import { ref, onMounted } from 'vue'
import { getSDKResult } from '../util'
const remote = window.require('@electron/remote')
const zoomMeetingParticipants = remote.app.zoomSdkModule.Meeting.GetMeetingParticipantsCtrl()

export default function () {
  const active_user = ref()
  const userList = ref<any[]>([])
  
  const getParticipantsList = async() => {
    userList.value = []
    let tempList = await zoomMeetingParticipants.GetParticipantsList();
    for (let i = 0; i < tempList.length; i++) {
      let userObj = await zoomMeetingParticipants.GetUserInfoByUserID(tempList[i].userid)
      let obj = {
        value: userObj.userID,
        label: `${userObj.userName} - ${userObj.isVideoOn ? 'video on': 'video off'}`
      }
      await userList.value.push(obj)
    }
    if (!active_user.value && userList.value.length > 0) {
      active_user.value = userList.value[0].value
    }
    return userList
  }
  
  const onHostChangeNotification = (userid: number) => {
    console.log('onHostChangeNotification', userid);
  }
  
  onMounted(() => {
    zoomMeetingParticipants.SetMeetingHostChangeCB(onHostChangeNotification)
  })

  return {
    active_user,
    userList,
    getParticipantsList
  }
}