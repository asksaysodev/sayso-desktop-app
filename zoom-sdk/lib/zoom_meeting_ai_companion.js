let { ZoomSDKError } = require('./settings.js');
const messages = require('./electron_sdk_pb.js');

var ZoomMeetingAICompanion = (function () {
  var instance;
  /**
   * Zoom Meeting AI Companion
   * @module zoom_meeting_ai_companion
   * @param {Function} onAICompanionFeatureTurnOffByParticipant Callback when the auto start AI Companion feature is turned off by a participant before the host join. Only the host or cohost can receive this callback.
   * @param {Function} onAICompanionFeatureSwitchRequested Callback when host receives the request to turn the AI Companion features on or off.
   * @param {Function} onAICompanionFeatureSwitchRequestResponse Callback when the host handles the request to turn the AI Companion features on or off.
   * @param {Function} onAICompanionFeatureCanNotBeTurnedOff Callback when the started AI Companion feature can't be turned off.
   * @param {Function} onSmartSummaryStateNotSupported Callback notify the meting does not support smart summary.
   * @param {Function} onSmartSummaryStateSupportedButDisabled Callback notify the meeting support smart summary but smart summary feature is disabled.
   * @param {Function} onSmartSummaryStateEnabledButNotStarted Callback notify the meeting smart summary is not started.
   * @param {Function} onSmartSummaryStateStarted Callback notify the meeting smart summary is started.
   * @param {Function} onFailedToStartSmartSummary Callback notify failed to start the smart summary.
   * @param {Function} onSmartSummaryEnableRequestReceived Callback notify receive request to enable smart summary.
   * @param {Function} onSmartSummaryStartRequestReceived Callback notify receive request to start smart summary.
   * @param {Function} onSmartSummaryEnableActionCallback Callback notify receive request to start smart summary.
   * @param {Function} onQueryStateNotSupported Callback event that the meeting does not support query.
   * @param {Function} onQueryStateSupportedButDisabled Callback event that the meeting supports query but query feature is disabled.
   * @param {Function} onQueryStateEnabledButNotStarted Callback event that the query is not started.
   * @param {Function} onQueryStateStarted Callback event that the query is started.
   * @param {Function} onQuerySettingChanged Callback event that the query setting is changed.
   * @param {Function} onFailedToStartQuery Callback event that the query failed to start.
   * @param {Function} onReceiveRequestToEnableQuery Callback event that receiving request to enable query.
   * @param {Function} onReceiveRequestToStartQuery Callback event that receiving request to start query.
   * @param {Function} onReceiveQueryAnswer Callback event that receiving query answer.
   * @param {Function} onQueryEnableActionCallback Callback event that receiving query enable action callback.
   * @param {Function} onSendQueryPrivilegeChanged Callback event that getting or losing send query question privilege.
   * @param {Function} onFailedToRequestSendQuery Callback event that failed to request send query.
   * @param {Function} onReceiveRequestToSendQuery Callback event that receiving request to send query.
   * @return {ZoomMeetingAICompanion}
   */
  function init(opts) {
    let clientOpts = opts || {};
    // Private methods and variables
    let _addon = clientOpts.addon.GetMeetingAICompanionCtrl() || null;
    let _onAICompanionFeatureTurnOffByParticipant = clientOpts.onAICompanionFeatureTurnOffByParticipant || null;
    let _onAICompanionFeatureSwitchRequested = clientOpts.onAICompanionFeatureSwitchRequested || null;
    let _onAICompanionFeatureSwitchRequestResponse = clientOpts.onAICompanionFeatureSwitchRequestResponse || null;
    let _onAICompanionFeatureCanNotBeTurnedOff = clientOpts.onAICompanionFeatureCanNotBeTurnedOff || null;
    let _onSmartSummaryStateNotSupported = clientOpts.onSmartSummaryStateNotSupported || null;
    let _onSmartSummaryStateSupportedButDisabled = clientOpts.onSmartSummaryStateSupportedButDisabled || null;
    let _onSmartSummaryStateEnabledButNotStarted = clientOpts.onSmartSummaryStateEnabledButNotStarted || null;
    let _onSmartSummaryStateStarted = clientOpts.onSmartSummaryStateStarted || null;
    let _onFailedToStartSmartSummary = clientOpts.onFailedToStartSmartSummary || null;
    let _onSmartSummaryEnableRequestReceived = clientOpts.onSmartSummaryEnableRequestReceived || null;
    let _onSmartSummaryStartRequestReceived = clientOpts.onSmartSummaryStartRequestReceived || null;
    let _onSmartSummaryEnableActionCallback = clientOpts.onSmartSummaryEnableActionCallback || null;
    let _onQueryStateNotSupported = clientOpts.onQueryStateNotSupported || null
    let _onQueryStateSupportedButDisabled = clientOpts.onQueryStateSupportedButDisabled || null
    let _onQueryStateEnabledButNotStarted = clientOpts.onQueryStateEnabledButNotStarted || null;
    let _onQueryStateStarted = clientOpts.onQueryStateStarted || null;
    let _onQuerySettingChanged = clientOpts.onQuerySettingChanged || null;
    let _onFailedToStartQuery = clientOpts.onFailedToStartQuery || null;
    let _onReceiveRequestToEnableQuery = clientOpts.onReceiveRequestToEnableQuery || null;
    let _onReceiveRequestToStartQuery = clientOpts.onReceiveRequestToStartQuery || null;
    let _onReceiveQueryAnswer = clientOpts.onReceiveQueryAnswer || null;
    let _onQueryEnableActionCallback = clientOpts.onQueryEnableActionCallback || null;
    let _onSendQueryPrivilegeChanged = clientOpts.onSendQueryPrivilegeChanged || null;
    let _onFailedToRequestSendQuery = clientOpts.onFailedToRequestSendQuery || null;
    let _onReceiveRequestToSendQuery = clientOpts.onReceiveRequestToSendQuery || null;

    /**
      Callback when the auto start AI Companion feature is turned off by a participant before the host join. Only the host or cohost can receive this callback.
      @event onAICompanionFeatureTurnOffByParticipant
    */
    function onAICompanionFeatureTurnOffByParticipant() {
      if (_onAICompanionFeatureTurnOffByParticipant) {
        _onAICompanionFeatureTurnOffByParticipant()
      }
    }

    /**
      Callback when host receives the request to turn the AI Companion features on or off.
      @event onAICompanionFeatureSwitchRequested
    */
    function onAICompanionFeatureSwitchRequested() {
      if (_onAICompanionFeatureSwitchRequested) {
        _onAICompanionFeatureSwitchRequested()
      }
    }

    /**
      Callback when the host handles the request to turn the AI Companion features on or off.
      @event onAICompanionFeatureSwitchRequestResponse
      @param {Boolean} bTimeout Specify the host not handle the request until timeout.
      @param {Boolean} bAgree Specify the host agrees to the request to turn the AI companion features on or off.
      @param {Boolean} bTurnOn Specify the host respond the request of turn on or off.
    */
    function onAICompanionFeatureSwitchRequestResponse(bTimeout, bAgree, bTurnOn) {
      if (_onAICompanionFeatureSwitchRequestResponse) {
        _onAICompanionFeatureSwitchRequestResponse(bTimeout, bAgree, bTurnOn)
      }
    }
  
    /**
      Callback when the started AI Companion feature can't be turned off.
      @event onAICompanionFeatureCanNotBeTurnedOff
      @param {Array} features Specify the AI Companion features that can't be turned off.
    */
    function onAICompanionFeatureCanNotBeTurnedOff(features) {
      if (_onAICompanionFeatureCanNotBeTurnedOff) {
        _onAICompanionFeatureCanNotBeTurnedOff(features)
      }
    }

    /**
      Callback notify the meting does not support smart summary.
      @event onSmartSummaryStateNotSupported
    */
    function onSmartSummaryStateNotSupported() {
      if (_onSmartSummaryStateNotSupported) {
        _onSmartSummaryStateNotSupported()
      }
    }

    /**
      Callback notify the meeting support smart summary but smart summary feature is disabled.
      @event onSmartSummaryStateSupportedButDisabled
    */
    function onSmartSummaryStateSupportedButDisabled() {
      if (_onSmartSummaryStateSupportedButDisabled) {
        _onSmartSummaryStateSupportedButDisabled()
      }
    }

    /**
      Callback notify the meeting smart summary is not started.
      @event onSmartSummaryStateEnabledButNotStarted
    */
    function onSmartSummaryStateEnabledButNotStarted() {
      if (_onSmartSummaryStateEnabledButNotStarted) {
        _onSmartSummaryStateEnabledButNotStarted()
      }
    }
  
    /**
      Callback notify the meeting smart summary is started.
      @event onSmartSummaryStateStarted
    */
    function onSmartSummaryStateStarted() {
      if (_onSmartSummaryStateStarted) {
        _onSmartSummaryStateStarted()
      }
    }

    /**
      Callback notify failed to start the smart summary.
      @event onFailedToStartSmartSummary
      @param {Boolean} bTimeout bTimeout True means timeout. Otherwise no timeout. May be declined by host or cohost.
    */
    function onFailedToStartSmartSummary(bTimeout) {
      if (_onFailedToStartSmartSummary) {
        _onFailedToStartSmartSummary(bTimeout)
      }
    }

    /**
      Callback notify receive request to enable smart summary.
      @event onSmartSummaryEnableRequestReceived
    */
    function onSmartSummaryEnableRequestReceived() {
      if (_onSmartSummaryEnableRequestReceived) {
        _onSmartSummaryEnableRequestReceived()
      }
    }

    /**
      Callback notify receive request to start smart summary.
      @event onSmartSummaryStartRequestReceived
    */
    function onSmartSummaryStartRequestReceived() {
      if (_onSmartSummaryStartRequestReceived) {
        _onSmartSummaryStartRequestReceived()
      }
    }

    /**
      Callback notify receive request to start smart summary.
      @event onSmartSummaryEnableActionCallback
    */
    function onSmartSummaryEnableActionCallback() {
      if (_onSmartSummaryEnableActionCallback) {
        _onSmartSummaryEnableActionCallback()
      }
    }

    /**
     * Callback event that the meeting does not support query.
     * @event onQueryStateNotSupported
    */
    function onQueryStateNotSupported() {
      if (_onQueryStateNotSupported) {
        _onQueryStateNotSupported()
      }
    }

    /**
     * Callback event that the meeting supports query but query feature is disabled.
     * @event onQueryStateSupportedButDisabled
    */
    function onQueryStateSupportedButDisabled() {
      if (_onQueryStateSupportedButDisabled) {
        _onQueryStateSupportedButDisabled()
      }
    }

    /**
     * Callback event that the query is not started.
     * @event onQueryStateEnabledButNotStarted
     */
    function onQueryStateEnabledButNotStarted() {
      if (_onQueryStateEnabledButNotStarted) {
        _onQueryStateEnabledButNotStarted();
      }
    }

    /**
     * Callback event that the query is started.
     * @event onQueryStateStarted
     */
    function onQueryStateStarted() {
      if (_onQueryStateStarted) {
        _onQueryStateStarted();
      }
    }

    /**
     * Callback event that the query setting is changed.
     * @event onQuerySettingChanged
     */
    function onQuerySettingChanged(eSetting) {
      if (_onQuerySettingChanged) {
        _onQuerySettingChanged(eSetting);
      }
    }

    /**
     * Callback event that the query failed to start.
     * @event onFailedToStartQuery
     * @param {Boolean} bTimeout true means is timeout. Otherwise not.
     */
    function onFailedToStartQuery(bTimeout) {
      if (_onFailedToStartQuery) {
        _onFailedToStartQuery(bTimeout);
      }
    }

    /**
     * Callback event that receiving request to enable query.
     * @event onReceiveRequestToEnableQuery
     */
    function onReceiveRequestToEnableQuery() {
      if (_onReceiveRequestToEnableQuery) {
        _onReceiveRequestToEnableQuery();
      }
    }

    /**
     * Callback event that receiving request to start query.
     * @event onReceiveRequestToStartQuery
     */
    function onReceiveRequestToStartQuery() {
      if (_onReceiveRequestToStartQuery) {
        _onReceiveRequestToStartQuery();
      }
    }

    /**
     * Callback event that receiving query answer.
     * @event onReceiveQueryAnswer
     * @param {Object} queryItem
     */
    function onReceiveQueryAnswer(queryItem) {
      if (_onReceiveQueryAnswer) {
        _onReceiveQueryAnswer(queryItem);
      }
    }

    /**
     * Callback event that receiving query enable action callback.
     * @event onQueryEnableActionCallback
     */
    function onQueryEnableActionCallback() {
      if (_onQueryEnableActionCallback) {
        _onQueryEnableActionCallback();
      }
    }

    /**
     * Callback event that getting or losing send query question privilege.
     * @event onSendQueryPrivilegeChanged
     * @param {Boolean} canSendQuery true means can send. Otherwise not.
     */
    function onSendQueryPrivilegeChanged(canSendQuery) {
      if (_onSendQueryPrivilegeChanged) {
        _onSendQueryPrivilegeChanged(canSendQuery);
      }
    }

    /**
     * Callback event that failed to request send query.
     * @event onFailedToRequestSendQuery
     * @param {Boolean} bTimeout true means is timeout. Otherwise means the user declines the request.
     */
    function onFailedToRequestSendQuery(bTimeout) {
      if (_onFailedToRequestSendQuery) {
        _onFailedToRequestSendQuery(bTimeout);
      }
    }

    /**
     * Callback event that receiving request to send query.
     * @event onReceiveRequestToSendQuery
     */
    function onReceiveRequestToSendQuery() {
      if (_onReceiveRequestToSendQuery) {
        _onReceiveRequestToSendQuery();
      }
    }

    if (_addon) {
      _addon.SetOnAICompanionFeatureTurnOffByParticipantCB(onAICompanionFeatureTurnOffByParticipant);
      _addon.SetOnAICompanionFeatureSwitchRequestedCB(onAICompanionFeatureSwitchRequested);
      _addon.SetOnAICompanionFeatureSwitchRequestResponseCB(onAICompanionFeatureSwitchRequestResponse);
      _addon.SetOnAICompanionFeatureCanNotBeTurnedOffCB(onAICompanionFeatureCanNotBeTurnedOff);
      _addon.SetOnSmartSummaryStateNotSupportedCB(onSmartSummaryStateNotSupported);
      _addon.SetOnSmartSummaryStateSupportedButDisabledCB(onSmartSummaryStateSupportedButDisabled);
      _addon.SetOnSmartSummaryStateEnabledButNotStartedCB(onSmartSummaryStateEnabledButNotStarted);
      _addon.SetOnSmartSummaryStateStartedCB(onSmartSummaryStateStarted);
      _addon.SetOnFailedToStartSmartSummaryCB(onFailedToStartSmartSummary);
      _addon.SetOnSmartSummaryEnableRequestReceivedCB(onSmartSummaryEnableRequestReceived);
      _addon.SetOnSmartSummaryStartRequestReceivedCB(onSmartSummaryStartRequestReceived);
      _addon.SetOnSmartSummaryEnableActionCallbackCB(onSmartSummaryEnableActionCallback);
      _addon.SetOnQueryStateNotSupportedCB(onQueryStateNotSupported);
      _addon.SetOnQueryStateSupportedButDisabledCB(onQueryStateSupportedButDisabled)
      _addon.SetOnQueryStateEnabledButNotStartedCB(onQueryStateEnabledButNotStarted);
      _addon.SetOnQueryStateStartedCB(onQueryStateStarted);
      _addon.SetOnQuerySettingChangedCB(onQuerySettingChanged);
      _addon.SetOnFailedToStartQueryCB(onFailedToStartQuery);
      _addon.SetOnReceiveRequestToEnableQueryCB(onReceiveRequestToEnableQuery);
      _addon.SetOnReceiveRequestToStartQueryCB(onReceiveRequestToStartQuery);
      _addon.SetOnReceiveQueryAnswerCB(onReceiveQueryAnswer);
      _addon.SetOnQueryEnableActionCallback(onQueryEnableActionCallback);
      _addon.SetOnSendQueryPrivilegeChangedCB(onSendQueryPrivilegeChanged);
      _addon.SetOnFailedToRequestSendQueryCB(onFailedToRequestSendQuery);
      _addon.SetOnReceiveRequestToSendQueryCB(onReceiveRequestToSendQuery);
    }

    return {
      // Public methods and variables
      /**
      * Set callback when the auto start AI Companion feature is turned off by a participant before the host join.
      * @method SetOnAICompanionFeatureTurnOffByParticipantCB
      * @param {Function} onAICompanionFeatureTurnOffByParticipant
      * @return {Boolean}
      */
      SetOnAICompanionFeatureTurnOffByParticipantCB: function (onAICompanionFeatureTurnOffByParticipant) {
        if (_addon && onAICompanionFeatureTurnOffByParticipant && onAICompanionFeatureTurnOffByParticipant instanceof Function) {
          _onAICompanionFeatureTurnOffByParticipant = onAICompanionFeatureTurnOffByParticipant;
          return true;
        }
        return false;
      },
      /**
      * Set notification user requests host to start smart summary callback.
      * @method SetOnAICompanionFeatureSwitchRequestedCB
      * @param {Function} onAICompanionFeatureSwitchRequested
      * @return {Boolean}
      */
      SetOnAICompanionFeatureSwitchRequestedCB: function (onAICompanionFeatureSwitchRequested) {
        if (_addon && onAICompanionFeatureSwitchRequested && onAICompanionFeatureSwitchRequested instanceof Function) {
          _onAICompanionFeatureSwitchRequested = onAICompanionFeatureSwitchRequested;
          return true;
        }
        return false;
      },
      /**
      * Set host handle the smart summary request callback.
      * @method SetOnAICompanionFeatureSwitchRequestResponseCB
      * @param {Function} onAICompanionFeatureSwitchRequestResponse
      * @return {Boolean}
      */
      SetOnAICompanionFeatureSwitchRequestResponseCB: function (onAICompanionFeatureSwitchRequestResponse) {
        if (_addon && onAICompanionFeatureSwitchRequestResponse && onAICompanionFeatureSwitchRequestResponse instanceof Function) {
          _onAICompanionFeatureSwitchRequestResponse = onAICompanionFeatureSwitchRequestResponse;
          return true;
        }
        return false;
      },
      /**
      * Set notification user need to go to web to enable the smart summary feature callback.
      * @method SetOnAICompanionFeatureCanNotBeTurnedOffCB
      * @param {Function} onAICompanionFeatureCanNotBeTurnedOff
      * @return {Boolean}
      */
      SetOnAICompanionFeatureCanNotBeTurnedOffCB: function (onAICompanionFeatureCanNotBeTurnedOff) {
        if (_addon && onAICompanionFeatureCanNotBeTurnedOff && onAICompanionFeatureCanNotBeTurnedOff instanceof Function) {
          _onAICompanionFeatureCanNotBeTurnedOff = onAICompanionFeatureCanNotBeTurnedOff;
          return true;
        }
        return false;
      },
      /**
      * Set notify the meting does not support smart summary callback.
      * @method SetOnSmartSummaryStateNotSupportedCB
      * @param {Function} onSmartSummaryStateNotSupported
      * @return {Boolean}
      */
      SetOnSmartSummaryStateNotSupportedCB: function (onSmartSummaryStateNotSupported) {
        if (_addon && onSmartSummaryStateNotSupported && onSmartSummaryStateNotSupported instanceof Function) {
          _onSmartSummaryStateNotSupported = onSmartSummaryStateNotSupported;
          return true;
        }
        return false;
      },
      /**
      * Set notify the meeting support smart summary but smart summary feature is disabled.
      * @method SetOnSmartSummaryStateSupportedButDisabledCB
      * @param {Function} onSmartSummaryStateSupportedButDisabled
      * @return {Boolean}
      */
      SetOnSmartSummaryStateSupportedButDisabledCB: function (onSmartSummaryStateSupportedButDisabled) {
        if (_addon && onSmartSummaryStateSupportedButDisabled && onSmartSummaryStateSupportedButDisabled instanceof Function) {
          _onSmartSummaryStateSupportedButDisabled = onSmartSummaryStateSupportedButDisabled;
          return true;
        }
        return false;
      },
      /**
      * Set notify the meeting smart summary is not started callback.
      * @method SetOnSmartSummaryStateEnabledButNotStartedCB
      * @param {Function} onSmartSummaryStateEnabledButNotStarted
      * @return {Boolean}
      */
      SetOnSmartSummaryStateEnabledButNotStartedCB: function (onSmartSummaryStateEnabledButNotStarted) {
        if (_addon && onSmartSummaryStateEnabledButNotStarted && onSmartSummaryStateEnabledButNotStarted instanceof Function) {
          _onSmartSummaryStateEnabledButNotStarted = onSmartSummaryStateEnabledButNotStarted;
          return true;
        }
        return false;
      },
      /**
      * Set notify the meeting smart summary is started callback.
      * @method SetOnSmartSummaryStateStartedCB
      * @param {Function} onSmartSummaryStateStarted
      * @return {Boolean}
      */
      SetOnSmartSummaryStateStartedCB: function (onSmartSummaryStateStarted) {
        if (_addon && onSmartSummaryStateStarted && onSmartSummaryStateStarted instanceof Function) {
          _onSmartSummaryStateStarted = onSmartSummaryStateStarted;
          return true;
        }
        return false;
      },
      /**
      * Set notify the meeting smart summary is started callback.
      * @method SetOnFailedToStartSmartSummaryCB
      * @param {Function} onFailedToStartSmartSummary
      * @return {Boolean}
      */
      SetOnFailedToStartSmartSummaryCB: function (onFailedToStartSmartSummary) {
        if (_addon && onFailedToStartSmartSummary && onFailedToStartSmartSummary instanceof Function) {
          _onFailedToStartSmartSummary = onFailedToStartSmartSummary;
          return true;
        }
        return false;
      },
      /**
      * Set notify receive request to enable smart summary callback.
      * @method SetOnSmartSummaryEnableRequestReceivedCB
      * @param {Function} onSmartSummaryEnableRequestReceived
      * @return {Boolean}
      */
      SetOnSmartSummaryEnableRequestReceivedCB: function (onSmartSummaryEnableRequestReceived) {
        if (_addon && onSmartSummaryEnableRequestReceived && onSmartSummaryEnableRequestReceived instanceof Function) {
          _onSmartSummaryEnableRequestReceived= onSmartSummaryEnableRequestReceived;
          return true;
        }
        return false;
      },
      /**
      * Set notify receive request to start smart summary.
      * @method SetOnSmartSummaryStartRequestReceivedCB
      * @param {Function} onSmartSummaryStartRequestReceived
      * @return {Boolean}
      */
      SetOnSmartSummaryStartRequestReceivedCB: function (onSmartSummaryStartRequestReceived) {
        if (_addon && onSmartSummaryStartRequestReceived && onSmartSummaryStartRequestReceived instanceof Function) {
          _onSmartSummaryStartRequestReceived= onSmartSummaryStartRequestReceived;
          return true;
        }
        return false;
      },
      /**
      * Set notify receive request to start smart summary.
      * @method SetOnSmartSummaryEnableActionCallbackCB
      * @param {Function} onSmartSummaryEnableActionCallback
      * @return {Boolean}
      */
      SetOnSmartSummaryEnableActionCallbackCB: function (onSmartSummaryEnableActionCallback) {
        if (_addon && onSmartSummaryEnableActionCallback && onSmartSummaryEnableActionCallback instanceof Function) {
          _onSmartSummaryEnableActionCallback = onSmartSummaryEnableActionCallback;
          return true;
        }
        return false;
      },
      /**
      * Set callback event that the meeting does not support query.
      * @method SetOnQueryStateNotSupportedCB
      * @param {Function} onQueryStateNotSupported
      * @return {Boolean}
      */
      SetOnQueryStateNotSupportedCB: function (onQueryStateNotSupported) {
        if (_addon && onQueryStateNotSupported && onQueryStateNotSupported instanceof Function) {
          _onQueryStateNotSupported = onQueryStateNotSupported;
          return true;
        }
        return false;
      },
      /**
       * Set callback event that the meeting supports query but query feature is disabled.
       * @method SetOnQueryStateSupportedButDisabledCB
       * @param {Function} onQueryStateSupportedButDisabled
       * @return {Boolean}
       */
      SetOnQueryStateSupportedButDisabledCB: function (onQueryStateSupportedButDisabled) {
        if (_addon && onQueryStateSupportedButDisabled && onQueryStateSupportedButDisabled instanceof Function) {
          _onQueryStateSupportedButDisabled = onQueryStateSupportedButDisabled;
          return true;
        }
        return false;
      }, 
      /**
       * Set callback event that the query is not started.
       * @method SetOnQueryStateEnabledButNotStartedCB
       * @param {Function} onQueryStateEnabledButNotStarted
       * @return {Boolean}
       */
      SetOnQueryStateEnabledButNotStartedCB: function (onQueryStateEnabledButNotStarted) {
        if (_addon && onQueryStateEnabledButNotStarted && onQueryStateEnabledButNotStarted instanceof Function) {
          _onQueryStateEnabledButNotStarted = onQueryStateEnabledButNotStarted;
          return true;
        }
        return false;
      }, 
      /**
       * Set callback event that the query is started.
       * @method SetOnQueryStateStartedCB
       * @param {Function} onQueryStateStarted
       * @return {Boolean}
       */
      SetOnQueryStateStartedCB: function (onQueryStateStarted) {
        if (_addon && onQueryStateStarted && onQueryStateStarted instanceof Function) {
          _onQueryStateStarted = onQueryStateStarted;
          return true;
        }
        return false;
      }, 
      /**
       * Set callback event that the query setting is changed.
       * @method SetOnQuerySettingChangedCB
       * @param {Function} onQuerySettingChanged
       * @return {Boolean}
       */
      SetOnQuerySettingChangedCB: function (onQuerySettingChanged) {
        if (_addon && onQuerySettingChanged && onQuerySettingChanged instanceof Function) {
          _onQuerySettingChanged = onQuerySettingChanged;
          return true;
        }
        return false;
      }, 
      /**
       * Set callback event that the query failed to start.
       * @method SetOnFailedToStartQueryCB
       * @param {Function} onFailedToStartQuery
       * @return {Boolean}
       */
      SetOnFailedToStartQueryCB: function (onFailedToStartQuery) {
        if (_addon && onFailedToStartQuery && onFailedToStartQuery instanceof Function) {
          _onFailedToStartQuery = onFailedToStartQuery;
          return true;
        }
        return false;
      }, 
      /**
       * Set callback event that receiving request to enable query.
       * @method SetOnReceiveRequestToEnableQueryCB
       * @param {Function} onReceiveRequestToEnableQuery
       * @return {Boolean}
       */
      SetOnReceiveRequestToEnableQueryCB: function (onReceiveRequestToEnableQuery) {
        if (_addon && onReceiveRequestToEnableQuery && onReceiveRequestToEnableQuery instanceof Function) {
          _onReceiveRequestToEnableQuery = onReceiveRequestToEnableQuery;
          return true;
        }
        return false;
      }, 
      /**
       * Set callback event that receiving request to start query.
       * @method SetOnReceiveRequestToStartQueryCB
       * @param {Function} onReceiveRequestToStartQuery
       * @return {Boolean}
       */
      SetOnReceiveRequestToStartQueryCB: function (onReceiveRequestToStartQuery) {
        if (_addon && onReceiveRequestToStartQuery && onReceiveRequestToStartQuery instanceof Function) {
          _onReceiveRequestToStartQuery = onReceiveRequestToStartQuery;
          return true;
        }
        return false;
      }, 
      /**
       * Set callback event that receiving query answer.
       * @method SetOnReceiveQueryAnswerCB
       * @param {Function} onReceiveQueryAnswer
       * @return {Boolean}
       */
      SetOnReceiveQueryAnswerCB: function (onReceiveQueryAnswer) {
        if (_addon && onReceiveQueryAnswer && onReceiveQueryAnswer instanceof Function) {
          _onReceiveQueryAnswer = onReceiveQueryAnswer;
          return true;
        }
        return false;
      }, 
      /**
       * Set callback event that receiving query enable action callback.
       * @method SetOnQueryEnableActionCallback
       * @param {Function} onQueryEnableActionCallback
       * @return {Boolean}
       */
      SetOnQueryEnableActionCallback: function (onQueryEnableActionCallback) {
        if (_addon && onQueryEnableActionCallback && onQueryEnableActionCallback instanceof Function) {
          _onQueryEnableActionCallback = onQueryEnableActionCallback;
          return true;
        }
        return false;
      },
      /**
       * Set callback event that getting or losing send query question privilege.
       * @method SetOnSendQueryPrivilegeChangedCB
       * @param {Function} onSendQueryPrivilegeChanged
       * @return {Boolean}
       */
      SetOnSendQueryPrivilegeChangedCB: function (onSendQueryPrivilegeChanged) {
        if (_addon && onSendQueryPrivilegeChanged && onSendQueryPrivilegeChanged instanceof Function) {
          _onSendQueryPrivilegeChanged = onSendQueryPrivilegeChanged;
          return true;
        }
        return false;
      }, 
      /**
       * Set callback event event that failed to request send query.
       * @method SetOnFailedToRequestSendQueryCB
       * @param {Function} onFailedToRequestSendQuery
       * @return {Boolean}
       */
      SetOnFailedToRequestSendQueryCB: function (onFailedToRequestSendQuery) {
        if (_addon && onFailedToRequestSendQuery && onFailedToRequestSendQuery instanceof Function) {
          _onFailedToRequestSendQuery = onFailedToRequestSendQuery;
          return true;
        }
        return false;
      },
      /**
       * Set callback event that receiving request to send query.
       * @method SetOnReceiveRequestToSendQueryCB
       * @param {Function} onReceiveRequestToSendQuery
       * @return {Boolean}
       */
      SetOnReceiveRequestToSendQueryCB: function (onReceiveRequestToSendQuery) {
        if (_addon && onReceiveRequestToSendQuery && onReceiveRequestToSendQuery instanceof Function) {
          _onReceiveRequestToSendQuery = onReceiveRequestToSendQuery;
          return true;
        }
        return false;
      }, 
      /**
      * Determine whether the current meeting supports turning off the AI Companion features.
      * @method IsTurnoffAllAICompanionsSupported
      * @return {Boolean} True indicates to support.
      */
      IsTurnoffAllAICompanionsSupported: function () {
        if (_addon) {
          return _addon.IsTurnoffAllAICompanionsSupported();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Determine whether the current meeting supports turning on the AI Companion features.
      * @method IsTurnOnAllAICompanionsSupported
      * @return {Boolean} True indicates to support.
      */
      IsTurnOnAllAICompanionsSupported: function () {
        if (_addon) {
          return _addon.IsTurnOnAllAICompanionsSupported();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Determine whether the current user can turn off the AI Companion features. 
      * @method CanTurnOffAllAICompanions
      * @return {Boolean} True indicates the user can turn off the AI Companion features.
      */
      CanTurnOffAllAICompanions: function () {
        if (_addon) {
          return _addon.CanTurnOffAllAICompanions();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Determine whether the current user can turn on the AI Companion features. 
      * @method CanTurnOnAllAICompanions
      * @return {Boolean} True indicates the user can turn on the AI Companion features.
      */
      CanTurnOnAllAICompanions: function () {
        if (_addon) {
          return _addon.CanTurnOnAllAICompanions();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Turn off the AI Companion features.
      * @method TurnOffAllAICompanions
      * @remark All AI features including smart summary, smart recording and query can be turned off at once.
      * @param {Boolean} deleteAssets Specify whether delete the meeting assets when turn off the AI Companion features.
      * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
        Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      TurnOffAllAICompanions: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let deleteAssets = clientOpts.deleteAssets;
          try {
            let TurnOffAllAICompanionsParams = new messages.TurnOffAllAICompanionsParams();
            TurnOffAllAICompanionsParams.setDeleteassets(deleteAssets);
            let bytes = TurnOffAllAICompanionsParams.serializeBinary();
            return _addon.TurnOffAllAICompanions(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Turn on the AI Companion features.
      * @method TurnOnAllAICompanions
      * @remark Only smart summary and query can be turned on at once.
      * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      TurnOnAllAICompanions: function () {
        if (_addon) {
          return _addon.TurnOnAllAICompanions();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Determine whether the current user can request host to turn off all started AI Companion features.
      * @method CanRequestTurnoffAllAICompanions
      * @remark All AI features include smart summary, smart recording and query can be requested to turn off at once.
      * @notice if the current user join the meeting before the host, they can check CanTurnOffAllAICompanions to turn off the AI Companion features by himself/herself.
      * @return {Boolean} True indicates the user can request host to turn off all started AI Companion features.
      */
      CanRequestTurnoffAllAICompanions: function () {
        if (_addon) {
          return _addon.CanRequestTurnoffAllAICompanions();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Determine whether the current user can request host to turn on all AI Companion features if they are enabled for the current meeting.
      * @method CanRequestTurnOnAllAICompanions
      * @remark Only smart summary and query can be requested to turn on at once.
      * @return {Boolean} True indicates the user can request host to turn on the AI Companion features.
      */
      CanRequestTurnOnAllAICompanions: function () {
        if (_addon) {
          return _addon.CanRequestTurnOnAllAICompanions();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * request host to turn off all started AI Companion features.
      * @method RequestTurnoffAllAICompanions
      * @remark All AI features include smart summary, smart recording and query can be requested to turn off at once.
      * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      RequestTurnoffAllAICompanions: function () {
        if (_addon) {
          return _addon.RequestTurnoffAllAICompanions();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * request host to turn on all AI Companion features if they are enabled for the current meeting.
      * @method RequestTurnOnAllAICompanions
      * @remark Only smart summary and query can be requested to turn on at once.
      * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      RequestTurnOnAllAICompanions: function () {
        if (_addon) {
          return _addon.RequestTurnOnAllAICompanions();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Get the list of features that the participant turns off.
      * @method GetFeatureList
      * @return {Array}
      */
      GetFeatureList: function () {
        if (_addon) {
          return _addon.GetFeatureList();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Get the list of features that the assets are deleted when the feature is turned off by participant.
      * @method GetAssetsDeletedFeatureList
      * @return {Array}
      */
      GetAssetsDeletedFeatureList: function () {
        if (_addon) {
          return _addon.GetAssetsDeletedFeatureList();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Turn on the auto AI Companion feature which was stopped by the participant before the host or cohost joined meeting.
      * @method TurnOnAgain
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      TurnOnAgain: function () {
        if (_addon) {
          return _addon.TurnOnAgain();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Agree the auto AI Companion feature turn off status. Keep the AI Companion feature off.
      * @method AgreeTurnOff
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      AgreeTurnOff: function () {
        if (_addon) {
          return _addon.AgreeTurnOff();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Get the user ID who requests host to turn the AI Companion features on or off.
      * @method GetRequestUserID
      * @return {Number} the request user ID.
      */
      GetRequestUserID: function () {
        if (_addon) {
          return _addon.GetRequestUserID();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Turn the AI Companion features on or off.
      * @method IsTurnOn
      * @return {Boolean} true means turn on the AI Companion features, false means turn off the AI Companion features.
      */
      IsTurnOn: function () {
        if (_addon) {
          return _addon.IsTurnOn();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Agree the request to turn the AI companion features on or off.
      * @method Agree
      * @param {Boolean} deleteAssets Specify whether delete the meeting assets when turning off the AI Companion features.
      * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
        Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      Agree: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let deleteAssets = clientOpts.deleteAssets;
          try {
            let AgreeAICompanionFeatureSwitchParams = new messages.AgreeAICompanionFeatureSwitchParams();
            AgreeAICompanionFeatureSwitchParams.setDeleteassets(deleteAssets);
            let bytes = AgreeAICompanionFeatureSwitchParams.serializeBinary();
            return _addon.Agree(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Decline the request to turn the AI companion features on or off.
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
      * Enable smart summary.
      * @method EnableSmartSummary
      * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      EnableSmartSummary: function () {
        if (_addon) {
          return _addon.EnableSmartSummary();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Determine if this handler is requesting enable smart summary.
      * @method IsForRequestEnableSmartSummary
      * @return {Boolean} True means this handler is for requesting enable smart summary. Otherwise not.
      */
      IsForRequestEnableSmartSummary: function () {
        if (_addon) {
          return _addon.IsForRequestEnableSmartSummary();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Start smart summary.
      * @method StartSmartSummary
      * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      StartSmartSummary: function () {
        if (_addon) {
          return _addon.StartSmartSummary();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Determine if this handler is for requesting to start the smart summary.
      * @method IsForRequestStartSmartSummary
      * @return {Boolean} True means this handler is requesting to start the smart summary. Otherwise not.
      */
      IsForRequestStartSmartSummary: function () {
        if (_addon) {
          return _addon.IsForRequestStartSmartSummary();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Stop smart summary.
      * @method StopSmartSummary
      * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      StopSmartSummary: function () {
        if (_addon) {
          return _addon.StopSmartSummary();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Get the user ID of the requester.
      * @method GetSenderEnableSmartSummaryUserID
      * @return {Number} The user ID of the requester.
      */
      GetSenderEnableSmartSummaryUserID: function () {
        if (_addon) {
          return _addon.GetSenderEnableSmartSummaryUserID();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Continue approve action.
      * @method ContinueApproveEnableSmartSummary
      * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      ContinueApproveEnableSmartSummary: function () {
        if (_addon) {
          return _addon.ContinueApproveEnableSmartSummary();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Get the user ID of the requester.
      * @method GetSenderStartSmartSummaryUserID
      * @return {Number} The user ID of the requester.
      */
      GetSenderStartSmartSummaryUserID: function () {
        if (_addon) {
          return _addon.GetSenderStartSmartSummaryUserID();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Approve request.
      * @method ApproveStartSmartSummary
      * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      ApproveStartSmartSummary: function () {
        if (_addon) {
          return _addon.ApproveStartSmartSummary();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * ConfirmEnableSmartSummaryAction request.
      * @method DeclineStartSmartSummary
      * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      DeclineStartSmartSummary: function () {
        if (_addon) {
          return _addon.DeclineStartSmartSummary();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Get the title of tip.
      * @method GetEnableSmartSummaryActionTipTitle
      * @return {String} The title of tip.
      */
      GetEnableSmartSummaryActionTipTitle: function () {
        if (_addon) {
          return _addon.GetEnableSmartSummaryActionTipTitle();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Get the string of tip.
      * @method GetEnableSmartSummaryActionTipString
      * @return {String} The string of tip.
      */
      GetEnableSmartSummaryActionTipString: function () {
        if (_addon) {
          return _addon.GetEnableSmartSummaryActionTipString();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Confirm enabling smart summary.
      * @method ConfirmEnableSmartSummaryAction
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
        Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      ConfirmEnableSmartSummaryAction: function () {
        if (_addon) {
          return _addon.ConfirmEnableSmartSummaryAction();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Cancel enabling smart summary.
      * @method CancelEnableSmartSummaryAction
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      CancelEnableSmartSummaryAction: function () {
        if (_addon) {
          return _addon.CancelEnableSmartSummaryAction();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Change query setting.
      * @return {Object} bCan True means that it can change the setting. Otherwise it can't change the setting.
      */
      CanChangeQuerySetting: function () {
        if (_addon) {
          return _addon.CanChangeQuerySetting();
        }
        return {};
      },
      /**
      * Change query setting.
      * @method ChangeQuerySettings
      * @param {Number} setting The query setting, For more details, see {@link MeetingAICompanionQuerySettingOptions}
      * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
        Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      ChangeQuerySettings: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let setting = clientOpts.setting;
          try {
            let ChangeQuerySettingsParams = new messages.ChangeQuerySettingsParams();
            ChangeQuerySettingsParams.setSetting(setting);
            let bytes = ChangeQuerySettingsParams.serializeBinary();
            return _addon.ChangeQuerySettings(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Get the selected query setting.
      * @method GetSelectedQuerySetting
      * @return {Number} If the function succeeds, it will return the selected query setting. Otherwise not.
      */
      GetSelectedQuerySetting: function () {
        if (_addon) {
          return _addon.GetSelectedQuerySetting();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Determine whether the legal notice for the AI Companion query is available.
      * @return {Object} bAvailable True indicates the legal notice for the AI Companion query is available. Otherwise the legal notice is not available.
      */
      IsAICompanionQueryLegalNoticeAvailable: function () {
        if (_addon) {
          return _addon.IsAICompanionQueryLegalNoticeAvailable();
        }
        return {};
      },
      /**
      * Get the AI Companion query legal notices prompt.
      * @return {String} If the function succeeds, it will return the AI Companion query legal notices prompt. Otherwise the function fails.
      */
      GetAICompanionQueryLegalNoticesPrompt: function () {
        if (_addon) {
          return _addon.GetAICompanionQueryLegalNoticesPrompt();
        }
        return "";
      },
      /**
      * Get the AI Companion query legal notices explained.
      * @return {String} If the function succeeds, it will return the AI Companion query legal notices explained. Otherwise the function fails.
      */
      GetAICompanionQueryLegalNoticesExplained: function () {
        if (_addon) {
          return _addon.GetAICompanionQueryLegalNoticesExplained();
        }
        return "";
      },
      /**
      * Determine whether the legal notice for the AI Companion query privacy is available.
      * @return {Object} bAvailable True indicates the legal notice for the AI Companion query privacy is available. Otherwise the legal notice is not available.
      */
      IsAICompanionQueryPrivacyLegalNoticeAvailable: function () {
        if (_addon) {
          return _addon.IsAICompanionQueryPrivacyLegalNoticeAvailable();
        }
        return {};
      },
      /**
      * Get the AI Companion query privacy legal notices prompt.
      * @return {String} If the function succeeds, it will return the AI Companion query privacy legal notices prompt. Otherwise the function fails.
      */
      GetAICompanionQueryPrivacyLegalNoticesPrompt: function () {
        if (_addon) {
          return _addon.GetAICompanionQueryPrivacyLegalNoticesPrompt();
        }
        return "";
      },
      /**
      * Enable meeting query.
      * @method EnableQuery
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      EnableQuery: function () {
        if (_addon) {
          return _addon.EnableQuery();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Determine if this handler for request enable query.
      * @method IsForRequestEnableQuery
      * @return {Boolean} True means this handler is for request enable query. Otherwise it returns false means this handler is for enable query directly.
      */
      IsForRequestEnableQuery: function () {
        if (_addon) {
          return _addon.IsForRequestEnableQuery();
        }
        return false;
      },
      /**
      * Start meeting query.
      * @method StartMeetingQuery
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      StartMeetingQuery: function () {
        if (_addon) {
          return _addon.StartMeetingQuery();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Determine if this handler for request start query.
      * @method IsForRequestStartMeetingQuery
      * @return {Boolean} True means this handler is for request start query. Otherwise it returns false means this handler is for start query directly.
      */
      IsForRequestStartMeetingQuery: function () {
        if (_addon) {
          return _addon.IsForRequestStartMeetingQuery();
        }
        return false;
      },
      /**
      * Get default query questions.
      * @method GetDefaultQueryQuestions
      * @return {Array} If the function succeeds, it returns the array of questions. Otherwise the function fails.
      */
      GetDefaultQueryQuestions: function () {
        if (_addon) {
          return _addon.GetDefaultQueryQuestions();
        }
        return [];
      },
      /**
      * Send query question.
      * @method SendQueryQuestion
      * @param {String} question The query question.
      * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
        Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      SendQueryQuestion: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let question = clientOpts.question;
          try {
            let SendQueryQuestionParams = new messages.SendQueryQuestionParams();
            SendQueryQuestionParams.setQuestion(question);
            let bytes = SendQueryQuestionParams.serializeBinary();
            return _addon.SendQueryQuestion(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Stop meeting query.
      * @method StopMeetingQuery
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      StopMeetingQuery: function () {
        if (_addon) {
          return _addon.StopMeetingQuery();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Determine if can send query question.
      * @method CanSendQuery
      * @return {Boolean} true means can, otherwise not.
      */
      CanSendQuery: function () {
        if (_addon) {
          return _addon.CanSendQuery();
        }
        return false;
      },
      /**
      * Request send query privilege.
      * @method RequestSendQueryPrivilege
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      RequestSendQueryPrivilege: function () {
        if (_addon) {
          return _addon.RequestSendQueryPrivilege();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Get the user ID of the requester.
      * @method GetSenderEnableQueryUserID
      * @return {Number} The user ID of the requester.
      */
      GetSenderEnableQueryUserID: function () {
        if (_addon) {
          return _addon.GetSenderEnableQueryUserID();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Continue approve action.
      * @method ContinueApproveEnableQuery
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      ContinueApproveEnableQuery: function () {
        if (_addon) {
          return _addon.ContinueApproveEnableQuery();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Get the user ID of the requester.
      * @method GetSenderStartQueryUserID
      * @return {Number} The user ID of the requester.
      */
      GetSenderStartQueryUserID: function () {
        if (_addon) {
          return _addon.GetSenderStartQueryUserID();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Approve request.
      * @method ApproveStartQuery
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      ApproveStartQuery: function () {
        if (_addon) {
          return _addon.ApproveStartQuery();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Decline request.
      * @method DeclineStartQuery
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      DeclineStartQuery: function () {
        if (_addon) {
          return _addon.DeclineStartQuery();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Send feedback of query answer.
      * @method AICompanionQueryFeedback
      * @param {Number} feedbackType The feedback type. For more details, see {@link MeetingAICompanionQueryFeedbackType}
      * @return {Number} If the function succeeds, the return value is SDKERR_SUCCESS.
        Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      AICompanionQueryFeedback: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let feedbackType = clientOpts.feedbackType;
          if (feedbackType == undefined) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
          try {
            let AICompanionQueryFeedbackParams = new messages.AICompanionQueryFeedbackParams();
            AICompanionQueryFeedbackParams.setFeedbacktype(feedbackType);
            let bytes = AICompanionQueryFeedbackParams.serializeBinary();
            return _addon.AICompanionQueryFeedback(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Get the title of the tip.
      * @return {String}
      */
      GetEnableQueryActionTipTitle: function () {
        if (_addon) {
          return _addon.GetEnableQueryActionTipTitle();
        }
        return "";
      },
      /**
      * Get the tip string.
      * @return {String}
      */
      GetEnableQueryActionTipString: function () {
        if (_addon) {
          return _addon.GetEnableQueryActionTipString();
        }
        return "";
      },
      /**
      * Confirm enable query. The object will be deleted after this interface call.
      * @method ConfirmEnableQueryAction
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      ConfirmEnableQueryAction: function () {
        if (_addon) {
          return _addon.ConfirmEnableQueryAction();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Cancel enable query. The object will be deleted after this interface call.
      * @method CancelEnableQueryAction
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      CancelEnableQueryAction: function () {
        if (_addon) {
          return _addon.CancelEnableQueryAction();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Get the requester's user ID.
      * @method GetSenderQueryUserID
      * @return {Number} the requester's user ID.
      */
      GetSenderQueryUserID: function () {
        if (_addon) {
          return _addon.GetSenderQueryUserID();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Approve the request.
      * @method ApproveSendQuery
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      ApproveSendQuery: function () {
        if (_addon) {
          return _addon.ApproveSendQuery();
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      /**
      * Decline the request.
      * @method DeclineSendQuery
      * @param {Boolean} bDeclineAll true means decline all request.
      * @return {Number} If the function succeed, the return value is SDKERR_SUCCESS.
	      Otherwise failed. To get extended error information, Defined in: {@link ZoomSDKError}
      */
      DeclineSendQuery: function (opts) {
        if (_addon) {
          let clientOpts = opts || {};
          let bDeclineAll = clientOpts.bDeclineAll;
          try {
            let DeclineSendQueryParams = new messages.DeclineSendQueryParams();
            DeclineSendQueryParams.setBdeclineall(bDeclineAll);
            let bytes = DeclineSendQueryParams.serializeBinary();
            return _addon.DeclineSendQuery(bytes);
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
  ZoomMeetingAICompanion: ZoomMeetingAICompanion
}