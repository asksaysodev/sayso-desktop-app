let { ZoomSDKError } = require('./settings.js');
const messages = require('./electron_sdk_pb.js');

var ZoomMeetingWaitingRoom = (function () {
  var instance;
  /**
   * Zoom Meeting Waiting Room
   * @module zoom_meeting_waiting_room
   * @param {Function} onWaitingRoomUserJoin Callback event of notification that user joins the waiting room.
   * @param {Function} onWaitingRoomUserLeft Callback event used to inform the user when the authority of chat changes in the meeting or webinar.
   * @param {Function} onWaitingRoomPresetAudioStatusChanged During the waiting room, this callback event will be triggered when host change audio status.
   * @param {Function} onWaitingRoomPresetVideoStatusChanged During the waiting room, this callback event will be triggered when host change video status.
   * @param {Function} onCustomWaitingRoomDataUpdated During the waiting room, this callback event will be triggered when RequestCustomWaitingRoomData called.
   * @param {Function} onWaitingRoomUserNameChanged Callback indicating that the name of a user in the waiting room has changed.
   * @param {Function} onWaitingRoomEntranceEnabled Callback event will be triggered when host or cohost enables or disables waiting room entrance.
   * @return {ZoomMeetingWaitingRoom}
   */
  function init(opts) {
    let clientOpts = opts || {};
    // Private methods and variables
    let _addon = clientOpts.addon.GetMeetingWaitingRoomCtrl() || null;
    let _onWaitingRoomUserJoin = clientOpts.onWaitingRoomUserJoin || null;
    let _onWaitingRoomUserLeft = clientOpts.onWaitingRoomUserLeft || null;
    let _onWaitingRoomPresetAudioStatusChanged = clientOpts.onWaitingRoomPresetAudioStatusChanged || null;
    let _onWaitingRoomPresetVideoStatusChanged = clientOpts.onWaitingRoomPresetVideoStatusChanged || null;
    let _onCustomWaitingRoomDataUpdated = clientOpts.onCustomWaitingRoomDataUpdated || null;
    let _onWaitingRoomUserNameChanged = clientOpts.onWaitingRoomUserNameChanged || null;
    let _onWaitingRoomEntranceEnabled = clientOpts.onWaitingRoomEntranceEnabled || null;

    /**
      Callback event of notification that user joins the waiting room.
      @event onWaitingRoomUserJoin
      @param {Number} userID The ID of user who joins the waiting room.
    */
    function onWaitingRoomUserJoin(userID) {
      if (_onWaitingRoomUserJoin) {
        _onWaitingRoomUserJoin(userID)
      }
    }

    /**
      Callback event of notification that user leaves the waiting room.
      @event onWaitingRoomUserLeft
      @param {Number} userID The ID of user who leaves the waiting room.
    */
    function onWaitingRoomUserLeft(userID) {
      if (_onWaitingRoomUserLeft) {
        _onWaitingRoomUserLeft(userID)
      }
    }

    /**
      During the waiting room, this callback event will be triggered when host change audio status.
      @event onWaitingRoomPresetAudioStatusChanged
      @param {Boolean} bAudioCanTurnOn True means audio can be turned on. Otherwise not.
    */
    function onWaitingRoomPresetAudioStatusChanged(bAudioCanTurnOn) {
      if (_onWaitingRoomPresetAudioStatusChanged) {
        _onWaitingRoomPresetAudioStatusChanged(bAudioCanTurnOn)
      }
    }
  
    /**
      During the waiting room, this callback event will be triggered when host change video status.
      @event onWaitingRoomPresetVideoStatusChanged
      @param {Boolean} bVideoCanTurnOn True means video can be turned on. Otherwise not.
    */
    function onWaitingRoomPresetVideoStatusChanged(bVideoCanTurnOn) {
      if (_onWaitingRoomPresetVideoStatusChanged) {
        _onWaitingRoomPresetVideoStatusChanged(bVideoCanTurnOn)
      }
    }

    /**
      During the waiting room, this callback event will be triggered when RequestCustomWaitingRoomData called.
      @event onCustomWaitingRoomDataUpdated
      @param {Object} bData WaitingRoom Customize Data Info
    */
    function onCustomWaitingRoomDataUpdated(bData) {
      let obj = {
        title: bData.title,
        description: bData.description,
        logo_path: bData.logo_path,
        video_path: bData.video_path,
        image_path: bData.image_path,
        type: bData.type,
        status: bData.status,
        page_color: bData.page_color,
        text_color: bData.text_color,
        button_color: bData.button_color
      }
      if (_onCustomWaitingRoomDataUpdated) {
        _onCustomWaitingRoomDataUpdated(obj)
      }
    }

    /**
      Callback indicating that the name of a user in the waiting room has changed.
      @event onWaitingRoomUserNameChanged
      @param {Number} userID The ID of the user whose user name have has changed.
      @param {String} userName The new user name.
    */
    function onWaitingRoomUserNameChanged(userID, userName) {
      if (_onWaitingRoomUserNameChanged) {
        _onWaitingRoomUserNameChanged(userID, userName)
      }
    }

    /**
      Callback event will be triggered when host or cohost enables or disables waiting room entrance.
      @event onWaitingRoomEntranceEnabled
      @param {Boolean} bIsEnabled True enables waiting room entrance, false means disables waiting room entrance.
    */
    function onWaitingRoomEntranceEnabled(bIsEnabled) {
      if (_onWaitingRoomEntranceEnabled) {
        _onWaitingRoomEntranceEnabled(bIsEnabled)
      }
    }

    if (_addon) {
      _addon.SetOnWaitingRoomUserJoinCB(onWaitingRoomUserJoin);
      _addon.SetOnWaitingRoomUserLeftCB(onWaitingRoomUserLeft);
      _addon.SetOnWaitingRoomPresetAudioStatusChangedCB(onWaitingRoomPresetAudioStatusChanged);
      _addon.SetOnWaitingRoomPresetVideoStatusChangedCB(onWaitingRoomPresetVideoStatusChanged);
      _addon.SetOnCustomWaitingRoomDataUpdatedCB(onCustomWaitingRoomDataUpdated);
      _addon.SetOnWaitingRoomUserNameChangedCB(onWaitingRoomUserNameChanged);
      _addon.SetOnWaitingRoomEntranceEnabledCB(onWaitingRoomEntranceEnabled);
    }

    return {
      // Public methods and variables
      /**
      * Set notification user joins the waiting room callback.
      * @method SetOnWaitingRoomUserJoinCB
      * @param {Function} onWaitingRoomUserJoin
      * @return {Boolean}
      */
      SetOnWaitingRoomUserJoinCB: function (onWaitingRoomUserJoin) {
        if (_addon && onWaitingRoomUserJoin && onWaitingRoomUserJoin instanceof Function) {
          _onWaitingRoomUserJoin = onWaitingRoomUserJoin;
          return true;
        }
        return false;
      },
      /**
      * Set notification user leaves the waiting room callback.
      * @method SetOnWaitingRoomUserLeftCB
      * @param {Function} onWaitingRoomUserLeft
      * @return {Boolean}
      */
      SetOnWaitingRoomUserLeftCB: function (onWaitingRoomUserLeft) {
        if (_addon && onWaitingRoomUserLeft && onWaitingRoomUserLeft instanceof Function) {
          _onWaitingRoomUserLeft = onWaitingRoomUserLeft;
          return true;
        }
        return false;
      },
      /**
      * Set host change audio status callback.
      * @method SetOnWaitingRoomPresetAudioStatusChangedCB
      * @param {Function} onWaitingRoomPresetAudioStatusChanged
      * @return {Boolean}
      */
      SetOnWaitingRoomPresetAudioStatusChangedCB: function (onWaitingRoomPresetAudioStatusChanged) {
        if (_addon && onWaitingRoomPresetAudioStatusChanged && onWaitingRoomPresetAudioStatusChanged instanceof Function) {
          _onWaitingRoomPresetAudioStatusChanged = onWaitingRoomPresetAudioStatusChanged;
          return true;
        }
        return false;
      },
      /**
      * Set share meeting chat status changed callback.
      * @method SetOnWaitingRoomPresetVideoStatusChangedCB
      * @param {Function} onWaitingRoomPresetVideoStatusChanged
      * @return {Boolean}
      */
      SetOnWaitingRoomPresetVideoStatusChangedCB: function (onWaitingRoomPresetVideoStatusChanged) {
        if (_addon && onWaitingRoomPresetVideoStatusChanged && onWaitingRoomPresetVideoStatusChanged instanceof Function) {
          _onWaitingRoomPresetVideoStatusChanged = onWaitingRoomPresetVideoStatusChanged;
          return true;
        }
        return false;
      },
      /**
      * Set triggered RequestCustomWaitingRoomData callback.
      * @method SetOnCustomWaitingRoomDataUpdatedCB
      * @param {Function} onCustomWaitingRoomDataUpdated
      * @return {Boolean}
      */
      SetOnCustomWaitingRoomDataUpdatedCB: function (onCustomWaitingRoomDataUpdated) {
        if (_addon && onCustomWaitingRoomDataUpdated && onCustomWaitingRoomDataUpdated instanceof Function) {
          _onCustomWaitingRoomDataUpdated = onCustomWaitingRoomDataUpdated;
          return true;
        }
        return false;
      },
      /**
      * Set the name of a user in the waiting room has changed callback.
      * @method SetOnWaitingRoomUserNameChangedCB
      * @param {Function} onWaitingRoomUserNameChanged
      * @return {Boolean}
      */
      SetOnWaitingRoomUserNameChangedCB: function (onWaitingRoomUserNameChanged) {
        if (_addon && onWaitingRoomUserNameChanged && onWaitingRoomUserNameChanged instanceof Function) {
          _onWaitingRoomUserNameChanged = onWaitingRoomUserNameChanged;
          return true;
        }
        return false;
      },
      /**
      * Set host or cohost enables or disables waiting room entrance callback.
      * @method SetOnWaitingRoomEntranceEnabledCB
      * @param {Function} onWaitingRoomEntranceEnabled
      * @return {Boolean}
      */
      SetOnWaitingRoomEntranceEnabledCB: function (onWaitingRoomEntranceEnabled) {
        if (_addon && onWaitingRoomEntranceEnabled && onWaitingRoomEntranceEnabled instanceof Function) {
          _onWaitingRoomEntranceEnabled = onWaitingRoomEntranceEnabled;
          return true;
        }
        return false;
      },
      /**
      * Determine whether the current meeting supports the waiting room or not. 
      * @method IsSupportWaitingRoom
      * @return {Boolean} True indicates to support.
      */
      IsSupportWaitingRoom: function () {
        if (_addon) {
          return _addon.IsSupportWaitingRoom();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Determine if the attendee is enabled to enter the waiting room when joining the meeting.
      * @method IsWaitingRoomOnEntryFlagOn
      * @return {Boolean} True indicates to enable to enter.
      */
      IsWaitingRoomOnEntryFlagOn: function () {
        if (_addon) {
          return _addon.IsWaitingRoomOnEntryFlagOn();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Set to enable the attendee to enter the waiting room when joining the meeting.
      * @method EnableWaitingRoomOnEntry
      * @param {Number} bEnable True indicates to enable to enter. False not.
      * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      EnableWaitingRoomOnEntry: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var bEnable = clientOpts.bEnable;
          try {
            let EnableWaitingRoomOnEntryParams = new messages.EnableWaitingRoomOnEntryParams();
            EnableWaitingRoomOnEntryParams.setBenable(bEnable);
            let bytes = EnableWaitingRoomOnEntryParams.serializeBinary();
            return _addon.EnableWaitingRoomOnEntry(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Get the list of attendees who are in the waiting room.
      * @method GetWaitingRoomList
      * @return {Array} If the function succeeds, the return value is the list of attendees. Otherwise failed, the return is null. 
      */
      GetWaitingRoomList: function () {
        if (_addon) {
          return _addon.GetWaitingRoomList();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Get the attendee information in the waiting room via user ID.
      * @method GetWaitingRoomUserInfoByID
      * @param {Number} userid Specifies the user ID.
      * @return {Object}
      */
      GetWaitingRoomUserInfoByID: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var userid = clientOpts.userid;
          if (userid == undefined) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
          try {
            let GetWaitingRoomUserInfoByIDParams = new messages.GetWaitingRoomUserInfoByIDParams();
            GetWaitingRoomUserInfoByIDParams.setUserid(userid);
            let bytes = GetWaitingRoomUserInfoByIDParams.serializeBinary();
            return _addon.GetWaitingRoomUserInfoByID(bytes)
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Permit the specified user to join the meeting.
      * @method AdmitToMeeting
      * @param {Number} userid Specifies the user ID.
      * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      AdmitToMeeting: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var userid = clientOpts.userid;
          if (userid == undefined) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
          try {
            let AdmitToMeetingParams = new messages.AdmitToMeetingParams();
            AdmitToMeetingParams.setUserid(userid);
            let bytes = AdmitToMeetingParams.serializeBinary();
            return _addon.AdmitToMeeting(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Permit all of the users currently in the waiting room to join the meeting.
      * @method AdmitAllToMeeting
      * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      AdmitAllToMeeting: function () {
        if (_addon) {
          return _addon.AdmitAllToMeeting();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Enable the specified user to enter the waiting room.
      * @method PutInWaitingRoom
      * @param {Number} userid Specifies the user ID.
      * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      PutInWaitingRoom: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var userid = clientOpts.userid;
          if (userid == undefined) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
          try {
            let PutInWaitingRoomParams = new messages.PutInWaitingRoomParams();
            PutInWaitingRoomParams.setUserid(userid);
            let bytes = PutInWaitingRoomParams.serializeBinary();
            return _addon.PutInWaitingRoom(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Determine if the attendee is enabled to turn on audio when joining the meeting.
      * @method IsAudioEnabledInWaitingRoom
      * @return {Boolean} True indicates to enable to turn on.
      */
      IsAudioEnabledInWaitingRoom: function () {
        if (_addon) {
          return _addon.IsAudioEnabledInWaitingRoom();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Determine if the attendee is enabled to turn on video when joining the meeting.
      * @method IsVideoEnabledInWaitingRoom
      * @return {Boolean} True indicates to enable to turn on.
      */
      IsVideoEnabledInWaitingRoom: function () {
        if (_addon) {
          return _addon.IsVideoEnabledInWaitingRoom();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Get the WaitingRoom CustomizeData information in the waiting room.
      * @method RequestCustomWaitingRoomData
      * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      RequestCustomWaitingRoomData: function () {
        if (_addon) {
          return _addon.RequestCustomWaitingRoomData();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Determine if the host or cohost can rename users in the waiting room.
      * @method CanRenameUser
      * @return {Boolean} True means the host or cohost can rename users in the waiting room. Otherwise they can't.
      */
      CanRenameUser: function () {
        if (_addon) {
          return _addon.CanRenameUser();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Change a user's screen name in the waiting room.
      * @method RenameUser
      * @param {Number} userid The ID of users put into the waiting room by a host or cohost.
      * @param {String} newName The new user name.
      * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      RenameUser: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var userid = clientOpts.userid;
          var newName = clientOpts.newName;
          try {
            let RenameUserParams = new messages.RenameUserParams();
            RenameUserParams.setUserid(userid);
            RenameUserParams.setNewname(newName);
            let bytes = RenameUserParams.serializeBinary();
            return _addon.RenameUser(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Determine if a host or cohost can expel user(s) in the waiting room.
      * @method CanExpelUser
      * @return {Boolean} True means that a host or cohost can expel user(s) in the waiting room. Otherwise they may not
      */
      CanExpelUser: function () {
        if (_addon) {
          return _addon.CanExpelUser();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Remove a specified user from the waiting room.
      * @method ExpelUser
      * @param {Number} userid The ID of the user removed from the waiting room by a host or cohost.
      * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      ExpelUser: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var userid = clientOpts.userid;
          try {
            let ExpelUserParams = new messages.ExpelUserParams();
            ExpelUserParams.setUserid(userid);
            let bytes = ExpelUserParams.serializeBinary();
            return _addon.ExpelUser(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Retry to Download the WaitingRoom CustomizeData information in the waiting room.
      * @method Retry
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
        Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      Retry: function () {
        if (_addon) {
          return _addon.Retry();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Ignore to GDownloadet the WaitingRoom CustomizeData information in the waiting room.
      * @method Ignore
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      Ignore: function () {
        if (_addon) {
          return _addon.Ignore();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      }
    };
  };

  return {
    getInstance: function (opts) {
      if (!instance) {
        instance = init(opts);
      }
      return instance;
    }
  };
})();

module.exports = {
  ZoomMeetingWaitingRoom: ZoomMeetingWaitingRoom
}
