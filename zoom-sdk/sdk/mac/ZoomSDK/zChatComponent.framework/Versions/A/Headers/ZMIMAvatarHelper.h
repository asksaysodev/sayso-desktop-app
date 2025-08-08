//
//  ZMIMAvatarHelper.h
//  ChatUI
//
//  Created by Groot Ding on 2022/4/25.
//  Copyright © 2022 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMBuddyAdapter;

@interface ZMIMAvatarHelper : NSObject

+ (NSImage*)getUserAvatarByID:(NSString*)JID;

+ (NSURL*)getUserAvatarURLByID:(NSString *)JID;

+ (NSImage*)getUserAvatarByID:(NSString*)JID avatarSize:(CGSize)avatarSize;

+ (NSImage*)getUserAvatarByBuddyInfo:(ZMBuddyAdapter*)buddyInfo;

+ (NSImage*)getUserAvatarByBuddyInfo:(ZMBuddyAdapter*)buddyInfo avatarSize:(CGSize)avatarSize;

+ (NSImage*)getUserBigAvatarByBuddyInfo:(ZMBuddyAdapter*)buddyInfo avatarSize:(CGSize)avatarSize;

/*  MCC Messages will not always get a ZMBuddyAdapter through JID. So, draw draw user avata using userName. */
+ (NSImage*)getUserAvatarByID:(nullable NSString*)JID
                     userName:(nullable NSString*)userName
                   avatarSize:(CGSize)avatarSize;

///  Create Avata for Bot App with JID or Name. When the app can not be searched by JID, drawing avata with App name is more suitable.
/// - Parameters:
///   - JID: Bot App JID
///   - name: Bot App Name
+ (NSImage*)getChatAppAvataByJID:(nonnull NSString*)JID
                         appName:(nullable NSString*)name;

@end

NS_ASSUME_NONNULL_END
