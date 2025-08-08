//
//  ZMShareMessageInfo.h
//  zChatComponent
//
//  Created by Zoro.Fu on 2023/4/13.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMShareMessageInfo : NSObject

@property (nonatomic, copy) NSString *messageBody;
@property (nonatomic, copy) NSString *toSessionId;
@property (nonatomic, copy) NSString *fromSessionId;
@property (nonatomic, copy) NSString *fromMessageId;

@property (nonatomic, assign) NSInteger messageType;

@property (nonatomic, copy) NSString *noteMessage;

@property (nonatomic, assign) BOOL justShareNote;

@property (nonatomic, assign) BOOL shareAsOriginMessage;

@end

NS_ASSUME_NONNULL_END
