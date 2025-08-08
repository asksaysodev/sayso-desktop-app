//
//  ZMAIC.h
//  ZoomAIC
//
//  Created by Jun Pang on 9/9/23.
//

NS_ASSUME_NONNULL_BEGIN

@class ZMAICChatViewController;
@interface ZMAIC : NSObject

+(ZMAICChatViewController*)defaultController;
+(ZMAICChatViewController*)meetingController;
@end

NS_ASSUME_NONNULL_END
