const { ZoomSDKError, SDKUserInfoType } = require('./settings.js');
const messages = require('./electron_sdk_pb.js');

var ZoomMeetingParticipantsCtrl = (function () {
  var instance;
  /**
   * Zoom Meeting Participants Ctrl
   * @module zoom_meeting_participants_ctrl
   * @param {Function} meetinguserjoincb Callback event of notification of users who are in the meeting.
   * @param {Function} meetinguserleftcb Callback event of notification of user who leaves the meeting.
   * @param {Function} meetinghostchangecb Callback event of notification of the new host.
   * @param {Function} onAllowParticipantsRequestCloudRecording Callback event that lets participants request that the host starts cloud recording.
   * @param {Function} onBotAuthorizerRelationChanged Callback event that the bot relationship changed in the meeting.
   * @return {ZoomMeetingParticipantsCtrl}
   */
  function init(opts) {
    var clientOpts = opts || {};

    // Private methods and variables
    var _addon = clientOpts.addon.GetMeetingParticipantsCtrl() || null;
    let _meetinguserjoincb = clientOpts.meetinguserjoincb || null;
    let _meetinguserleftcb = clientOpts.meetinguserleftcb || null;
    let _meetinghostchangecb = clientOpts.meetinghostchangecb || null;
    let _onAllowParticipantsRequestCloudRecording = clientOpts.onAllowParticipantsRequestCloudRecording || null;
    let _onBotAuthorizerRelationChanged = clientOpts.onBotAuthorizerRelationChanged || null;

    /**
      Callback event of notification of users who are in the meeting.
      @event meetinguserjoincb
      @param {String} lstUserID List of the user ID.
      @param {Array} strUserList List of user in json format. This function is currently invalid, hereby only for reservations.
    */
    function meetinguserjoincb(lstUserID, strUserList) {
      if (_meetinguserjoincb) {
        _meetinguserjoincb(lstUserID, strUserList)
      }
    }

    /**
      Callback event of notification of user who leaves the meeting.
      @event meetinguserleftcb
      @param {String} lstUserID List of the user ID who leaves the meeting.
      @param {Array} strUserList List of the user in json format. This function is currently invalid, hereby only for reservations.
    */
    function meetinguserleftcb(lstUserID, strUserList) {
      if (_meetinguserleftcb) {
        _meetinguserleftcb(lstUserID, strUserList)
      }
    }

    /**
      Callback event of notification of the new host.
      @event meetinghostchangecb
      @param {Number} userId Specify the ID of the new host.
    */
    function meetinghostchangecb(userId) {
      if (_meetinghostchangecb) {
        _meetinghostchangecb(userId)
      }
    }

    /**
      Callback event that lets participants request that the host starts cloud recording.
      @event onAllowParticipantsRequestCloudRecording
      @param {Boolean} bAllow True allow. If false, disallow.
    */
    function onAllowParticipantsRequestCloudRecording(bAllow) {
      if (_onAllowParticipantsRequestCloudRecording) {
        _onAllowParticipantsRequestCloudRecording(bAllow)
      }
    }

    /**
      Callback event that the bot relationship changed in the meeting.
      @event onBotAuthorizerRelationChanged
      @param {Number} authorizeUserID Specify the authorizer user ID.
    */
    function onBotAuthorizerRelationChanged(authorizeUserID) {
      if (_onBotAuthorizerRelationChanged) {
        _onBotAuthorizerRelationChanged(authorizeUserID)
      }
    }

    if (_addon) {
      _addon.SetMeetingUserJoinCB(meetinguserjoincb);
      _addon.SetMeetingUserLeftCB(meetinguserleftcb);
      _addon.SetMeetingHostChangeCB(meetinghostchangecb);
      _addon.SetOnAllowParticipantsRequestCloudRecordingCB(onAllowParticipantsRequestCloudRecording);
      _addon.SetOnBotAuthorizerRelationChangedCB(onBotAuthorizerRelationChanged);
    }

    return {
      // Public methods and variables
      /**
       * Set callback function of notification of users who are in the meeting.
       * @method SetMeetingUserJoinCB
       * @param {Function} meetinguserjoincb
       * @return {Boolean}
       */
      SetMeetingUserJoinCB: function(meetinguserjoincb) {
        if (_addon && meetinguserjoincb && meetinguserjoincb instanceof Function) {
          _meetinguserjoincb = meetinguserjoincb;
          return true;
        }
        return false;
      },
      /**
       * Set callback function of notification of users who leave the meeting.
       * @method SetMeetingUserLeftCB
       * @param {Function} meetinguserleftcb
       * @return {Boolean}
       */
      SetMeetingUserLeftCB: function(meetinguserleftcb) {
        if (_addon && meetinguserleftcb && meetinguserleftcb instanceof Function) {
          _meetinguserleftcb = meetinguserleftcb;
          return true
        }
        return false
      },
      /**
       * Set callback function of notification of the new host.
       * @method SetMeetingHostChangeCB
       * @param {Function} onHostChangeNotification
       * @return {Boolean}
       */
      SetMeetingHostChangeCB: function(onHostChangeNotification) {
        if (_addon && onHostChangeNotification && onHostChangeNotification instanceof Function) {
          _onHostChangeNotification = onHostChangeNotification;
          return true;
        }
        return false;
      },
      /**
       * Set callback event that lets participants request host to start cloud recording.
       * @method SetOnAllowParticipantsRequestCloudRecordingCB
       * @param {Function} onAllowParticipantsRequestCloudRecording
       * @return {Boolean}
       */
      SetOnAllowParticipantsRequestCloudRecordingCB: function(onAllowParticipantsRequestCloudRecording) {
        if (_addon && onAllowParticipantsRequestCloudRecording && onAllowParticipantsRequestCloudRecording instanceof Function) {
          _onAllowParticipantsRequestCloudRecording = onAllowParticipantsRequestCloudRecording;
          return true;
        }
        return false;
      },
      /**
       * Set callback event that the bot relationship changed in the meeting.
       * @method SetOnBotAuthorizerRelationChangedCB
       * @param {Function} onBotAuthorizerRelationChanged
       * @return {Boolean}
       */
      SetOnBotAuthorizerRelationChangedCB: function(onBotAuthorizerRelationChanged) {
        if (_addon && onBotAuthorizerRelationChanged && onBotAuthorizerRelationChanged instanceof Function) {
          _onBotAuthorizerRelationChanged = onBotAuthorizerRelationChanged;
          return true;
        }
        return false;
      },
      /**
       * Get the list of all the panelists in the meeting.
       * @method GetParticipantsList
       * @return {Array} If the function succeed, the return value is the list of the panelists in the meeting.
	        Otherwise failed, the return value is NULL.
       */
      GetParticipantsList: function () {
        if (_addon) {
          return _addon.GetParticipantsList();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Get the information of specified user.
       * @method GetUserInfoByUserID
       * @param {Number} userid Specify the user ID for which you want to get the information.
	        Zero(0) indicates to get the information of the current user.
       * @return {Object} If the function succeed, the return value is an object which includes the user's infomation.
	        Otherwise failed, the return value is an empty object.
       */
      GetUserInfoByUserID: function(userid) {
        if (_addon) {
          try {
            let GetUserInfoByUserIDParams = new messages.GetUserInfoByUserIDParams();
            GetUserInfoByUserIDParams.setUserid(Number(userid));
            let bytes = GetUserInfoByUserIDParams.serializeBinary();
            let userInfo = _addon.GetUserInfoByUserID(bytes);
            if (userInfo.userInfoType == SDKUserInfoType.FAKE_USERINFO) {
              return {}
            } else {
              return userInfo
            }
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Get the information about the bot's authorized user.
       * @method GetBotAuthorizedUserInfoByUserID
       * @param {Number} userid Specify the user ID for which to get the information.
       * @return {Object} If the function succeed, the return value is an object which includes the user's infomation.
	        Otherwise failed, the return value is an empty object.
       */
      GetBotAuthorizedUserInfoByUserID: function(opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let userid = clientOpts.userid;
          try {
            let GetBotAuthorizedUserInfoByUserIDParams = new messages.GetBotAuthorizedUserInfoByUserIDParams();
            GetBotAuthorizedUserInfoByUserIDParams.setUserid(Number(userid));
            let bytes = GetBotAuthorizedUserInfoByUserIDParams.serializeBinary();
            return _addon.GetBotAuthorizedUserInfoByUserID(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Get the authorizer's bot list.
       * @method GetAuthorizedBotListByUserID
       * @param {Number} userid Specify the user ID for which to get the information.
       * @return {Array} If the function succeeds, the return value is the authorizer's bot list in the meeting.
	        Otherwise failed, the return value is an empty list.
       */
      GetAuthorizedBotListByUserID: function(opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let userid = clientOpts.userid;
          try {
            let GetAuthorizedBotListByUserIDParams = new messages.GetAuthorizedBotListByUserIDParams();
            GetAuthorizedBotListByUserIDParams.setUserid(Number(userid));
            let bytes = GetAuthorizedBotListByUserIDParams.serializeBinary();
            return _addon.GetAuthorizedBotListByUserID(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Determine if the current user can enable participant request clould recording.
      * @method CanEnableParticipantRequestCloudRecording
      * @return {Boolean} True means the current user can enable participant request clould recording.
      */
      CanEnableParticipantRequestCloudRecording: function () {
        if (_addon) {
          return _addon.CanEnableParticipantRequestCloudRecording();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Check whether the current meeting allows participants to send cloud recording privilege request, This can only be used in regular meeetings and webinar(no breakout rooms).
      * @method IsParticipantRequestCloudRecordingAllowed
      * @return {Boolean} If allows participants to send request, the return value is true.
      */
      IsParticipantRequestCloudRecordingAllowed: function () {
        if (_addon) {
          return _addon.IsParticipantRequestCloudRecordingAllowed();
        }
        return false;
      },
      /**
      * Toggle whether attendees can requests for the host to start a cloud recording. This can only be used in regular meeetings and webinar(no breakout rooms).
      * @method AllowParticipantsToRequestCloudRecording
      * @param {Boolean} bAllow TRUE indicates that participants are allowed to send cloud recording privilege requests.
      * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise the function fails. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      AllowParticipantsToRequestCloudRecording: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var bAllow = clientOpts.bAllow;
          try {
            let AllowParticipantsToRequestCloudRecordingParams = new messages.AllowParticipantsToRequestCloudRecordingParams();
            AllowParticipantsToRequestCloudRecordingParams.setBallow(bAllow);
            let bytes = AllowParticipantsToRequestCloudRecordingParams.serializeBinary();
            return _addon.AllowParticipantsToRequestCloudRecording(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
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
      return instance
    }
  };
})();

module.exports = {
  ZoomMeetingParticipantsCtrl: ZoomMeetingParticipantsCtrl
}
