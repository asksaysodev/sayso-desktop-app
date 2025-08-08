/**
 * @file ZoomSDKPollingController.h
 * @brief Interfaces for managing polling and quiz functionalities in Zoom SDK.
 *
 * This file defines the @c ZoomSDKPollingController class and related polling data models,
 * which allow the host or attendees to create, manage, and participate in polling or quiz during the meeting.
 * Polling types include single choice, multiple choice, fill-in-the-blank, short answer, long answer, etc.
 * This interface supports retrieving poll status, questions, answers, submitting answers,
 * sharing results, and managing poll permissions.
 */

#import <Foundation/Foundation.h>
#import <ZoomSDK/ZoomSDKErrors.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * @brief Enumeration of polling statuses.
 */
typedef enum
{
    /** The initial status. */
    ZoomSDKPollingStatus_Initial,
     /** Polling has started. */
    ZoomSDKPollingStatus_Started,
     /** Polling results have been shared. */
    ZoomSDKPollingStatus_ShareResult,
     /** Polling has been stopped. */
    ZoomSDKPollingStatus_Stopped
}ZoomSDKPollingStatus;


/**
 * @brief Enumeration of polling types.
 */
typedef enum
{
    /** Unknown polling type. */
    ZoomSDKPollingType_Unknown = 0,
    /** Standard polling type, typically used for gathering opinions or votes. */
    ZoomSDKPollingType_Poll,
    /** Quiz type, used for knowledge checks or assessments. */
    ZoomSDKPollingType_Quiz
}ZoomSDKPollingType;


/**
 * @brief Enumeration of polling question types.
 * For more information, please visit <https://support.zoom.com/hc/en/article?id=zm_kb&sysparm_article=KB0057587>.
 */
typedef enum
{
    /** Unknown question type. */
    ZoomSDKPollingQuestionType_Unknown = 0,
    /** Single choice question type. */
    ZoomSDKPollingQuestionType_Single,
    /** Multiple choice question type. */
    ZoomSDKPollingQuestionType_Multi,
    /** Matching question type. */
    ZoomSDKPollingQuestionType_Matching,
    /** Rank order question type. */
    ZoomSDKPollingQuestionType_RankOrder,
    /** Short answer question type. */
    ZoomSDKPollingQuestionType_ShortAnswer,
    /** Long answer question type. */
    ZoomSDKPollingQuestionType_LongAnswer,
    /** Fill in the blank question type. */
    ZoomSDKPollingQuestionType_FillBlank,
    /** Rating scale question type. */
    ZoomSDKPollingQuestionType_NPS,
    /** Dropdown question type. */
    ZoomSDKPollingQuestionType_Dropdown
}ZoomSDKPollingQuestionType;


/**
 * @brief Enumeration of polling action types.
 */
typedef enum
{
    /** Unknown action type. */
    ZoomSDKPollingActionType_Unknown = 0,
    /** Start the polling session. */
    ZoomSDKPollingActionType_Start,
    /** Stop the polling session. */
    ZoomSDKPollingActionType_Stop,
    /** Share the polling results with participants. */
    ZoomSDKPollingActionType_ShareResult,
    /** Stop sharing the polling results. */
    ZoomSDKPollingActionType_StopShareResult,
    /** Duplicate the polling. */
    ZoomSDKPollingActionType_Duplicate,
    /** Delete the polling. */
    ZoomSDKPollingActionType_Delete,
    /** Submit polling responses. */
    ZoomSDKPollingActionType_Submit,
    /** An error occurred with the polling action. */
    ZoomSDKPollingActionType_Error
}ZoomSDKPollingActionType;

/**
 * @class ZoomSDKPollingItem
 * @brief Polling item interface.
 */
@interface ZoomSDKPollingItem : NSObject
/**
 * @brief Polling ID.
 */
@property(nonatomic, copy, readonly, nullable) NSString* pollingID;
/**
 * @brief Polling name.
 */
@property(nonatomic, copy, readonly, nullable) NSString* pollingName;
/**
 * @brief Polling type.
 */
@property(nonatomic, assign, readonly) ZoomSDKPollingType pollingType;
/**
 * @brief Polling status.
 */
@property(nonatomic, assign, readonly) ZoomSDKPollingStatus pollingStatus;
/**
 * @brief Polling question count.
 */
