//
//  ZMVoiceItemInfo.h
//  zChatComponent
//
//  Created by Zoro.Fu on 2024/7/24.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMMessageAdapter;

@interface ZMVoiceItemInfo : NSObject

@property (nonatomic, assign) NSInteger audioLength;
@property (nonatomic, copy)   NSString *audioFilePath;
@property (nonatomic, copy)   NSString *sessionID;
@property (nonatomic, copy)   NSString *msgID;
@property (nonatomic, assign) BOOL isSendByMe;

@end

NS_ASSUME_NONNULL_END
