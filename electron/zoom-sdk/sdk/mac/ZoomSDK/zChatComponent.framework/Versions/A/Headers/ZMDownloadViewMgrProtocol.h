//
//  ZMDownloadViewMgrProtocol.h
//  ChatUI
//
//  Created by Michael Lin on 2021/12/20.
//  Copyright © 2021 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMOndemandProtocol.h>

NS_ASSUME_NONNULL_BEGIN

@protocol ZMDownloadPanelProtocol <NSObject>

- (void)download;
- (void)downloadWithProgress:(nullable ZMDownloadProgressBlock)progress completion:(nullable ZMDownloadCompletionBlock)completion;
- (void)downloadInWindow:(nullable NSWindow *)window withProgress:(nullable ZMDownloadProgressBlock)progress completion:(nullable ZMDownloadCompletionBlock)completion;
- (void)close;
- (nullable NSWindow *)presentedInWindow;

@end

typedef NS_ENUM(NSUInteger, ZMDownloadViewType) {
    ZMDownloadViewTypeCodeSnippet,
    ZMDownloadViewTypeAomHostForVBSetting,
    ZMDownloadViewTypeAomHostForFaceMakeup,
    ZMDownloadViewTypeAomHostForStudioEffects,
    ZMDownloadViewTypeAomHostForShareVideo,
    ZMDownloadViewTypeAomHostForSharePPT,
    ZMDownloadViewTypeAomHostForZoomApp,
    ZMDownloadViewTypeAirHost,
    ZMDownloadViewTypeViperEX,
};
@protocol ZMDownloadViewMgrProtocol <ZMRoutableObject>

+ (instancetype)shared;

- (id<ZMDownloadPanelProtocol>)dialogueForType:(ZMDownloadViewType)type;

- (void)closeDialogueForDownloadType:(ZMDownloadType)downloadType inWindow:(nullable NSWindow *)window;

@end
NS_ASSUME_NONNULL_END