@property(nonatomic, assign, readonly) unsigned int pollingQuestionCount;
/**
 * @brief Count of total voted users.
 */
@property(nonatomic, assign, readonly) unsigned int totalVotedUserCount;
/**
 * @brief Determine if is library polling.
 */
@property(nonatomic, assign, readonly) BOOL isLibraryPolling;
@end



/**
 * @class ZoomSDKPollingAnswerItem
 * @brief Polling answer item interface.
 */
@interface ZoomSDKPollingAnswerItem : NSObject
/**
 * @brief Polling ID of answer item.
 */
@property(nonatomic, copy, readonly, nullable) NSString* pollingID;
/**
 * @brief Polling question ID.
 */
@property(nonatomic, copy, readonly, nullable) NSString* pollingQuestionID;
/**
 * @brief Polling sub-question ID.
 */
@property(nonatomic, copy, readonly, nullable) NSString* pollingSubQuestionID;
/**
 * @brief Polling answer ID.
 */
@property(nonatomic, copy, readonly, nullable) NSString* pollingAnswerID;
/**
 * @brief Polling answer name.
 */
@property(nonatomic, copy, readonly, nullable) NSString* pollingAnswerName;
/**
 * @brief Polling answered content.
 */
@property(nonatomic, copy, readonly, nullable) NSString* pollingAnsweredContent;
/**
 * @brief Determine if is checked.
 * @note This property has no meaning for the corrent answer.
 */
@property(nonatomic, assign, readonly) BOOL isChecked;
@end


/**
 * @class ZoomSDKPollingAnswerResultItem
 * @brief Polling answer result item interface.
 */
@interface ZoomSDKPollingAnswerResultItem : NSObject
/**
 * @brief Polling ID of answer result item.
 */
@property(nonatomic, copy, readonly, nullable) NSString* pollingID;
/**
 * @brief Polling queston ID.
 */
@property(nonatomic, copy, readonly, nullable) NSString* pollingQuestionID;
/**
 * @brief Polling sub-queston ID.
 */
@property(nonatomic, copy, readonly, nullable) NSString* pollingSubQuestionID;
/**
 * @brief Polling answer ID.
 */
@property(nonatomic, copy, readonly, nullable) NSString* pollingAnswerID;
/**
 * @brief Polling answer name.
 */
@property(nonatomic, copy, readonly, nullable) NSString* pollingAnswerName;
/**
 * @brief Query how many participants selected this answer.
 */
@property(nonatomic, assign, readonly) unsigned int selectedCount;
@end


/**
 * @class ZoomSDKPollingQuestionItem
 * @brief Polling question item interface.
 */
@interface ZoomSDKPollingQuestionItem : NSObject
/**
 * @brief Polling ID of question item.
 */
@property(nonatomic, copy, readonly, nullable) NSString* pollingID;
/**
 * @brief Polling question ID.
 */
@property(nonatomic, copy, readonly, nullable) NSString* pollingQuestionID;
/**
 * @brief Polling question name.
 */
@property(nonatomic, copy, readonly, nullable) NSString* pollingQuestionName;
/**
 * @brief Polling question type.
 */
@property(nonatomic, assign, readonly) ZoomSDKPollingQuestionType pollingQuestionType;
/**
 * @brief The count of answered questions.
 */
@property(nonatomic, assign, readonly) unsigned int answeredCount;
/**
 * @brief Determine if is required.
 */
@property(nonatomic, assign, readonly) BOOL isRequired;
/**
 * @brief Get the list of polling question's subquestion.
 */
@property(nonatomic, retain, readonly, nullable) NSArray<ZoomSDKPollingQuestionItem*>* pollingSubQuestionItemList;
/**
 * @brief Get the list of polling question or subquestion's answer.
 */
@property(nonatomic, retain, readonly, nullable) NSArray<ZoomSDKPollingAnswerItem*>* pollingAnswerItemList;
@end



/**
 * @protocol ZoomSDKPollingControllerDelegate
 * @brief Polling controller callback event delegate.
 */
@protocol ZoomSDKPollingControllerDelegate <NSObject>
@optional
/**
 * @brief Polling status changed callback. Use this function to inform the user that the polling has been started, share result or stopped.
 * @param pollingID Specify the status changed poll's ID.
 * @param status Specify current polling status.
 */
