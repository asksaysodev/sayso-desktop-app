//
//  ZMChannelPreviewController.h
//  zChatUI
//
//  Created by kevin.li on 2021/10/27.
//  Copyright © 2021 Zoom. All rights reserved.
//

#import <ZoomKit/ZoomKit.h>

NS_ASSUME_NONNULL_BEGIN

static const NSInteger kPreviewViewWidth = 560;

@class ZMPreviewGroupInfo;

@interface ZMChannelPreviewController : ZMBaseViewController

+ (ZMChannelPreviewController*)sharedInstance;
+ (void)releaseInstance;

@property (nullable, nonatomic, copy) void(^onBack)(NSString* param);

@property (nonatomic, copy, nullable) void(^previewGroupInfoReceived)(ZMPreviewGroupInfo *groupInfo);

@property (nonatomic, copy, nullable) void(^didJoinChannel)(NSString* channelId,NSError *error);

- (void)requestPreviewInfo:(NSString*)channelId title:(NSString*)title fromMentionChannel:(BOOL)fromMentionChannel;
- (void)requestPreviewInfo:(NSString*)channelId title:(NSString*)title fromMentionChannel:(BOOL)fromMentionChannel linkId:(NSString* _Nullable)linkId;
- (void)showPreview:(ZMPreviewGroupInfo*)groupInfo;
@end



NS_ASSUME_NONNULL_END
