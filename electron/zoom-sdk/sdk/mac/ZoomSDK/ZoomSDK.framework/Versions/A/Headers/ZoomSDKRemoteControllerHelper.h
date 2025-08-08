/**
 * @file ZoomSDKRemoteController.h
 * @brief Interface for Zoom SDK remote control feature, including delegate callbacks and control methods.
 */

#import <Foundation/Foundation.h>
#import <ZoomSDK/ZoomSDKErrors.h>
NS_ASSUME_NONNULL_BEGIN

/**
 * @protocol ZoomSDKRemoteControllerDelegate
 * @brief The delegate protocol to receive remote control related callbacks.
 */
@protocol ZoomSDKRemoteControllerDelegate <NSObject>
/**
 * @brief Notification of remote control status.
 * @param status The remote control's status.
 * @param userID The ID of user who can control others remotely.
 * @param shareSourceID The source ID of user who is sharing.
 */
- (void)onRemoteControlStatus:(ZoomSDKRemoteControlStatus)status User:(unsigned int)userID shareSourceID:(unsigned int)shareSourceID;

/**
 * @brief Callback event when the request of remote control is forbidden for guest user.
 * @param userId The remote control user ID.
 */
- (void)onUserRemoteControlForbidden:(unsigned int)userId;
@end


/**
 * @class ZoomSDKRemoteControllerHelper
 * @brief Helper class to manage remote control operations in Zoom meeting.
 */
@interface ZoomSDKRemoteControllerHelper : NSObject
{
    id<ZoomSDKRemoteControllerDelegate> _delegate;
    
}
/**
 * @brief The delegate to receive remote control status updates.
 */
@property(nonatomic, assign, nullable) id<ZoomSDKRemoteControllerDelegate> delegate;
/**
 * @brief Determine if the current user to request to control others remotely.
 * @param shareSourceID The source ID of the user to be controlled remotely.
 * @return If the function succeeds, it will return the @c ZoomSDKError_Success, otherwise failed.
 * @note For custom style mode, this interface is only valid after subscribing the sharing content from the specified user by successfully calling "- (ZoomSDKError)createShareElement:(ZoomSDKShareElement**)element" and set the share element userid.
 */
- (ZoomSDKError)canRequestRemoteControl:(unsigned int)shareSourceID;

/**
 * @brief Determine if user has privilege to control the specified user remotely.
 * @param shareSourceID The source ID of user to be checked.
 * @return If the function succeeds, it returns ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)haveRemoteControlPrivilege:(unsigned int)shareSourceID;

/**
 * @brief Determine if the current user is controlling the specified user remotely.
 * @param shareSourceID The source ID of user to be checked.
 * @return If the function succeeds, it returns ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)isInRemoteControlling:(unsigned int)shareSourceID;

/**
 * @brief Start remotely controlling the specified user's computer.
 * @param shareSourceID The source ID of user to be controlled remotely.
 * @return If the function succeeds, it returns ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)startRemoteControl:(unsigned int)shareSourceID;

/**
 * @brief Stop controlling remotely.
 * @param shareSourceID The source ID of user who is controlled remotely by the current user.
 * @return If the function succeeds, it returns ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)stopRemoteControl:(unsigned int)shareSourceID;

/**
 * @brief Request remotely control the specified user.
 * @param shareSourceID The source ID of user to be controlled.
 * @return If the function succeeds, it returns ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)requestRemoteControl:(unsigned int)shareSourceID;

/**
 * @brief Give up remotely controlling the specified user.
 * @param shareSourceID The source ID of user that was remotely controlled.
 * @return If the function succeeds, it returns ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)giveUpRemoteControl:(unsigned int)shareSourceID;

/**
 * @brief Give the remote control privilege to the specified user.
 * @param userid The ID of user that you ask to control yourself remotely.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)giveRemoteControlPrivilegeTo:(unsigned int)userid;

/**
 * @brief Refuse the request to remote control from the specified user.
 * @param userid The ID of demander.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)declineRemoteControlRequest:(unsigned int)userid;

/**
 * @brief Get back the authority of remote control.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)revokeRemoteControl;

/**
 * @brief Get the identity of current controller. 
 * @param userid The pointer to unsigned int. If the function calls successfully, it will return the user id of current remote controller.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)getCurrentRemoteController:(unsigned int*)userid;

/**
 * @brief Send remote control action. 
 * @param theEvent The mouse or keyboard event.
 * @param shareView The view that you want to control remotely.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)sendRemoteControlEvent:(NSEvent *)theEvent ShareView:(NSView*)shareView;

/**
 * @brief Determine if the auto accept remote control requests feature is enabled in the meeting.
 * @return YES means that the feature enabled, NO means that the feature isn't enabled.
 */
- (BOOL)IsAutoAcceptRemoteControlFeatureEnabled;

/**
 * @brief Whether the current user can enable the auto accept remote control requests feature.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)canEnableAutoAcceptRemoteControlFeature;

/**
 * @brief Enable or disable auto accept remote control requests feature.
 * @param enable YES means that auto accept remote control requests are enabled, NO means disabled.
 * @return If the function succeeds, it will return the @c ZoomSDKError_Success, otherwise failed.
 * @note Before using this interface, it is necessary to give the app accessibility permissions in the System Settings.
 */
- (ZoomSDKError)enableAutoAcceptRemoteControlFeature:(BOOL)enable;
@end
NS_ASSUME_NONNULL_END
