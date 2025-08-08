let { ZoomSDKError } = require("./settings.js");
const messages = require("./electron_sdk_pb.js");

var ZoomMeetingCloseCaption = (function () {
    var instance;
  /**
   * Zoom Close Captions
   * @module zoom_meeting_close_caption
   * @param {Function} onLiveTranscriptionMsgInfoReceived Callback event used to inform the user once a live transcription message is received.
   * @param {Function} onOriginalLanguageMsgReceived Callback event used to inform the user once the original language message is received.
   * @return {ZoomMeetingCloseCaption}
   */
  function init(opts) {
    let clientOpts = opts || {};
    // Private methods and variables
    let _addon = clientOpts.addon.GetMeetingCloseCaptionCtrl() || null;

    let _onLiveTranscriptionMsgInfoReceived = clientOpts.onLiveTranscriptionMsgInfoReceived || null;
    let _onOriginalLanguageMsgReceived = clientOpts.onOriginalLanguageMsgReceived || null;

    /**
     * Callback event used to inform the user once a live transcription message is received
      @event onLiveTranscriptionMsgInfoReceived
      @param {Object} messageInfo The live transcription message
     */
    function onLiveTranscriptionMsgInfoReceived(messageInfo) {
      let meeting_close_caption = {
        messageID: messageInfo.messageID,
        speakerID: messageInfo.speakerID,
        speakerName: messageInfo.speakerName,
        messageContent: messageInfo.messageContent,
        timeStamp: messageInfo.timeStamp,
        messageType: messageInfo.messageType,
      }
      if (_onLiveTranscriptionMsgInfoReceived) {
        _onLiveTranscriptionMsgInfoReceived(meeting_close_caption);
      }
    }

    /**
     * Callback event used to inform the user once the original language message is received.
      @event onOriginalLanguageMsgReceived
      @param {Object} messageInfo The live transcription message
     */
    function onOriginalLanguageMsgReceived(messageInfo) {
        let meeting_close_caption = {
            messageID: messageInfo.messageID,
            speakerID: messageInfo.speakerID,
            speakerName: messageInfo.speakerName,
            messageContent: messageInfo.messageContent,
            timeStamp: messageInfo.timeStamp,
            messageType: messageInfo.messageType,
        }
        if (_onOriginalLanguageMsgReceived) {
            _onOriginalLanguageMsgReceived(meeting_close_caption);
        }
    }

    if (_addon) {
      _addon.SetOnLiveTranscriptionMsgInfoReceivedCB(onLiveTranscriptionMsgInfoReceived);
      _addon.SetOnOriginalLanguageMsgReceivedCB(onOriginalLanguageMsgReceived);
    }

    return {
      // Public methods and variables
      /**
       * Set live transcription message info received callback
       * @method SetOnLiveTranscriptionMsgInfoReceivedCB
       * @param {Function} onLiveTranscriptionMsgInfoReceived
       * @return {Boolean}
       */
      SetOnLiveTranscriptionMsgInfoReceivedCB: function (onLiveTranscriptionMsgInfoReceived) {
        if (_addon && onLiveTranscriptionMsgInfoReceived && onLiveTranscriptionMsgInfoReceived instanceof Function) {
            _onLiveTranscriptionMsgInfoReceived = onLiveTranscriptionMsgInfoReceived;
          return true;
        }
        return false;
      },
      /**
       * Set live transcription message info received callback
       * @method SetOnOriginalLanguageMsgReceivedCB
       * @param {Function} onOriginalLanguageMsgReceived
       * @return {Boolean}
       */
      SetOnOriginalLanguageMsgReceivedCB: function (onOriginalLanguageMsgReceived) {
        if (_addon && onOriginalLanguageMsgReceived && onOriginalLanguageMsgReceived instanceof Function) {
            _onOriginalLanguageMsgReceived = onOriginalLanguageMsgReceived;
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
    ZoomMeetingCloseCaption: ZoomMeetingCloseCaption,
}