- (void)onPollingStatusChanged:(NSString*)pollingID status:(ZoomSDKPollingStatus)status;

/**
 * @brief Polling result updated callback. When a participant submit polling.
 * @param pollingID Specify the result updated poll's ID.
 */
- (void)onPollingResultUpdated:(NSString*)pollingID;

/**
 * @brief Polling list updated callback. This is triggered when a host adds, edits, duplicates, or deletes a poll.
 */
- (void)onPollingListUpdated;

/**
 * @brief Polling action result callback. This is triggered when a user perform an action for a poll.
 * @param actionType Specify the action type.
 * @param pollingID Specify the action poll's ID.
 * @param bSuccess Specify whether the action succeeds.
 * @param errorMsg Specify the error message when the action fails. It is only for  ZoomSDKPollingActionType_Error.
 * @note If actionType is ZoomSDKPollingActionType_Error, use errorMsg. This errorMsg may be triggered by any action.
 */
- (void)onPollingActionResult:(ZoomSDKPollingActionType)actionType pollingID:(NSString*)pollingID bSuccess:(BOOL)bSuccess errorMsg:(NSString*_Nullable)errorMsg;

/**
 * @brief Polling question item image downloaded callback. This is triggered when a user starts or joins a meeting, if any polling question has an image, or the host uploads an image to a polling question in the meeting.
 * @param questionID Specify the polling question's ID.
 * @param path Specify the image downloaded path.
 */
- (void)onPollingQuestionImageDownloaded:(NSString*)questionID path:(NSString*)path;

/**
 * @brief Polling elapsed time callback. This is triggered when a host starts a poll or user changes to host from non host.
 * @param pollingID Specify the polling ID.
 * @param elapsedTime Specify the polling elapsed time in milliseconds.
 */
- (void)onGetPollingElapsedTime:(NSString*)pollingID elapsedTime:(unsigned int)elapsedTime;

/**
 * @brief Callback event for the user get right answer list privilege changed. This is triggered when the host calls EnableGetRightAnswerList in a quiz.
 * @param bCan Specify whether the user has get right answer list privilege or not.
 */
- (void)onGetRightAnswerListPrivilege:(BOOL)bCan;

/**
 * @brief Polling inactive callback. When the user starts a meeting in another device by same account, this callback is triggered. Then call CanDoPolling return false.
 */
- (void)onPollingInactive;
@end


/**
* @class ZoomSDKPollingController
* @brief Polling controller interface.
*/
@interface ZoomSDKPollingController : NSObject
{
    id<ZoomSDKPollingControllerDelegate> _delegate;
}
/**
 * @brief Delegate object to receive polling events.
 */
@property(nonatomic, assign, nullable)id<ZoomSDKPollingControllerDelegate> delegate;

//Common (for all)
/**
 * @brief Determine whether the current meeting can do the polling.
 * @return YES indicates you can do polling.
 */
- (BOOL)canDoPolling;

/**
 * @brief Determine if the answer result list can be show.
 * @param pollingID Specify the answer result's polling ID.
 * @return YES means can. Otherwise, the return value is NO.
 */
- (BOOL)canShowAnswerResultList:(NSString*)pollingID;

/**
 * @brief Get the list of poll's question.
 * @param pollingID Specify the question's polling ID.
 * @return If the function succeeds, the return value is an NSArray of ZoomSDKPollingQuestionItem objects. Otherwise, the return value is nil.
 */
- (NSArray<ZoomSDKPollingQuestionItem*>*)getPollingQuestionItemList:(NSString*)pollingID;

/**
 * @brief Get the list of polling question or subquestion's right answer.
 * @param pollingID Specify the right answer's polling ID.
 * @return If the function succeeds, the return value is an NSArray of ZoomSDKPollingAnswerItem objects. Otherwise, the return value is nil.
 */
- (NSArray<ZoomSDKPollingAnswerItem*>*)getPollingRightAnswerItemList:(NSString*)pollingID;

/**
 * @brief Get the list of polling answer result item.
 * @param pollingID Specify the answer result's polling ID.
 * @return If the function succeeds, the return value is an NSArray of ZoomSDKPollingAnswerResultItem objects. Otherwise, the return value is nil.
 */
