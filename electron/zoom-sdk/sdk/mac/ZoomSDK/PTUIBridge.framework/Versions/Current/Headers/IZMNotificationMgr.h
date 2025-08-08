//
//  IZMNotificationMgr.h
//  PTUIBridge
//
//  Created by fistice on 2024/10/25.
//

#ifndef IZMNotificationMgr_h
#define IZMNotificationMgr_h

#import <ZoomKit/ZMNotificationManager.h>

//@class ZMUserNotification;
@protocol IZMNotificationMgr <NSObject>
- (void)deliverNotification:(ZMUserNotification *)notification;

@end
#endif /* IZMNotificationMgr_h */
// ZMSharedFor(IZMNotificationMgr)
