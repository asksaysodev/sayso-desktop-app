/**
 * @file ZoomSDKMeetingAICompanionController.h
 * @brief Zoom SDK AI Companion feature control interface.
 *
 * This file defines interfaces for managing AI Companion features during meetings,
 * including smart summary, query helper, and controlling feature states.
 */


#import <Foundation/Foundation.h>
#import <ZoomSDK/ZoomSDKErrors.h>
#import <ZoomSDK/ZoomSDKMeetingSmartSummaryController.h>
#import <ZoomSDK/ZoomSDKAICompanionQueryHelper.h>
#import <ZoomSDK/ZoomSDKAICompanionSmartSummaryHelper.h>

NS_ASSUME_NONNULL_BEGIN
/**
 * @class ZoomSDKAICompanionFeatureTurnOnAgainHandler
 * @brief Handler to turn on AI Companion features again.
 */
@interface ZoomSDKAICompanionFeatureTurnOnAgainHandler : NSObject
/**
 * @brief Get the list of features that the attendee turns off. For more information, see \link ZoomSDKAICompanionFeature \endlink.
 */
@property(nonatomic,retain,readonly,nullable)NSArray *featuresList;
/**
 * @brief Get the feature list that the assets are deleted when the feature is turned off by attendee. For more information, see \link ZoomSDKAICompanionFeature \endlink.
 */
@property(nonatomic,retain,readonly,nullable)NSArray *assetsDeletedFeatureList;
/**
 * @brief Turn on the auto AI Companion feature which was stopped by the attendee before the host or cohost joined meeting.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)turnOnAgain;

/**
 * @brief Agree the auto AI Companion feature turn off status. Keep the AI Companion feature off.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)agreeTurnOff;
@end

/**
 * @class ZoomSDKAICompanionFeatureSwitchHandler
 * @brief Handler to respond to requests for turning AI Companion features on or off.
 */
@interface ZoomSDKAICompanionFeatureSwitchHandler : NSObject
/**
 * @brief The user ID who requests the host to turn  the AI Companion features on or off.
 */
@property(nonatomic,assign,readonly)unsigned int requestUserID;
/**
 * @brief Turn the AI Companion features on or off.
 */
@property(nonatomic,assign,readonly)BOOL isTurnOn;
/**
 * @brief Agree to the request to turn  the AI Companion features on or off.
 * @param deleteAssets YES means delete the meeting assets when turning off the AI Companion features.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)agree:(BOOL)deleteAssets;

/**
 * @brief Decline the request to turn the AI Companion features on or off.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)decline;
@end


/**
 * @protocol ZoomSDKMeetingAICompanionControllerDelegate
 * @brief Callback events for AI Companion feature control.
 */
@protocol ZoomSDKMeetingAICompanionControllerDelegate <NSObject>

/**
 * @brief Sink the event when the auto start AI Companion feature is turned off by an attendee before the host joins. Only the host or cohost can receive the callback.
 * @param handler The handler to turn on the AI feature again or leave the feature turned off.
 */
- (void)onAICompanionFeatureTurnOffByParticipant:(ZoomSDKAICompanionFeatureTurnOnAgainHandler*)handler;

/**
 * @brief Sink the event when the host receives the request to turn  the AI Companion features on or off.
 * @param handler The handler to turn the AI Companions features on or off.
 */
- (void)onAICompanionFeatureSwitchRequested:(ZoomSDKAICompanionFeatureSwitchHandler*)handler;

/**
 * @brief Sink the event when the host handles the request to turn  the AI Companion features on or off.
 * @param timeout Specify the host not handle the request until timeout.
 * @param agree Specify the host agrees to the request to turn the AI companion features on or off.
 * @param turnOn Specify the host respond the request of turn on or off.
 */
- (void)onAICompanionFeatureSwitchRequestResponse:(BOOL)timeout isAgree:(BOOL)agree isTurnOn:(BOOL)turnOn;

/**
 * @brief Sink the event when the started AI Companion feature can't be turned off.
 * @param features The AI features that can't be turned off. For more information, see \link ZoomSDKAICompanionFeature \endlink.
 */
- (void)onAICompanionFeatureCanNotBeTurnedOff:(NSArray*)features;
@end


/**
 * @class ZoomSDKMeetingAICompanionController
 * @brief AI Companion feature control class.
 */
@interface ZoomSDKMeetingAICompanionController : NSObject

/**
 * @brief Delegate to receive AI Companion feature callbacks.
 */
@property(nonatomic, assign, nullable)id<ZoomSDKMeetingAICompanionControllerDelegate> delegate;

/**
 * @brief Get the smart summary controller.
 * @return If the function succeeds, the return value is a pointer to ZoomSDKMeetingSmartSummaryController. Otherwise the function returns nil.
 * @deprecated Use \link ZoomSDKMeetingAICompanionController::getSmartSummaryHelper \endlink instead.
 */
