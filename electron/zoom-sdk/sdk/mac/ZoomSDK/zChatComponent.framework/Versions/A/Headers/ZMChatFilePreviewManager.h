//
//  ZMChatFilePreviewManager.h
//  zChatUI
//
//  Created by groot.ding on 3/10/20.
//  Copyright © 2020 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMIMScreenShotMgr.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMChatMsgItemInfo;
@class ZMFileEntity,ZMChatFileItem;
@class ZMCBotInlineVideoPreviewAppContext;
@class ZMChatFilePreviewParam;
@class ZMChatAppContext;

extern const long long ZMImagePreviewMaxBit;

typedef NS_ENUM(NSUInteger, ZMChatFilePreviewDockOutType) {
    ZMChatFilePreviewDockOutTypeFile,
    ZMChatFilePreviewDockOutTypeAppViewer,
};

@protocol ZMAnnotationCallback <NSObject>
@required

- (void)didShareAnnotatedImage:(ZMScreenShotItem *)item forThreadID:(NSString *)threadID;


@end

@interface ZMChatFilePreviewManager : NSObject

@property (nonatomic,assign) BOOL shouldMouseUpOutsizeClosePreview;

@property (readonly) NSString *sessionId;

@property (readonly) NSWindow *window;

+ (instancetype)sharedInstance;

+ (void)releaseInstance;

- (void)openFilePreviewByItems:(NSArray <ZMFileEntity *> *)fileItems sessionId:(NSString *)sessionId currentIndex:(NSInteger)currentIndex  window:(nullable NSWindow *)window;
- (void)openFilePreviewByItems:(NSArray <ZMFileEntity *> *)fileItems sessionId:(NSString *)sessionId currentIndex:(NSInteger)currentIndex  window:(nullable NSWindow *)window isDockOut:(BOOL)isDockout;
- (void)openFilePreviewByItems:(NSArray <ZMFileEntity *> *)fileItems sessionId:(NSString *)sessionId currentIndex:(NSInteger)currentIndex  window:(NSWindow *)window isDockOut:(BOOL)isDockout delegate:(id<ZMAnnotationCallback>)delegate;
- (void)openDocumentPreviewByItem:(ZMFileEntity *)fileItem sessionId:(NSString *)sessionId threadId:(NSString *)threadId window:(NSWindow *)window isDockOut:(BOOL)isDockout;
- (void)openDocumentPreviewByFileID:(NSString *)fileID window:(NSWindow *)window dockout:(BOOL)dockout;
- (void)openDocumentPreview:(NSWindow *)window params:(ZMChatFilePreviewParam *)params;
- (void)openBotInlineVideoPreviewWithAppID:(NSString *)appID appContext:(ZMCBotInlineVideoPreviewAppContext *)context window:(NSWindow *)window;
- (void)openAppViewerWithAppContext:(ZMChatAppContext *)appContext window:(nullable NSWindow *)window;

- (void)dockout;

- (void)dismiss;
- (void)dismissIfInMainWindow;

- (BOOL)isDockOut;

@end

@interface ZMChatFilePreviewManager (Deprecated)

- (BOOL)isShow NS_DEPRECATED_MAC(10.9, 10.9, "This method is no longer supported");

- (BOOL)isDisplayImage NS_DEPRECATED_MAC(10.9, 10.9, "This method is no longer supported");

- (void)rotateLeft NS_DEPRECATED_MAC(10.9, 10.9, "This method is no longer supported");
- (void)rotateRight NS_DEPRECATED_MAC(10.9, 10.9, "This method is no longer supported");

- (void)annotate NS_DEPRECATED_MAC(10.9, 10.9, "This method is no longer supported");
- (void)cancelAnnotationnModel NS_DEPRECATED_MAC(10.9, 10.9, "This method is no longer supported");

@end

NS_ASSUME_NONNULL_END
