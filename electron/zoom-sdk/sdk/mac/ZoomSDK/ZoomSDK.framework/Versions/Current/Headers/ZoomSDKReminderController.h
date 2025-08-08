/**
 * @file ZoomSDKReminderController.h
 * @brief Interfaces for managing meeting reminder notifications in Zoom SDK.
 */


#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN
/**
 * @class ZoomSDKReminderContent
 * @brief Represents the content of a reminder dialog.
 */
@interface ZoomSDKReminderContent : NSObject
/**
 * @brief Get the reminder’s type.
 */
@property(nonatomic,assign,readonly)ZoomSDKReminderType type;
/**
 * @brief Get the reminder dialog content’s title.
 */
@property(nonatomic,copy,readonly,nullable)NSString *title;
/**
 * @brief Get the detail content of the reminder dialog content.
 */
@property(nonatomic,copy,readonly,nullable)NSString *content;
/**
 * @brief Whether to block the user from joining or staying in the meeting.
 */
@property(nonatomic,assign,readonly)BOOL isBlocking;
/**
 * @brief Get the type of the action which user should take after receiving this reminder content.
 */
@property(nonatomic,assign,readonly)ZoomSDKReminderActionType actionType;
/**
 * @brief Get a list of reminder’s type.
 */
@property(nonatomic,copy,readonly)NSArray *multiReminderTypes;
@end


/**
 * @class ZoomSDKReminderHandler
 * @brief Provides the interface to respond to a reminder dialog.
 */
@interface ZoomSDKReminderHandler : NSObject
/**
 * @brief Accept the reminder.
 * @return If the function succeeds, it will return ZoomSDKError_success, otherwise not.
 */
- (ZoomSDKError)accept;
/**
 * @brief Declined the reminder.
 * @return If the function succeeds, it will return ZoomSDKError_success, otherwise not.
 */
- (ZoomSDKError)decline;
/**
 * @brief Ignore the reminder.
 * @return If the function succeeds, it will return ZoomSDKError_success, otherwise not.
 */
- (ZoomSDKError)ignore;
/**
 * @brief Set not show the disclaimer in subsequent meetings.
 * @return If the function succeeds, it will return ZoomSDKError_success, otherwise not.
 */
- (ZoomSDKError)setHideFeatureDisclaimers;

/**
 * @brief Is need explicit consent for AI custom disclaimer. Only valid for  @c ZoomSDKReminderType_CustomAICompanionDisclaimer.
 * @return YES means explicit consent is required. Before agreeing to AIC disclaimer, the user's video and audio will be blocked. NO means explicit consent is not required and video and audio will not be blocked.
 */
- (BOOL)isNeedExplicitConsent4AICustomDisclaimer;
@end


/**
 * @class ZoomSDKMeetingEnableReminderHandler
 * @brief Handles reminders related to enabling smart features (e.g., smart recording).
 */
@interface ZoomSDKMeetingEnableReminderHandler : NSObject

/**
 * @brief Set the option indicating which meetings smart recording will be enabled for.
 * @param option Specify the option.
 * @return If the function succeeds, it will return ZoomSDKError_success, otherwise not.
 */
- (ZoomSDKError)setEnableOption:(ZoomSDKMeetingFeatureEnableOption)option;

/**
 * @brief Start the smart recording.
 * @return If the function succeeds, it will return ZoomSDKError_success, otherwise not.
 */
- (ZoomSDKError)start;

/**
 * @brief Decline the reminder.
 * @param declineAll YES means decline all reminders,and participants cannot send requests again until the host change the setting. NO means that the host declines only this specific request , not all requests.
 * @return If the function succeeds, it will return ZoomSDKError_success, otherwise not.
 */
- (ZoomSDKError)decline:(BOOL)declineAll;

/**
 * @brief Ignore the reminder.
 * @return If the function succeeds, it will return ZoomSDKError_success, otherwise not.
 */
- (ZoomSDKError)ignore;

@end


/**
 * @protocol ZoomSDKReminderControllerDelegate
 * @brief Callback interface for receiving reminder dialog notifications.
 */
@protocol ZoomSDKReminderControllerDelegate <NSObject>
/**
 * @brief Callback event when the reminder dialog displays.
 * @param handle The helper to handle the reminder dialog.
 * @param content The detail content in the reminder dialog.
 */
- (void)onReminderNotify:(ZoomSDKReminderHandler*)handle reminderContent:(ZoomSDKReminderContent*)content;

/**
 * @brief Callback event to enable showing the reminder dialog.
 * @param handle A pointer to the ZoomSDKMeetingEnableReminderHandler.
 * @param content The detail content in the reminder dialog.
 */
- (void)onEnableReminderNotify:(ZoomSDKMeetingEnableReminderHandler*)handle reminderContent:(ZoomSDKReminderContent*)content;

@end


/**
 * @class ZoomSDKCustomMultiReminderUIConfigItem
 * @brief UI configuration for a single type of reminder popup.
 */
@interface ZoomSDKCustomMultiReminderUIConfigItem : NSObject
/**
 * @brief Set the customized center point position for multiple reminder pop-ups. You can modify the alert's position by calling setMultiReminderDisclaimerUIConfig when the alert is displayed. If the center you set is not enough for display, it will be automatically corrected.
 */
@property(nonatomic,assign) NSPoint center;

/**
 * @brief Set the customized backgroundColor for multiple reminder pop-ups.
 */
@property(nonatomic,retain, nullable) NSColor *backgroundColor;
@end


/**
 * @class ZoomSDKCustomMultiReminderUIConfig
 * @brief Configuration for customizing multiple reminder popups.
 */
@interface ZoomSDKCustomMultiReminderUIConfig : NSObject
/**
 * @brief Set the customized config for multiple reminder alert pop-ups.
 */
@property(nonatomic,retain, nullable) ZoomSDKCustomMultiReminderUIConfigItem *alertConfig;

/**
 * @brief Set the customized config for multiple reminder banner pop-ups.
 */
@property(nonatomic,retain ,nullable) ZoomSDKCustomMultiReminderUIConfigItem *bannerConfig;
@end

@interface ZoomSDKReminderController : NSObject
/**
 * @brief Delegate for receiving reminder-related callbacks.
 */
@property(nonatomic,assign, nullable) id<ZoomSDKReminderControllerDelegate> delegate;

/**
 * @brief Set the customized config for multiple reminder pop-ups.
 * @param config The config object.
 * @return If the function succeeds, the return value is ZoomSDKError_Success, Otherwise  the function fails.
 */
- (ZoomSDKError)setMultiReminderDisclaimerUIConfig:(ZoomSDKCustomMultiReminderUIConfig *_Nullable)config;
@end
NS_ASSUME_NONNULL_END
