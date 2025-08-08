let { ZoomSDKError, FeatureEnableOption } = require('./settings.js');
const messages = require('./electron_sdk_pb.js');

var ZoomMeetingReminder = (function () {
  var instance;
  /**
   * Zoom Meeting Reminder
   * @module zoom_meeting_reminder
   * @param {Function} onReminderNotify Callback event of the reminder dialog show.
   * @param {Function} onEnableReminderNotify Callback event of the enable reminder dialog show.
   * @return {ZoomMeetingReminder}
   */
  function init(opts) {
    let clientOpts = opts || {};
    // Private methods and variables
    let _addon = clientOpts.addon.GetMeetingReminderCtrl() || null;
    let _onReminderNotify = clientOpts.onReminderNotify || null;
    let _onEnableReminderNotify = clientOpts.onEnableReminderNotify || null;

    /**
      Callback event of the reminder dialog show.
      @event onReminderNotify
      @param {Object} content The detail content in the reminder dialog, Defined in: {@link MeetingReminderType}
    */
    function onReminderNotify(content) {
      let meeting_reminder = {
        type: content.type,
        title: content.title,
        content: content.content,
        is_blocking: content.is_blocking
      }
      if (_onReminderNotify) {
        _onReminderNotify(meeting_reminder)
      }
    }

    /**
      Callback event of the enable reminder dialog show.
      @event onEnableReminderNotify
      @param {Object} content The detail content in the reminder dialog, Defined in: {@link MeetingReminderType}
    */
    function onEnableReminderNotify(content) {
      let meeting_reminder = {
        type: content.type,
        title: content.title,
        content: content.content,
        is_blocking: content.is_blocking
      }
      if (_onEnableReminderNotify) {
        _onEnableReminderNotify(meeting_reminder)
      }
    }

    if (_addon) {
      _addon.SetOnReminderNotifyCB(onReminderNotify);
      _addon.SetOnEnableReminderNotifyCB(onEnableReminderNotify);
    }

    return {
      // Public methods and variables
      /**
      * Set meeting reminder callback function.
      * @method SetOnReminderNotifyCB
      * @param {Function} onReminderNotify
      * @return {Boolean}
      */
      SetOnReminderNotifyCB: function (onReminderNotify) {
        if (_addon && onReminderNotify && onReminderNotify instanceof Function) {
          _onReminderNotify = onReminderNotify;
          return true;
        }
        return false;
      },
      /**
      * Ignore the reminder.
      * @method Ignore
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      Ignore: function () {
        if (_addon) {
          return _addon.Ignore();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Accept the reminder.
      * @method Accept
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      Accept: function () {
        if (_addon) {
          return _addon.Accept();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Decline the reminder.
      * @method Decline
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      Decline: function () {
        if (_addon) {
          return _addon.Decline();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Set enable reminder dialog show callback.
      * @method SetOnEnableReminderNotifyCB
      * @param {Function} onEnableReminderNotify
      * @return {Boolean}
      */
      SetOnEnableReminderNotifyCB: function (onEnableReminderNotify) {
        if (_addon && onEnableReminderNotify && onEnableReminderNotify instanceof Function) {
          _onEnableReminderNotify = onEnableReminderNotify;
          return true;
        }
        return false;
      },
      /**
      * Set the option indicating which meetings smart recording will be enabled for.
      * @method SetEnableOption
      * @param {Number} option Specify the option, Defined in: {@link FeatureEnableOption}
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise the function fails. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      SetEnableOption: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let option = clientOpts.option || FeatureEnableOption.EnableOption_Once;
          try {
            let SetEnableOptionParams = new messages.SetEnableOptionParams();
            SetEnableOptionParams.setOption(option);
            let bytes = SetEnableOptionParams.serializeBinary();
            return _addon.SetEnableOption(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Start the smart recording feature.
      * @method StartEnable
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise the function fails. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      StartEnable: function () {
        if (_addon) {
          return _addon.StartEnable();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
       * Decline the reminder.
       * @method DeclineEnable
       * @param {Boolean} bDeclineAll TRUE means decline all reminders,and participants cannot send requests again until the host changes the setting. FALSE means that the host declines only this specific request, not all requests.
       * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
        Otherwise the function fails. To get extended error information, Defined in: {@link ZoomSDKError}
       */
      DeclineEnable: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let bDeclineAll = clientOpts.bDeclineAll;
          try {
            let DeclineEnableParams = new messages.DeclineEnableParams();
            DeclineEnableParams.setBdeclineall(bDeclineAll);
            let bytes = DeclineEnableParams.serializeBinary();
            return _addon.DeclineEnable(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Ignore the reminder.
      * @method IgnoreEnable
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise the function fails. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      IgnoreEnable: function () {
        if (_addon) {
          return _addon.IgnoreEnable();
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
  ZoomMeetingReminder: ZoomMeetingReminder
}
