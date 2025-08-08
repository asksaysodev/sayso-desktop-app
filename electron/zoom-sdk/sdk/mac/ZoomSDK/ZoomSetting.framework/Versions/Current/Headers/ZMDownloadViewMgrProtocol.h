//
//  ZMDownloadViewMgrProtocol.h
//  ChatUI
//
//  Created by Michael Lin on 2021/12/20.
//  Copyright © 2021 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ZoomSetting/ZMOndemandProtocol.h>
#import <ZoomKit/ZMRoutableObject.h>
#import <AppKit/AppKit.h>

NS_ASSUME_NONNULL_BEGIN

@protocol ZMDownloadPanelProtocol <NSObject>

- (void)download;
- (void)downloadWithProgress:(nullable ZMDownloadProgressBlock)progress completion:(nullable ZMDownloadCompletionBlock)completion;
- (void)downloadInWindow:(nullable NSWindow *)window
            withProgress:(nullable ZMDownloadProgressBlock)progress
              completion:(nullable ZMDownloadCompletionBlock)completion;
- (void)updateFrameCenter:(NSPoint)center;
- (void)close;
- (nullable NSWindow *)presentedInWindow;

@end

typedef NS_ENUM(NSUInteger, ZMDownloadViewType) {
    ZMDownloadViewTypeCodeSnippet,
    ZMDownloadViewTypeAomHostForVBSetting,
    ZMDownloadViewTypeAomHostForClipsVBSetting,
    ZMDownloadViewTypeAomHostForFaceMakeup,
    ZMDownloadViewTypeForAvatars,
    ZMDownloadViewTypeForClipsAvatars,
    ZMDownloadViewTypeAomHostForClipsImmersive,
    ZMDownloadViewTypeAomHostForStudioEffects,
    ZMDownloadViewTypeAomHostForShareVideo,
    ZMDownloadViewTypeAomHostForSharePPT,
    ZMDownloadViewTypeAomHostForZoomApp,
    ZMDownloadViewTypeAirHost,
    ZMDownloadViewTypeViperEX,
    ZMDownloadViewTypeViperEXForPSE,
    ZMDownloadViewTypeAomHostForHandGesture,
    ZMDownloadViewTypeAomHostForForceVB,
    ZMDownloadViewTypeCefAgent,
    ZMDownloadViewTypeAomHostForClipsBioInfoVerify,
    ZMDownloadViewTypeAomHostForHandGestureInMeeting,
    ZMDownloadViewTypeAomHostForAttentionLight,
    ZMDownloadViewTypeCefFrameworkPlugin,
    ZMDownloadViewTypeCefWebViewPlugin,
};

@protocol ZMDownloadViewMgrProtocol <ZMRoutableObject>

+ (instancetype)shared;

- (id<ZMDownloadPanelProtocol>)dialogueForType:(ZMDownloadViewType)type;

- (void)closeDialogueForDownloadType:(ZMDownloadType)downloadType inWindow:(nullable NSWindow *)window;
- (void)closeDialogueForType:(ZMDownloadViewType)viewType;

@end
NS_ASSUME_NONNULL_END
