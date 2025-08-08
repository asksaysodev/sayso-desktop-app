const { ZoomMeetingStatus, ZoomUserType, ZoomSDKError } = require('./settings.js');
const ZoomMeetingINFOMOD = require('./zoom_meeting_info.js');
const ZoomMeetingUIMOD = require('./zoom_meeting_ui_ctrl.js');
const ZoomAnnotationMOD = require('./zoom_annotation_ctrl.js');
const ZoomMeetingAudioMOD = require('./zoom_meeting_audio.js');
const ZoomMeetingVideoMOD = require('./zoom_meeting_video.js');
const ZoomMeetingShareMOD = require('./zoom_meeting_share.js');
const ZoomMeetingH323MOD = require('./zoom_h323.js');
const ZoomMeetingConfigurationMOD = require('./zoom_meeting_configuration.js');
const ZoomUpdateAccountMOD = require('./zoom_upgrade_account.js');
const ZoomMeetingParticipantsMOD = require('./zoom_meeting_participants_ctrl.js');
const ZoomMeetingRecordingMOD = require('./zoom_meeting_recording.js');
const ZoomMeetingAANMOD = require('./zoom_meeting_aan.js');
const ZoomMeetingRawArchivingCtrlMOD = require('./zoom_meeting_raw_archiving_ctrl.js');
const ZoomMeetingLiveStreamCtrlMOD = require('./zoom_meeting_live_stream_ctrl.js');
const ZoomMeetingRequestRawLiveStreamPrivilegeHandlerMOD = require('./zoom_meeting_request_raw_live_stream_privilege_handler.js');
const ZoomMeetingReminderMOD = require('./zoom_meeting_reminder.js');
const ZoomMeetingChatMOD = require('./zoom_meeting_chat.js');
const ZoomMeetingWaitingRoomMOD = require('./zoom_meeting_waiting_room.js');
const ZoomMeetingAICompanionMOD = require('./zoom_meeting_ai_companion.js');
const ZoomMeetingIndicatorMOD = require('./zoom_meeting_indicatory.js');
const ZoomMeetingWebinarMOD = require('./zoom_meeting_webinar.js');
const ZoomMeetingQAMOD = require('./zoom_meeting_qa.js');
const ZoomMeetingReactionMOD = require('./zoom_meeting_reaction.js');
const ZoomMeetingCloseCaptionMOD = require('./zoom_meeting_close_caption.js');
const ZoomMeetingPollingMOD = require('./zoom_meeting_polling.js');
const ZoomMeetingWhiteboardMOD = require('./zoom_meeting_whiteboard.js');
const ZoomMeetingDocsMOD = require('./zoom_meeting_docs.js');
const messages = require('./electron_sdk_pb.js');
const { app } = require('electron')

let ZoomMeetingInfo;
let ZoomMeetingUICtrl;
let ZoomAnnotationCtrl;
let ZoomMeetingAudio;
let ZoomMeetingVideo;
let ZoomMeetingShare;
let ZoomH323;
let ZoomMeetingConfiguration;
let ZoomPaymentReminder;
let ZoomMeetingParticipantsCtrl;
let ZoomMeetingRecording;
let ZoomMeetingAAN;
let ZoomMeetingRawArchivingCtrl;
let ZoomMeetingLiveStreamCtrl;
let ZoomMeetingRequestRawLiveStreamPrivilegeHandler;
let ZoomMeetingReminder;
let ZoomMeetingChat;
let ZoomMeetingWaitingRoom;
let ZoomMeetingAICompanion;
let ZoomMeetingIndicator;
let ZoomMeetingWebinar;
let ZoomMeetingQA;
let ZoomMeetingReaction;
let ZoomMeetingCloseCaption;
let ZoomMeetingPolling;
let ZoomMeetingWhiteboard;
let ZoomMeetingDocs;
let startOrJoinWithRawdata;

