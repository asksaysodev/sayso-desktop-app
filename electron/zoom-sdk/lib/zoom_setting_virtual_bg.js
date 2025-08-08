const { ZoomSDKError } = require('./settings.js');
const messages = require('./electron_sdk_pb.js');

var ZoomVirtualBGSetting = (function () {
  var instance;
  
  /**
   * Zoom Virtual BG Setting
   * @module zoom_setting_virtual_bg
   * @param {Function} onVBImageDidDownloaded Callback event of notification that the default virtual background images supplied by ZOOM are downloaded.
   * @param {Function} onGreenVBDidUpdateWithReplaceColor Callback event of notification that the virtual background effect is updated with the selected color.
   * @param {Function} onSelectedVBImageChanged Callback event of notification that the virtual background image is changed.
   * @param {Function} onVBVideoUploadedResult Callback event of notification that the result of adding video virtual background.
   * @return {ZoomVirtualBGSetting}
   */
  function init(opts) {
    let clientOpts = opts || {};
    let _addon = clientOpts.addon.GetSettingVirtualBGCtrl() || null;

    let _onVBImageDidDownloaded = clientOpts.onVBImageDidDownloaded || null;
    let _onGreenVBDidUpdateWithReplaceColor = clientOpts.onGreenVBDidUpdateWithReplaceColor || null;
    let _onSelectedVBImageChanged = clientOpts.onSelectedVBImageChanged || null;
    let _onVBVideoUploadedResult = clientOpts.onVBVideoUploadedResult || null;
    
    /**
     * Callback event of notification that the default virtual background images supplied by ZOOM are downloaded.
     * @event onVBImageDidDownloaded
    */
    function onVBImageDidDownloaded() {
      if (_onVBImageDidDownloaded) {
        _onVBImageDidDownloaded();
      }
    }
    
    /**
     * Callback event of notification that the virtual background effect is updated with the selected color.
     * @event onGreenVBDidUpdateWithReplaceColor
     * @param {Number} selectedColor The RGB value of the selected color, organized in the format 0xFFRRGGBB.
    */
    function onGreenVBDidUpdateWithReplaceColor(selectedColor) {
      if (_onGreenVBDidUpdateWithReplaceColor) {
        _onGreenVBDidUpdateWithReplaceColor(selectedColor);
      }
    }
    
    /**
     * Callback event of notification that the virtual background image is changed.
     * @event onSelectedVBImageChanged
    */
    function onSelectedVBImageChanged() {
      if (_onSelectedVBImageChanged) {
        _onSelectedVBImageChanged();
      }
    }
    
    /**
     * Callback event of notification that the result of adding video virtual background.
     * @event onVBVideoUploadedResult
     * @param {Boolean} success True means is successfully added, otherwise not.
     * @param {Number} error If failed adding the video virtual background, the error will be notified.
    */
    function onVBVideoUploadedResult(success, error) {
      if (_onVBVideoUploadedResult) {
        _onVBVideoUploadedResult(success, error);
      }
    }

    if (_addon) {
      _addon.SetOnVBImageDidDownloadedCB(onVBImageDidDownloaded);
      _addon.SetOnGreenVBDidUpdateWithReplaceColorCB(onGreenVBDidUpdateWithReplaceColor);
      _addon.SetOnSelectedVBImageChangedCB(onSelectedVBImageChanged);
      _addon.SetOnVBVideoUploadedResultCB(onVBVideoUploadedResult);
    }

    return {
      /**
       * Set callback event of notification that the default virtual background images supplied by ZOOM are downloaded.
       * @method SetOnVBImageDidDownloadedCB
       * @param {Function} onVBImageDidDownloaded
       * @return {Boolean}
       */
      SetOnVBImageDidDownloadedCB: function (onVBImageDidDownloaded) {
        if (_addon && onVBImageDidDownloaded && onVBImageDidDownloaded instanceof Function) {
          _onVBImageDidDownloaded = onVBImageDidDownloaded;
          return true;
        }
        return false;
      },
      /**
       * Set callback event of notification that the virtual background effect is updated with the selected color.
       * @method SetOnGreenVBDidUpdateWithReplaceColorCB
       * @param {Function} onGreenVBDidUpdateWithReplaceColor
       * @return {Boolean}
       */
      SetOnGreenVBDidUpdateWithReplaceColorCB: function (onGreenVBDidUpdateWithReplaceColor) {
        if (_addon && onGreenVBDidUpdateWithReplaceColor && onGreenVBDidUpdateWithReplaceColor instanceof Function) {
          _onGreenVBDidUpdateWithReplaceColor = onGreenVBDidUpdateWithReplaceColor;
          return true;
        }
        return false;
      },
      /**
       * Set callback event of notification that the virtual background image is changed.
       * @method SetOnSelectedVBImageChangedCB
       * @param {Function} onSelectedVBImageChanged
       * @return {Boolean}
       */
      SetOnSelectedVBImageChangedCB: function (onSelectedVBImageChanged) {
        if (_addon && onSelectedVBImageChanged && onSelectedVBImageChanged instanceof Function) {
          _onSelectedVBImageChanged = onSelectedVBImageChanged;
          return true;
        }
        return false;
      },
      /**
       * Set callback event of notification that the result of adding video virtual background.
       * @method SetOnVBVideoUploadedResultCB
       * @param {Function} onVBVideoUploadedResult
       * @return {Boolean}
       */
      SetOnVBVideoUploadedResultCB: function (onVBVideoUploadedResult) {
        if (_addon && onVBVideoUploadedResult && onVBVideoUploadedResult instanceof Function) {
          _onVBVideoUploadedResult = onVBVideoUploadedResult;
          return true;
        }
        return false;
      },
      /**
       * Determine if the virtual background feature is supported by the meeting.
       * @method IsSupportVirtualBG
       * @return {Boolean} TRUE indicates that the meeting supports the virtual background feature.
      */
      IsSupportVirtualBG: function () {
        if (_addon) {
          return _addon.IsSupportVirtualBG();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      }, 
      /**
       * Determine if the smart virtual background feature can be supported by the machine.
       * @method IsDeviceSupportSmartVirtualBG
       * @return {Boolean} TRUE indicates that the machine can supports to use smart virtual background feature.
      */
      IsDeviceSupportSmartVirtualBG: function () {
        if (_addon) {
          return _addon.IsDeviceSupportSmartVirtualBG();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Determine if the video virtual background feature is supported by the meeting.
       * @method IsSupportVirtualBackgroundVideo
       * @return {Boolean} TRUE indicates that the meeting supports the video virtual background feature.
      */
      IsSupportVirtualBackgroundVideo: function () {
        if (_addon) {
          return _addon.IsSupportVirtualBackgroundVideo();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Determine if the smart virtual background video feature can be supported by the machine.
       * @method IsDeviceSupportSmartVirtualBackgroundVideo
       * @note For more information, please visit <https://support.zoom.com/hc/en/article?id=zm_kb&sysparm_article=KB0060007>
       * @return {Boolean} TRUE indicates that the machine can supports to use smart virtual background video feature.
      */
      IsDeviceSupportSmartVirtualBackgroundVideo: function () {
        if (_addon) {
          return _addon.IsDeviceSupportSmartVirtualBackgroundVideo();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Determine if the green virtual background video feature can be supported by the machine.
       * @method IsDeviceSupportGreenVirtualBackgroundVideo
       * @note For more information, please visit <https://support.zoom.com/hc/en/article?id=zm_kb&sysparm_article=KB0060007>
       * @return {Boolean} TRUE indicates that the machine can supports to use green virtual background video feature.
      */
      IsDeviceSupportGreenVirtualBackgroundVideo: function () {
        if (_addon) {
          return _addon.IsDeviceSupportGreenVirtualBackgroundVideo();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Determine if the green screen is using for the virtual background feature in the meeting.
       * @method IsUsingGreenScreenOn
       * @return {Boolean} TRUE indicates to use the green screen for the virtual background feature.
      */
      IsUsingGreenScreenOn: function () {
        if (_addon) {
          return _addon.IsUsingGreenScreenOn();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Set to use the green screen for the virtual background feature.
       * @method SetUsingGreenScreen
       * @param {Boolean} bUse Specify to use the green screen or not.TRUE means using the green screen. FALSE means using smart virtual background feature.
       * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
       * @remark If the machine can not support smart virtual background feature, Calling of this interface with parameter 'FALSE'will return SDKERR_WRONG_USAGE.
      */
      SetUsingGreenScreen: function (opts) {
        if (_addon) {
          const clientOpts = opts || {};
          let bUse = clientOpts.bUse;
          try {
            const SetUsingGreenScreenParams = new messages.SetUsingGreenScreenParams();
            SetUsingGreenScreenParams.setBuse(bUse);
            const bytes = SetUsingGreenScreenParams.serializeBinary();
            return _addon.SetUsingGreenScreen(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Determine if the adding new virtual background item feature is supported by the meeting
       * @method IsAllowToAddNewVBItem
       * @return {Boolean} TRUE indicates that the meeting supports adding new virtual background item feature.
      */
      IsAllowToAddNewVBItem: function () {
        if (_addon) {
          return _addon.IsAllowToAddNewVBItem();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Determine if the removing virtual background item feature is supported by the meeting
       * @method IsAllowToRemoveVBItem
       * @return {Boolean} TRUE indicates that the meeting supports removing virtual background item feature.
       * @deprecated This interface is marked as deprecated.
      */
      IsAllowToRemoveVBItem: function () {
        if (_addon) {
          return _addon.IsAllowToRemoveVBItem();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Add a new image as the virtual background image and to the image list.
       * @method AddBGImage
       * @param {String} filePath Specify the file name of the image. It must be the full path with the file name.
       * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      AddBGImage: function (opts) {
        if (_addon) {
          const clientOpts = opts || {};
          let filePath = clientOpts.filePath;
          try {
            const AddBGImageParams = new messages.AddBGImageParams();
            AddBGImageParams.setFilepath(filePath);
            const bytes = AddBGImageParams.serializeBinary();
            return _addon.AddBGImage(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Remove an image from the virtual background image list.
       * @method RemoveBGImage
       * @param {Number} imageHandle Specify the image to remove.
       * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      RemoveBGImage: function (opts) {
        if (_addon) {
          const clientOpts = opts || {};
          let imageHandle = clientOpts.imageHandle;
          try {
            const RemoveBGImageParams = new messages.RemoveBGImageParams();
            RemoveBGImageParams.setImagehandle(imageHandle);
            const bytes = RemoveBGImageParams.serializeBinary();
            return _addon.RemoveBGImage(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Get the list of the virtual background images.
       * @method GetBGImageList
       * @return {Array} If there are images in the list, the return value is a list of the poiters to IVirtualBGImageInfo.
      */
      GetBGImageList: function () {
        if (_addon) {
          return _addon.GetBGImageList();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Specify an image to be the virtual background image.
       * @method UseBGImage
       * @param {Number} imageHandle Specify the image to use.
       * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      UseBGImage: function (opts) {
        if (_addon) {
          const clientOpts = opts || {};
          let imageHandle = clientOpts.imageHandle;
          try {
            const UseBGImageParams = new messages.UseBGImageParams();
            UseBGImageParams.setImagehandle(imageHandle);
            const bytes = UseBGImageParams.serializeBinary();
            return _addon.UseBGImage(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Get the selected color after called BeginSelectReplaceVBColor() and selected a color.
       * @method GetBGReplaceColor
       * @return {Number} If the function succeeds, the return value is the selected color.
      */
      GetBGReplaceColor: function () {
        if (_addon) {
          return _addon.GetBGReplaceColor();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Start to capture a color from video preview.
       * @method BeginSelectReplaceVBColor
       * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      BeginSelectReplaceVBColor: function () {
        if (_addon) {
          return _addon.BeginSelectReplaceVBColor();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Add a new video as the virtual background video and to the video list.
       * @method AddBGVideo
       * @param {String} filePath Specify the file name of the video. It must be the full path with the file name.
       * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      AddBGVideo: function (opts) {
        if (_addon) {
          const clientOpts = opts || {};
          let filePath = clientOpts.filePath;
          try {
            const AddBGVideoParams = new messages.AddBGVideoParams();
            AddBGVideoParams.setFilepath(filePath);
            const bytes = AddBGVideoParams.serializeBinary();
            return _addon.AddBGVideo(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Remove a video from the virtual background video list.
       * @method RemoveBGVideo
       * @param {Object} imageHandle Specify the video to remove.
       * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      RemoveBGVideo: function (opts) {
        if (_addon) {
          const clientOpts = opts || {};
          let imageHandle = clientOpts.imageHandle;
          try {
            const RemoveBGVideoParams = new messages.RemoveBGVideoParams();
            RemoveBGVideoParams.setImagehandle(imageHandle);
            const bytes = RemoveBGVideoParams.serializeBinary();
            return _addon.RemoveBGVideo(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Get the list of the virtual background videoes.
       * @method GetBGVideoList
       * @return {Array} If there are videoes in the list, the return value is a list of the poiters to IVirtualBGImageInfo.
      */
      GetBGVideoList: function () {
        if (_addon) {
          return _addon.GetBGVideoList();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Specify a video to be the virtual background video.
       * @method UseBGVideo
       * @param {Number} imageHandle Specify the video to use.
       * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      UseBGVideo: function (opts) {
        if (_addon) {
          const clientOpts = opts || {};
          let imageHandle = clientOpts.imageHandle;
          try {
            const UseBGVideoParams = new messages.UseBGVideoParams();
            UseBGVideoParams.setImagehandle(imageHandle);
            const bytes = UseBGVideoParams.serializeBinary();
            return _addon.UseBGVideo(bytes);
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
  ZoomVirtualBGSetting: ZoomVirtualBGSetting
};