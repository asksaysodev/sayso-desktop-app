/**
 * @file ZoomSDKVideoContainer.h
 * @brief Interfaces for managing video render elements for customized UI in Zoom SDK.
 */


#import <Foundation/Foundation.h>
#import <ZoomSDK/ZoomSDKErrors.h>
NS_ASSUME_NONNULL_BEGIN

@class  ZoomSDKVideoContainer;

/**
 * @class ZoomSDKVideoElement
 * @brief Interface for video element rendering in custom UI.
 * @note This class is available only for custom UI.
 */
@interface ZoomSDKVideoElement : NSObject
{
    VideoRenderElementType _elementType;
    VideoRenderDataType    _dataType;
    unsigned int           _userid;
    NSView*                _videoView;
    NSRect                 _viewFrame;
}
/**
 * @brief The user ID for this video element.
 */
@property(nonatomic, assign)unsigned int userid;
/**
 * @brief The view used to render video.
 */
@property(nonatomic, retain, nullable)NSView*  videoView;
/**
 * @brief Create object of video elements for each user.
 * @param rect Frame of video view.
 */
- (id)initWithFrame:(NSRect)rect;
/**
 * @brief Set whether to show video.
 * @param show YES means displaying video, otherwise not.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)showVideo:(BOOL)show;
/**
 * @brief Get the type of the video render element: preview/active/normal.
 * @return The type of the video render element.
 */
- (VideoRenderElementType)getElementType;
/**
 * @brief Get data type of video render: avatar/video.
 * @return The data type of the video render.
 */
- (VideoRenderDataType)getDataType;
/**
 * @brief Get NSView object in the element.  
 * @return The point of the video view. 
 */
- (NSView*)getVideoView;
/**
 * @brief Resize the video view according to your requirements.
 * @param frame Custom frame of video view.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)resize:(NSRect)frame;

/**
 * @brief Config the video view resolution.
 * @param resolution Custom resolution of video view.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)setResolution:(ZoomSDKVideoRenderResolution)resolution;
@end


/**
 * @class ZoomSDKPreViewVideoElement
 * @brief Subclass of ZoomSDKVideoElement for preview video.
 */
@interface ZoomSDKPreViewVideoElement : ZoomSDKVideoElement
/**
 * @brief Set whether to preview video.
 * @param start YES means starting preview, otherwise not.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)startPreview:(BOOL)start;
@end


/**
 * @class ZoomSDKActiveVideoElement
 * @brief Subclass of ZoomSDKVideoElement for active speaker video.
 */
@interface ZoomSDKActiveVideoElement : ZoomSDKVideoElement
/**
 * @brief Set whether to display active video.
 * @param start YES means starting active video, otherwise not.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)startActiveView:(BOOL)start;
@end


/**
 * @class ZoomSDKNormalVideoElement
 * @brief Subclass of ZoomSDKVideoElement for normal participant video.
 */
@interface ZoomSDKNormalVideoElement : ZoomSDKVideoElement
/**
 * @brief Set whether to display user's video. 
 * @param subscribe YES means to display user's avatar or video, otherwise display a black background.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)subscribeVideo:(BOOL)subscribe;
@end


/**
 * @protocol ZoomSDKVideoContainerDelegate
 * @brief Protocol for receiving callbacks from video container.
 */
@protocol ZoomSDKVideoContainerDelegate <NSObject>
/**
 * @brief Callback of user ID changes in the video container.
 * @param element Element of the new user.
 * @param userid The ID of changed user.
 */
- (void)onRenderUserChanged:(ZoomSDKVideoElement*_Nullable)element User:(unsigned int)userid;

/**
 * @brief Callback of user data changes in the video container.
 * @param element Element of the new user.
 * @param type Data type of the current user.
 */
- (void)onRenderDataTypeChanged:(ZoomSDKVideoElement*_Nullable)element DataType:(VideoRenderDataType)type;

/**
 * @brief Callback of user's subscription failed.
 * @param error The error of the failed reason.
 * @param element The point of video element to the user.
 */
- (void)onSubscribeUserFail:(ZoomSDKVideoSubscribeFailReason)error videoElement:(ZoomSDKVideoElement*)element;
@end


/**
 * @class ZoomSDKVideoContainer
 * @brief Container class to manage multiple video elements.
 */
@interface ZoomSDKVideoContainer : NSObject
{
    id<ZoomSDKVideoContainerDelegate> _delegate;
    NSMutableArray*                          _elementArray;
}
/**
 * @brief Delegate for video container callbacks.
 */
@property(nonatomic,assign, nullable) id<ZoomSDKVideoContainerDelegate> delegate;
/**
 * @brief Create a video element in the video container.
 * @param element An object of ZoomSDKVideoElement.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)createVideoElement:(ZoomSDKVideoElement*_Nullable*_Nonnull)element;
/**
 * @brief Destroy an existed video element in the video container.
 * @param element An object of ZoomSDKVideoElement.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)cleanVideoElement:(ZoomSDKVideoElement*)element;
/**
 * @brief Get the list of video element.
 * @return If the function succeeds, it will return an array containing ZoomSDKVideoElement object.
 */
- (NSArray*_Nullable)getVideoElementList;
@end
NS_ASSUME_NONNULL_END
