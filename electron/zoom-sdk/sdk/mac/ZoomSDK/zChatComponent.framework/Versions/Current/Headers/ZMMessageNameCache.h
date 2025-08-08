//
//  ZMMessageNameCache.h
//  zChatUI
//
//  Created by groot.ding on 03/08/2020.
//  Copyright © 2020 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMChatMsgItemInfo.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMMessageNameCache : NSObject

+ (instancetype)sharedInstance;

+ (void)releaseInstance;

- (NSAttributedString *)getMessageNameAttributedStringCacheByJid:(NSString *)jid
                                                            flag:(NSInteger)flag
                                                            font:(NSFont *)font
                                                        addition:(NSString *)addition;

- (void)cacheNameAttributedString:(NSAttributedString *)attributedString
                              jid:(NSString *)jid
                             flag:(NSInteger)flag
                             font:(NSFont *)font
                         addition:(NSString *)addition;

- (void)cleanNameCacheByJids:(NSArray <NSString *> *)jids;

- (void)cleanAllCache;
@end

NS_ASSUME_NONNULL_END
