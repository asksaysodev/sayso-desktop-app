/**
 * @file ZoomSDKCloseCaptionController.h
 * @brief Interface for managing closed caption and live transcription features in Zoom meetings.
 */


#import <Foundation/Foundation.h>
#import <ZoomSDK/ZoomSDKErrors.h>

NS_ASSUME_NONNULL_BEGIN
/**
 * @class ZoomSDKCCRequestHandler
 * @brief The helper to handle the requested of start captions.
 * @note When isRequestTranslationOn is YES, use \link ZoomSDKCCRequestTranslationOnHandler::approveStartCaptionsRequest \endlink to approve start captions request.
       When isRequestTranslationOn is NO, use \link ZoomSDKCCRequestTranslationOffHandler::approveStartCaptionsRequest: \endlink to approve start captions request.
 */
@interface ZoomSDKCCRequestHandler : NSObject
/**
 * @brief The user ID of the user who send the start captions request.
 */
@property (nonatomic, assign, readonly) unsigned int senderUserID;

/**
 * @return YES means request to start captions with translation on. Otherwise NO.
 */
@property (nonatomic, assign, readonly) BOOL isRequestTranslationOn;
/**
 * @brief Deny the request to start captions.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)deny;
@end


/**
 * @class ZoomSDKCCRequestTranslationOnHandler
 * @brief Subclass of ZoomSDKCCRequestHandler for requests with translation enabled.
 */
@interface ZoomSDKCCRequestTranslationOnHandler : ZoomSDKCCRequestHandler
/**
 * @brief Approve the start captions request.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)approveStartCaptionsRequest;
@end

/**
 * @class ZoomSDKCCRequestTranslationOffHandler
 * @brief Subclass of ZoomSDKCCRequestHandler for requests without translation.
 */
@interface ZoomSDKCCRequestTranslationOffHandler : ZoomSDKCCRequestHandler
/**
 * @brief Approve the start captions request.
 * @param languageID The language to be set for all participants in meeting.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)approveStartCaptionsRequest:(int)languageID;
@end

/**
 * @class ZoomSDKLiveTranscriptionMessageInfo
 * @brief Interface representing a live transcription message.
 */
@interface ZoomSDKLiveTranscriptionMessageInfo : NSObject
/**
 * @brief Get the message ID of the current message.
 */
@property (nonatomic, copy, readonly, nullable) NSString *messageID;

/**
 * @brief Get the speaker's ID.
 */
@property (nonatomic, assign, readonly) unsigned int speakerID;

/**
 * @brief Get the speaker's name.
 */
@property (nonatomic, copy, readonly, nullable) NSString *speakerName;

/**
 * @brief Get the content of the current message.
 */
@property (nonatomic, copy, readonly, nullable) NSString *messageContent;

/**
 * @brief Get the time stamp of the current message.
 */
@property (nonatomic, assign, readonly) time_t timeStamp;

/**
 * @brief Get the type of the current message.
 */
@property (nonatomic, assign, readonly) ZoomSDKLiveTranscriptionOperationType messageType;

@end


/**
 * @class ZoomSDKLiveTranscriptionLanguage
 * @brief Represents a language supported for live transcription.
 */
@interface ZoomSDKLiveTranscriptionLanguage : NSObject
/**
 * @brief The language ID.
 */
@property(nonatomic, assign, readonly) int languageID;
/**
 * @brief The language name.
 */
@property(nonatomic, copy, readonly, nullable) NSString* languageName;
@end


/**
 * @protocol ZoomSDKCloseCaptionControllerDelegate
 * @brief Delegate protocol to receive closed caption and live transcription related events.
 */
@protocol ZoomSDKCloseCaptionControllerDelegate <NSObject>
@optional

/**
 * @brief Callback of getting the privilege of close caption. 
 */
- (void)onGetCCPrivilege;

/**
 * @brief Callback of losing privilege of close caption. 
 */
- (void)onLostCCPrivilege;

/**
 * @brief Notify the current user that close caption is received.
 * @param inString The content of the close caption message.
 * @param senderID The sender id of a closed caption message.
 * @note If the meeting supports multi- language transcription and the host sets manual captions to true, attendees must set the translation language id to -1 to receive closed captioned messages.
 */
- (void)onReceiveCCMessageWithString:(NSString*)inString senderID:(unsigned int)senderID;