- (ZoomSDKMeetingSmartSummaryController*)getInMeetingSmartSummaryController DEPRECATED_MSG_ATTRIBUTE("Use -getSmartSummaryHelper instead");

/**
 * @brief Get the smart summary controller.
 * @return If the function succeeds, the return value is a pointer to ZoomSDKAICompanionSmartSummaryHelper. Otherwise the function returns nil.
 */
- (ZoomSDKAICompanionSmartSummaryHelper*)getSmartSummaryHelper;
/**
 * @brief Get object of ZoomSDKAICompanionQueryHelper.
 * @return If the function succeeds, it will return a ZoomSDKAICompanionQueryHelper object.
 */
- (ZoomSDKAICompanionQueryHelper*)getQueryHelper;

/**
 * @brief Whether the meeting supports turning off the AI Companion features.
 * @return YES means the meeting can support turning off the AI Companion features. Otherwise the value is NO.
 */
- (BOOL)isTurnoffAllAICompanionsSupported;

/**
 * @brief Whether the current user can turn off the AI Companion features.
 * @return YES means the user can turn off the AI Companion features.
 */
- (BOOL)canTurnOffAllAICompanions;

/**
 * @brief Turn off the AI Companion features.
 * @param deleteMeetingAssets YES whether delete the meeting assets when turn off the AI Companion features.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success.
 * @note All AI features including \link ZoomSDKAICompanionFeature \endlink can be turned off at once.
 */
- (ZoomSDKError)turnOffAllAICompanion:(BOOL)deleteMeetingAssets;
//
/**
 * @brief Whether the meeting supports turning on the AI Companion features.
 * @return YES means the meeting does support turn on the AI Companion features. Otherwise the value is NO.
 */
- (BOOL)isTurnOnAllAICompanionSupported;

/**
 * @brief Whether the current user can turn off the AI Companion features.
 * @return YES means the user can turn off the AI Companion features.
 */
- (BOOL)canTurnOnAllAICompanion;

/**
 * @brief Turn on all the AI Companion features. Only \link @c ZoomSDKAICompanionFeature::ZoomSDKAICompanionFeature_Query \endlink and \link @c ZoomSDKAICompanionFeature::ZoomSDKAICompanionFeature_SmartSummary \endlink  can be turned on at once.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)turnOnAllAICompanion;

/**
 * @brief Whether the current user can ask the  host to turn off all started AI Companion features.
 * @return YES means the user can request host to turn off the AI Companion features.
 * @note if the current user joins the meeting before the host, they can check \link ZoomSDKMeetingAICompanionController::canTurnOffAllAICompanions \endlink to turn off the AI Companion features by himself/herself.
 * All AI features include \link @c ZoomSDKAICompanionFeature::ZoomSDKAICompanionFeature_Query \endlink , \link @c ZoomSDKAICompanionFeature::ZoomSDKAICompanionFeature_SmartSummary \endlink , \link @c ZoomSDKAICompanionFeature::ZoomSDKAICompanionFeature_SmartRecording \endlink can be requested to turn off at once.
 */
- (BOOL)canRequestTurnoffAllAICompanion;

/**
 * @brief Request host to turn off all started AI Companion features.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success. Otherwise failed.
 * @note All AI features include \link @c ZoomSDKAICompanionFeature::ZoomSDKAICompanionFeature_Query \endlink , \link @c ZoomSDKAICompanionFeature::ZoomSDKAICompanionFeature_SmartSummary \endlink , \link @c ZoomSDKAICompanionFeature::ZoomSDKAICompanionFeature_SmartRecording \endlink can be turned off at once.
 */
- (ZoomSDKError)requestTurnoffAllAICompanion;

/**
 * @brief Whether the current user can ask the host to turn on all AI Companion features if they are enabled for the current meeting.
 * @return YES means the user can ask the host to turn on the AI Companion features.
 * @note Only \link @c ZoomSDKAICompanionFeature::ZoomSDKAICompanionFeature_Query \endlink , \link @c ZoomSDKAICompanionFeature::ZoomSDKAICompanionFeature_SmartSummary \endlink can be turned on at once.
 */
- (BOOL)canRequestTurnOnAllAICompanion;

/**
 * @brief Ask the host to turn on all AI Companion features if they are enabled for the current meeting.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success. Otherwise failed.
 * @note Only \link @c ZoomSDKAICompanionFeature:ZoomSDKAICompanionFeature#ZoomSDKAICompanionFeature_Query \endlink , \link @c ZoomSDKAICompanionFeature::ZoomSDKAICompanionFeature_SmartSummary \endlink can be turned on at once.
 */
- (ZoomSDKError)requestTurnOnAllAICompanion;
@end

NS_ASSUME_NONNULL_END
