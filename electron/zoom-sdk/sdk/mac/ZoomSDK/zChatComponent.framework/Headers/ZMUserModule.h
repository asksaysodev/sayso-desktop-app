//
//  ZMUserModule.h
//  zChatUI
//
//  Created by groot.ding on 2018/4/25.
//  Copyright © 2018 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMBuddyProtocal.h>
#import <ZoomUnit/ZMLoginProtocol.h>

@interface ZMUserModule : NSObject <ZMBuddyProtocol,ZMLoginProtocol>

+ (nonnull instancetype)shareInstance;
+ (nonnull instancetype)sharedInstance;

@end
