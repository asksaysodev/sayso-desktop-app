//
//  ZMUserProfileDataAndActionHelper.h
//  zChatComponent
//
//  Created by Jed Zheng on 8/6/2023.
//

#import <Foundation/Foundation.h>
#import "ZMUserProfileProtocol.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZMUserProfileDataAndActionHelper : NSObject <ZMUserProfileProtocol>
- (BOOL)isPtProcess;
@end

NS_ASSUME_NONNULL_END
