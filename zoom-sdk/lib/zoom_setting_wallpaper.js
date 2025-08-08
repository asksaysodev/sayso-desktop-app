const { ZoomSDKError } = require('./settings.js');
const messages = require('./electron_sdk_pb.js');

var ZoomWallpaperSetting = (function () {

  var instance;
  /**
   * Zoom Wallpaper Setting
   * @module zoom_setting_wallpaper
   * @param {Function} onMeetingWallpaperChanged Callback event of notification that the meeting wall-paper item is changed.
   * @param {Function} onMeetingWallpaperImageDownloadStatus Callback event of notification that download status of the meeting wall-paper is changed.
   * @param {Function} onPersonalWallpaperChanged Callback event of notification that the personal wall-paper item is changed.
   * @param {Function} onPersonalWallpaperImageDownloadStatus Callback event of notification that download status of the personal wall-paper is changed.
   * @return {ZoomWallpaperSetting}
   */
  function init(opts) {
    let clientOpts = opts || {};
    // Private methods and variables
    let _addon = clientOpts.addon.GetSettingWallpaperCtrl() || null
    let _onMeetingWallpaperChanged = clientOpts.onMeetingWallpaperChanged || null
    let _onMeetingWallpaperImageDownloadStatus = clientOpts.onMeetingWallpaperImageDownloadStatus || null
    let _onPersonalWallpaperChanged = clientOpts.onPersonalWallpaperChanged || null
    let _onPersonalWallpaperImageDownloadStatus = clientOpts.onPersonalWallpaperImageDownloadStatus || null

    /**
      Callback event of notification that the meeting wall-paper item is changed.
      @event onMeetingWallpaperChanged
      @param {Object} item The config changed.
    */
    function onMeetingWallpaperChanged(item) {
      if (_onMeetingWallpaperChanged) {
        _onMeetingWallpaperChanged(item)
      }
    }

    /**
      Callback event of notification that download status of the meeting wall-paper is changed.
      @event onMeetingWallpaperImageDownloadStatus
      @param {Number} status The download status of the meeting wall-peper. see {@link ZoomSDKWallpaperSettingStatus}.
      @param {String} wallpaperId The download meeting wall-paper image ID.
    */
    function onMeetingWallpaperImageDownloadStatus(status, wallpaperId) {
      if (_onMeetingWallpaperImageDownloadStatus) {
        _onMeetingWallpaperImageDownloadStatus(status, wallpaperId)
      }
    }

    /**
      Callback event of notification that the personal wall-paper item is changed.
      @event onPersonalWallpaperChanged
      @param {Object} item The config changed.
    */
    function onPersonalWallpaperChanged(item) {
      if (_onPersonalWallpaperChanged) {
        _onPersonalWallpaperChanged(item)
      }
    }

    /**
      Callback event of notification that download status of the personal wall-paper is changed.
      @event onPersonalWallpaperImageDownloadStatus
      @param {Number} status The download status of the personal wall-peper. see {@link ZoomSDKWallpaperSettingStatus}.
      @param {String} wallpaperId The download personal wall-paper image ID.
    */
    function onPersonalWallpaperImageDownloadStatus(status, wallpaperId) {
      if (_onPersonalWallpaperImageDownloadStatus) {
        _onPersonalWallpaperImageDownloadStatus(status, wallpaperId)
      }
    }
    
    if (_addon) {
      _addon.SetOnMeetingWallpaperChangedCB(onMeetingWallpaperChanged)
      _addon.SetOnMeetingWallpaperImageDownloadStatusCB(onMeetingWallpaperImageDownloadStatus)
      _addon.SetOnPersonalWallpaperChangedCB(onPersonalWallpaperChanged);
      _addon.SetOnPersonalWallpaperImageDownloadStatusCB(onPersonalWallpaperImageDownloadStatus);
    }

    return {
      /**
      * Set callback event of notification that the meeting wall-paper item is changed.
      * @method SetOnMeetingWallpaperChangedCB
      * @param {Function} onMeetingWallpaperChanged
      * @return {Boolean}
      */
      SetOnMeetingWallpaperChangedCB: function (onMeetingWallpaperChanged) {
        if (_addon && onMeetingWallpaperChanged && onMeetingWallpaperChanged instanceof Function) {
          _onMeetingWallpaperChanged = onMeetingWallpaperChanged;
          return true;
        }
        return false;
      },
      /**
      * Set callback event of notification that download status of the meeting wall-paper is changed.
      * @method SetOnMeetingWallpaperImageDownloadStatusCB
      * @param {Function} onMeetingWallpaperImageDownloadStatus
      * @return {Boolean}
      */
      SetOnMeetingWallpaperImageDownloadStatusCB: function (onMeetingWallpaperImageDownloadStatus) {
        if (_addon && onMeetingWallpaperImageDownloadStatus && onMeetingWallpaperImageDownloadStatus instanceof Function) {
          _onMeetingWallpaperImageDownloadStatus = onMeetingWallpaperImageDownloadStatus;
          return true;
        }
        return false;
      },
      /**
      * Set callback event of notification that the personal wall-paper item is changed.
      * @method SetOnPersonalWallpaperChangedCB
      * @param {Function} onPersonalWallpaperChanged
      * @return {Boolean}
      */
      SetOnPersonalWallpaperChangedCB: function (onPersonalWallpaperChanged) {
        if (_addon && onPersonalWallpaperChanged && onPersonalWallpaperChanged instanceof Function) {
          _onPersonalWallpaperChanged = onPersonalWallpaperChanged;
          return true;
        }
        return false;
      },
      /**
      * Set callback event of notification that download status of the personal wall-paper is changed.
      * @method SetOnPersonalWallpaperImageDownloadStatusCB
      * @param {Function} onPersonalWallpaperImageDownloadStatus
      * @return {Boolean}
      */
      SetOnPersonalWallpaperImageDownloadStatusCB: function (onPersonalWallpaperImageDownloadStatus) {
        if (_addon && onPersonalWallpaperImageDownloadStatus && onPersonalWallpaperImageDownloadStatus instanceof Function) {
          _onPersonalWallpaperImageDownloadStatus = onPersonalWallpaperImageDownloadStatus;
          return true;
        }
        return false;
      },
      /** 
      * Determine if the meeting wall-paper feature enabled by OP.
      * @method IsMeetingWallpaperEnabled
      * @return {Boolean} True indicates the feature enabled. Otherwise false.
      */
      IsMeetingWallpaperEnabled: function () {
        if (_addon) {
          return _addon.IsMeetingWallpaperEnabled();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /** 
      * Determine if meeting wall-paper thumbnail ready.
      * @method IsMeetingWallpaperThumbsReady
      * @return {Boolean} True indicates ready. Otherwise false.
      */
      IsMeetingWallpaperThumbsReady: function () {
        if (_addon) {
          return _addon.IsMeetingWallpaperThumbsReady();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /** 
      * Get the meeting wall-paper item.
      * @method GetCurrentMeetingWallpaperItem
      * @return {Object} The current using meeting wall-paper config.
      * @remark If select None, the wall-paper ID is empty.
      */
      GetCurrentMeetingWallpaperItem: function () {
        if (_addon) {
          return _addon.GetCurrentMeetingWallpaperItem();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /** 
      * Get the meeting wall-paper list.
      * @method GetMeetingWallpaperList
      * @return {Array} The meeting wall-paper list.
      */
      GetMeetingWallpaperList: function () {
        if (_addon) {
          return _addon.GetMeetingWallpaperList();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /** 
      * Set the meeting wall-paper item.
      * @method SetMeetingWallpaper
      * @param {Number} wallpaperId
      * @param {Number} transparency transparency of the wallpaper.
      * @param {Number} mode layout mode of the wallpaper.
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError} 
      */
      SetMeetingWallpaper: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let wallpaperId = clientOpts.wallpaperId;
          let transparency = clientOpts.transparency;
          let mode = clientOpts.mode;
          try {
            let SetWallpaperParams = new messages.SetWallpaperParams();
            SetWallpaperParams.setWallpaperid(wallpaperId);
            SetWallpaperParams.setTransparency(transparency);
            SetWallpaperParams.setMode(mode);
            let bytes = SetWallpaperParams.serializeBinary();
            return _addon.SetMeetingWallpaper(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /** 
      * Get the meeting wall-paper item by wall-paper ID.
      * @method GetMeetingWallpaperItemByID
      * @param {String} wallpaperId
      * @return {Object} The meeting wall-paper with the wall-paper ID.
      */
      GetMeetingWallpaperItemByID: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let wallpaperId = clientOpts.wallpaperId;
          try {
            let GetWallpaperItemByIDParams = new messages.GetWallpaperItemByIDParams();
            GetWallpaperItemByIDParams.setWallpaperid(wallpaperId);
            let bytes = GetWallpaperItemByIDParams.serializeBinary();
            return _addon.GetMeetingWallpaperItemByID(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /** 
      * Determine if the personal wall-paper feature enabled by OP.
      * @method IsPersonalWallpaperEnabled
      * @return {Boolean} True indicates the feature enabled. Otherwise false.
      */
      IsPersonalWallpaperEnabled: function () {
        if (_addon) {
          return _addon.IsPersonalWallpaperEnabled();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /** 
      * Get the current user's persional wall-paper item.
      * @method GetCurrentPersonalWallpaperItem
      * @return {Object} The current user's personal wall-paper config.
      * @remark Only login user has this config. If select None, the wall-paper ID is empty.
      */
      GetCurrentPersonalWallpaperItem: function () {
        if (_addon) {
          return _addon.GetCurrentPersonalWallpaperItem();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /** 
      * Get the current user's personal wall-paper list.
      * @method GetPersonalWallpaperList
      * @return {Array} The current user's personal wall-paper list.
      * @remark Only login user has this config list.
      */
      GetPersonalWallpaperList: function () {
        if (_addon) {
          return _addon.GetPersonalWallpaperList();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /** 
      * Set the current user's personal wall-paper.
      * @method SetPersonalWallpaper
      * @param {Number} transparency transparency of the wallpaper.
      * @param {Number} mode layout mode of the wallpaper.
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      * @remark Only valid for login user.
      */
      SetPersonalWallpaper: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let wallpaperId = clientOpts.wallpaperId;
          let transparency = clientOpts.transparency;
          let mode = clientOpts.mode;
          try {
            let SetWallpaperParams = new messages.SetWallpaperParams();
            SetWallpaperParams.setWallpaperid(wallpaperId);
            SetWallpaperParams.setTransparency(transparency);
            SetWallpaperParams.setMode(mode);
            let bytes = SetWallpaperParams.serializeBinary();
            return _addon.SetPersonalWallpaper(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /** 
      * Get the personal wall-paper item by wall-paper ID.
      * @method GetPersonalWallpaperItemByID
      * @param {String} wallpaperId
      * @return {Object} The personal wall-paper item with the wall-paper ID.
      * @remark Only valid for login user.
      */
      GetPersonalWallpaperItemByID: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let wallpaperId = clientOpts.wallpaperId;
          try {
            let GetWallpaperItemByIDParams = new messages.GetWallpaperItemByIDParams();
            GetWallpaperItemByIDParams.setWallpaperid(wallpaperId);
            let bytes = GetWallpaperItemByIDParams.serializeBinary();
            return _addon.GetPersonalWallpaperItemByID(bytes);
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
      return instance;
    }
  };
})();

module.exports = {
  ZoomWallpaperSetting: ZoomWallpaperSetting
}