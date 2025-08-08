const { ZoomSDKError } = require('./settings.js');
const messages = require('./electron_sdk_pb.js');

var ZoomMeetingLiveStreamCtrl = (function () {
  let instance;
  /**
   * Zoom Meeting Live Stream Ctrl
   * @module zoom_meeting_live_stream_ctrl
   * @return {ZoomMeetingLiveStreamCtrl}
   */
  function init(opts) {
    
    let clientOpts = opts || {};

    // Private methods and variables
    let _addon = clientOpts.addon.GetMeetingLiveStreamCtrl() || null;
    let _onLiveStreamStatusChange = clientOpts.onLiveStreamStatusChange || null;
    let _onRawLiveStreamPrivilegeChanged = clientOpts.onRawLiveStreamPrivilegeChanged || null;
    let _onUserRawLiveStreamPrivilegeChanged = clientOpts.onUserRawLiveStreamPrivilegeChanged || null;
    let _onRawLiveStreamPrivilegeRequested = clientOpts.onRawLiveStreamPrivilegeRequested || null;
    let _onUserRawLiveStreamingStatusChanged = clientOpts.onUserRawLiveStreamingStatusChanged || null;
    let _onRawLiveStreamPrivilegeRequestTimeout = clientOpts.onRawLiveStreamPrivilegeRequestTimeout || null;
    let _onLiveStreamReminderStatusChanged = clientOpts.onLiveStreamReminderStatusChanged || null;
    let _onLiveStreamReminderStatusChangeFailed = clientOpts.onLiveStreamReminderStatusChangeFailed || null;
    let _onUserThresholdReachedForLiveStream = clientOpts.onUserThresholdReachedForLiveStream || null;

    /**
      Callback event when live stream status changes.
      @event onLiveStreamStatusChange
      @param {String} status Live stream status. For more details, see {@link LiveStreamStatus} enum.
    */
    function onLiveStreamStatusChange(status) {
      if (_onLiveStreamStatusChange) {
        _onLiveStreamStatusChange(status)
      }
    }

    /**
      Callback event when the current user's raw live streaming privilege changes.
      @event onRawLiveStreamPrivilegeChanged
      @param {Boolean} hasPrivilege Specify whether or not the user has privileg.
    */
    function onRawLiveStreamPrivilegeChanged(hasPrivilege) {
      if (_onRawLiveStreamPrivilegeChanged) {
        _onRawLiveStreamPrivilegeChanged(hasPrivilege)
      }
    }

    /**
      Callback event when another user's raw live streaming privilege has changed.
      @event onUserRawLiveStreamPrivilegeChanged
      @param {Number} userid The ID of the user whose privilege has changed.
      @param {Boolean} hasPrivilege Specify whether the user has privilege or not.
    */
    function onUserRawLiveStreamPrivilegeChanged(userid, hasPrivilege) {
      if (_onUserRawLiveStreamPrivilegeChanged) {
        _onUserRawLiveStreamPrivilegeChanged(userid, hasPrivilege)
      }
    }

    /**
      Callback event when a user requests raw live streaming privilege.
      @event onRawLiveStreamPrivilegeRequested
    */
    function onRawLiveStreamPrivilegeRequested() {
      if (_onRawLiveStreamPrivilegeRequested) {
        _onRawLiveStreamPrivilegeRequested()
      }
    }

    /**
      Callback event when users start/stop raw live streaming.
      @event onUserRawLiveStreamingStatusChanged
      @param {Array} livingList list of users with an active raw live stream. contains object with broadcastUrl and userId
    */
    function onUserRawLiveStreamingStatusChanged(livingList) {
      if (_onUserRawLiveStreamingStatusChanged) {
        _onUserRawLiveStreamingStatusChanged(livingList)
      }
    }

    /**
      Callback event when the current user's request has time out.
      @event onRawLiveStreamPrivilegeRequestTimeout
    */
    function onRawLiveStreamPrivilegeRequestTimeout() {
      if (_onRawLiveStreamPrivilegeRequestTimeout) {
        _onRawLiveStreamPrivilegeRequestTimeout()
      }
    }

    /**
      Callback event when the live stream reminder enable status changes.
      @event onLiveStreamReminderStatusChanged
      @param {Boolean} enable True means the live stream reminder is enabled.
    */
    function onLiveStreamReminderStatusChanged(enable) {
      if (_onLiveStreamReminderStatusChanged) {
        _onLiveStreamReminderStatusChanged(enable)
      }
    }

    /**
      Callback event when the live stream reminder enable status change fails.
      @event onLiveStreamReminderStatusChangeFailed
    */
    function onLiveStreamReminderStatusChangeFailed() {
      if (_onLiveStreamReminderStatusChangeFailed) {
        _onLiveStreamReminderStatusChangeFailed()
      }
    }

    /**
      Callback event when the meeting or webinar user has nearly reached the meeting capacity, like 80% or 100% for the meeting or webinar capacity. The host can start live stream to let unjoined user watch live stream.
      @event onUserThresholdReachedForLiveStream
      @param {Number} percent Proportion of joined users to the total capacity.
    */
    function onUserThresholdReachedForLiveStream(percent) {
      if (_onUserThresholdReachedForLiveStream) {
        _onUserThresholdReachedForLiveStream(percent)
      }
    }

    if (_addon) {
      _addon.SetOnLiveStreamStatusChangeCB(onLiveStreamStatusChange);
      _addon.SetOnRawLiveStreamPrivilegeChangedCB(onRawLiveStreamPrivilegeChanged);
      _addon.SetOnUserRawLiveStreamPrivilegeChangedCB(onUserRawLiveStreamPrivilegeChanged);
      _addon.SetOnRawLiveStreamPrivilegeRequestedCB(onRawLiveStreamPrivilegeRequested);
      _addon.SetOnUserRawLiveStreamingStatusChangedCB(onUserRawLiveStreamingStatusChanged);
      _addon.SetOnRawLiveStreamPrivilegeRequestTimeoutCB(onRawLiveStreamPrivilegeRequestTimeout);
      _addon.SetOnLiveStreamReminderStatusChangedCB(onLiveStreamReminderStatusChanged);
      _addon.SetOnLiveStreamReminderStatusChangeFailedCB(onLiveStreamReminderStatusChangeFailed);
      _addon.SetOnUserThresholdReachedForLiveStreamCB(onUserThresholdReachedForLiveStream);
    }

    return {
      /**
       * Set the callback of live stream status change.
       * @method SetOnLiveStreamStatusChangeCB
       * @param {Function} onLiveStreamStatusChange
       * @return {Boolean}
       */
      SetOnLiveStreamStatusChangeCB: function (onLiveStreamStatusChange) {
        if (_addon && onLiveStreamStatusChange && onLiveStreamStatusChange instanceof Function) {
          _onLiveStreamStatusChange = onLiveStreamStatusChange;
          return true
        }
        return false
      },
      /**
       * Set the callback of raw live stream privilege changed.
       * @method SetOnRawLiveStreamPrivilegeChangedCB
       * @param {Function} onRawLiveStreamPrivilegeChanged
       * @return {Boolean}
       */
      SetOnRawLiveStreamPrivilegeChangedCB: function (onRawLiveStreamPrivilegeChanged) {
        if (_addon && onRawLiveStreamPrivilegeChanged && onRawLiveStreamPrivilegeChanged instanceof Function) {
          _onRawLiveStreamPrivilegeChanged = onRawLiveStreamPrivilegeChanged;
          return true
        }
        return false
      },
      /**
       * Set the callback of user raw live stream privilege changed.
       * @method SetOnUserRawLiveStreamPrivilegeChangedCB
       * @param {Function} onUserRawLiveStreamPrivilegeChanged
       * @return {Boolean}
       */
      SetOnUserRawLiveStreamPrivilegeChangedCB: function (onUserRawLiveStreamPrivilegeChanged) {
        if (_addon && onUserRawLiveStreamPrivilegeChanged && onUserRawLiveStreamPrivilegeChanged instanceof Function) {
          _onRawLiveStreamPrivilegeChanged = onUserRawLiveStreamPrivilegeChanged;
          return true
        }
        return false
      },
      /**
       * Set the callback of raw live stream privilege requested.
       * @method SetOnRawLiveStreamPrivilegeRequestedCB
       * @param {Function} onRawLiveStreamPrivilegeRequested
       * @return {Boolean}
       */
      SetOnRawLiveStreamPrivilegeRequestedCB: function (onRawLiveStreamPrivilegeRequested) {
        if (_addon && onRawLiveStreamPrivilegeRequested && onRawLiveStreamPrivilegeRequested instanceof Function) {
          _onRawLiveStreamPrivilegeRequested = onRawLiveStreamPrivilegeRequested;
          return true
        }
        return false
      },
      /**
       * Set the callback of user raw live streaming status changed.
       * @method SetOnUserRawLiveStreamingStatusChangedCB
       * @param {Function} onUserRawLiveStreamingStatusChanged
       * @return {Boolean}
       */
      SetOnUserRawLiveStreamingStatusChangedCB: function (onUserRawLiveStreamingStatusChanged) {
        if (_addon && onUserRawLiveStreamingStatusChanged && onUserRawLiveStreamingStatusChanged instanceof Function) {
          _onUserRawLiveStreamingStatusChanged = onUserRawLiveStreamingStatusChanged;
          return true
        }
        return false
      },
      /**
       * Set the callback of raw live stream privilege request timeout.
       * @method SetOnRawLiveStreamPrivilegeRequestTimeoutCB
       * @param {Function} onRawLiveStreamPrivilegeRequestTimeout
       * @return {Boolean}
       */
      SetOnRawLiveStreamPrivilegeRequestTimeoutCB: function (onRawLiveStreamPrivilegeRequestTimeout) {
        if (_addon && onRawLiveStreamPrivilegeRequestTimeout && onRawLiveStreamPrivilegeRequestTimeout instanceof Function) {
          _onRawLiveStreamPrivilegeRequestTimeout = onRawLiveStreamPrivilegeRequestTimeout;
          return true
        }
        return false
      },
      /**
       * Set callback event when the live stream reminder enable status changes.
       * @method SetOnLiveStreamReminderStatusChangedCB
       * @param {Function} onLiveStreamReminderStatusChanged
       * @return {Boolean}
       */
      SetOnLiveStreamReminderStatusChangedCB: function (onLiveStreamReminderStatusChanged) {
        if (_addon && onLiveStreamReminderStatusChanged && onLiveStreamReminderStatusChanged instanceof Function) {
          _onLiveStreamReminderStatusChanged = onLiveStreamReminderStatusChanged;
          return true
        }
        return false
      },
      /**
       * Set callback event when the live stream reminder enable status change fails.
       * @method SetOnLiveStreamReminderStatusChangeFailedCB
       * @param {Function} onLiveStreamReminderStatusChangeFailed
       * @return {Boolean}
       */
      SetOnLiveStreamReminderStatusChangeFailedCB: function (onLiveStreamReminderStatusChangeFailed) {
        if (_addon && onLiveStreamReminderStatusChangeFailed && onLiveStreamReminderStatusChangeFailed instanceof Function) {
          _onLiveStreamReminderStatusChangeFailed = onLiveStreamReminderStatusChangeFailed;
          return true
        }
        return false
      },
      /**
       * Set callback event when the meeting or webinar user has nearly reached the meeting capacity, like 80% or 100% for the meeting or webinar capacity.
       * @method SetOnUserThresholdReachedForLiveStreamCB
       * @param {Function} onUserThresholdReachedForLiveStream
       * @return {Boolean}
       */
      SetOnUserThresholdReachedForLiveStreamCB: function (onUserThresholdReachedForLiveStream) {
        if (_addon && onUserThresholdReachedForLiveStream && onUserThresholdReachedForLiveStream instanceof Function) {
          _onUserThresholdReachedForLiveStream = onUserThresholdReachedForLiveStream;
          return true
        }
        return false
      },
      /** 
      * Determine if it is able to start live streaming.
      * @method CanStartLiveStream
      * @return {Number} If it is enabled to start the live streaming, the return value is SDKERR_SUCCESS.
        Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      CanStartLiveStream: function () {
        if (_addon) {
          return _addon.CanStartLiveStream();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE
      },
      /** 
      * Start live streaming.
      * @method StartLiveStream
      * @param {Number} liveStreamHandle the handle of live stream item
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      StartLiveStream: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let liveStreamHandle = clientOpts.liveStreamHandle;
          if (!liveStreamHandle) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER; 
          }
          try {
            let StartLiveStreamParams = new messages.StartLiveStreamParams();
            StartLiveStreamParams.setLivestreamhandle(Number(liveStreamHandle));
            let bytes = StartLiveStreamParams.serializeBinary();
            return _addon.StartLiveStream(bytes);            
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE
      },
      /** 
      * Start live streaming. Get the parameters from the third party of live stream service
      * @method StartLiveStreamWithSteamingURL
      * @param {String} streamingURL The URL of live streaming.
      * @param {String} streamingKey The key of live streaming. 
      * @param {String} broadcastURL The broadcast URL of live-stream.
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      StartLiveStreamWithSteamingURL: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let streamingURL = clientOpts.streamingURL;
          let streamingKey = clientOpts.streamingKey;
          let broadcastURL = clientOpts.broadcastURL;
          try {
            let StartLiveStreamWithSteamingURLParams = new messages.StartLiveStreamWithSteamingURLParams();
            StartLiveStreamWithSteamingURLParams.setStreamingurl(streamingURL);
            StartLiveStreamWithSteamingURLParams.setStreamingkey(streamingKey);
            StartLiveStreamWithSteamingURLParams.setBroadcasturl(broadcastURL);
            let bytes = StartLiveStreamWithSteamingURLParams.serializeBinary();
            return _addon.StartLiveStreamWithSteamingURL(bytes);            
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE
      },
      /** 
      * Stop live streaming.
      * @method StopLiveStream
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
        Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      StopLiveStream: function () {
        if (_addon) {
          return _addon.StopLiveStream();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE
      },
      /** 
      * Get the list of handles used by live streaming in the current meeting. You can get live stream information by GetLiveStreamItemInfo interface.
      * @method GetSupportLiveStreamItems
      * @return {Array} If the function succeeds, the return value is the meeting information to be live streamed.
        Otherwise failed, the return value is NULL
      */
      GetSupportLiveStreamItems: function () {
        if (_addon) {
          return _addon.GetSupportLiveStreamItems();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE
      },
      /** 
      * Get the current live stream object. 
      * @method GetCurrentLiveStreamItem
      * @return {Object} If the function succeeds, the return value is the current live stream object.
        Otherwise failed, the return value is NULL
      */
      GetCurrentLiveStreamItem: function () {
        if (_addon) {
          return _addon.GetCurrentLiveStreamItem();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE
      },
      /** 
      * Get live stream status of current meeting.
      * @method GetCurrentLiveStreamStatus
      * @return {Number} If the function succeeds, the return value is the live stream status of current meeting.  
	      Otherwise failed. For more details, see {@link LiveStreamStatus}.
      */
      GetCurrentLiveStreamStatus: function () {
        if (_addon) {
          return _addon.GetCurrentLiveStreamStatus();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE
      },
      /** 
      * Stop raw live streaming.
      * @method StopRawLiveStream
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
        Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      StopRawLiveStream: function () {
        if (_addon) {
          return _addon.StopRawLiveStream();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE
      },
      /** 
      * get the live stream item info.
      * @method GetLiveStreamItemInfo
      * @param {Number} liveStreamHandle the handle of live stream
      * @return {Object} contains liveStreamURL, liveStreamURLDescription, liveStreamViewerURL
      */
      GetLiveStreamItemInfo: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let liveStreamHandle = clientOpts.liveStreamHandle;
          if (!liveStreamHandle) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER; 
          }
          try {
            let GetLiveStreamItemInfoParams = new messages.GetLiveStreamItemInfoParams();
            GetLiveStreamItemInfoParams.setLivestreamhandle(Number(liveStreamHandle));
            let bytes = GetLiveStreamItemInfoParams.serializeBinary();
            return _addon.GetLiveStreamItemInfo(bytes);            
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE
      },
      /** 
      * Query Whether the meeting supports raw live streams.
      * @method IsRawLiveStreamSupported
      * @return {Boolean} true if supported, false if not supported.
      */
      IsRawLiveStreamSupported: function () {
        if (_addon) {
          return _addon.IsRawLiveStreamSupported();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE
      },
      /** 
      * Whether if the current user is able to start raw live streaming.
      * @method CanStartRawLiveStream
      * @return {Number} If the current user is able to start raw live streaming, the return value is SDKERR_SUCCESS.
	      Otherwise it fails,and returns NULL. To get extended error information, see {@link ZoomSDKError}.
      */
      CanStartRawLiveStream: function () {
        if (_addon) {
          return _addon.CanStartRawLiveStream();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE
      },
      /** 
      * Remove the raw live stream privilege.
      * @method RemoveRawLiveStreamPrivilege
      * @param {Number} userID Specify the ID of the user whose privilege will be removed.
      * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise it fails.
        To get extended error information, see {@link ZoomSDKError}.
      */
      RemoveRawLiveStreamPrivilege: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let userID = clientOpts.userID;
          if (!userID) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER; 
          }
          try {
            let RemoveRawLiveStreamPrivilegeParams = new messages.RemoveRawLiveStreamPrivilegeParams();
            RemoveRawLiveStreamPrivilegeParams.setUserid(Number(userID));
            let bytes = RemoveRawLiveStreamPrivilegeParams.serializeBinary();
            return _addon.RemoveRawLiveStreamPrivilege(bytes);            
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE
      },
      /** 
      * Get a list of current active raw live streams.
      * @method GetRawLiveStreamingInfoList
      * @return {Array} If the function succeeds, the return value is a list of current active raw live streams.
      */
      GetRawLiveStreamingInfoList: function () {
        if (_addon) {
          return _addon.GetRawLiveStreamingInfoList();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE
      },
      /** 
      * Get the list of users'IDs who have raw live stream privileges.
      * @method GetRawLiveStreamPrivilegeUserList
      * @return {Array} If the function succeeds, the return value is a list of users'IDs who have raw live stream privileges.
      */
      GetRawLiveStreamPrivilegeUserList: function () {
        if (_addon) {
          return _addon.GetRawLiveStreamPrivilegeUserList();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE
      },
      /** 
      * Check if the live stream reminder is enabled. When the live stream reminder is enabled, the new join user is notified that the meeting is at capacity but that they can watch the meeting live stream with the callback onMeetingFullToWatchLiveStream when the meeting user has reached the meeting capability.
      * @method IsLiveStreamReminderEnabled
      * @return {Boolean} True means the live stream reminder is enabled.
      */
      IsLiveStreamReminderEnabled: function () {
        if (_addon) {
          return _addon.IsLiveStreamReminderEnabled();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE
      },
      /** 
      * Check if the current user can enable/disable the live stream reminder.
      * @method CanEnableLiveStreamReminder
      * @return {Boolean} True means the current user can enable/disable the live stream reminder.
      */
      CanEnableLiveStreamReminder: function () {
        if (_addon) {
          return _addon.CanEnableLiveStreamReminder();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE
      },
      /** 
      * Enable or disable the live stream reminder.
      * @method EnableLiveStreamReminder
      * @param {Boolean} enable True means enable the live stream reminder. False means disable the live stream reminder.
      * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS. Otherwise it fails.
        To get extended error information, see {@link ZoomSDKError}.
      */
      EnableLiveStreamReminder: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let enable = clientOpts.enable;
          try {
            let EnableLiveStreamReminderParams = new messages.EnableLiveStreamReminderParams();
            EnableLiveStreamReminderParams.setEnable(enable);
            let bytes = EnableLiveStreamReminderParams.serializeBinary();
            return _addon.EnableLiveStreamReminder(bytes);            
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
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
  ZoomMeetingLiveStreamCtrl: ZoomMeetingLiveStreamCtrl
}