import { createStore } from 'vuex'

export default createStore({
  state: {
    zoomAuth: {},
    zoomMeeting: {}
  },
  mutations: {
    setZoomAuth (state, zoomAuth) {
      state.zoomAuth = zoomAuth
    },
    setZoomMeeting (state, zoomMeeting) {
      state.zoomMeeting = zoomMeeting
    }
  },
  actions: {
  },
  modules: {
  }
})
