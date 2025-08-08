let { ZoomSDKError } = require("./settings.js");
const messages = require("./electron_sdk_pb.js");

var ZoomMeetingQA = (function () {
  var instance;
  /**
   * Zoom Meeting Q&A
   * @module zoom_meeting_qa
   * @param {Function} onAddQuestion Callback event used to inform the user once a question is added.
   * @param {Function} onReceiveQuestion Callback event used to inform the user once a question is received.
   * @param {Function} onDeleteQuestions Callback event used to inform the user once a question is deleted.
   * @param {Function} onQuestionMarkedAsDismissed Callback event used to inform the user once a question is marked as dismiss.
   * @param {Function} onReopenQuestion Callback event used to inform the user once a question is reopened.
   * @param {Function} onUserLivingReply Callback event used to inform the user once a question is answered live
   * @param {Function} onUserEndLiving Callback event used to inform the user once a question has ended live answer.
   * @param {Function} onAddAnswer Callback event used to inform the user once an answer is added.
   * @param {Function} onReceiveAnswer Callback event used to inform the user once an answer is received.
   * @param {Function} onDeleteAnswers Callback event used to inform the user once an answer is deleted.
   * @param {Function} onVoteupQuestion Callback event used to inform the user once a question has been voted.
   * @param {Function} onRevokeVoteupQuestion Callback event used to inform the user once a question's vote has been revoked.
   * @return {ZoomMeetingQA}
   */
  function init(opts) {
    let clientOpts = opts || {};
    // Private methods and variables
    let _addon = clientOpts.addon.GetMeetingQACtrl() || null;
    let _onAddQuestion = clientOpts.onAddQuestion || null;
    let _onReceiveQuestion = clientOpts.onReceiveQuestion || null;
    let _onDeleteQuestions = clientOpts.onDeleteQuestions || null;
    let _onQuestionMarkedAsDismissed =
      clientOpts.onQuestionMarkedAsDismissed || null;
    let _onReopenQuestion = clientOpts.onReopenQuestion || null;
    let _onUserLivingReply = clientOpts.onUserLivingReply || null;
    let _onUserEndLiving = clientOpts.onUserEndLiving || null;
    let _onReceiveAnswer = clientOpts.onReceiveAnswer || null;
    let _onAddAnswer = clientOpts.onAddAnswer || null;
    let _onDeleteAnswers = clientOpts.onDeleteAnswers || null;
    let _onVoteupQuestion = clientOpts.onVoteupQuestion || null;
    let _onRevokeVoteupQuestion = clientOpts.onRevokeVoteupQuestion || null;

    /**
     * Callback event used to inform the user once a question is added
      @event onAddQuestion
      @param {String} questionID The question id
      @param {Boolean} bSuccess Add question successfully or not
     */
    function onAddQuestion(questionID, bSuccess) {
      if (_onAddQuestion) {
        _onAddQuestion(questionID, bSuccess);
      }
    }

    /**
     * Callback event used to inform the user once a question is received
      @event onReceiveQuestion
      @param {String} questionID The question id
     */
    function onReceiveQuestion(questionID) {
      if (_onReceiveQuestion) {
        _onReceiveQuestion(questionID);
      }
    }

    /**
     * Callback event used to inform the usee once a question is deleted
      @event onDeleteQuestions
      @param {Array} questions The array ofquestion id
     */
    function onDeleteQuestions(questions) {
      if (_onDeleteQuestions) {
        _onDeleteQuestions(questions);
      }
    }

    /**
     * Callback event used to inform the user once a question is marked as dismiss
      @event onQuestionMarkedAsDismissed
      @param {String} questionID The question id
     */
    function onQuestionMarkedAsDismissed(questionID) {
      if (_onQuestionMarkedAsDismissed) {
        _onQuestionMarkedAsDismissed(questionID);
      }
    }

    /**
     * Callback event used to inform the user once a question is reopened.
      @event onReopenQuestion
      @param {String} questionID The question id
     */
    function onReopenQuestion(questionID) {
      if (_onReopenQuestion) {
        _onReopenQuestion(questionID);
      }
    }

    /**
     * Callback event used to inform the user once a question is answered live
      @event onUserLivingReply
      @param {String} questionID The question id
     */
    function onUserLivingReply(questionID) {
      if (_onUserLivingReply) {
        _onUserLivingReply(questionID);
      }
    }

    /**
     * Callback event used to inform the user once a question has ended live answer.
      @event onUserEndLiving
      @param {String} questionID The question id
     */
    function onUserEndLiving(questionID) {
      if (_onUserEndLiving) {
        _onUserEndLiving(questionID);
      }
    }

    /**
     * Callback event used to inform the user once an answer is added.
      @event onAddAnswer
      @param {String} answerID The answer id
      @param {Boolean} bSuccess If the success is YES means add answer is success,otherwise not.
     */
    function onAddAnswer(answerID, bSuccess) {
      if (_onAddAnswer) {
        _onAddAnswer(answerID, bSuccess);
      }
    }

    /**
     * Callback event used to inform the user once an answer is received.
      @event onReceiveAnswer
      @param {String} answerID The answer id
     */
    function onReceiveAnswer(answerID) {
      if (_onReceiveAnswer) {
        _onReceiveAnswer(answerID);
      }
    }

    /**
     * Callback event used to inform the user once an answer is deleted.
      @event onDeleteAnswers
      @param {Array} answer The array of answer id
     */
    function onDeleteAnswers(answer) {
      if (_onDeleteAnswers) {
        _onDeleteAnswers(answer);
      }
    }

    /**
     * Callback event used to inform the user once a question has been voted.
      @event onVoteupQuestion
      @param {String} questionID The vote question id
      @param {Boolean} isChanged If YES means the question order will change,otherwise not.
     */
    function onVoteupQuestion(questionID, isChanged) {
      if (_onVoteupQuestion) {
        _onVoteupQuestion(questionID, isChanged);
      }
    }

    /**
     * Callback event used to inform the user once a question's vote has been revoked.
      @event onRevokeVoteupQuestion
      @param {String} questionID The vote question id
      @param {Boolean} isChanged If YES means the question order will change,otherwise not.
     */
    function onRevokeVoteupQuestion(questionID, isChanged) {
      if (_onRevokeVoteupQuestion) {
        _onRevokeVoteupQuestion(questionID, isChanged);
      }
    }

    if (_addon) {
      _addon.SetOnAddQuestionCB(onAddQuestion);
      _addon.SetOnReceiveQuestionCB(onReceiveQuestion);
      _addon.SetOnDeleteQuestionsCB(onDeleteQuestions);
      _addon.SetOnQuestionMarkedAsDismissedCB(onQuestionMarkedAsDismissed);
      _addon.SetOnReopenQuestionCB(onReopenQuestion);
      _addon.SetOnUserLivingReplyCB(onUserLivingReply);
      _addon.SetOnUserEndLivingCB(onUserEndLiving);
      _addon.SetOnAddAnswerCB(onAddAnswer);
      _addon.SetOnReceiveAnswerCB(onReceiveAnswer);
      _addon.SetOnDeleteAnswersCB(onDeleteAnswers);
      _addon.SetOnVoteupQuestionCB(onVoteupQuestion);
      _addon.SetOnRevokeVoteupQuestionCB(onRevokeVoteupQuestion);
    }

    return {
      // Public methods and variables
      /**
       * Set add question callback
       * @method SetOnAddQuestion
       * @param {Function} onAddQuestion
       * @return {Boolean}
       */
      SetOnAddQuestionCB: function (onAddQuestion) {
        if (_addon && onAddQuestion && onAddQuestion instanceof Function) {
          _onAddQuestion = onAddQuestion;
          return true;
        }
        return false;
      },
      /**
       * Set receive question callback
       * @method SetOnReceiveQuestionCB
       * @param {Function} onReceiveQuestion
       * @return {Boolean}
       */
      SetOnReceiveQuestionCB: function (onReceiveQuestion) {
        if (
          _addon &&
          onReceiveQuestion &&
          onReceiveQuestion instanceof Function
        ) {
          _onReceiveQuestion = onReceiveQuestion;
          return true;
        }
        return false;
      },
      /**
       * Set delete question callback
       * @method SetOnDeleteQuestionsCB
       * @param {Function} onDeleteQuestions
       * @return {Boolean}
       */
      SetOnDeleteQuestionsCB: function (onDeleteQuestions) {
        if (
          _addon &&
          onDeleteQuestions &&
          onDeleteQuestions instanceof Function
        ) {
          _onDeleteQuestions = onDeleteQuestions;
          return true;
        }
        return false;
      },
      /**
       * Set dismiss question callback
       * @method SetOnQuestionMarkedAsDismissedCB
       * @param {Function} onQuestionMarkedAsDismissed
       * @return {Boolean}
       */
      SetOnQuestionMarkedAsDismissedCB: function (onQuestionMarkedAsDismissed) {
        if (
          _addon &&
          onQuestionMarkedAsDismissed &&
          onQuestionMarkedAsDismissed instanceof Function
        ) {
          _onQuestionMarkedAsDismissed = onQuestionMarkedAsDismissed;
          return true;
        }
        return false;
      },
      /**
       * Set reopen question callback
       * @method SetOnReopenQuestionCB
       * @param {Function} onReopenQuestion
       * @return {Boolean}
       */
      SetOnReopenQuestionCB: function (onReopenQuestion) {
        if (
          _addon &&
          onReopenQuestion &&
          onReopenQuestion instanceof Function
        ) {
          _onReopenQuestion = onReopenQuestion;
          return true;
        }
        return false;
      },
      /**
       * Set living reply question callback
       * @method SetOnUserLivingReplyCB
       * @param {Function} onUserLivingReply
       * @return {Boolean}
       */
      SetOnUserLivingReplyCB: function (onUserLivingReply) {
        if (
          _addon &&
          onUserLivingReply &&
          onUserLivingReply instanceof Function
        ) {
          _onUserLivingReply = onUserLivingReply;
          return true;
        }
        return false;
      },
      /**
       * Set end living reply question callback
       * @method SetOnUserEndLivingCB
       * @param {Function} onUserEndLiving
       * @return {Boolean}
       */
      SetOnUserEndLivingCB: function (onUserEndLiving) {
        if (_addon && onUserEndLiving && onUserEndLiving instanceof Function) {
          _onUserEndLiving = onUserEndLiving;
          return true;
        }
        return false;
      },
      /**
       * Set on add answer callback
       * @method SetOnAddAnswerCB
       * @param {Function} onAddAnswer
       * @return {Boolean}
       */
      SetOnAddAnswerCB: function (onAddAnswer) {
        if (_addon && onAddAnswer && onAddAnswer instanceof Function) {
          _onAddAnswer = onAddAnswer;
          return true;
        }
        return false;
      },
      /**
       * Set receive answer callback
       * @method SetOnReceiveAnswerCB
       * @param {Function} onReceiveAnswer
       * @return {Boolean}
       */
      SetOnReceiveAnswerCB: function (onReceiveAnswer) {
        if (_addon && onReceiveAnswer && onReceiveAnswer instanceof Function) {
          _onReceiveAnswer = onReceiveAnswer;
          return true;
        }
        return false;
      },
      /**
       * Set delete answer callback
       * @method SetOnDeleteAnswersCB
       * @param {Function} onDeleteAnswers
       * @return {Boolean}
       */
      SetOnDeleteAnswersCB: function (onDeleteAnswers) {
        if (_addon && onDeleteAnswers && onDeleteAnswers instanceof Function) {
          _onDeleteAnswers = onDeleteAnswers;
          return true;
        }
        return false;
      },
      /**
       * Set receive question voted callback
       * @method SetOnVoteupQuestionCB
       * @param {Function} onVoteupQuestion
       * @return {Boolean}
       */
      SetOnVoteupQuestionCB: function (onVoteupQuestion) {
        if (
          _addon &&
          onVoteupQuestion &&
          onVoteupQuestion instanceof Function
        ) {
          _onVoteupQuestion = onVoteupQuestion;
          return true;
        }
        return false;
      },
      /**
       * Set receive question's vote revoked callback
       * @method SetOnRevokeVoteupQuestionCB
       * @param {Function} onRevokeVoteupQuestion
       * @return {Boolean}
       */
      SetOnRevokeVoteupQuestionCB: function (onRevokeVoteupQuestion) {
        if (
          _addon &&
          onRevokeVoteupQuestion &&
          onRevokeVoteupQuestion instanceof Function
        ) {
          _onRevokeVoteupQuestion = onRevokeVoteupQuestion;
          return true;
        }
        return false;
      },
      /**
       * Get a certain question with the question id
       * @method GetQuestion
       * @param {String} questionID is the question id.
       * @return {Object}
       */
      GetQuestion: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var questionID = clientOpts.questionID;
          // (!questionID)
          if (!clientOpts) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
          try {
            let QuestionIDParams = new messages.QuestionIDParams();
            // QuestionIDParams.setQuestionID(questionID);
            QuestionIDParams.setQid(clientOpts);
            let bytes = QuestionIDParams.serializeBinary();
            let questionList = _addon.GetQuestion(bytes),
              questionObj = {};
            if (questionList) {
              questionObj = questionList[0];

              let answerList = questionList[1];

              questionObj.answerList = answerList;
            }
            return questionObj;
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
      },
      GetAnswer: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          if (!clientOpts) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
          try {
            let AnswerIDParams = new messages.AnswerIDParams();
            AnswerIDParams.setAnswerid(clientOpts);
            let bytes = AnswerIDParams.serializeBinary();
            let answerList = _addon.GetAnswer(bytes),
              answerObj = {};
            if (answerList) {
              answerObj = answerList[0];
            }
            return answerObj;
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
      },
      AnswerQuestionPublicWithQuestionID: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var questionID = clientOpts.questionID;
          var content = clientOpts.questionContent;
          // (!questionID)
          if (!questionID || !content) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
          try {
            let AnswerQuestionParams = new messages.AnswerQuestionParams();
            // QuestionIDParams.setQuestionID(questionID);
            AnswerQuestionParams.setQid(questionID);
            AnswerQuestionParams.setContent(content);
            let bytes = AnswerQuestionParams.serializeBinary();
            return _addon.AnswerQuestionPublicWithQuestionID(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
      },
      AnswerQuestionPrivateWithQuestionID: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var questionID = clientOpts.questionID;
          var content = clientOpts.questionContent;
          if (!questionID || !content) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
          try {
            let AnswerQuestionParams = new messages.AnswerQuestionParams();
            AnswerQuestionParams.setQid(questionID);
            AnswerQuestionParams.setContent(content);
            let bytes = AnswerQuestionParams.serializeBinary();
            return _addon.AnswerQuestionPrivateWithQuestionID(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
      },
      GetAllQuestionList: function () {
        if (_addon) {
          // try {
          //   return _addon.GetAllQuestionList();
          // } catch (error) {
          //   return ZoomSDKError.SDKERR_UNINITIALIZE;
          // }
          try {
            let questionList = _addon.GetAllQuestionList();

            if (questionList) {
              return questionList.map(([question, answerList], index) => {
                return { ...question, answerList };
              });
            }
            return [];
          } catch (error) {
            return ZoomSDKError.SDKERR_UNINITIALIZE;
          }
        }
      },
      StartLiving: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var questionID = clientOpts.questionID;
          if (!questionID) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }

          try {
            let QuestionIDParams = new messages.QuestionIDParams();
            QuestionIDParams.setQid(questionID);
            let bytes = QuestionIDParams.serializeBinary();

            return _addon.StartLiving(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      EndLiving: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var questionID = clientOpts.questionID;
          if (!questionID) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }

          try {
            let QuestionIDParams = new messages.QuestionIDParams();
            QuestionIDParams.setQid(questionID);
            let bytes = QuestionIDParams.serializeBinary();

            return _addon.EndLiving(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      DismissQuestionWithQuestionID: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var questionID = clientOpts.questionID;
          if (!questionID) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }

          try {
            let QuestionIDParams = new messages.QuestionIDParams();
            QuestionIDParams.setQid(questionID);
            let bytes = QuestionIDParams.serializeBinary();

            return _addon.DismissQuestionWithQuestionID(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      DeleteQuestion: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var questionID = clientOpts.questionID;
          if (!questionID) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }

          try {
            let QuestionIDParams = new messages.QuestionIDParams();
            QuestionIDParams.setQid(questionID);
            let bytes = QuestionIDParams.serializeBinary();

            return _addon.DeleteQuestion(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      DeleteAnswer: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var answerID = clientOpts.answerID;
          if (!answerID) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }

          try {
            let AnswerIDParams = new messages.AnswerIDParams();
            AnswerIDParams.setAnswerid(answerID);
            let bytes = AnswerIDParams.serializeBinary();

            return _addon.DeleteAnswer(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      ReopenQuestionWithQuestionID: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var questionID = clientOpts.questionID;
          if (!questionID) {
            console.log("hi there is an eerror");
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }

          try {
            let QuestionIDParams = new messages.QuestionIDParams();
            QuestionIDParams.setQid(questionID);
            let bytes = QuestionIDParams.serializeBinary();

            return _addon.ReopenQuestionWithQuestionID(bytes);
          } catch (error) {
            console.log("hi there is an eerror2");
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
        return ZoomSDKError.SDKERR_UNINITIALIZE;
      },
      VoteupQuestionWithQuestionID: function (opts) {
        if (_addon) {
          var clientOpts = opts || {};
          var questionID = clientOpts.questionID;
          var enableQuestion = clientOpts.enabled;
          if (!questionID) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }

          if (enableQuestion == null ) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
          try {
            let VoteQuestionParams = new messages.VoteQuestionParams();
            VoteQuestionParams.setQid(questionID);
            VoteQuestionParams.setEnabled(enableQuestion);
            let bytes = VoteQuestionParams.serializeBinary();
            return _addon.VoteupQuestionWithQuestionID(bytes);
          } catch (error) {
            return ZoomSDKError.SDKERR_INVALID_PARAMETER;
          }
        }
      }
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
  ZoomMeetingQA: ZoomMeetingQA,
};
