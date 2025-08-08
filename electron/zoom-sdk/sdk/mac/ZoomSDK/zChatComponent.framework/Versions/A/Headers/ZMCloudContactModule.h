//
//  ZMCloudContactModule.h
//  zChatComponent
//
//  Created by lisa.si on 2025/3/12.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN


@protocol ZMCloudContactCallBack <ZMRoutableObject>
- (void)onCloudContactPermissonChanged;
@end

@interface ZMCloudContactModule : NSObject

+ (BOOL)hasAccessGoogleContactPermissions;
+ (BOOL)hasAccessOutLookContactPermissions;
+ (BOOL)hasAccessOffice365Permissions;
+ (BOOL)hasAccessNWSPermissions;
+ (NSString*)getCalendarUrl;
+ (BOOL)hasAccessContactPermissions;
+ (BOOL)getContactsIsHaveWritePermisson;
+ (NSMutableArray *)getCloudContactFolder;

+ (BOOL)isEnableCreatePersonalContact;
+ (BOOL)sendPersonalContactRequest:(zPersonalContactRequest)contactRequest;

+ (void)onCloudContactPermissonChanged;
@end

NS_ASSUME_NONNULL_END
