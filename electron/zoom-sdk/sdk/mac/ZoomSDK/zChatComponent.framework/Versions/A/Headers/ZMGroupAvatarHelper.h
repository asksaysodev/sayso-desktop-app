//
//  ZMGroupAvatarHelper.h
//  zChatComponent
//
//  Created by groot Ding on 2/4/25.
//

#import <Foundation/Foundation.h>
#import <ZoomUnit/ZMLoginProtocol.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMGroupAvatarHelper : NSObject <ZMLoginProtocol>

+ (instancetype)sharedInstance;

- (nullable NSImage *)getAvatarByGroupId:(nonnull NSString *)groupId;

- (BOOL)haveGroupAvatar:(nonnull NSString *)groupId;

- (BOOL)needGenerateGroupAvatar:(nonnull NSString *)groupId;

- (NSString *)getAvatarPathByGroupId:(nonnull NSString *)groupId;

@end

NS_ASSUME_NONNULL_END