/**
 * @brief Notify the current user of realtime message.
 * @param realtimeMessage The message that is being input.
 */
- (void)onReceiveCCRealtimeMessage:(NSString *)realtimeMessage;

/**
 * @brief Notify the live transcription status changed.
 * @param status The type of live transcription status.
 */
- (void)onLiveTranscriptionStatus:(ZoomSDKLiveTranscriptionStaus)status;

/**
 * @brief Callback: Invoked when a live transcription message is received.
 * @param messageInfo The live transcription message.
 */
- (void)onLiveTranscriptionMsgInfoReceived:(ZoomSDKLiveTranscriptionMessageInfo*_Nullable)messageInfo;

/**
 * @brief Callback: Original language message received callback.
 * @param messageInfo messageInfo The spoken language message.
 */
- (void)onOriginalLanguageMsgReceived:(ZoomSDKLiveTranscriptionMessageInfo*_Nullable)messageInfo;

/**
 * @brief The translation message error callback.
 * @param spokenLanguage An object of the spoken message language.
 * @param transcriptLanguage An object of the message language you want to translate.
 */
- (void)onLiveTranscriptionMsgError:(ZoomSDKLiveTranscriptionLanguage*_Nullable)spokenLanguage transcriptLanguage:(ZoomSDKLiveTranscriptionLanguage*_Nullable)transcriptLanguage;

/**
 * @brief Notify the request for live transcript received.
 * @param requesterID The user id of requester.
 * @param bAnonymous Request anonymously of not, if bAnonymous is YES, requesterID has no meanings.
 * @note only host can retrieve this callback
 */
- (void)onRequestForLiveTranscriptReceived:(unsigned int)requesterID anonymous:(BOOL)bAnonymous;

/**
 * @brief Notify the request for live transcript status changed.
 * @param bEnabled YES means enabled, otherwise not.
 */
- (void)onRequestLiveTranscriptionStatusChange:(BOOL)bEnabled;

/**
 * @brief Sink the event of captions enabled status changed.
 * @param enable YES means the host enables the captions, otherwise means the host disables the captions.
 */
- (void)onCaptionStatusChanged:(BOOL)enable;

/**
 * @brief Sink the event to start captions request.
 * @param handler The helper to handle the start captions request.
 */
- (void)onStartCaptionsRequestReceived:(ZoomSDKCCRequestHandler *)handler;

/**
 * @brief Sink the event to start captions request was approved.
 */
- (void)onStartCaptionsRequestApproved;

/**
 * @brief Sink the event of manual captions enablement status change.
 * @param bEnabled YES means the host enableds the manual captions. Otherwise, the host disabled the manual captions.
 */
- (void)onManualCaptionStatusChanged:(BOOL)bEnabled;

/**
 * @brief Callback when the spoken language is changed.
 * @param spokenLanguage A pointer to the current spoken language object.
 */
- (void)onSpokenLanguageChanged:(ZoomSDKLiveTranscriptionLanguage*_Nullable)spokenLanguage;
@end


/**
 * @class ZoomSDKCloseCaptionController
 * @brief Controller interface to manage closed captions and live transcription in meetings.
 */
@interface ZoomSDKCloseCaptionController : NSObject
{
    id<ZoomSDKCloseCaptionControllerDelegate> _delegate;
}

/**
 * @brief The delegate to receive closed caption events.
 */
@property(nonatomic, assign, nullable)id<ZoomSDKCloseCaptionControllerDelegate> delegate;

/**
 * @brief Query if Close Caption is supported in the current meeting.
 * @return YES means supported, otherwise not.
 */
- (BOOL)isMeetingSupportCloseCaption;

/**
 * @brief Query if it is able to assign others to send Close Caption.
 * @return YES means able, otherwise not.
 */
- (BOOL)canAssignOthersToSendCC;

/**
 * @brief Query if the specified user can be assigned to send close caption.
 * @param userID The ID of user who you want to assign to send close caption
 * @return YES means able, otherwise not.
 */
- (BOOL)canBeAssignedToSendCC:(unsigned int)userID;

/**
 * @brief Query if the current user can send Close Caption.
 * @return YES means able, otherwise not.
 */
- (BOOL)canSendClosedCaption;

/**
 * @brief Query if user can save Close Caption.
 * @return YES means able, otherwise not.
 */
