const { ZoomSDKError } = require('./settings.js');
const messages = require('./electron_sdk_pb.js');

var ZoomMeetingDocs = (function () {
  var instance;
  
  /**
   * Zoom Meeting Docs
   * @module zoom_meeting_docs
   * @param {Function} onDocsSharingSourceInfoChanged Callback for document status change. Notifies when document sharing starts/stops. All meeting participants receive this event.
   * @param {Function} ondocspermissionchanged Callback event of docs share setting type changed.
   * @return {ZoomMeetingDocs}
   */
  function init(opts) {
    let clientOpts = opts || {};
    let _addon = clientOpts.addon.GetMeetingDocsCtrl() || null;
    let _onDocsSharingSourceInfoChanged = clientOpts.onDocsSharingSourceInfoChanged || null;
    let _onDocsPermissionChanged = clientOpts.onDocsPermissionChanged || null;
    
    /**
     * Callback for document status change. Notifies when document sharing starts/stops. All meeting participants receive this event.
     * @event onDocsSharingSourceInfoChanged
     * @param {Object} info
    */
    function onDocsSharingSourceInfoChanged(info) {
      if (_onDocsSharingSourceInfoChanged) {
        _onDocsSharingSourceInfoChanged(info);
      }
    }
    
    /**
     * Callback event of docs share setting type changed.
     * @event onDocsPermissionChanged
     * @param {Number} createOption Docs who can create new docs type. For more details, see \link SDKDocsCreateOption \endlink enum.
     * @param {Number} option Docs who can share type. For more details, see \link SDKDocsShareOption \endlink enum.
    */
    function ondocspermissionchanged(createOption, option) {
      if (_onDocsPermissionChanged) {
        _onDocsPermissionChanged(createOption, option);
      }
    }

    if (_addon) {
      _addon.SetOnDocsSharingSourceInfoChangedCB(onDocsSharingSourceInfoChanged);
      _addon.SetOnDocsPermissionChangedCB(ondocspermissionchanged);
    }

    return {
      /**
       * Set document status change callback. Notifies when document sharing starts/stops. All meeting participants receive this event.
       * @method SetOnDocsSharingSourceInfoChangedCB
       * @param {Function} onDocsSharingSourceInfoChanged
       * @return {Boolean}
       */
      SetOnDocsSharingSourceInfoChangedCB: function (onDocsSharingSourceInfoChanged) {
        if (_addon && onDocsSharingSourceInfoChanged && onDocsSharingSourceInfoChanged instanceof Function) {
          _onDocsSharingSourceInfoChanged = onDocsSharingSourceInfoChanged;
          return true;
        }
        return false;
      },
      /**
       * Set callback event of docs share setting type changed.
       * @method SetOnDocsPermissionChangedCB
       * @param {Function} onDocsPermissionChanged
       * @return {Boolean}
       */
      SetOnDocsPermissionChangedCB: function (onDocsPermissionChanged) {
        if (_addon && onDocsPermissionChanged && onDocsPermissionChanged instanceof Function) {
          _onDocsPermissionChanged = onDocsPermissionChanged;
          return true;
        }
        return false;
      },
      /**
       * Determine whether the current meeting supports the docs or not.
       * @method IsSupportDocs
       * @return {Boolean} True indicates to support.
      */
      IsSupportDocs: function () {
        if (_addon) {
          return _addon.IsSupportDocs();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Determine whether the current user can start sharinge the doc or not.
       * @method CanStartShareDocs
       * @return {Boolean} True indicates you can start sharing the doc.
      */
      CanStartShareDocs: function () {
        if (_addon) {
          return _addon.CanStartShareDocs();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Determine whether the current user can set docs options or not.
       * @method CanSetDocsOption
       * @return {Boolean} True means the current user can set docs options, false means the user can' not set docs options.
      */
      CanSetDocsOption: function () {
        if (_addon) {
          return _addon.CanSetDocsOption();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Set the setting option for who can share docs.
       * @method SetDocsShareOption
       * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      SetDocsShareOption: function (opts) {
        if (_addon) {
          const clientOpts = opts || {};
          let option = clientOpts.option;
          try {
            const SetDocsShareOptionParams = new messages.SetDocsShareOptionParams();
            SetDocsShareOptionParams.setOption(option);
            const bytes = SetDocsShareOptionParams.serializeBinary();
            return _addon.SetDocsShareOption(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Get the setting option for who can share docs.
       * @method GetDocsShareOption
       * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      GetDocsShareOption: function () {
        if (_addon) {
          return _addon.GetDocsShareOption();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Set the option for who can initiate a new doc.
       * @method SetDocsCreateOption
       * @param {Number} option Setting option for who can initiate a new doc, see \link SDKDocsCreateOption \endlink enum.
       * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      SetDocsCreateOption: function (opts) {
        if (_addon) {
          const clientOpts = opts || {};
          let option = clientOpts.option;
          try {
            const SetDocsCreateOptionParams = new messages.SetDocsCreateOptionParams();
            SetDocsCreateOptionParams.setOption(option);
            const bytes = SetDocsCreateOptionParams.serializeBinary();
            return _addon.SetDocsCreateOption(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Get the option for who can initiate a new doc.
       * @method GetDocsCreateOption
       * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      GetDocsCreateOption: function () {
        if (_addon) {
          return _addon.GetDocsCreateOption();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Query if other user is sharing docs.
       * @method IsOtherSharingDocs
       * @return {Boolean} True means sharing.
      */
      IsOtherSharingDocs: function () {
        if (_addon) {
          return _addon.IsOtherSharingDocs();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Query if the current user is sharing docs successfully.
       * @method IsSharingDocsOut
       * @return {Boolean} True means sharing successfully.
      */
      IsSharingDocsOut: function () {
        if (_addon) {
          return _addon.IsSharingDocsOut();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      }, 
      /**
       * Show the docs dashboard web view window.
       * @method ShowDocsDashboard
       * @param {Number} x The original point to display docs web view window. Default is 0. only for mac
       * @param {Number} y The original point to display docs web view window. Default is 0. only for mac
       * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
       * @remark Valid only for custom user interface mode.
      */
      ShowDocsDashboard: function (opts) {
        if (_addon) {
          const clientOpts = opts || {};
          let x = clientOpts.x;
          let y = clientOpts.y;
          try {
            const ShowDocsDashboardParams = new messages.ShowDocsDashboardParams();
            ShowDocsDashboardParams.setX(x);
            ShowDocsDashboardParams.setY(y);
            const bytes = ShowDocsDashboardParams.serializeBinary();
            return _addon.ShowDocsDashboard(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Set the dashboard web view window size.
       * @method SetDocDashboardSize
    	 * @param {Number} width Specify the width of the window. 
       * @param {Number} height Specify the height of the window. 
    	 * @note If height == -1 and width == -1, the window size will be default.
    	 * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
    	 * @note Valid only for custom user interface mode.
    	 */
      SetDocDashboardSize: function (opts) {
        if (_addon) {
          const clientOpts = opts || {};
          let width = clientOpts.width;
          let height = clientOpts.height;
          try {
            const SetDocDashboardSizeParams = new messages.SetDocDashboardSizeParams();
            SetDocDashboardSizeParams.setWidth(width);
            SetDocDashboardSizeParams.setHeight(height);
            const bytes = SetDocDashboardSizeParams.serializeBinary();
            return _addon.SetDocDashboardSize(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },  
      /**
       * Set the docs web view window position.
       * @method SetDocsViewPos
       * @param {Number} x Specifies the X-axis coordinate of the top-left corner of the docs web view window in the parent window.
       * @param {Number} y Specifies the Y-axis coordinate of the top-left corner of the docs web view window in the parent window.
       * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
       * @remark Valid only for custom user interface mode.
      */
      SetDocsViewPos: function (opts) {
        if (_addon) {
          const clientOpts = opts || {};
          let x = clientOpts.x;
          let y = clientOpts.y;
          try {
            const SetDocsViewPosParams = new messages.SetDocsViewPosParams();
            SetDocsViewPosParams.setX(x);
            SetDocsViewPosParams.setY(y);
            const bytes = SetDocsViewPosParams.serializeBinary();
            return _addon.SetDocsViewPos(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Set the docs web view window size.
       * @method SetDocsViewSize
       * @param {Number} height Specify the height of the docs web view window.
       * @param {Number} width Specify the width of the docs web view window.
       * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
       * @remark Valid only for custom user interface mode.
      */
      SetDocsViewSize: function (opts) {
        if (_addon) {
          const clientOpts = opts || {};
          let width = clientOpts.width;
          let height = clientOpts.height;
          try {
            const SetDocsViewSizeParams = new messages.SetDocsViewSizeParams();
            SetDocsViewSizeParams.setWidth(width);
            SetDocsViewSizeParams.setHeight(height);
            const bytes = SetDocsViewSizeParams.serializeBinary();
            return _addon.SetDocsViewSize(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Show the docs web view window.
       * @method ShowDocsViewWindow
    	 * @param {Number} sourceID The share source ID that is sharing.
    	 * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
    	 * @note Valid only for custom user interface mode.
    	 */
      ShowDocsViewWindow: function (opts) {
        if (_addon) {
          const clientOpts = opts || {};
          let sourceID = clientOpts.sourceID;
          try {
            const ShowDocsViewWindowParams = new messages.ShowDocsViewWindowParams();
            ShowDocsViewWindowParams.setSourceid(sourceID);
            const bytes = ShowDocsViewWindowParams.serializeBinary();
            return _addon.ShowDocsViewWindow(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Close the docs web view window.
       * @method CloseDocsViewWindow
    	 * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
    	 * @note Valid only for custom user interface mode.
    	 */
      CloseDocsViewWindow: function () {
        if (_addon) {
          return _addon.CloseDocsViewWindow();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Get the docs sharing source information list from the specified sharer.
       * @method GetDocsSharingSourceInfoList
    	 * @param {Number} userID The ID of the user who is sharing docs.
    	 * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
    	 */
      GetDocsSharingSourceInfoList: function (opts) {
        if (_addon) {
          const clientOpts = opts || {};
          let userID = clientOpts.userID;
          try {
            const GetDocsSharingSourceInfoListParams = new messages.GetDocsSharingSourceInfoListParams();
            GetDocsSharingSourceInfoListParams.setUserid(userID);
            const bytes = GetDocsSharingSourceInfoListParams.serializeBinary();
            return _addon.GetDocsSharingSourceInfoList(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Get the ID of users who are sharing docs.
       * @method GetViewableDocsSharingUserList
    	 * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
    	 */
      GetViewableDocsSharingUserList: function () {
        if (_addon) {
          return _addon.GetViewableDocsSharingUserList();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Set the docs web view and dashboard web view owner window. only for windows
       * @method SetOwnerWindow
       * @param {String} windowID Specify the owner window.
       * Call this function before calling ShowDocsDashboardView. Call this function when receiving the meeting status MEETING_STATUS_INMEETING.
       * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
       * @remark Valid only for user custom interface mode.
      */
      SetOwnerWindow: function (opts) {
        if (_addon) {
          if (platform == 'darwin') {
            return ZoomSDKError.SDKERR_NO_IMPL;
          }
          const clientOpts = opts || {};
          let windowID = clientOpts.windowID;
          try {
            const SetOwnerWindowParams = new messages.SetOwnerWindowParams();
            SetOwnerWindowParams.setWindowid(windowID);
            const bytes = SetOwnerWindowParams.serializeBinary();
            return _addon.SetOwnerWindow(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
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
  ZoomMeetingDocs: ZoomMeetingDocs
};