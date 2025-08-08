const { ZoomSDKError } = require('./settings.js');

var ZoomMeetingRequestRawLiveStreamPrivilegeHandler = (function () {
  let instance;
  /**
   * Zoom Meeting Request Raw Live Stream Privilege Handler
   * @module zoom_meeting_request_raw_live_stream_privilege_handler
   * @return {ZoomMeetingRequestRawLiveStreamPrivilegeHandler}
   */
  function init(opts) {
    
    let clientOpts = opts || {};

    // Private methods and variables
    let _addon = clientOpts.addon.GetRequestRawLiveStreamPrivilegeHandler() || null;

    return {
      /** 
      * Get the request ID.
      * @method GetRequestId
      * @return {String} If the function succeeds, the return value is the request ID.
      */
      GetRequestId: function () {
        if (_addon) {
          return _addon.GetRequestId();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE
      },
      /** 
      * Get the user ID who requested privilege.
      * @method GetRequesterId
      * @return {Number} If the function succeeds, the return value is the user ID. Otherwise, this return 0.
      */
      GetRequesterId: function () {
        if (_addon) {
          return _addon.GetRequesterId();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE
      },
      /** 
      * Get the user name who requested privileges.
      * @method GetRequesterName
      * @return {String} If the function succeeds, the return value is the user name.
      */
      GetRequesterName: function () {
        if (_addon) {
          return _addon.GetRequesterName();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE
      },
      /** 
      * Get the broadcast URL.
      * @method GetBroadcastUrl
      * @return {String} If the function succeeds, the return value is the broadcast URL.
      */
      GetBroadcastUrl: function () {
        if (_addon) {
          return _addon.GetBroadcastUrl();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE
      },
      /** 
      * Allows the user to start raw live stream and finally self-destroy.
      * @method GrantRawLiveStreamPrivilege
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
        Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      GrantRawLiveStreamPrivilege: function () {
        if (_addon) {
          return _addon.GrantRawLiveStreamPrivilege();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE
      },
      /** 
      * Denies the user permission to start raw live stream and finally self-destroy.
      * @method DenyRawLiveStreamPrivilege
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
        Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      DenyRawLiveStreamPrivilege: function () {
        if (_addon) {
          return _addon.DenyRawLiveStreamPrivilege();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE
      }
    };
  };
  return {
    getInstance: function (opts) {
      if (!instance) {
        instance = init(opts)
      }
      return instance
    }
  };
})();

module.exports = {
  ZoomMeetingRequestRawLiveStreamPrivilegeHandler: ZoomMeetingRequestRawLiveStreamPrivilegeHandler
}