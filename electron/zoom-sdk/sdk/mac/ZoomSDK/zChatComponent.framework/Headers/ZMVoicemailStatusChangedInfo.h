//
//  ZMVoicemailStatusChangedInfo.h
//  zChatComponent
//
//  Created by Simon Shang on 2024/12/11.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, ZMVoicemailChangedAction) {
    ZMVoicemailActionNone,
    ZMVoicemailActionCreate,
    ZMVoicemailActionDelete
};

NS_ASSUME_NONNULL_BEGIN

@interface ZMVoicemailStatusChangedInfo : NSObject

@property (nonatomic, copy) NSString *callID;
@property (nonatomic, copy) NSString *sessionId;
@property (nonatomic, copy) NSString *messageId;
@property (nonatomic, assign) ZMVoicemailChangedAction voicemailStatus;


@end

NS_ASSUME_NONNULL_END