const ZoomMeeting = (function () {
  let instance;
  /**
   * Zoom Meeting
   * @module zoom_meeting
   * @param {Function} meetingstatuscb Meeting status changed callback.
   * @return {ZoomMeeting}
   */
  function init(opts) {
    let clientOpts = opts || {};
    // Private methods and variables
    let _addon = clientOpts.addon.GetMeetingObj() || null;
    let _rawdataAddon = clientOpts.addon.GetRawdataAPIWrap() || null;
    let _meetingstatuscb = clientOpts.meetingstatuscb || null;
    let _onMeetingStatisticsWarningNotification = clientOpts.onMeetingStatisticsWarningNotification || null;
    let _onMeetingParameterNotification = clientOpts.onMeetingParameterNotification || null;
    let _onMeetingTopicChanged = clientOpts.onMeetingTopicChanged || null;
    let _onMeetingFullToWatchLiveStream = clientOpts.onMeetingFullToWatchLiveStream || null;

    /**
      Meeting status changed callback.
      @event meetingstatuscb
      @param {String} meetingStatus The value of meeting. Defined in: {@link ZoomMeetingStatus}
      @param {String} result Detailed reasons for special meeting status.
	      If the status is MEETING_STATUS_FAILED, the value of iResult is one of those listed in MeetingFailCode enum.
	      If the status is MEETING_STATUS_ENDED, the value of iResult is one of those listed in MeetingEndReason.
    */
    function meetingstatuscb(meetingStatus, result) {
      switch (meetingStatus) {
        case ZoomMeetingStatus.MEETING_STATUS_CONNECTING:
          if (startOrJoinWithRawdata) {
            SetPipeServeInitParam()
            StartPipeServe()
          }
          break;
        case ZoomMeetingStatus.MEETING_STATUS_DISCONNECTING:
        case ZoomMeetingStatus.MEETING_STATUS_RECONNECTING:
          if (startOrJoinWithRawdata) {
            StopPipeServe();
          }
          break;
        default:
          break;
      }
      if (_meetingstatuscb) {
        _meetingstatuscb(meetingStatus, result)
      }
    }

    /**
      Meeting statistics warning notification callback.
      @event onMeetingStatisticsWarningNotification
      @param {Number} type The warning type of the meeting statistics. Defined in: {@link StatisticsWarningType}
    */
    function onMeetingStatisticsWarningNotification(type) {
      if (_onMeetingStatisticsWarningNotification) {
        _onMeetingStatisticsWarningNotification(type)
      }
    }

    /**
      Meeting parameter notification callback. The callback will be triggered right before the meeting starts. The meeting_param will be destroyed once the function calls end.
      @event onMeetingParameterNotification
      @param {Object} meeting_param Meeting parameter.
    */
    function onMeetingParameterNotification(item) {
      if (_onMeetingParameterNotification) {
        let meeting_param = {
          meeting_type: item.meetingType,
          is_view_only: item.isViewOnly,
          is_auto_recording_local: item.isAutoRecordingLocal,
          is_auto_recording_cloud: item.isAutoRecordingCloud,
          meeting_number: item.meetingNumber,
          meeting_topic: item.meetingTopic,
          meeting_host: item.meetingHost
        }
        _onMeetingParameterNotification(meeting_param)
      }
    }

    /**
      Callback event for the meeting topic changed. 
      @event onMeetingTopicChanged
      @param {String} sTopic The new meeting topic.
    */
    function onMeetingTopicChanged(sTopic) {
      if (_onMeetingTopicChanged) {
        _onMeetingTopicChanged(sTopic)
      }
    }

    /**
      Calback event that the meeting users have reached the meeting capacity. The new join user can not join meeting, but they can watch the meeting live stream.
      @event onMeetingFullToWatchLiveStream
      @param {String} sLiveStreamUrl The live stream URL to watch the meeting live stream.
    */
    function onMeetingFullToWatchLiveStream(sLiveStreamUrl) {
      if (_onMeetingFullToWatchLiveStream) {
        _onMeetingFullToWatchLiveStream(sLiveStreamUrl)
      }
    }

    function SetPipeServeInitParam() {
      if (_rawdataAddon) {
        try {
          let pipeParams = app.pipeParams
          let SetPipeInitParams = new messages.SetPipeInitParams();
          SetPipeInitParams.setVideopipename(pipeParams.videoPipeName);
          SetPipeInitParams.setSharepipename(pipeParams.sharePipeName);
          SetPipeInitParams.setAudiopipename(pipeParams.audioPipeName);
          SetPipeInitParams.setMaxreadlength(pipeParams.maxReadLength);
          let bytes = SetPipeInitParams.serializeBinary();
          return _rawdataAddon.SetPipeServeInitParam(bytes);
        } catch (error) {
          return ZoomSDKError.SDKERR_INVALID_PARAMETER;
        }
      }
      return ZoomSDKError.SDKERR_UNINITIALIZE
    }

    function StartPipeServe() {
      if (_rawdataAddon) {
        return _rawdataAddon.StartPipeServe();
      }
      return ZoomSDKError.SDKERR_UNINITIALIZE
    }

    function StopPipeServe() {
      if (_rawdataAddon) {
        return _rawdataAddon.StopPipeServe();
      }
      return ZoomSDKError.SDKERR_UNINITIALIZE
    }

    if (_addon) {
      _addon.SetMeetingStatusCB(meetingstatuscb);
      _addon.SetMeetingStatisticsWarningNotificationCB(onMeetingStatisticsWarningNotification);
      _addon.SetMeetingParameterNotificationCB(onMeetingParameterNotification);
      _addon.SetOnMeetingTopicChangedCB(onMeetingTopicChanged);
      _addon.SetOnMeetingFullToWatchLiveStreamCB(onMeetingFullToWatchLiveStream);
    }

    return {
      // Public methods and variables
      /**
      * Set meeting status changed callback function.
      * @method SetMeetingStatusCB
      * @param {Function} meetingstatuscb
      * @return {Boolean}
      */
      SetMeetingStatusCB: function (meetingstatuscb) {
        if (_addon && meetingstatuscb && meetingstatuscb instanceof Function) {
          _meetingstatuscb = meetingstatuscb;
          return true;
        }
        return false;
      },
      /**
      * Set meeting statistics warning notification callback function.
      * @method SetMeetingStatisticsWarningNotificationCB
      * @param {Function} onMeetingStatisticsWarningNotification
      * @return {Boolean}
      */
      SetMeetingStatisticsWarningNotificationCB: function (onMeetingStatisticsWarningNotification) {
        if (_addon && onMeetingStatisticsWarningNotification && onMeetingStatisticsWarningNotification instanceof Function) {
          _onMeetingStatisticsWarningNotification = onMeetingStatisticsWarningNotification;
          return true;
        }
        return false;
      },
      /**
      * Set meeting parameter notification callback function.
      * @method SetMeetingParameterNotificationCB
      * @param {Function} onMeetingParameterNotification
      * @return {Boolean}
      */
      SetMeetingParameterNotificationCB: function (onMeetingParameterNotification) {
        if (_addon && onMeetingParameterNotification && onMeetingParameterNotification instanceof Function) {
          _onMeetingParameterNotification = onMeetingParameterNotification;
          return true;
        }
        return false;
      },
      /**
      * Set callback event for the meeting topic changed. 
      * @method SetOnMeetingTopicChangedCB
      * @param {Function} onMeetingTopicChanged
      * @return {Boolean}
      */
      SetOnMeetingTopicChangedCB: function (onMeetingTopicChanged) {
        if (_addon && onMeetingTopicChanged && onMeetingTopicChanged instanceof Function) {
          _onMeetingTopicChanged = onMeetingTopicChanged;
          return true;
        }
        return false;
      },
      /**
      * Set calback event that the meeting users have reached the meeting capacity.
      * @method SetOnMeetingFullToWatchLiveStreamCB
      * @param {Function} onMeetingFullToWatchLiveStream
      * @return {Boolean}
      */
      SetOnMeetingFullToWatchLiveStreamCB: function (onMeetingFullToWatchLiveStream) {
        if (_addon && onMeetingFullToWatchLiveStream && onMeetingFullToWatchLiveStream instanceof Function) {
          _onMeetingFullToWatchLiveStream = onMeetingFullToWatchLiveStream;
          return true;
        }
        return false;
      },
      /**
      * Set Pipe Name.
      * @method SetPipeName
      * @param {String} videoPipeName
      * @param {String} sharePipeName
      * @param {String} audioPipeName
      * @param {Number} maxReadLength max value is 3840*2160
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      SetPipeName: function (opts) {
        let clientOpts = opts || {};
        let videoPipeName = clientOpts.videoPipeName;
        let sharePipeName = clientOpts.sharePipeName;
        let audioPipeName = clientOpts.audioPipeName;
        let maxReadLength = clientOpts.maxReadLength || 1920 * 1080 * 3 / 2;
        if (!videoPipeName || !sharePipeName || !audioPipeName) {
          return ZoomSDKError.SDKERR_INVALID_PARAMETER;
        }
        let pipeParams = {
          videoPipeName,
          sharePipeName,
          audioPipeName,
          maxReadLength
        }
        app.pipeParams = pipeParams
        return ZoomSDKError.SDKERR_SUCCESS;
      },
      /**
      * Start meeting.
      * @method StartMeeting
      * @param {Number} meetingnum A number to the meeting to be started.
      * @param {String} directshareappwndhandle Windows handle of which window you want to share directly (require hexadecimal)
      * @param {String} customer_key The customer key that need the app intergrated with sdk to specify. The SDK will set this value when the associated settings are turned on. The max length of customer_key is 35
      * @param {Boolean} isvideooff
      * @param {Boolean} isaudiooff
      * @param {Boolean} isdirectsharedesktop
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      StartMeeting: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          if (clientOpts.withRawdata) {
            let pipeParams = app.pipeParams
            if (!pipeParams.videoPipeName || !pipeParams.sharePipeName || !pipeParams.audioPipeName) {
              return ZoomSDKError.SDKERR_INVALID_PARAMETER;
            }
          } 
          let meetingnum = clientOpts.meetingnum || 0;
          let directshareappwndhandle = clientOpts.directshareappwndhandle || 0;
          let customer_key = clientOpts.customer_key || '';
          let isvideooff = (clientOpts.isvideooff === undefined) ? false : clientOpts.isvideooff;
          let isaudiooff = (clientOpts.isaudiooff === undefined) ? false : clientOpts.isaudiooff;
          let isdirectsharedesktop = (clientOpts.isdirectsharedesktop === undefined) ? false : clientOpts.isdirectsharedesktop;
          try {
            let StartMeetingParams = new messages.StartMeetingParams();
            StartMeetingParams.setMeetingnumber(Number(meetingnum));
            StartMeetingParams.setHdirectshareappwnd(directshareappwndhandle.toString());
            StartMeetingParams.setCustomerkey(customer_key);
            StartMeetingParams.setIsvideooff(isvideooff);
            StartMeetingParams.setIsaudiooff(isaudiooff);
            StartMeetingParams.setIsdirectsharedesktop(isdirectsharedesktop);
            let bytes = StartMeetingParams.serializeBinary();
            let result = _addon.Start(bytes);
            if (result == ZoomSDKError.SDKERR_SUCCESS) {
              startOrJoinWithRawdata = clientOpts.withRawdata;
            }
            return result;
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Start meeting without login
      * @method StartMeetingWithOutLogin
      * @param {String} usertoken [Required] User token
      * @param {String} zoomaccesstoken [Required] Zoom access token
      * @param {String} username [Required] User Name
      * @param {Number} zoomusertype [Required] User type, See ZoomUserType in settings.js
      * @param {Number} meetingnum [Optinal] Meeting number, meetingnum or vanityid is Required
      * @param {String} vanityid [Optinal] vanityid is suffix of Personal Link, meetingnum or vanityid is Required
      * @param {Number} directshareappwndhandle [Optinal] The window handle of the direct sharing application (require hexadecimal)
      * @param {String} customer_key [Optinal] The customer key that need the app intergrated with sdk to specify. The SDK will set this value when the associated settings are turned on. The max length of customer_key is 35
      * @param {Boolean} isdirectsharedesktop [Optinal] Share the desktop directly or not. True indicates to share
      * @param {Boolean} isvideooff [Optinal] Turn off the video or not, True indicates to turn off. In addition, this flag is affected by meeting attributes
      * @param {Boolean} isaudiooff [Optinal] Turn off the audio or not, True indicates to turn off. In addition, this flag is affected by meeting attributes.
      * @return {Number} Defined in: {@link ZoomSDKError}
      */
      StartMeetingWithOutLogin: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          if (clientOpts.withRawdata) {
            let pipeParams = app.pipeParams
            if (!pipeParams.videoPipeName || !pipeParams.sharePipeName || !pipeParams.audioPipeName) {
              return ZoomSDKError.SDKERR_INVALID_PARAMETER;
            }
          } 
          let zoomaccesstoken = clientOpts.zoomaccesstoken || '';
          let username = clientOpts.username || '';
          let zoomusertype = clientOpts.zoomusertype || ZoomUserType.ZoomUserType_APIUSER;
          let meetingnumber = clientOpts.meetingnum || 0;
          let vanityid = clientOpts.vanityid || '';
          let directshareappwndhandle = clientOpts.directshareappwndhandle || 0;
          let customer_key = clientOpts.customer_key || '';
          let isdirectsharedesktop = (clientOpts.isdirectsharedesktop === undefined) ? false : clientOpts.isdirectsharedesktop;
          let isvideooff = (clientOpts.isvideooff === undefined) ? false : clientOpts.isvideooff;
          let isaudiooff = (clientOpts.isaudiooff === undefined) ? false : clientOpts.isaudiooff;
          try {
            let StartWithoutLoginParams = new messages.StartWithoutLoginParams();
            StartWithoutLoginParams.setUserzak(zoomaccesstoken);
            StartWithoutLoginParams.setUsername(username);
            StartWithoutLoginParams.setNodeusertype(Number(zoomusertype));
            StartWithoutLoginParams.setMeetingnumber(Number(meetingnumber));
            StartWithoutLoginParams.setSdkvanityid(vanityid);
            StartWithoutLoginParams.setHdirectshareappwnd(directshareappwndhandle.toString());
            StartWithoutLoginParams.setCustomerkey(customer_key);
            StartWithoutLoginParams.setIsdirectsharedesktop(isdirectsharedesktop);
            StartWithoutLoginParams.setIsvideooff(isvideooff);
            StartWithoutLoginParams.setIsaudiooff(isaudiooff);
            let bytes = StartWithoutLoginParams.serializeBinary();
            let result = _addon.Start_WithoutLogin(bytes);
            if (result == ZoomSDKError.SDKERR_SUCCESS) {
              startOrJoinWithRawdata = clientOpts.withRawdata;
            }
            return result;
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Join the meeting.
      * @method JoinMeeting
      * @param {Number} meetingnum A number to the meeting to be joined, meetingnum or vanityid is Required
      * @param {String} vanityid vanityid is suffix of Personal Link, meetingnum or vanityid is Required
      * @param {String} username
      * @param {String} psw Meeting password
      * @param {Number} directshareappwndhandle Windows handle of which window you want to share directly (require hexadecimal)
      * @param {String} customer_key The customer key that need the app intergrated with sdk to specify. The SDK will set this value when the associated settings are turned on. The max length of customer_key is 35
      * @param {String} webinartoken webinar token
      * @param {Boolean} isvideooff
      * @param {Boolean} isaudiooff
      * @param {Boolean} isdirectsharedesktop
      * @param {String} app_privilege_token
      * @param {String} join_token
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      JoinMeeting: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          if (clientOpts.withRawdata) {
            let pipeParams = app.pipeParams
            if (!pipeParams.videoPipeName || !pipeParams.sharePipeName || !pipeParams.audioPipeName) {
              return ZoomSDKError.SDKERR_INVALID_PARAMETER;
            }
          }
          startOrJoinWithRawdata = clientOpts.withRawdata;
          let meetingnum = clientOpts.meetingnum || 0;
          let vanityid = clientOpts.vanityid || '';
          let username = clientOpts.username || '';
          let psw = clientOpts.psw || '';
          let directshareappwndhandle = clientOpts.directshareappwndhandle || 0;
          let customer_key = clientOpts.customer_key || '';
          let webinartoken = clientOpts.webinartoken || '';
          let isvideooff = (clientOpts.isvideooff === undefined) ? false : clientOpts.isvideooff;
          let isaudiooff = (clientOpts.isaudiooff === undefined) ? false : clientOpts.isaudiooff;
          let isdirectsharedesktop = (clientOpts.isdirectsharedesktop === undefined) ? false : clientOpts.isdirectsharedesktop;
          let app_privilege_token = clientOpts.app_privilege_token || '';
          let join_token = clientOpts.join_token || '';
          try {
            let JoinMeetingParams = new messages.JoinMeetingParams();
            JoinMeetingParams.setMeetingnumber(Number(meetingnum));
            JoinMeetingParams.setVanityid(vanityid);
            JoinMeetingParams.setUsername(username);
            JoinMeetingParams.setPsw(psw);
            JoinMeetingParams.setHdirectshareappwnd(directshareappwndhandle.toString());
            JoinMeetingParams.setCustomerkey(customer_key);
            JoinMeetingParams.setWebinartoken(webinartoken);
            JoinMeetingParams.setIsvideooff(isvideooff);
            JoinMeetingParams.setIsaudiooff(isaudiooff);
            JoinMeetingParams.setIsdirectsharedesktop(isdirectsharedesktop);
            JoinMeetingParams.setAppprivilegetoken(app_privilege_token);
            JoinMeetingParams.setJoinToken(join_token);
            let bytes = JoinMeetingParams.serializeBinary();
            let result = _addon.Join(bytes);
            if (result == ZoomSDKError.SDKERR_SUCCESS) {
              startOrJoinWithRawdata = clientOpts.withRawdata;
            }
            return result;
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Join meeting withoutlogin
      * @method JoinMeetingWithoutLogin
      * @param {Number} meetingnum [Required] Meeting Number, meetingnum or vanityid is Required
      * @param {String} vanityid [Optinal] vanityid is suffix of Personal Link, meetingnum or vanityid is Required
      * @param {String} username [Optinal] User Name
      * @param {String} psw [Optinal] Meeting password
      * @param {Number} directshareappwndhandle: [Optinal] The window handle of the direct sharing application (require hexadecimal)
      * @param {String} toke4enfrocelogin [Optinal] Use the token if the meeting required to login at first
      * @param {String} customer_key [Optinal] The customer key that need the app intergrated with sdk to specify. The SDK will set this value when the associated settings are turned on. The max length of customer_key is 35
      * @param {String} webinartoken [Optinal] Webinar token, if try to join webinar as a panelist
      * @param {Boolean} isdirectsharedesktop [Optinal] Share the desktop directly or not. True indicates to share
      * @param {Boolean} isvideooff [Optinal] Turn off the video or not. True indicates to turn off. In addition, this flag is affected by meeting attributes
      * @param {Boolean} isaudiooff [Optinal] Turn off the audio or not. True indicates to turn off. In addition, this flag is affected by meeting attributes
      * @param {String} app_privilege_token
      * @param {String} join_token
      * @return {Number} Defined in: {@link ZoomSDKError}
      */
      JoinMeetingWithoutLogin: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          if (clientOpts.withRawdata) {
            let pipeParams = app.pipeParams
            if (!pipeParams.videoPipeName || !pipeParams.sharePipeName || !pipeParams.audioPipeName) {
              return ZoomSDKError.SDKERR_INVALID_PARAMETER;
            }
          } 
          let meetingnum = clientOpts.meetingnum || 0;
          let vanityid = clientOpts.vanityid || '';
          let username = clientOpts.username || '';
          let psw = clientOpts.psw || '';
          let directshareappwndhandle = clientOpts.directshareappwndhandle || 0;
          let userZAK = clientOpts.userZAK || '';
          let customer_key = clientOpts.customer_key || '';
          let webinartoken = clientOpts.webinartoken || '';
          let isdirectsharedesktop = (clientOpts.isdirectsharedesktop === undefined) ? false : clientOpts.isdirectsharedesktop;
          let isvideooff = (clientOpts.isvideooff === undefined) ? false : clientOpts.isvideooff;
          let isaudiooff = (clientOpts.isaudiooff === undefined) ? false : clientOpts.isaudiooff;
          let app_privilege_token = clientOpts.app_privilege_token || '';
          let join_token = clientOpts.join_token || '';
          let onBehalfToken = clientOpts.onBehalfToken || '';
          try {
            let JoinWithoutLoginParams = new messages.JoinWithoutLoginParams();
            JoinWithoutLoginParams.setMeetingnumber(Number(meetingnum));
            JoinWithoutLoginParams.setVanityid(vanityid);
            JoinWithoutLoginParams.setUsername(username);
            JoinWithoutLoginParams.setPsw(psw);
            JoinWithoutLoginParams.setHdirectshareappwnd(directshareappwndhandle.toString());
            JoinWithoutLoginParams.setUserzak(userZAK);
            JoinWithoutLoginParams.setCustomerkey(customer_key);
            JoinWithoutLoginParams.setWebinartoken(webinartoken);
            JoinWithoutLoginParams.setIsdirectsharedesktop(isaudiooff);
            JoinWithoutLoginParams.setIsdirectsharedesktop(isdirectsharedesktop);
            JoinWithoutLoginParams.setIsvideooff(isvideooff);
            JoinWithoutLoginParams.setIsaudiooff(isaudiooff);
            JoinWithoutLoginParams.setAppprivilegetoken(app_privilege_token);
            JoinWithoutLoginParams.setJoinToken(join_token);
            JoinWithoutLoginParams.setOnbehalftoken(onBehalfToken);
            let bytes = JoinWithoutLoginParams.serializeBinary();
            let result = _addon.Join_WithoutLogin(bytes);
            if (result == ZoomSDKError.SDKERR_SUCCESS) {
              startOrJoinWithRawdata = clientOpts.withRawdata;
            }
            return result;
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Leave meeting.
      * @method LeaveMeeting
      * @param {Boolean} endMeeting
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      LeaveMeeting: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let endMeeting = clientOpts.endMeeting == undefined ? false : clientOpts.endMeeting;
          try {
            let LeaveMeetingParams = new messages.LeaveMeetingParams();
            LeaveMeetingParams.setBend(endMeeting);
            let bytes = LeaveMeetingParams.serializeBinary();
            return _addon.Leave(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Lock the current meeting.
       * @method Lock_Meeting
       * @return {Number} If the function succeed, the return value is the SDKERR_SUCCESS.
	        Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      Lock_Meeting: function () {
        if (_addon) {
          return _addon.Lock();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Unlock the current meeting.
      * @method Un_lock_Meeting
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      Un_lock_Meeting: function () {
        if (_addon) {
          return _addon.Unlock();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Determine if the current user can change the meeting topic.
      * @method CanSetMeetingTopic
      * @return {Boolean} If it can change the meeting topic, the return value is true.
      */
      CanSetMeetingTopic: function () {
        if (_addon) {
          return _addon.CanSetMeetingTopic();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Change the meeting topic.
      * @method SetMeetingTopic
      * @param {String} sTopic The new meeting topic. 
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      SetMeetingTopic: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let sTopic = clientOpts.sTopic;
          try {
            let SetMeetingTopicParam = new messages.SetMeetingTopicParam();
            SetMeetingTopicParam.setStopic(sTopic);
            let bytes = SetMeetingTopicParam.serializeBinary();
            return _addon.SetMeetingTopic(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Get the quality of Internet connection when sharing.
      * If you are not in the meeting, the Conn_Quality_Unknown will be returned.
      * @method GetSharingConnQuality
      * @return {Number} If the function succeed, the return is one of those enums Defined in: {@link ConnectionQuality}
      */
      GetSharingConnQuality: function () {
        if (_addon) {
          return _addon.GetSharingConnQuality();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Get the Internet connection quality of video.
       * If you are not in the meeting, the Conn_Quality_Unknown will be returned.
       * @method GetVideoConnQuality
       * @return {Number} If the function succeed, the return is one of those enums Defined in: {@link ConnectionQuality}
       */
      GetVideoConnQuality: function () {
        if (_addon) {
          return _addon.GetVideoConnQuality();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Get the Internet connection quality of audio.
       * If you are not in the meeting, the Conn_Quality_Unknown will be returned.
       * @method GetAudioConnQuality
       * @return {AudioConnQuality} If the function succeed, the return is one of those enums Defined in: {@link ConnectionQuality}
       */
      GetAudioConnQuality: function () {
        if (_addon) {
          return _addon.GetAudioConnQuality();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Join meeting with web uri
       * @method HandleZoomWebUriProtocolAction
       * @param {String} protocol_action
       * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	        Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
       */
      HandleZoomWebUriProtocolAction: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          if (clientOpts.withRawdata) {
            let pipeParams = app.pipeParams
            if (!pipeParams.videoPipeName || !pipeParams.sharePipeName || !pipeParams.audioPipeName) {
              return ZoomSDKError.SDKERR_INVALID_PARAMETER;
            }
          }
          let protocol_action = clientOpts.protocol_action;
          try {
            let HandleZoomWebUriProtocolActionParams = new messages.HandleZoomWebUriProtocolActionParams();
            HandleZoomWebUriProtocolActionParams.setProtocolaction(protocol_action);
            let bytes = HandleZoomWebUriProtocolActionParams.serializeBinary();
            let result = _addon.HandleZoomWebUriProtocolAction(bytes);
            if (result == ZoomSDKError.SDKERR_SUCCESS) {
              startOrJoinWithRawdata = clientOpts.withRawdata;
            }
            return result;
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      GetMeetingInfo: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          clientOpts.addon = _addon;
          if (!ZoomMeetingInfo) {
            ZoomMeetingInfo = ZoomMeetingINFOMOD.ZoomMeetingInfo.getInstance(clientOpts);
          }
          return ZoomMeetingInfo;
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      GetMeetingUICtrl: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomMeetingUICtrl) {
            ZoomMeetingUICtrl = ZoomMeetingUIMOD.ZoomMeetingUICtrl.getInstance(clientOpts);
          }
          return ZoomMeetingUICtrl;
        }
      },
      GetAnnotationCtrl: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomAnnotationCtrl) {
            ZoomAnnotationCtrl = ZoomAnnotationMOD.ZoomAnnotationCtrl.getInstance(clientOpts);
          }
          return ZoomAnnotationCtrl;
        }
      },
      GetMeetingAudio: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomMeetingAudio) {
            ZoomMeetingAudio = ZoomMeetingAudioMOD.ZoomMeetingAudio.getInstance(clientOpts);
          }
          return ZoomMeetingAudio;
        }
      },
      GetMeetingVideo: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomMeetingVideo) {
            ZoomMeetingVideo = ZoomMeetingVideoMOD.ZoomMeetingVideo.getInstance(clientOpts);
          }
          return ZoomMeetingVideo
        }
      },
      GetMeetingShare: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomMeetingShare) {
            ZoomMeetingShare = ZoomMeetingShareMOD.ZoomMeetingShare.getInstance(clientOpts);
          }
          return ZoomMeetingShare;
        }
      },
      GetMeetingH323: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomH323) {
            ZoomH323 = ZoomMeetingH323MOD.ZoomH323.getInstance(clientOpts);
          }
          return ZoomH323;
        }
      },
      GetMeetingConfiguration: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomMeetingConfiguration) {
            ZoomMeetingConfiguration = ZoomMeetingConfigurationMOD.ZoomMeetingConfiguration.getInstance(clientOpts);
          }
          return ZoomMeetingConfiguration;
        }
      },
      GetUpdateAccount: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomPaymentReminder) {
            ZoomPaymentReminder = ZoomUpdateAccountMOD.ZoomPaymentReminder.getInstance(clientOpts);
          }
          return ZoomPaymentReminder;
        }
      },
      GetMeetingParticipantsCtrl: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomMeetingParticipantsCtrl) {
            ZoomMeetingParticipantsCtrl = ZoomMeetingParticipantsMOD.ZoomMeetingParticipantsCtrl.getInstance(clientOpts);
          }
          return ZoomMeetingParticipantsCtrl;
        }
      },
      GetMeetingRecording: function(opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomMeetingRecording) {
            ZoomMeetingRecording = ZoomMeetingRecordingMOD.ZoomMeetingRecording.getInstance(clientOpts);
          }
          return ZoomMeetingRecording;
        }
      },
      GetMeetingAAN: function(opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomMeetingAAN) {
            ZoomMeetingAAN = ZoomMeetingAANMOD.ZoomMeetingAAN.getInstance(clientOpts);
          }
          return ZoomMeetingAAN;
        }
      },
      GetMeetingRawArchivingCtrl: function(opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomMeetingRawArchivingCtrl) {
            ZoomMeetingRawArchivingCtrl = ZoomMeetingRawArchivingCtrlMOD.ZoomMeetingRawArchivingCtrl.getInstance(clientOpts);
          }
          return ZoomMeetingRawArchivingCtrl;
        }
      },
      GetMeetingLiveStreamCtrl: function(opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomMeetingLiveStreamCtrl) {
            ZoomMeetingLiveStreamCtrl = ZoomMeetingLiveStreamCtrlMOD.ZoomMeetingLiveStreamCtrl.getInstance(clientOpts);
          }
          return ZoomMeetingLiveStreamCtrl;
        }
      },
      GetMeetingRequestRawLiveStreamPrivilegeHandler: function(opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomMeetingRequestRawLiveStreamPrivilegeHandler) {
            ZoomMeetingRequestRawLiveStreamPrivilegeHandler = ZoomMeetingRequestRawLiveStreamPrivilegeHandlerMOD.ZoomMeetingRequestRawLiveStreamPrivilegeHandler.getInstance(clientOpts);
          }
          return ZoomMeetingRequestRawLiveStreamPrivilegeHandler;
        }
      },
      GetMeetingReminder: function(opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomMeetingReminder) {
            ZoomMeetingReminder = ZoomMeetingReminderMOD.ZoomMeetingReminder.getInstance(clientOpts);
          }
          return ZoomMeetingReminder;
        }
      },
      GetMeetingChat: function(opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomMeetingChat) {
            ZoomMeetingChat = ZoomMeetingChatMOD.ZoomMeetingChat.getInstance(clientOpts);
          }
          return ZoomMeetingChat;
        }
      },
      GetMeetingWaitingRoom: function(opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomMeetingWaitingRoom) {
            ZoomMeetingWaitingRoom = ZoomMeetingWaitingRoomMOD.ZoomMeetingWaitingRoom.getInstance(clientOpts);
          }
          return ZoomMeetingWaitingRoom;
        }
      },
      GetMeetingAICompanion: function(opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomMeetingAICompanion) {
            ZoomMeetingAICompanion = ZoomMeetingAICompanionMOD.ZoomMeetingAICompanion.getInstance(clientOpts);
          }
          return ZoomMeetingAICompanion;
        }
      },
      GetMeetingIndicator: function(opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomMeetingIndicator) {
            ZoomMeetingIndicator = ZoomMeetingIndicatorMOD.ZoomMeetingIndicator.getInstance(clientOpts);
          }
          return ZoomMeetingIndicator;
        }
      },
      GetMeetingWebinar: function(opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomMeetingWebinar) {
            ZoomMeetingWebinar = ZoomMeetingWebinarMOD.ZoomMeetingWebinar.getInstance(clientOpts);
          }
          return ZoomMeetingWebinar;
        }
      },
      GetMeetingQA: function(opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomMeetingQA) {
            ZoomMeetingQA = ZoomMeetingQAMOD.ZoomMeetingQA.getInstance(clientOpts);
          }
          return ZoomMeetingQA;
        }
      },
      GetMeetingReaction: function(opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomMeetingReaction) {
            ZoomMeetingReaction = ZoomMeetingReactionMOD.ZoomMeetingReaction.getInstance(clientOpts);
          }
          return ZoomMeetingReaction;
        }
      },
      GetMeetingCloseCaption: function(opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomMeetingCloseCaption) {
            ZoomMeetingCloseCaption = ZoomMeetingCloseCaptionMOD.ZoomMeetingCloseCaption.getInstance(clientOpts);
          }
          return ZoomMeetingCloseCaption;
        }
      },
      GetMeetingPolling: function(opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomMeetingPolling) {
            ZoomMeetingPolling = ZoomMeetingPollingMOD.ZoomMeetingPolling.getInstance(clientOpts);
          }
          return ZoomMeetingPolling;
        }
      },
      GetMeetingWhiteboard: function(opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomMeetingWhiteboard) {
            ZoomMeetingWhiteboard = ZoomMeetingWhiteboardMOD.ZoomMeetingWhiteboard.getInstance(clientOpts);
          }
          return ZoomMeetingWhiteboard;
        }
      },
      GetMeetingDocs: function(opts) {
        if (_addon) {
          let clientOpts = opts || {};
          clientOpts.addon = _addon;
          clientOpts.zoommeeting = instance;
          if (!ZoomMeetingDocs) {
            ZoomMeetingDocs = ZoomMeetingDocsMOD.ZoomMeetingDocs.getInstance(clientOpts);
          }
          return ZoomMeetingDocs;
        }
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
  ZoomMeeting: ZoomMeeting
}
