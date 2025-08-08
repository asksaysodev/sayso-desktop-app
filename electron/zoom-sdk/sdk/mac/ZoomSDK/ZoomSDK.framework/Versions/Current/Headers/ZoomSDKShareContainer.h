/**
 * @file ZoomSDKShareContainer.h
 * @brief Interfaces for customized UI share rendering in Zoom SDK.
 */

#import <ZoomSDK/ZoomSDKErrors.h>

NS_ASSUME_NONNULL_BEGIN
/**
 * @protocol ZoomSDKShareElementDelegate
 * @brief The delegate protocol to handle share element related events.
 */
@protocol ZoomSDKShareElementDelegate <NSObject>
/**
 * @brief Callback event of sharer sending data.
 */
- (void)onShareContentStartReceiving;
/**
 * @brief Callback of the user share source ID changing when sharing.
 * @param sharingID The new sharer's share ID.
 */
- (void)onShareSourceNotify:(unsigned int)sharingID;

@end


/**
 * @class ZoomSDKShareElement
 * @brief The share element object to handle individual shared content and render view.
 */
@interface ZoomSDKShareElement : NSObject
{
    unsigned int         _sharingID;
    ViewShareMode        _viewMode;
    NSView*              _shareView;
    id<ZoomSDKShareElementDelegate>   _delegate;
    NSRect                            _frame;
}
/**
 * @brief The unique ID of the sharing source.
 */
@property(nonatomic, assign) unsigned int sharingID;
/**
 * @brief The view mode of the shared content.
 */
@property(nonatomic, assign) ViewShareMode viewMode;
/**
 * @brief The view used to render the shared content.
 */
@property(nonatomic, assign, nullable) NSView*  shareView;
/**
 * @brief Delegate for share element callbacks.
 */
@property(nonatomic, assign, nullable) id<ZoomSDKShareElementDelegate> delegate;
/**
 * @brief Create a sharing element.
 * @param frame Frame of sharing view owned by the element.
 */
- (id)initWithFrame:(NSRect)frame;
/**
 * @brief Resize the frame of the shared view owned by this element
 * @param frame The coordinates of _shareview.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)resize:(NSRect)frame;
/**
 * @brief Set whether to show the share view or not.
 * @param show YES means to show, NO to hide.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)ShowShareRender:(BOOL)show;
@end


/**
 * @protocol ZoomSDKShareContainerDelegate
 * @brief The delegate protocol to receive notifications from share container.
 */
@protocol ZoomSDKShareContainerDelegate <NSObject>
/**
 * @brief Callback of that element is destroyed. 
 * @param element The object of ZoomSDKShareElement.
 */
- (void)onShareElementDestroy:(ZoomSDKShareElement*)element;
@end


/**
 * @class ZoomSDKShareContainer
 * @brief Container to manage all share elements.
 */
@interface ZoomSDKShareContainer : NSObject
{
    NSMutableArray*                          _elementArray;
    id<ZoomSDKShareContainerDelegate>        _delegate;
}
/**
 * @brief Delegate for container-level share element callbacks.
 */
@property(nonatomic, assign, nullable)id<ZoomSDKShareContainerDelegate>  delegate;
/**
 * @brief Create shared elements.
 * @param element The pointer to ZoomSDKShareElement object.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)createShareElement:(ZoomSDKShareElement*_Nullable*_Nonnull)element;
/**
 * @brief Clean shared elements.
 * @param element The pointer to ZoomSDKShareElement object.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)cleanShareElement:(ZoomSDKShareElement*)element;
/**
 * @brief Get an array of shared elements.
 * @return If the function succeeds, it will return a NSArray containing all sharing elements.
 */ 
- (NSArray*_Nullable)getShareElementArray;
/**
 * @brief Get an array of shared elements by user ID.
 * @param shareSourceID The specified source ID.
 * @return If the function succeeds, it returns an object of ZoomSDKShareElement.
 */
- (ZoomSDKShareElement*_Nullable)getShareElementBySourceID:(unsigned int)shareSourceID;

@end


/**
 * @protocol ZoomSDKShareActionDelegate
 * @brief The delegate protocol to receive events related to share actions.
 */
@protocol ZoomSDKShareActionDelegate <NSObject>
@optional
/**
 * @brief Callback event the moment the user receives the shared content.
 */
- (void)onSharingContentStartReceiving;
/**
 * @brief The callback is triggered before the shared action is released.
 * @param sharingID Specify the sharing ID.
 * @note The specified shared action is destroyed once the function calls end. The user should complete the operations related to the shared action before the function calls end.
 */
- (void)onActionBeforeDestroyed:(unsigned int)sharingID;
@end


/**
 * @class ZoomSDKShareAction
 * @brief Representing a share action, including subscription and rendering controls.
 */
@interface ZoomSDKShareAction : NSObject
/**
 * @brief Delegate for share action events.
 */
@property(nonatomic,assign, nullable)id<ZoomSDKShareActionDelegate>  delegate;
/**
 * @brief Unique ID of the sharing action.
 */
@property(nonatomic, readonly, assign) unsigned int sharingID;
/**
 * @brief Display name of the user sharing the content.
 */
@property(nonatomic, readonly, copy, nullable) NSString *sharingUserName;
/**
 * @brief View for rendering the share content.
 */
@property(nonatomic, readonly, assign, nullable) NSView*  shareView;
/**
 * @brief The view mode for displaying the share content.
 */
@property(nonatomic, readwrite, assign) ViewShareMode viewMode;

/**
 * @brief Subscribe to the sharing content.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)subscribe;

/**
 * @brief Unsubscribe from the sharing content.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)unsubscribe;

/**
 * @brief Resize the share render frame.
 * @param frame Specify the frame.
 * @return If the function succeeds,the return value is ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)resize:(NSRect)frame;
@end
NS_ASSUME_NONNULL_END