- (NSArray<ZoomSDKPollingAnswerResultItem*>*)getPollingAnswerResultItemList:(NSString*)pollingID;

/**
 * @brief Get the polling item object of specified polling ID.
 * @param pollingID Specify the polling ID for which you want to get the information.
 * @return If the function succeeds, the return value is a pointer to the ZoomSDKPollingItem. Otherwise, the return value is nil.
 */
- (ZoomSDKPollingItem*)getPollingItemByID:(NSString*)pollingID;

/**
 * @brief Determine if the right answer item list can be allowed to get.
 * @param pollingID Specify the right answer's polling ID.
 * @return YES means can. Otherwise, the return value is NO.
 */
- (BOOL)canGetRightAnswerItemList:(NSString*)pollingID;

/**
 * @brief Get the polling question's image path.
 * @param pollingID Specify the answer's polling ID.
 * @param questionID Specify the answer's question ID.
 * @return If the function succeeds, the return value is the question's image path, Otherwise the function fails, and the return value is nil.
 */
- (NSString*)getPollingQuestionImagePath:(NSString*)pollingID questionID:(NSString*)questionID;

/**
 * @brief Get the polling question's minimum character and maximum character count.
 * @param pollingID Specify the question's polling ID.
 * @param questionID Specify the question's question ID.
 * @param minLen Specify the minimum number of words to submit.
 * @param maxLen Specify the maximum number of words to submit.
 * @return If the function succeeds, the return value is @c ZoomSDKError_Success. Otherwise failed.
 * @note This function can only be used by short answer question and long answer question.
 */
- (ZoomSDKError)getQuestionCharactersLengthRange:(NSString*)pollingID questionID:(NSString*)questionID minLen:(unsigned int*)minLen maxLen:(unsigned int*)maxLen;

/**
 * @brief Determine if the question is case sensitive.
 * @param pollingID Specify the question's polling ID.
 * @param questionID Specify the question's question ID.
 * @return YES means is case sensitive. Otherwise, the return value is NO.
 * @note This function can only be used by fill blank questions.
 */
- (BOOL)isQuestionCaseSensitive:(NSString*)pollingID questionID:(NSString*)questionID;

/**
 * @brief Get the active poll's ID.
 * @return The shared result or started poll's ID.
 */
- (NSString*)getActivePollingID;


//Host
/**
 * @brief Determine if the host can add polling.
 * @return YES means can. Otherwise, the return value is NO.
 * @note Only the origin host can add polling.
 */
- (BOOL)canAddPolling;

/**
 * @brief Open create polling in a web browser.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)addPolling;

/**
 * @brief Determine if the host can edit polling.
 * @param pollingID Specify the edit poll's polling ID.
 * @return YES means can. Otherwise, the return value is NO.
 * @note Only the origin host can edit polling.
 */
- (BOOL)canEditPolling:(NSString*)pollingID;

/**
 * @brief Open edit polling in a web browser.
 * @param pollingID Specify the edit poll's polling ID.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)editPolling:(NSString*)pollingID;

/**
 * @brief Determine if the host can delete polling.
 * @param pollingID Specify the delete poll's polling ID.
 * @return YES means can. Otherwise, the return value is NO.
 * @note Only the origin host can delete polling.
 */
- (BOOL)canDeletePolling:(NSString*)pollingID;

/**
 * @brief Delete the polling.
 * @param pollingID Specify the delete poll's polling ID.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)deletePolling:(NSString*)pollingID;

/**
 * @brief Determine if the host can duplicate polling.
 * @param pollingID Specify the duplicate poll's polling ID.
 * @return YES means can. Otherwise, the return value is NO.
 * @note Only the origin host can duplicate polling.
 */
- (BOOL)canDuplicatePolling:(NSString*)pollingID;

/**
 * @brief Duplicate the polling.
 * @param pollingID Specify the duplicate poll's polling ID.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)duplicatePolling:(NSString*)pollingID;

/**
 * @brief Determine if the user can view polling result in the browser.
 * @param pollingID Specify the poll's polling ID.
 * @return YES means can. Otherwise, the return value is NO.
 * @note Only the origin host can view.
 */
- (BOOL)canViewPollingResultFromBrowser:(NSString*)pollingID;

