//
//  ZMWebUserProfileViewController.h
//  zChatUI
//
//  Created by Jed Zheng on 18/7/2022.
//  Copyright © 2022 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ZMBaseWKWebViewController.h"
NS_ASSUME_NONNULL_BEGIN

@interface ZMWKWebUserProfileViewController : ZMBaseWKWebViewController
@property (nonatomic, assign) int panelSourceType;
@property (nonatomic, copy) NSString *userId;
@property(nonatomic, assign) ZMUserProfileTrackType trackType;
@property (nonatomic, retain) ZMProfileConfig *floatConfig;
@property (nonatomic, copy) void (^updateComplete)(void);

- (void)updateCurrentProfileJid:(NSString *)jid dataUpdateComplete:(nullable void (^)(void))complete;
- (void)updateCurrentProfileJid:(NSString *)jid dataUpdateComplete:(nullable void (^)(void))complete tabSwithchComplete:(nullable void(^)(void))tabSwitchComplete;
@end

NS_ASSUME_NONNULL_END
