let { ZoomSDKError } = require('./settings.js');
const messages = require('./electron_sdk_pb.js');

var ZoomMeetingRecording = (function () {
  var instance;
  /**
  * Zoom Meeting Recording
  * @module zoom_meeting_recording
  * @param {Function} onRecording2MP4Done Callback event of ending the conversion to MP4 format.
  * @param {Function} onRecording2MP4Processing Callback event of the process of the conversion to MP4 format.
  * @param {Function} onRecordingStatus Callback event that the status of local recording changes, Defined in: {@link RecordingStatus}
  * @param {Function} onCloudRecordingStatus Callback event that the status of cloud recording changes, Defined in: {@link RecordingStatus}
  * @param {Function} onRecordPrivilegeChanged Callback event that the recording authority changes.
  * @param {Function} onCloudRecordingStorageFull Callback event that the cloud recording storage is full.
  * @param {Function} onRequestCloudRecordingResponse Callback event for when the host responds to a cloud recording permission request, Defined in: {@link RequestStartCloudRecordingStatus}
  * @param {Function} onStartCloudRecordingRequested Callback event received only by the host when a user requests to start cloud recording.
  * @param {Function} onEnableAndStartSmartRecordingRequested Callback event received only by the host when a user requests to enable and start smart cloud recording.
  * @param {Function} onSmartRecordingEnableActionCallback Callback event received when you call {@link EnableSmartRecording}. You can use the handler to confirm or cancel to enable the smart recording.
  * @return {ZoomMeetingRecording}
  */
  function init(opts) {
    let clientOpts = opts || {};
    // Private methods and variables
    let _addon = clientOpts.addon.GetMeetingRecordingCtrl() || null;
    let _onRecording2MP4Done = clientOpts.onRecording2MP4Done || null;
    let _onRecording2MP4Processing = clientOpts.onRecording2MP4Processing || null;
    let _onRecordingStatus = clientOpts.onRecordingStatus || null;
    let _onCloudRecordingStatus = clientOpts.onCloudRecordingStatus || null;
    let _onRecordPrivilegeChanged = clientOpts.onRecordPrivilegeChanged || null;
    let _onCloudRecordingStorageFull = clientOpts.onCloudRecordingStorageFull || null;
    let _onRequestCloudRecordingResponse = clientOpts.onRequestCloudRecordingResponse || null;
    let _onStartCloudRecordingRequested = clientOpts.onStartCloudRecordingRequested || null;
    let _onEnableAndStartSmartRecordingRequested = clientOpts.onEnableAndStartSmartRecordingRequested || null;
    let _onSmartRecordingEnableActionCallback = clientOpts.onSmartRecordingEnableActionCallback || null;

    /**
      Callback event of ending the conversion to MP4 format.
      In order to trigger this callback correctly, you need call EnableLocalRecordingConvertProgressBarDialog(false) before you start a meeting.
      @event onRecording2MP4Done
      @param {Boolean} success TRUE indicates to convert successfully. FALSE not.
      @param {String} result This value is used to save the error code only when the convert fails.
      @param {String} path If the conversion is successful, this value is used to save the path of the recording file.
    */
    function onRecording2MP4Done(success, result, path) {
      if (_onRecording2MP4Done) {
        _onRecording2MP4Done(success, result, path)
      }
    }

    /**
      Callback event of the process of the conversion to MP4 format.
      In order to trigger this callback correctly, you need call EnableLocalRecordingConvertProgressBarDialog(false) before you start a meeting.
      @event onRecording2MP4Processing
      @param {String} percentage Percentage of conversion process. Range from ZERO(0) to ONE HUNDREAD(100).
    */
    function onRecording2MP4Processing(percentage) {
      if (_onRecording2MP4Processing) {
        _onRecording2MP4Processing(percentage)
      }
    }

    /**
      Callback event that the status of local recording changes.
      @event onRecordingStatus
      @param {String} status Value of recording status. For more details, Defined in: {@link RecordingStatus}
    */
    function onRecordingStatus(status) {
      if (_onRecordingStatus) {
        _onRecordingStatus(status)
      }
    }

    /**
      Callback event that the status of cloud recording changes.
      @event onCloudRecordingStatus
      @param {String} status Value of recording status. For more details, Defined in: {@link RecordingStatus}
    */
    function onCloudRecordingStatus(status) {
      if (_onCloudRecordingStatus) {
        _onCloudRecordingStatus(status)
      }
    }

    /**
      Callback event that the recording authority changes.
      @event onRecordPrivilegeChanged
      @param {Boolean} canRec TRUE indicates to enable to record.
    */
    function onRecordPrivilegeChanged(canRec) {
      if (_onRecordPrivilegeChanged) {
        _onRecordPrivilegeChanged(canRec)
      }
    }

    /**
      Callback event that the cloud recording storage is full.
      @event onCloudRecordingStorageFull
      @param {Number} gracePeriodDate a point in time, in milliseconds, in UTC. You can use the cloud recording storage until the gracePeriodDate.
    */
    function onCloudRecordingStorageFull(gracePeriodDate) {
      if (_onCloudRecordingStorageFull) {
        _onCloudRecordingStorageFull(gracePeriodDate)
      }
    }

    /**
      Callback event for when the host responds to a cloud recording permission request.
      @event onRequestCloudRecordingResponse
      @param {String} status Value of request host to start cloud recording response status. For more details, see {@link RequestStartCloudRecordingStatus}
    */
    function onRequestCloudRecordingResponse(status) {
      if (_onRequestCloudRecordingResponse) {
        _onRequestCloudRecordingResponse(status)
      }
    }

    /**
      Callback event received only by the host when a user requests to start cloud recording.
      @event onStartCloudRecordingRequested
    */
    function onStartCloudRecordingRequested() {
      if (_onStartCloudRecordingRequested) {
        _onStartCloudRecordingRequested()
      }
    }

    /**
      Callback event received only by the host when a user requests to enable and start smart cloud recording.
      @event onEnableAndStartSmartRecordingRequested
    */
    function onEnableAndStartSmartRecordingRequested() {
      if (_onEnableAndStartSmartRecordingRequested) {
        _onEnableAndStartSmartRecordingRequested()
      }
    }

    /**
      Callback event received when you call {@link EnableSmartRecording}. You can use the handler to confirm or cancel to enable the smart recording.
      @event onSmartRecordingEnableActionCallback
    */
    function onSmartRecordingEnableActionCallback() {
      if (_onSmartRecordingEnableActionCallback) {
        _onSmartRecordingEnableActionCallback()
      }
    }

    if (_addon) {
      _addon.SetonRecording2MP4DoneCB(onRecording2MP4Done);
      _addon.SetonRecording2MP4ProcessingCB(onRecording2MP4Processing);
      _addon.SetonRecordingStatusCB(onRecordingStatus);
      _addon.SetonCloudRecordingStatusCB(onCloudRecordingStatus);
      _addon.SetonRecordPrivilegeChangedCB(onRecordPrivilegeChanged);
      _addon.SetonCloudRecordingStorageFullCB(onCloudRecordingStorageFull);
      _addon.SetonRequestCloudRecordingResponseCB(onRequestCloudRecordingResponse);
      _addon.SetonStartCloudRecordingRequestedCB(onStartCloudRecordingRequested);
      _addon.SetonEnableAndStartSmartRecordingRequestedCB(onEnableAndStartSmartRecordingRequested);
      _addon.SetonSmartRecordingEnableActionCallback(onSmartRecordingEnableActionCallback);
    }

    return {
      /**
       * Set on Recording 2MP4 Done Callback
       * @method SetonRecording2MP4DoneCB
       * @param {Function} onRecording2MP4Done
       * @return {Boolean}
       */
      SetonRecording2MP4DoneCB: function (onRecording2MP4Done) {
        if (_addon && onRecording2MP4Done && onRecording2MP4Done instanceof Function) {
          _onRecording2MP4Done = onRecording2MP4Done;
          return true;
        }
        return false
      },
      /**
       * Set on Recording 2MP4 Processing Callback
       * @method SetonRecording2MP4ProcessingCB
       * @param {Function} onRecording2MP4Processing
       * @return {Boolean}
       */
      SetonRecording2MP4ProcessingCB: function (onRecording2MP4Processing) {
        if (_addon && onRecording2MP4Processing && onRecording2MP4Processing instanceof Function) {
          _onRecording2MP4Processing = onRecording2MP4Processing;
          return true;
        }
        return false;
      },
      /**
       * Set on Recording Status Callback
       * @method SetonRecordingStatusCB
       * @param {Function} onRecordingStatus
       * @return {Boolean}
       */
      SetonRecordingStatusCB: function (onRecordingStatus) {
        if (_addon && onRecordingStatus && onRecordingStatus instanceof Function) {
          _onRecordingStatus = onRecordingStatus;
          return true;
        }
        return false;
      },
      /**
       * Set on Cloud Recording Status Callback
       * @method SetonCloudRecordingStatusCB
       * @param {Function} onCloudRecordingStatus
       * @return {Boolean}
       */
      SetonCloudRecordingStatusCB: function (onCloudRecordingStatus) {
        if (_addon && onCloudRecordingStatus && onCloudRecordingStatus instanceof Function) {
          _onCloudRecordingStatus = onCloudRecordingStatus;
          return true;
        }
        return false;
      },
      /**
       * Set on Record Privilege Changed Callback
       * @method SetonRecordPrivilegeChangedCB
       * @param {Function} onRecordPrivilegeChanged
       * @return {Boolean}
       */
      SetonRecordPrivilegeChangedCB: function (onRecordPrivilegeChanged) {
        if (_addon && onRecordPrivilegeChanged && onRecordPrivilegeChanged instanceof Function) {
          _onRecordPrivilegeChanged = onRecordPrivilegeChanged;
          return true;
        }
        return false;
      },
      /**
       * Set the cloud recording storage is full Callback
       * @method SetonCloudRecordingStorageFullCB
       * @param {Function} onCloudRecordingStorageFull
       * @return {Boolean}
       */
      SetonCloudRecordingStorageFullCB: function (onCloudRecordingStorageFull) {
        if (_addon && onCloudRecordingStorageFull && onCloudRecordingStorageFull instanceof Function) {
          _onCloudRecordingStorageFull = onCloudRecordingStorageFull;
          return true;
        }
        return false;
      },
      /**
       * Set callback event for when the host responds to a cloud recording permission request.
       * @method SetonRequestCloudRecordingResponseCB
       * @param {Function} onRequestCloudRecordingResponse
       * @return {Boolean}
       */
      SetonRequestCloudRecordingResponseCB: function (onRequestCloudRecordingResponse) {
        if (_addon && onRequestCloudRecordingResponse && onRequestCloudRecordingResponse instanceof Function) {
          _onRequestCloudRecordingResponse = onRequestCloudRecordingResponse;
          return true;
        }
        return false;
      },
      /**
       * Set callback event received only by the host when a user requests to start cloud recording.
       * @method SetonStartCloudRecordingRequestedCB
       * @param {Function} onStartCloudRecordingRequested
       * @return {Boolean}
       */
      SetonStartCloudRecordingRequestedCB: function (onStartCloudRecordingRequested) {
        if (_addon && onStartCloudRecordingRequested && onStartCloudRecordingRequested instanceof Function) {
          _onStartCloudRecordingRequested = onStartCloudRecordingRequested;
          return true;
        }
        return false;
      },
      /**
       * Set callback event received only by the host when a user requests to enable and start smart cloud recording.
       * @method SetonEnableAndStartSmartRecordingRequestedCB
       * @param {Function} onEnableAndStartSmartRecordingRequested
       * @return {Boolean}
       */
      SetonEnableAndStartSmartRecordingRequestedCB: function (onEnableAndStartSmartRecordingRequested) {
        if (_addon && onEnableAndStartSmartRecordingRequested && onEnableAndStartSmartRecordingRequested instanceof Function) {
          _onEnableAndStartSmartRecordingRequested = onEnableAndStartSmartRecordingRequested;
          return true;
        }
        return false;
      },
      /**
       * Set callback event received when you call {@link EnableSmartRecording}. You can use the handler to confirm or cancel to enable the smart recording.
       * @method SetonSmartRecordingEnableActionCallback
       * @param {Function} onSmartRecordingEnableActionCallback
       * @return {Boolean}
       */
      SetonSmartRecordingEnableActionCallback: function (onSmartRecordingEnableActionCallback) {
        if (_addon && onSmartRecordingEnableActionCallback && onSmartRecordingEnableActionCallback instanceof Function) {
          _onSmartRecordingEnableActionCallback = onSmartRecordingEnableActionCallback;
          return true;
        }
        return false;
      },
      /**
       * Start recording.
       * @method StartRecording
       * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
          Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
       */
      StartRecording: function () {
        if (_addon) {
          try {
            return _addon.StartRecording();
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Stop recording.
       * @method StopRecording
       * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
          Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
       */
      StopRecording: function () {
        if (_addon) {
          return _addon.StopRecording();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Determine if the specified user is enabled to start recording.
       * @method CanStartRecording
       * @param {Boolean} cloud_recording TRUE indicates to determine whether to enable the cloud recording. FALSE local recording.
       * @param {Number} userid Specifies the user ID.
       * @return {Number} If the value of cloud_recording is set to TRUE and the cloud recording is enabled, the return value is SDKERR_SUCCESS.
          If the value of cloud_recording is set to FALSE and the local recording is enabled, the return value is SDKERR_SUCCESS.
          Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
       */
      CanStartRecording: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let cloud_recording = clientOpts.cloud_recording;
          let userid = clientOpts.userid;
          try {
            let CanStartRecordingParams = new messages.CanStartRecordingParams();
            CanStartRecordingParams.setBcloudrecording(cloud_recording);
            CanStartRecordingParams.setUserid(Number(userid));
            let bytes = CanStartRecordingParams.serializeBinary();
            return _addon.CanStartRecording(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Determine if the current user own the authority to change the recording permission of the others.
      * @method CanAllowDisAllowLocalRecording
      * @return {Number} If the user own the authority, the return value is SDKERR_SUCCESS.
        Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      CanAllowDisAllowLocalRecording: function () {
        if (_addon) {
          return _addon.CanAllowDisAllowLocalRecording();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Start cloud recording.
       * @method StartCloudRecording
       * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
          Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
       */
      StartCloudRecording: function () {
        if (_addon) {
          return _addon.StartCloudRecording();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Stop cloud recording.
       * @method StopCloudRecording
       * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
          Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
       */
      StopCloudRecording: function () {
        if (_addon) {
          return _addon.StopCloudRecording();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Determine if the user owns the authority to enable the local recording.
       * @method IsSupportLocalRecording
       * @param {Number} userid Specifies the user ID.
       * @return {Number} If the specified user is enabled to start local recording, the return value is SDKERR_SUCCESS.
          Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
       */
      IsSupportLocalRecording: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let userid = clientOpts.userid;
          try {
            let IsSupportLocalRecordingParams = new messages.IsSupportLocalRecordingParams();
            IsSupportLocalRecordingParams.setUserid(userid);
            let bytes = IsSupportLocalRecordingParams.serializeBinary();
            return _addon.IsSupportLocalRecording(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Give the specified user authority for local recording.
       * @method AllowLocalRecording
       * @param {Number} userid Specifies the user ID.
       * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
          Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
       */
      AllowLocalRecording: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let userid = clientOpts.userid;
          try {
            let AllowLocalRecordingParams = new messages.AllowLocalRecordingParams();
            AllowLocalRecordingParams.setUserid(userid);
            let bytes = AllowLocalRecordingParams.serializeBinary();
            return _addon.AllowLocalRecording(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Abrogate the authority of the specified user for local recoding.
       * @method DisAllowLocalRecording
       * @param {Number} userid Specifies the user ID.
       * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
        Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
       */
      DisAllowLocalRecording: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let userid = clientOpts.userid;
          try {
            let DisAllowLocalRecordingParams = new messages.DisAllowLocalRecordingParams();
            DisAllowLocalRecordingParams.setUserid(userid);
            let bytes = DisAllowLocalRecordingParams.serializeBinary();
            return _addon.DisAllowLocalRecording(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Send a request to ask the host to start cloud recording.
       * @method RequestStartCloudRecording
       * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS and the SDK sends the request.
          Otherwise it fails and the request is not sent. To get extended error information, Defined in: {@link ZoomSDKError}
       */
      RequestStartCloudRecording: function () {
        if (_addon) {
          return _addon.RequestStartCloudRecording();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Determine if the smart recording feature is enabled in the meeting.
       * @method IsSmartRecordingEnabled
       * @return {Boolean} true means that the feature enabled, false means that the feature isn't enabled.
       */
      IsSmartRecordingEnabled: function () {
        if (_addon) {
          return _addon.IsSmartRecordingEnabled();
        }
        return false;
      },
      /**
       * Whether the current user can enable the smart recording feature.
       * @method CanEnableSmartRecordingFeature
       * @return {Boolean} True means the current user can enable the smart recording feature.
       */
      CanEnableSmartRecordingFeature: function () {
        if (_addon) {
          return _addon.CanEnableSmartRecordingFeature();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Enable the smart recording feature.
       * @method EnableSmartRecording
       * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
          Otherwise the function fails. To get extended error information, Defined in: {@link ZoomSDKError}
       */
      EnableSmartRecording: function () {
        if (_addon) {
          return _addon.EnableSmartRecording();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Get the user ID who requested that the host start cloud recording.
       * @method GetRequesterId
       * @return {Number} If the function succeeds, the return value is the user ID. Otherwise, this returns 0.
       */
      GetRequesterId: function () {
        if (_addon) {
          return _addon.GetRequesterId();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Get the user name who requested that the host start cloud recording.
       * @method GetRequesterName
       * @return {String} If the function succeeds, the return value is the user name.
       */
      GetRequesterName: function () {
        if (_addon) {
          return _addon.GetRequesterName();
        }
        return "";
      },
      /**
       * Accept the request to start cloud recording and then destroys the IRequestCloudRecordingHandler instance.
       * @method Start
       * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
          Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
       */
      Start: function () {
        if (_addon) {
          return _addon.Start();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Deny the request to start cloud recording and then destroys the IRequestCloudRecordingHandler instance.
       * @method Deny
       * @param {Boolean} bDenyAll TRUE indicates to deny all requests. Participants can't send requests again until the host change the setting.
       * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
        Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
       */
      Deny: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let bDenyAll = clientOpts.bDenyAll;
          try {
            let DenyRequestStartCloudRecordingParams = new messages.DenyRequestStartCloudRecordingParams();
            DenyRequestStartCloudRecordingParams.setBdenyall(bDenyAll);
            let bytes = DenyRequestStartCloudRecordingParams.serializeBinary();
            return _addon.Deny(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Get the user ID who requested to enable and start smart cloud recording.
       * @method GetRequestEnableAndStartSmartRecordingUserId
       * @return {Number} If the function succeeds, the return value is the user ID.
       */
      GetRequestEnableAndStartSmartRecordingUserId: function () {
        if (_addon) {
          return _addon.GetRequestEnableAndStartSmartRecordingUserId();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Get the legal tip that you should agree to handle the user request.
       * @method GetRequestEnableAndStartSmartRecordingTipString
       * @return {String} If the function succeeds, the return value is the legal notice about enable and start smart cloud recording.
       */
      GetRequestEnableAndStartSmartRecordingTipString: function () {
        if (_addon) {
          return _addon.GetRequestEnableAndStartSmartRecordingTipString();
        }
        return "";
      },
      /**
       * Normal cloud recording without enable smart recording.
       * @method StartCloudRecordingWithoutEnableSmartRecording
       * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
          Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
       */
      StartCloudRecordingWithoutEnableSmartRecording: function () {
        if (_addon) {
          return _addon.StartCloudRecordingWithoutEnableSmartRecording();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Agree the legal notice to enable and start smart cloud recording.
       * @method AgreeToEnableAndStartSmartRecording
       * @param {Boolean} bAllMeetings bAllMeetings True indicates to enable smart recording for all future meetings (include the current meeting). False indicates to only enable smart recording for the current meeting.
       * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
        Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
       */
      AgreeToEnableAndStartSmartRecording: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let bAllMeetings = clientOpts.bAllMeetings;
          try {
            let AgreeToEnableAndStartSmartRecordingParams = new messages.AgreeToEnableAndStartSmartRecordingParams();
            AgreeToEnableAndStartSmartRecordingParams.setBallmeetings(bAllMeetings);
            let bytes = AgreeToEnableAndStartSmartRecordingParams.serializeBinary();
            return _addon.AgreeToEnableAndStartSmartRecording(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Decline the request to start cloud recording.
       * @method DeclineEnableAndStartSmartRecording
       * @param {Boolean} bDenyAll True indicates to deny all attendees' requests for the host to start cloud recording. Participants can't send these types of requests again until the host change the setting.
       * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
        Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
       */
      DeclineEnableAndStartSmartRecording: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let bDenyAll = clientOpts.bDenyAll;
          try {
            let DeclineEnableAndStartSmartRecordingParams = new messages.DeclineEnableAndStartSmartRecordingParams();
            DeclineEnableAndStartSmartRecordingParams.setBdenyall(bDenyAll);
            let bytes = DeclineEnableAndStartSmartRecordingParams.serializeBinary();
            return _addon.DeclineEnableAndStartSmartRecording(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Get the legal tip to enable the smart recording.
       * @method GetSmartRecordingEnableActionTipString
       * @return {String} The legal notice.
       */
      GetSmartRecordingEnableActionTipString: function () {
        if (_addon) {
          return _addon.GetSmartRecordingEnableActionTipString();
        }
        return "";
      },
      /**
       * Confirm to enable and start the smart recording.
       * @method ActionConfirmEnableSmartRecording
       * @param {Boolean} bAllMeetings True indicates to enable smart recording for all future meetings (include the current meeting). False indicates to only enable smart recording for the current meeting.
       * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
        Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
       */
      ActionConfirmEnableSmartRecording: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let bAllMeetings = clientOpts.bAllMeetings;
          try {
            let ActionConfirmEnableSmartRecordingParams = new messages.ActionConfirmEnableSmartRecordingParams();
            ActionConfirmEnableSmartRecordingParams.setBallmeetings(bAllMeetings);
            let bytes = ActionConfirmEnableSmartRecordingParams.serializeBinary();
            return _addon.ActionConfirmEnableSmartRecording(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Cancel enable and start the smart recording.
       * @method ActionCancelEnableSmartRecording
       * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
          Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
       */
      ActionCancelEnableSmartRecording: function () {
        if (_addon) {
          return _addon.ActionCancelEnableSmartRecording();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      }
    };
  }
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
  ZoomMeetingRecording: ZoomMeetingRecording
};
