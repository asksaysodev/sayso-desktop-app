//
//  ZMSyncDownloadMediator.h
//  ChatUI
//
//  Created by Michael Lin on 2022/8/25.
//  Copyright © 2022 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ZMDownloadViewMgrProtocol.h"

NS_ASSUME_NONNULL_BEGIN

@class ZMSyncDownloadMediator;
@protocol ZMSyncDownloadMediatorDelegate <NSObject>

- (void)syncDownloadMediator:(ZMSyncDownloadMediator *)mediator updateDownloadStage:(ZMDownloadStage)stage process:(CGFloat)process;

- (void)syncDownloadMediator:(ZMSyncDownloadMediator *)mediator completeWithResult:(ZMDownloadResult)result error:(nullable NSError *)error shouldClear:(BOOL *)shouldClear;

@end

@interface ZMSyncDownloadMediator : NSObject

@property (nonatomic, weak) id<ZMSyncDownloadMediatorDelegate> delegate;

@property (nonatomic, retain, nullable) NSArray<NSNumber *> *downloadTypes;

- (BOOL)needsDownload;
- (void)updateProgress:(nullable ZMDownloadProgressBlock)progress completion:(nullable ZMDownloadCompletionBlock)completion;
- (void)startDownload;
- (void)cancelDownload;
- (BOOL)isAllInProgress;


@end

NS_ASSUME_NONNULL_END
