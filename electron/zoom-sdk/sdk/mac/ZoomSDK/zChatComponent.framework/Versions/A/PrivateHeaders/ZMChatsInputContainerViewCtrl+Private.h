//
//  ZMChatsInputContainerViewCtrl+Private.h
//  zChatComponent
//
//  Created by Cyan.Yang on 2023/7/27.
//

#import <zChatComponent/zChatComponent.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMChatsInputContainerViewCtrl ()

+ (NSString *)processMessageContents:(NSString *)contents
           forRobotCommandWithPrefix:(NSString *)prefix
                          echoString:(NSString * _Nonnull * _Nullable)outEchoString;

@end

NS_ASSUME_NONNULL_END