- (BOOL)isCanSaveClosedCaption;

/**
 * @brief Query if the third party close caption server is available.
 * @return YES means available, otherwise not.
 */
- (BOOL)is3rdPartCCServerAvailable;

/**
 * @brief This method is used for host to withdraw CC privilege from another user.
 * @param userID The ID of user that you want to withdraw CC privilege.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)withdrawCCPrivilegeForUser:(unsigned int)userID;

/**
 * @brief This method is used for host to assign CC privilege to another user.
 * @param userID The ID of user whom you want to assign CC privilege to.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)assignCCPrivilegeTo:(unsigned int)userID;

/**
 * @brief Send CC message.
 * @param ccString The content of CC.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)sendClosedCaptionMessage:(NSString*)ccString;

/**
 * @brief Save CC.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)saveCCHistory;

/**
 * @brief Get the path where the CC is saved.
 * @return If the function succeeds, it will return a NSString, otherwise failed.
 */
- (NSString*)getClosedCaptionHistorySavedPath;

/**
 * @brief Get the third party URL which is used to input CC.
 * @param thirdPartyURL The URL of the third party service.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)getClosedCaptionUrlFor3rdParty:(NSString*_Nullable*_Nonnull)thirdPartyURL;

/**
 * @brief Determine whether the legal notice for Live transcript is available.
 * @return YES indicates the legal notice for Live transcript is available., otherwise not.
 */
- (BOOL)isLiveTranscriptLegalNoticeAvailable;

/**
 * @brief Get the CC legal notices prompt.
 * @return If the function succeeds, it will return the CC legal notices prompt, otherwise nil.
 */
- (NSString *)getLiveTranscriptLegalNoticesPrompt;

/**
 * @brief Get the CC legal notices explained.
 * @return If the function succeeds, it will return the CC legal notices explained, otherwise nil.
 */
- (NSString *)getLiveTranscriptLegalNoticesExplained;

/**
 * @brief Determine whether the live transcription feature is enabled.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isLiveTranscriptionFeatureEnabled;

/**
 * @brief Determine whether the multi-language transcription feature is enabled.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isMultiLanguageTranscriptionEnabled;

/**
 * @brief Determine whether the translated captions feature is enabled.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isTextLiveTranslationEnabled;

/**
 * @brief Query the status of live transcription.
 * @return If the function succeeds, it will return the status of live transcription with type ZoomSDKLiveTranscriptionStaus.
 */
- (ZoomSDKLiveTranscriptionStaus)getLiveTranscriptionStatus;

/**
 * @brief Query if it is able to start live transcription.
 * @return ZoomSDKError_Success means able, otherwise not.
 */
- (ZoomSDKError)canStartLiveTranscription;

/**
 * @brief Start live transcription.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success, otherwise not.
 * @note If the meeting allows multi- language transcription, all users can start live transcription. Otherwise, only the host can start it.
 */
- (ZoomSDKError)startLiveTranscription;

/**
 * @brief Stop live transcription.
 * @return If the function succeeds,  it will return @c ZoomSDKError_Success, otherwise not.
 * @note If the meeting allows multi- language transcription, all users can stop live transcription. Otherwise only the host can stop it.
*/
- (ZoomSDKError)stopLiveTranscription;

/**
 * @brief Enable or disable the ability for attendees to request live transcriptions.
 * @param bEnable YES means enable, otherwise not.
 * @return If the function succeeds,  it will return @c ZoomSDKError_Success, otherwise not.
 * @note If the meeting allows multi- language transcription, the return value is ZoomSDKError_WrongUsage.
 */
- (ZoomSDKError)enableRequestLiveTranscription:(BOOL)bEnable;

/**
 * @brief Determine whether the request to start live transcription is enabled.
 * @return @c ZoomSDKError_Success  means enabled, otherwise not.
 * @note If the meeting allows multi-language transcription, the return value is ZoomSDKError_WrongUsage.
 */
- (ZoomSDKError)isRequestToStartLiveTranscriptionEnabled;

/**
 * @brief Request the host to start ive transcription.
 * @param bAnonymous YES indicates the user anonymous request.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success, otherwise not.
 * @note If the meeting allows multi-language transcription, the return value is ZoomSDKError_WrongUsage.
 */
