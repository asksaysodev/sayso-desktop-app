/**
 * @file ZoomSDKCustomImmersiveController.h
 * @brief Interfaces for managing Zoom immersive view, layout templates, and user seat assignment.
 */

#import <Foundation/Foundation.h>
#import "ZoomSDKErrors.h"

NS_ASSUME_NONNULL_BEGIN
/**
 * @class ZoomSDKCustomImmersiveContainer
 * @brief Represents the immersive container used in custom UI mode.
 */
@interface ZoomSDKCustomImmersiveContainer : NSObject
/**
 * @brief The container view to render the immersive UI.
 */
@property(nonatomic, assign, readonly) NSView* containerView;

/**
 * @brief Resize the immersive container in the specified area and reset the parent window.
 * @param rect a new display area. The coordinate value of the structure is that of the parent window of immersive container.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)repositionContainer:(NSRect)rect;

/**
 * @brief Show the waiting area before start immersive.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)showWaitingArea;

/**
 * @brief Hide the waiting area before start immersive.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)hideWaitingArea;
@end

/**
 * @brief Enumeration of immersive template.
 */
typedef enum
{
    /** The default template. */
    ZoomSDKCustomImmersiveTemplateType_Default,
    /** Template using a custom image. */
    ZoomSDKCustomImmersiveTemplateType_CustomImage,
    /** Template showing user's own video. */
    ZoomSDKCustomImmersiveTemplateType_MyVideo
}ZoomSDKCustomImmersiveTemplateType;


/**
 * @class ZoomSDKSeatPlacementInfo
 * @brief Defines a seat's ID and its position within a template.
 */
@interface ZoomSDKSeatPlacementInfo : NSObject
/**
 * @brief The seat ID.
 */
@property(nonatomic, copy, readonly, nullable) NSString* seatID;
/**
 * @brief The position of seat.
 */
@property(nonatomic, assign, readonly) NSRect position;
@end


/**
 * @class ZoomSDKCustomImmersiveLayoutData
 * @brief Layout data for immersive view seat assignment.
 */
@interface ZoomSDKCustomImmersiveLayoutData : NSObject
/**
 * @brief Whether this user is in a seat.
 */
@property(nonatomic,assign) BOOL isSeatFree;
/**
 * @brief The seat ID.
 */
@property(nonatomic,copy, nullable) NSString* seatID;
/**
 * @brief The user ID.
 */
@property(nonatomic,assign) unsigned int userID;
/**
 * @brief The seat z order.
 */
@property(nonatomic,assign, readonly) unsigned int zOrder;
/**
 * @brief The seat position.
 */
@property(nonatomic,assign) NSRect position;
@end


/**
 * @class ZoomSDKCustomImmersiveTemplateData
 * @brief Represents seat layout and canvas size for a template.
 */
@interface ZoomSDKCustomImmersiveTemplateData : NSObject
/**
 * @brief The list of template seats.
 */
@property(nonatomic,retain, readonly, nullable) NSArray<ZoomSDKSeatPlacementInfo*>* seatList;
/**
 * @brief The size of this template.
 */
@property(nonatomic,assign, readonly) NSSize canvasSize;
@end



/**
 * @class ZoomSDKCustomImmersiveTemplate
 * @brief Represents an immersive template used in a meeting.
 */
@interface ZoomSDKCustomImmersiveTemplate : NSObject
/**
 * @brief The name of template.
 */
@property(nonatomic,copy, readonly, nullable) NSString* templateName;
/**
 * @brief The bitmap of thumbnail.
 */
@property(nonatomic,retain, readonly, nullable) NSBitmapImageRep* thumbnailBitmap;
/**
 * @brief The capacity of template.
 */
@property(nonatomic,assign, readonly) unsigned int capacity;
/**
 * @brief Determine if the template is ready.
 */
@property(nonatomic,assign, readonly) BOOL isTemplateReady;
/**
 * @brief Determine if the template support free seat.
 */
@property(nonatomic,assign, readonly) BOOL isSupportFreeSeat;
/**
 * @brief The type of this template.
 */
@property(nonatomic,assign, readonly) ZoomSDKCustomImmersiveTemplateType templateType;
/**
 * @brief The template data of immersive when in share mode.
 */
