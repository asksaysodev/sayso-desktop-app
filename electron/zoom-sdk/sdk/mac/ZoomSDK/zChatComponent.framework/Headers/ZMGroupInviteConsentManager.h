//
//  ZMGroupInviteConsentManager.h
//  zChatComponent
//
//  Created by Felipe Bastos on 8/1/23.
//

#import <Foundation/Foundation.h>

@class ZMGroupInviteConsentRequestInfo;

NS_ASSUME_NONNULL_BEGIN

@interface ZMGroupInviteConsentManager : NSObject

+ (BOOL)isPendingConsent:(nonnull NSString *)sessionID;
+ (nullable ZMGroupInviteConsentRequestInfo *)getPendingConsentInfo:(nonnull NSString *)sessionID;

// err is a value defined in NS_SSB_XMPP::XmppError. It's kept as NSUInteger to ensure compatibility with Objective-C.
+ (BOOL)acceptPendingConsentForSession:(nonnull NSString *)sessionID
                              callback:(void (^_Nullable)(NSUInteger err, BOOL shouldRemoveSession))callback
                            NS_SWIFT_NAME(acceptPendingConsent(session:callback:));

+ (BOOL)declinePendingConsentForSession:(nonnull NSString *)sessionID
                               callback:(void (^_Nullable)(NSUInteger err, BOOL shouldRemoveSession))callback
                            NS_SWIFT_NAME(declinePendingConsent(session:callback:));

@end

NS_ASSUME_NONNULL_END
