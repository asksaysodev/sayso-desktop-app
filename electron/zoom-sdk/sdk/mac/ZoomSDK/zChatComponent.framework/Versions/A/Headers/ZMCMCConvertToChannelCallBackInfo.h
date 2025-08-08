//
//  ZMCMCConvertToChannelCallBackInfo.h
//  zChatComponent
//
//  Created by Simon Shang on 2024/5/23.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMCMCConvertToChannelCallBackInfo : NSObject

@property (nonatomic, copy) NSString *subGroupId;
@property (nonatomic, copy) NSString *mainGroupId;
@property (nonatomic, copy) NSString *actionOwner;
@property (nonatomic, assign) NSInteger result;
@property (nonatomic, copy) NSString *reqId;

@end

NS_ASSUME_NONNULL_END
