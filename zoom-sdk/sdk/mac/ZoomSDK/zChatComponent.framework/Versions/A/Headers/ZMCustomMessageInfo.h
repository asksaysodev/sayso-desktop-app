//
//  ZMCustomMessageInfo.h
//  ZoomMsgUI
//
//  Created by Groot Ding on 2022/4/27.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMCustomMessageInfo : NSObject

@property (copy) NSString*      sessionID;
@property (copy) NSString*      msgID;
@property (copy) NSString*      threadID;
@property (assign)  NSInteger   threadSvrSideTime;
@property (assign)  NSInteger   svrSideTime;
@property (assign)  BOOL        isComment;

+ (ZMCustomMessageInfo*)createZMCustomMessageInfo:(void*)inSdkMessageInfo;

@end

NS_ASSUME_NONNULL_END