- (ZoomSDKError)requestToStartLiveTranscription:(BOOL)bAnonymous;

/**
 * @brief Enable or disable manual captions for the meeting.
 * @param bEnable YES means enable, otherwise not.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)enableMeetingManualCaption:(BOOL)bEnable;

/**
 * @brief Determine whether manually added closed captions is enabled for the meeting.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isMeetingManualCaptionEnabled;

/**
 * @brief Get the list of all available spoken languages in a meeting.
 * @return If the function succeeds, it will return an array contains the available spoken languages in a meeting, otherwise nil.
 */
- (NSArray<ZoomSDKLiveTranscriptionLanguage*>*)getAvailableMeetingSpokenLanguages;

/**
 * @brief Set the spoken language.
 * @param languageID The spoken language ID.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success, otherwise not.
 * @deprecated Use \link ZoomSDKCloseCaptionController::setMeetingSpokenLanguage:isForAll: \endlink instead.
 */
- (ZoomSDKError)setMeetingSpokenLanguage:(int)languageID DEPRECATED_MSG_ATTRIBUTE("Use -setMeetingSpokenLanguage:isForAll: instead");

/**
 * @brief Set the spoken language.
 * @param languageID The spoken language ID.
 * @param isForAll YES means set spoken language for all users. NO means set the language only for myself.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)setMeetingSpokenLanguage:(int)languageID isForAll:(BOOL)isForAll;

/**
 * @brief Get the spoken language of the current user.
 * @return If the function succeeds, it will an object of ZoomSDKLiveTranscriptionLanguage, otherwise nil.
 */
- (ZoomSDKLiveTranscriptionLanguage*)getMeetingSpokenLanguage;

/**
 * @brief Get the list of all available translation languages in a meeting.
 * @return If the function succeeds, it will return an array contains all available translation languages in a meeting, otherwise nil.
 */
- (NSArray<ZoomSDKLiveTranscriptionLanguage*>*)getAvailableTranslationLanguages;

/**
 * @brief Set the translation language of the current user.
 * @param languageID The translation language id. If the language ID is set to -1, live translation will be disabled. You can reveive closed captions if the  host sets manual captions to true for the meeting.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)setTranslationLanguage:(int)languageID;

/**
 * @brief Get the translation language of the current user.
 * @return If the function succeeds, it will an object of ZoomSDKLiveTranscriptionLanguage, otherwise nil.
 */
- (ZoomSDKLiveTranscriptionLanguage*)getTranslationLanguage;

/**
 * @brief Enable or disable to receive original and translated content.If enable this feature,you need start live transcription.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise not.
 */
- (ZoomSDKError)enableReceiveSpokenLanguageContent:(BOOL)enable;

/**
 * @brief Determine whether receive original and translated is available.
 * @return YES indicates receive original and translated is available. Otherwise NO.
 */
- (BOOL)isReceiveSpokenLanguageContentEnabled;

/**
 * @brief Query if the user can disable captions.
 * @return YES means that the host can disable captions.
 */
- (BOOL)canDisableCaptions;

/**
 * @brief Enable or disable captions.
 * @param enable YES means that captions are enabled; NO means that captions are disabled.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise the function returns an error.
 */
- (ZoomSDKError)enableCaptions:(BOOL)enable;

/**
 * @brief Query if the captions enabled.
 * @return YES means that captions are enabled.
 */
- (BOOL)isCaptionsEnabled;

/**
 * @brief Determine whether users can request to start captions.
 * @return YES indicates users can request to start captions. Otherwise NO.
 */
- (BOOL)isSupportRequestCaptions;

/**
 * @brief Determine whether support translation when users request to start captions.
 * @return YES indicates translation is available when users request to start captions. Otherwise NO.
 */
- (BOOL)isSupportTranslationWhenRequestToStartCaptions;

/**
 * @brief Request the host to start captions. If the host approves your request, you receive the callback \link ZoomSDKCloseCaptionControllerDelegate::onStartCaptionsRequestApproved \endlink, and you should start captions or translation there.
 * @param enableTranslation YES indicates to enable translation at the same time.
 * @return YES indicates translation is available when users request to start captions. Otherwise NO.
 */
- (ZoomSDKError)requestToStartCaptions:(BOOL)enableTranslation;
@end
NS_ASSUME_NONNULL_END
