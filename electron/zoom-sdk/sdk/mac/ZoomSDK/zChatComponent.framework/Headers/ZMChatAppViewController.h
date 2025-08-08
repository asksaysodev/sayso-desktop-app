//
//  ZMChatAppViewController.h
//  ChatUI
//
//  Created by Zoro.Fu on 2022/8/24.
//  Copyright © 2022 Zoom. All rights reserved.
//

#import <ZoomKit/ZoomKit.h>
#ifdef ENABLE_ZAPP
#import <ZoomAppUISDK/ZoomAppUISDK.h>
#endif

@class ZMChatAppContext, ZMChatAppViewController, ZMChatAppComposeMessageInfo, ZMChatAppWebSetting, ZMChatAppViewModel;
NS_ASSUME_NONNULL_BEGIN

@protocol ZMChatAppDelegate <NSObject>

- (void)chatAppReceivedCloseAction:(ZMChatAppViewController *)vc;

@optional

- (BOOL)chatApp:(ZMChatAppViewController *)vc editCompose:(ZMChatAppComposeMessageInfo *)composeInfo;
- (void)chatApp:(ZMChatAppViewController *)vc didChangeSize:(NSSize)size;

@end

@interface ZMChatAppViewController : ZMBaseViewController

@property (nonatomic, readonly) ZMChatAppContext *chatAppContext;

@property (nonatomic, readonly) NSString *chatAppId;

@property (nonatomic, weak) id<ZMChatAppDelegate> delegate;

@property (nonatomic, strong, readonly) ZMImageView *appIconView;

@property (nonatomic, strong, readonly) ZMChatAppViewModel *appViewModel;

#ifdef ENABLE_ZAPP
@property(nonatomic, readonly) NSView<ZMAppWebViewInstance> *zAppWebView;
#endif

- (instancetype)initWithChatAppContext:(ZMChatAppContext *)context;

- (void)relayoutUIAfterDockOut;

- (void)relaunchWebView;

- (void)updateWithChatAppContext:(ZMChatAppContext *)context;

- (void)setCloseButtonHidden:(BOOL)isHidden;

- (NSString *)registerFilePathId:(NSString*)filePath intention:(int)intention;

- (void)showMenuWithConfig:(NSDictionary *)config;

- (void)updateWithWebConfig:(NSDictionary *)config;

@end

NS_ASSUME_NONNULL_END
