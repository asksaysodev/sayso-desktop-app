let { ZoomSDKError } = require("./settings.js");
const messages = require("./electron_sdk_pb.js");

var ZoomMeetingReaction = (function () {
  var instance;
  /**
   * Zoom Meeting Reactions
   * @module zoom_meeting_reaction
   * @param {Function} onEmojiReactionReceived Callback event used to inform the user once an emoji is received
   * @param {Function} onEmojiReactionReceivedInWebinar Callback event used to inform the user once an emoji is received in a webinar
   * @return {ZoomMeetingReaction}
   */
  function init(opts) {
    let clientOpts = opts || {};
    // Private methods and variables
    let _addon = clientOpts.addon.GetMeetingReactionCtrl() || null;
    let _onEmojiReactionReceived = clientOpts.onEmojiReactionReceived || null;
    let _onEmojiReactionReceivedInWebinar = clientOpts.onEmojiReactionReceivedInWebinar || null;

    /**
     * Callback event used to inform the user once an emoji is received
      @event onEmojiReactionReceived
      @param {number} userid The user id of the send emoji reaction
      @param {number} type The send emoji reaction type
      @param {number} skinTone The send emoji reaction skinstone
     */
    function onEmojiReactionReceived(userid, type, skinTone) {
      if (_onEmojiReactionReceived) {
        _onEmojiReactionReceived(userid, type, skinTone);
      }
    }

    /**
     * Callback event used to inform the user once an emoji is received
      @event onEmojiReactionReceivedInWebinar
      @param {number} type The send emoji reaction type
     */
    function onEmojiReactionReceivedInWebinar(type) {
      if (_onEmojiReactionReceivedInWebinar) {
        _onEmojiReactionReceivedInWebinar(type);
      }
    }

    if (_addon) {
      _addon.SetOnEmojiReactionReceivedCB(onEmojiReactionReceived);
      _addon.SetOnEmojiReactionReceivedInWebinarCB(onEmojiReactionReceivedInWebinar);
    }

    return {
      // Public methods and variables
      /**
       * Set emoji reaction received callback
       * @method SetOnEmojiReactionReceivedCB
       * @param {Function} onEmojiReactionReceived
       * @param {Function} onEmojiReactionReceivedInWebinar
       * @return {Boolean}
       */
      SetOnEmojiReactionReceivedCB: function (onEmojiReactionReceived) {
        if (_addon && onEmojiReactionReceived && onEmojiReactionReceived instanceof Function) {
          _onEmojiReactionReceived = onEmojiReactionReceived;
          return true;
        }
        return false;
      },
      SetOnEmojiReactionReceivedInWebinarCB: function (onEmojiReactionReceivedInWebinar) {
        if (_addon && onEmojiReactionReceivedInWebinar && onEmojiReactionReceivedInWebinar instanceof Function) {
          _onEmojiReactionReceivedInWebinar = onEmojiReactionReceivedInWebinar;
          return true;
        }
        return false;
      },
    };
  }

  return {
    getInstance: function (opts) {
      if (!instance) {
        instance = init(opts);
      }
      return instance;
    },
  };
})();

module.exports = {
  ZoomMeetingReaction: ZoomMeetingReaction,
};
