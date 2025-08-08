//
//  ZMNewChatsContentViewController.h
//  ChatUI
//
//  Created by Jed Zheng on 20/8/2021.
//  Copyright © 2021 Zoom. All rights reserved.
//

#import <ZoomKit/ZoomKit.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMMessageContentViewController;
@class ZMChatsInputContainerViewCtrl;

@protocol ZMNewChatsDelegate <NSObject>

@optional
- (void)sendMessageToSessionId:(NSString *)sessionId data:(NSDictionary *)data;

- (void)jumpToSessionFromNewChat:(NSString *)sessionId;

- (ZMMessageContentViewController *)getMessageContentViewControllerSessionId:(NSString *)sessionId;

@end

@interface ZMNewChatsContentViewController : ZMBaseViewController

@property (nonatomic, copy) void (^onMembersInfoChanged)();

@property (nonatomic,retain) Class messageContentViewControllerClass;

@property (nonatomic, weak) id <ZMNewChatsDelegate> delegate;

- (ZMMessageContentViewController *)messageContentCtrl;
- (ZMChatsInputContainerViewCtrl *)inputViewCtrl;

- (instancetype)initWithSessionId:(NSString*)sessionID;

- (NSDictionary*)getNewChatMembersInfo;

- (NSString *)currentShowSessionId;

- (void)updateUIForFontSizeRate:(NSInteger)sizeOffset;

- (void)scrollToEndOfDocumentIfFormatMenuExpanding;

@end

NS_ASSUME_NONNULL_END
