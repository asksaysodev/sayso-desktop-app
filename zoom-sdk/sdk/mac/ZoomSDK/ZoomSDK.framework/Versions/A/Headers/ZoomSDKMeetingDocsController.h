/**
 * @file ZoomSDKMeetingDocsController.h
 * @brief Defines interfaces for controlling and managing Zoom Docs sharing in meetings.
 *
 * This file declares the ZoomSDKMeetingDocsController class and related delegate protocol
 * for managing Zoom Docs sharing, permissions, and view control during a meeting.
 * It also defines the ZoomSDKDocsSharingSourceInfo class, which provides information about
 * docs sharing sources.
 */

#import <Foundation/Foundation.h>
#import <ZoomSDK/ZoomSDKErrors.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * @class ZoomSDKDocsSharingSourceInfo
 * @brief Zoom Docs sharing source information class.
 */
@interface ZoomSDKDocsSharingSourceInfo: NSObject
/**
 * @brief Get the share source ID of who is sharing Zoom docs.
 */
@property(nonatomic, assign, readonly) unsigned int shareSourceID;
/**
 * @brief Get the user ID of who is sharing Zoom docs.
 */
@property(nonatomic, assign, readonly) unsigned int userID;
/**
 * @brief Get the status of Zoom docs sharing.
 */
@property(nonatomic, assign, readonly) ZoomSDKDocsStatus status;
/**
 * @brief Get the title of the shared Zoom docs.
 */
@property(nonatomic, copy, readonly) NSString *docTitle;
@end



/**
 * @protocol ZoomSDKMeetingDocsControllerDelegate
 * @brief Callback event for Zoom Docs sharing events.
 */
@protocol ZoomSDKMeetingDocsControllerDelegate <NSObject>
/**
 * @brief Docs status changed callback. Use this function to inform that the user docs status changed, and all users in the meeting can get the event.
 * @param shareSourceInfo Share source info of Zoom docs.
 */
- (void)onDocsSharingSourceInfoChanged:(ZoomSDKDocsSharingSourceInfo *)shareSourceInfo;

/**
 * @brief Callback event of docs share setting type changed.
 * @param createOption Docs who can create new docs type.
 * @param shareOption Docs who can share type.
 */
- (void)onDocsPermissionChanged:(ZoomSDKDocsCreateOption)createOption shareOption:(ZoomSDKDocsShareOption)shareOption;
@end


/**
 * @class ZoomSDKMeetingDocsController
 * @brief Zoom SDK meeting docs sharing controller.
 */
@interface ZoomSDKMeetingDocsController : NSObject
/**
 * @brief The delegate object to receive docs sharing events.
 */
@property(nonatomic,assign,nullable) id<ZoomSDKMeetingDocsControllerDelegate> delegate;

/**
 * @brief Determine whether the current meeting supports docs or not.
 * @return YES indicates that the current meeting supports docs.
 */
- (BOOL)isSupportDocs;

/**
 * @brief Determine whether the current user can start sharing the docs or not.
 * @param reason The reason why no one can start sharing the docs.
 * @return YES indicates you can start sharing the docs.
 */
- (BOOL)canStartShareDocs:(ZoomSDKCannotShareReasonType*)reason;

/**
 * @brief Determine whether the current user can set docs options or not.
 * @return YES means the current user can set docs options, NO means the user can' not set docs options.
 */
- (BOOL)canSetDocsOption;

/**
 * @brief Query if other user is sharing docs.
 * @return YES means sharing.
 */
- (BOOL)isOtherSharingDocs;

/**
 * @brief Query if the current user is sharing docs successfully.
 * @return YES means sharing successfully.
 */
- (BOOL)isSharingDocsOut;

/**
 * @brief Set the docs'setting option for who can share docs.
 * @param option The setting for who can share docs.
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)setDocsShareOption:(ZoomSDKDocsShareOption)option;

/**
 * @brief Get the setting option indicating who can share documents.
 * @return The docs share option.
 */
- (ZoomSDKDocsShareOption)getDocsShareOption;

/**
 * @brief Set the setting option for who can initiate new docs.
 * @param option Docs setting option for who can initiate new docs.
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)setDocsCreateOption:(ZoomSDKDocsCreateOption)option;

/**
 * @brief Get the setting option indicating whether the user can initiate new docs.
 * @return The docs creation option.
 */
- (ZoomSDKDocsCreateOption)getDocsCreateOption;

/**
 * @brief Show the dashboardView web view window.
 * @param point The original point to display dashboard web view window. Default is (0,0).
 * @return If the function succeeds, it returns @c ZoomSDKError_Success. Otherwise the function fails.
 * @note Only available for the custom UI.
 */
- (ZoomSDKError)showDashboardWindow:(NSPoint)point;

/**
 * @brief Set the docs web view window position.
 * @param point The original point to display docs web view window. Default is (0,0).
 * @return If the function succeeds, it returns @c ZoomSDKError_Success. Otherwise the function fails.
 * @note Only available for the custom UI.
 */
- (ZoomSDKError)setDocsWindowPos:(NSPoint)point;

/**
 * @brief Set the docs web view window size.
 * @param size The size of display docs web view window. If the size is (0,0), the window size will be default size.
 * @return If the function succeeds, it returns @c ZoomSDKError_Success. Otherwise the function fails.
 * @note Only available for the custom UI.
 */
- (ZoomSDKError)setDocsWindowSize:(NSSize)size;

/**
 * @brief Show the docs web view window.
 * @param shareSourceID The share source ID that is sharing.
 * @return If the function succeeds, it returns @c ZoomSDKError_Success. Otherwise, the function fails.
 * @note Only available for the custom UI.
 */
- (ZoomSDKError)showDocsWindow:(unsigned int)shareSourceID;

/**
 * @brief Close the docs web view window.
 * @return If the function succeeds, it returns @c ZoomSDKError_Success. Otherwise, the function fails.
 * @note Only available for the custom UI.
 */
- (ZoomSDKError)closeDocsWindow;

/**
 * @brief Get the ID of users who are sharing docs.
 * @return A list of userID of all users who are sharing docs.
 */
- (NSArray<NSNumber *>* _Nullable)getViewableSharingUserList;

/**
 * @brief Get the list of docs sharing source info.
 * @param userID The ID of the user who is sharing docs.
 * @return If the function succeeds, the return value is a pointer to the NSArray<ZoomSDKDocsSharingSourceInfo*>. Otherwise, the return value is nil.
 */
- (NSArray<ZoomSDKDocsSharingSourceInfo *>* _Nullable)getSharingSourceInfoList:(unsigned int)userID;
@end

NS_ASSUME_NONNULL_END