@property(nonatomic,retain, readonly, nullable) ZoomSDKCustomImmersiveTemplateData* shareTemplateData;

/**
 * @brief The template data of immersive when in default mode.
 */
@property(nonatomic,retain, readonly, nullable) ZoomSDKCustomImmersiveTemplateData* videoTemplateData;
@end


/**
 * @protocol ZoomSDKCustomImmersiveDelegate
 * @brief Callback events for immersive view operations.
 */
@protocol ZoomSDKCustomImmersiveDelegate <NSObject>
/**
 * @brief Callback event that the immersive view was enabled/disabled.
 * @param isOn The status of immersive status.
 */
- (void)onImmersiveStatusChanged:(BOOL)isOn;

/**
 * @brief Callback event that the selected immersive template changed.
 * @param immersiveTemplate  The new template.
 */
- (void)onSelectedImmersiveTemplateChanged:(ZoomSDKCustomImmersiveTemplate*)immersiveTemplate;

/**
 * @brief Callback event that the immersive seat layout changed.
 * @param data The list contains the seat layout of immersive.
 */
- (void)onImmersiveSeatLayoutUpdated:(NSArray<ZoomSDKCustomImmersiveLayoutData*>*)data;

/**
 * @brief Callback event for the immersive template download process.
 * @param immersiveTemplate  The new template.
 * @param progress The process.
 */
- (void)onTemplateDownloadProgress:(ZoomSDKCustomImmersiveTemplate*)immersiveTemplate progress:(unsigned int)progress;

/**
 * @brief Callback event for the immersive template download end.
 * @param immersiveTemplate The new template.
 * @param bSuccess The download result.
 */
- (void)onTemplateDownloadEnded:(ZoomSDKCustomImmersiveTemplate*)immersiveTemplate bSuccess:(BOOL)bSuccess;

/**
 * @brief Callback event that template thumbnails download end.
 * @param bSuccess YES means download success. Otherwise not.
 */
- (void)onTemplateThumbnailsDownloadEnded:(BOOL)bSuccess;

/**
 * @brief The callback will be triggered before the immersive container is destroyed.
 * @note The immersive container will be destroyed once the function calls end. The user should complete the operations related to the immersive container before the function calls end.
 */
- (void)onImmersiveContainerDestroyed;
@end


/**
 * @class ZoomSDKCustomImmersiveController
 * @brief Interface for controlling the meeting immersive view feature. For more information on this feature, see <https://support.zoom.us/hc/en-us/articles/360060220511-Immersive-View>.
 */
@interface ZoomSDKCustomImmersiveController : NSObject
/**
 * @brief Delegate to receive callbacks related to immersive view events.
 */
@property(nonatomic, assign, nullable) id<ZoomSDKCustomImmersiveDelegate> delegate;

/**
 * @brief Determine if immersive view is active.
 * @return YES means the immersive view is active. Otherwise NO.
 */
- (BOOL)isImmersiveViewOn;

/**
 * @brief Determine if displaying sharing contents in immersive mode.
 * @param bInShare YES means displaying sharing contents in immersive mode. Otherwise NO.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)isInImmersiveShareMode:(BOOL*)bInShare;

/**
 * @brief Update the share source ID to view share, available only for host.
 * @param shareSourceID The sepecified source ID.
 * @return If the function succeeds, it returns ZoomSDKError_Success. Otherwise fails.
 */
- (ZoomSDKError)viewShare:(unsigned int)shareSourceID;

/**
 * @brief Query the share source ID when viewing share in immersive mode. Only available for host.
 * @param shareSourceID The specified source ID.
 * @return If the function succeeds, it returns ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)getViewingShareSourceID:(unsigned int*)shareSourceID;

/**
 * @brief Determine if immersive is supported.
 * @return YES means support immersive. Otherwise NO.
 */
- (BOOL)isSupportImmersive;

/**
 * @brief Determine if the thumbnails are ready.
 * @return YES means the immersive thumbnails are ready, NO not.
 */
- (BOOL)isTemplateThumbnailsReady;

/**
 * @brief Determine if the immersive template can be started.
 * @param immersiveTemplate The selected template in immersive view.
 * @return YES means the immersive can be started, otherwise not.
 */
