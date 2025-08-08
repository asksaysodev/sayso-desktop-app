let { ZoomSDKError } = require("./settings.js");
const messages = require("./electron_sdk_pb.js");

var ZoomMeetingPolling = (function () {
  var instance;
  /**
   * Zoom Polling
   * @module zoom_meeting_polling
   * @param {Function} onPollingStatusChanged Callback event used to inform the user once the polling status has changed
   * @return {ZoomMeetingPolling}
   */
  function init(opts) {
    let clientOpts = opts || {};
    // Private methods and variables
    let _addon = clientOpts.addon.GetMeetingPollingCtrl() || null;
    let _onPollingStatusChanged = clientOpts.onPollingStatusChanged || null;
    let _onPollingResultUpdated = clientOpts.onPollingResultUpdated || null;
    let _onPollingListUpdated = clientOpts.onPollingListUpdated || null;
    let _onPollingActionResult = clientOpts.onPollingActionResult || null;

    /**
       * Callback event used to inform the user once an emoji is received
        @event onPollingStatusChanged
        @param {string} pollingID
        @param {number} status
       */
    function onPollingStatusChanged(pollingID, status) {
      if (_onPollingStatusChanged) {
        _onPollingStatusChanged(pollingID, status);
      }
    }

    /**
       * Callback event used to inform the user once an emoji is received
        @event onPollingResultUpdated
        @param {string} pollingID
       */
    function onPollingResultUpdated(pollingID) {
      if (_onPollingResultUpdated) {
        _onPollingResultUpdated(pollingID);
      }
    }

    /**
       * Callback event used to inform the user once an emoji is received
        @event onPollingListUpdated
       */
    function onPollingListUpdated() {
      if (_onPollingListUpdated) {
        _onPollingListUpdated();
      }
    }

    /**
       * Callback event used to inform the user once an emoji is received
        @event onPollingActionResult
       */
    function onPollingActionResult(actionType, pollingID, bSuccess, errorMsg) {
      if (_onPollingActionResult) {
        _onPollingActionResult(actionType, pollingID, bSuccess, errorMsg);
      }
    }

    if (_addon) {
      _addon.SetOnPollingStatusChangedCB(onPollingStatusChanged);
      _addon.SetOnPollingResultUpdatedCB(onPollingResultUpdated);
      _addon.SetOnPollingListUpdatedCB(onPollingListUpdated);
      _addon.SetOnPollingActionResultCB(onPollingActionResult);
    }

    return {
      // Public methods and variables
      SetOnPollingStatusChangedCB: function (onPollingStatusChanged) {
        if (
          _addon &&
          onPollingStatusChanged &&
          onPollingStatusChanged instanceof Function
        ) {
          _onPollingStatusChanged = onPollingStatusChanged;
          return true;
        }
        return false;
      },
      SetOnPollingResultUpdatedCB: function (onPollingResultUpdated) {
        if (
          _addon &&
          onPollingResultUpdated &&
          onPollingResultUpdated instanceof Function
        ) {
          _onPollingResultUpdated = onPollingResultUpdated;
          return true;
        }
        return false;
      },
      SetOnPollingListUpdatedCB: function (onPollingListUpdated) {
        if (
          _addon &&
          onPollingListUpdated &&
          onPollingListUpdated instanceof Function
        ) {
          _onPollingListUpdated = onPollingListUpdated;
          return true;
        }
        return false;
      },
      SetOnPollingActionResultCB: function (onPollingActionResult) {
        if (
          _addon &&
          onPollingActionResult &&
          onPollingActionResult instanceof Function
        ) {
          _onPollingActionResult = onPollingActionResult;
          return true;
        }
        return false;
      },
      CanGetRightAnswerItemList: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var pollingID = clientOpts.pollingID;
          if (!pollingID) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
          try {
            let PollingIDParams = new messages.PollingIDParams();
            PollingIDParams.setPid(pollingID);
            let bytes = PollingIDParams.serializeBinary();
            return _addon.CanGetRightAnswerItemList(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      GetPollingQuestionItemList: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var pollingID = clientOpts.pollingID;
          if (!pollingID) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
          try {
            let PollingIDParams = new messages.PollingIDParams();
            PollingIDParams.setPid(pollingID);
            let bytes = PollingIDParams.serializeBinary();
            let pollingQuestionList = _addon.GetPollingQuestionItemList(bytes),
              final_pollingQuestionList = [];
            if (pollingQuestionList) {
              for (let item of pollingQuestionList) {
                let questionObj = {};
                questionObj = item[0];
                let pollingSubQuestionItemList = item[1];
                let final_subQuestionItemList = [];

                for (let subQuestionItem of pollingSubQuestionItemList) {
                  let subQuestionObj = {};
                  subQuestionObj = subQuestionItem[0];
                  let subsSubQuestionList = subQuestionItem[1];
                  let subsAnswerList = subQuestionItem[2];
                  subQuestionObj.pollingSubQuestionItemList = subsSubQuestionList;
                  subQuestionObj.pollingAnswerItemList = subsAnswerList;
                  final_subQuestionItemList.push(subQuestionObj);
                }

                let pollingAnswerItemList = item[2];
                questionObj.pollingSubQuestionItemList =
                  final_subQuestionItemList;
                questionObj.pollingAnswerItemList = pollingAnswerItemList;
                final_pollingQuestionList.push(questionObj);
              }
              return final_pollingQuestionList;
            } else {
              return [];
            }
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
      },
      GetPollingRightAnswerItemList: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var pollingID = clientOpts.pollingID;
          if (!pollingID) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
          try {
            let PollingIDParams = new messages.PollingIDParams();
            PollingIDParams.setPid(pollingID);
            let bytes = PollingIDParams.serializeBinary();
            let pollingRightAnswerList =
              _addon.GetPollingRightAnswerItemList(bytes);
            if (pollingRightAnswerList) {
              return pollingRightAnswerList;
            } else {
              return [];
            }
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
      },
      GetPollingAnswerResultItemList: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var pollingID = clientOpts.pollingID;
          if (!pollingID) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
          try {
            let PollingIDParams = new messages.PollingIDParams();
            PollingIDParams.setPid(pollingID);
            let bytes = PollingIDParams.serializeBinary();
            let pollingResultAnswerList =
              _addon.GetPollingAnswerResultItemList(bytes);
            if (pollingResultAnswerList) {
              return pollingResultAnswerList;
            } else {
              return [];
            }
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
      },
      GetPollingItemByID: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var pollingID = clientOpts.pollingID;
          if (!pollingID) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
          try {
            let PollingIDParams = new messages.PollingIDParams();
            PollingIDParams.setPid(pollingID);
            let bytes = PollingIDParams.serializeBinary();
            let pollingList = _addon.GetPollingItemByID(bytes),
              pollingObj = {};
            if (pollingList) {
              pollingObj = pollingList[0];
            }
            return pollingObj;
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
      },
      CanDoPolling: function () {
        if (_addon) {
          return _addon.CanDoPolling();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      GetPollingItemList: function () {
        if (_addon) {
          return _addon.GetPollingItemList();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      StartPolling: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var pollingID = clientOpts.pollingID;
          if (!pollingID) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
          try {
            let PollingIDParams = new messages.PollingIDParams();
            PollingIDParams.setPid(pollingID);
            let bytes = PollingIDParams.serializeBinary();
            return _addon.StartPolling(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_INVALID_PARAMETER;
      },
      StopPolling: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var pollingID = clientOpts.pollingID;
          if (!pollingID) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
          try {
            let PollingIDParams = new messages.PollingIDParams();
            PollingIDParams.setPid(pollingID);
            let bytes = PollingIDParams.serializeBinary();
            return _addon.StopPolling(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_INVALID_PARAMETER;
      },
      RestartPolling: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var pollingID = clientOpts.pollingID;
          if (!pollingID) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
          try {
            let PollingIDParams = new messages.PollingIDParams();
            PollingIDParams.setPid(pollingID);
            let bytes = PollingIDParams.serializeBinary();
            return _addon.RestartPolling(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_INVALID_PARAMETER;
      },
      IsQuestionCaseSensitive: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var pollingID = clientOpts.pollingID;
          var questionID = clientOpts.questionID;
          if (!pollingID || !questionID) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
          try {
            let QuestionCaseSensitiveParams = new messages.QuestionCaseSensitiveParams();
            QuestionCaseSensitiveParams.setPid(pollingID);
            QuestionCaseSensitiveParams.setQid(questionID);
            let bytes = QuestionCaseSensitiveParams.serializeBinary();
            return _addon.IsQuestionCaseSensitive(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      GetPollingQuestionImagePath: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var pollingID = clientOpts.pollingID;
          var questionID = clientOpts.questionID;
          if (!pollingID || !questionID) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
          try {
            let QuestionImagePathParams = new messages.QuestionImagePathParams();
            QuestionImagePathParams.setPid(pollingID);
            QuestionImagePathParams.setQid(questionID);
            let bytes = QuestionImagePathParams.serializeBinary();
            return _addon.GetPollingQuestionImagePath(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
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
  ZoomMeetingPolling: ZoomMeetingPolling,
};
