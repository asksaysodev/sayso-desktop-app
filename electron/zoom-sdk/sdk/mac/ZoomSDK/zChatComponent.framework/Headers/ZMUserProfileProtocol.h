//
//  ZMUserProfileProtocol.h
//  zChatComponent
//
//  Created by Jed Zheng on 31/5/2023.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol ZMUserProfileProtocol <NSObject>

@optional
+ (instancetype)shared;

//- (NSString *)currentUserJid;
- (NSString *)getLocalUserProfilePath;
- (void)refreshProfileBigPictureWithJid:(NSString *)jid;
- (void)onProfileCardWillDisappear;
- (NSString *)localJsResourcePath;
- (BOOL)isEnableLocalWebProfileCard;
- (BOOL)isEnableHostWebProfileCard;
- (NSString *)myJid;
- (void)webProfileCardSelectedChanged:(NSString *)jid webView:(NSView *)webview;
- (void)webProfileCardSelectedChanged:(NSString *)jid;
- (NSString *)peopleHubProfileCardHostUrl;
@end

NS_ASSUME_NONNULL_END