- (BOOL)canStartImmersiveView:(ZoomSDKCustomImmersiveTemplate*)immersiveTemplate;

/**
 * @brief Start immersive view.
 * @param immersiveTemplate The selected template in immersive view.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)startImmersiveView:(ZoomSDKCustomImmersiveTemplate*)immersiveTemplate;

/**
 * @brief Change template in immersive view.
 * @param immersiveTemplate The selected template in immersive view.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)changeTemplate:(ZoomSDKCustomImmersiveTemplate*)immersiveTemplate;

/**
 * @brief Exit immersive view.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)endImmersiveView;

/**
 * @brief Determine if the user can be shown in immersive view.
 * @param userID The user ID.
 * @return YES means the user can be shown in immersive view, NO if they cannot.
 */
- (BOOL)canUserShowInImmersiveView:(unsigned int)userID;

/**
 * @brief Get the immersive seat layout data.
 * @return If the function succeeds, it will return the immersive seat layout data. Otherwise the function returns nil.
 */
- (NSArray<ZoomSDKCustomImmersiveLayoutData*>*_Nullable)getLayoutData;

/**
 * @brief Update the layout data.
 * @param @param dataArray An array of ZoomSDKCustomImmersiveLayoutData objects representing the new layout data.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise the function returns an error.
 */
- (ZoomSDKError)updateLayoutData:(NSArray<ZoomSDKCustomImmersiveLayoutData*>*)dataArray;

/**
 * @brief Put the user in the seat.
 * @param userID The user ID.
 * @param seatID The seat ID.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)assignUser:(unsigned int)userID seatID:(NSString*)seatID;

/**
 * @brief Put the user in the free seat.
 * @param userID The user ID.
 * @param pos The position.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)putUserToFreeSeat:(unsigned int)userID pos:(NSRect)pos;

/**
 * @brief Remove user from immersive view.
 * @param userID The user ID.
 * @return If the function succeeds, the return value is ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)removeUser:(unsigned int)userID;

/**
 * @brief Determine if the user is in immersive view.
 * @param userID The user ID.
 * @return YES means the user is in immersive view, NO means that they are not.
 */
- (BOOL)isUserInImmersiveView:(unsigned int)userID;

/**
 * @brief Download the template thumbnails. See \link ZoomSDKCustomImmersiveDelegate \endlink for updates on the download.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)downloadTemplateThumbnails;

/**
 * @brief Download complete template resource.
 * @param immersiveTemplate The template be downloaded.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)downloadTemplate:(ZoomSDKCustomImmersiveTemplate*)immersiveTemplate;

/**
 * @brief Add a template based on a custom image.
 * @param filePath The image file path.
 * @param immersiveTemplate If the function succeeds, the object will be ZoomSDKCustomImmersiveTemplate, otherwise nil.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)addCustomImageTemplate:(NSString*)filePath template:(ZoomSDKCustomImmersiveTemplate*_Nullable*_Nonnull)immersiveTemplate;

/**
 * @brief Remove custom immersive template.
 * @param immersiveTemplate The custom image template that want to remove.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)removeCustomImageTemplate:(ZoomSDKCustomImmersiveTemplate*)immersiveTemplate;

/**
 * @brief Get the current template.
 * @return If the function succeeds, the return value is the current template.
 */
- (ZoomSDKCustomImmersiveTemplate*_Nullable)getCurrentTemplate;

/**
 * @brief Get the list of templates.
 * @return The list of templates. ZERO(0) indicates that there are no templates.
 */
- (NSArray<ZoomSDKCustomImmersiveTemplate*>*_Nullable)getTemplates;

/**
 * @brief Create the immersive container for customUI
 * @param rect Specify the position of client area of the immersive container.
 * @return If the function succeeds, it will return the object of ZoomSDKCustomImmersiveContainer. Otherwise nil.
 */
- (ZoomSDKCustomImmersiveContainer*_Nullable)createImmersiveContainer:(NSRect)rect;

/**
 * @brief Destroy the immersive container. Once destroyed, the container can no longer be used.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)destroyImmersiveContainer;
@end
NS_ASSUME_NONNULL_END