/**
 * @brief Open a polling result in the web browser.
 * @param pollingID Specify the poll's polling ID.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)viewPollingResultFromBrowser:(NSString*)pollingID;

/**
 * @brief Get the list of poll item.
 * @return If the function succeeds, the return value is a pointer to the NSArray<ZoomSDKPollingItem*>. Otherwise, the return value is nil.
 */
- (NSArray<ZoomSDKPollingItem*>*)getPollingItemList;

/**
 * @brief Determine if the host or co-host can start the polling.
 * @param pollingID Specify the poll's polling ID.
 * @return YES means can. Otherwise, the return value is NO.
 */
- (BOOL)canStartPolling:(NSString*)pollingID;

/**
 * @brief Start the polling.
 * @param pollingID Specify the poll's polling ID.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise, the function fails.
 */
- (ZoomSDKError)startPolling:(NSString*)pollingID;

/**
 * @brief Stop the polling.
 * @param pollingID Specify the poll's polling ID.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise, the function fails.
 */
- (ZoomSDKError)stopPolling:(NSString*)pollingID;

/**
 * @brief Determine if the host or co-host can restart the polling.
 * @param pollingID Specify the poll's polling ID.
 * @return YES means can. Otherwise, the return value is NO.
 */
- (BOOL)canRestartPolling:(NSString*)pollingID;

/**
 * @brief Restart the polling.
 * @param pollingID Specify the poll's polling ID.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise, the function fails.
 */
- (ZoomSDKError)restartPolling:(NSString*)pollingID;

/**
 * @brief Determine if the host or co-host can share the poll's result.
 * @param pollingID Specify the poll's polling ID.
 * @return YES means can. Otherwise, the return value is NO.
 */
- (BOOL)canSharePollingResult:(NSString*)pollingID;

/**
 * @brief Share the poll's result.
 * @param pollingID Specify the poll's polling ID.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise, the function fails.
 */
- (ZoomSDKError)startSharePollingResult:(NSString*)pollingID;

/**
 * @brief Stop to share the poll's result.
 * @param pollingID Specify the poll's polling ID.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise, the function fails.
 */
- (ZoomSDKError)stopSharePollingResult:(NSString*)pollingID;

/**
 * @brief Set to enable show right answer to participants when share quiz's result.
 * @param bEnable YES indicates to enable. NO means not.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise, the function fails.
 */
- (ZoomSDKError)enableGetRightAnswerList:(BOOL)bEnable;

/**
 * @brief Determine if the host can download poll's result from browser.
 * @return YES means can. Otherwise, the return value is NO.
 * @note Only the origin host can download.
 */
- (BOOL)canDownloadResult;

/**
 * @brief Download all stopped poll's result from browser.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise, the function fails.
 */
- (ZoomSDKError)downLoadResult;


/**
 * @brief Attendee set answer's check.
 * @param answerItem Specify the object of ZoomSDKPollingAnswerItem.
 * @param bCheck Specify whether you want to select the answer or not.
 * @remarks This function should only be used by single, matching, rank order, multi, rating scale, or drop down questions.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise, the function fails.
 */
- (ZoomSDKError)setAnswerCheck:(ZoomSDKPollingAnswerItem*)answerItem bCheck:(BOOL)bCheck;

/**
 * @brief Attendee set answer's content.
 * @param answerItem Specify the object of ZoomSDKPollingAnswerItem.
 * @param answerText Specify you want to reply answer's content.
 * @return If the function succeeds, the return value @c ZoomSDKError_Success. Otherwise, the function fails.
 * @note This function should only be used by fill blank, short answer and long answer questions.
 */
- (ZoomSDKError)setAnswerContent:(ZoomSDKPollingAnswerItem*)answerItem answerText:(NSString*)answerText;

/**
 * @brief Determine if the attendee can submit the polling.
 * @param pollingID Specify the submit poll's polling ID.
 * @return YES means can. Otherwise, the return value is NO.
 */
- (BOOL)canSubmitPolling:(NSString*)pollingID;

/**
 * @brief Submit the polling.
 * @param pollingID Specify the submit poll's polling ID.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise, the function fails.
 */
- (ZoomSDKError)submitPolling:(NSString*)pollingID;
@end
NS_ASSUME_NONNULL_END
