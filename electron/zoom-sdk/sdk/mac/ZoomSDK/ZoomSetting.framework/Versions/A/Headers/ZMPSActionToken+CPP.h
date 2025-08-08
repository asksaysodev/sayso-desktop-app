//
//  ZMPSActionToken+CPP.h
//  ZoomSetting
//
//  Created by Michael Lin on 2024/1/12.
//

#import <Foundation/Foundation.h>
#import <ZoomSetting/ZMPSActionToken.h>
#import <zPSApp/define/PSDefine.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMPSActionToken (CPP)

+ (ps::PSAsyncRecordingActionType)psActionType:(ZMPSActionType)type;
+ (ZMPSActionType)actionType:(ps::PSAsyncRecordingActionType)type;

- (instancetype)initWithActionToken:(const ps::PSAsyncRecordingActionToken &)token;

- (ps::PSAsyncRecordingActionToken)psActionToken;
- (ps::PSAsyncRecordingActionType)psActionType;

@end

NS_ASSUME_NONNULL_END
