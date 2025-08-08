

#import <Foundation/Foundation.h>
#include "sdk_native_error.h"
NS_ASSUME_NONNULL_BEGIN

@interface authServiceDelegate : NSObject<ZoomSDKAuthDelegate>
@property(nonatomic,assign)ZNAuthResult authResult;
@property(nonatomic,assign)ZNLOGINSTATUS loginStatus;

+(authServiceDelegate *)share;

-(ZNAuthResult)getAuthResult;
-(ZNLOGINSTATUS)getLoginStatus;
@end

@interface ZoomSDKRemindDelegate : NSObject <ZoomSDKReminderControllerDelegate>
+(ZoomSDKRemindDelegate *)share;
@property(nonatomic,strong)ZoomSDKReminderHandler *reminderHandler;
@property(nonatomic,strong)ZoomSDKMeetingEnableReminderHandler *enableReminderHandler;
@end

@interface directShareServiceDelegate : NSObject <ZoomSDKDirectShareHelperDelegate>
+(directShareServiceDelegate *)share;
@property(nonatomic,assign)DirectShareStatus  directStatus;
@property(nonatomic,strong)ZoomSDKDirectShareHandler *directShareHandler;
-(DirectShareStatus)getDirectShare;
@end
NS_ASSUME_NONNULL_END
