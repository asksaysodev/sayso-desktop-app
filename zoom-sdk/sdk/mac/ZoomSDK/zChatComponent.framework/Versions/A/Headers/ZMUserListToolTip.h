//
//  ZMUserListToolTip.h
//  SaasBeePTUIModule
//
//  Created by groot.ding on 1/18/21.
//  Copyright © 2021 Zoom. All rights reserved.
//

#import <AppKit/AppKit.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMUserInfo;

@interface ZMUserListToolTip : NSObject 

 + (ZMUserListToolTip*)sharedInstance;
 + (void)releaseInstance;

- (void)showToolTipWithUserList:(NSArray <ZMUserInfo *> * _Nonnull)userLists ofView:(NSView * _Nonnull)positioningView;

- (void)showToolTipWithUserList:(NSArray <ZMUserInfo *> * _Nonnull)userLists ignoreUserList:(NSArray <ZMUserInfo *> * _Nullable)ignoreUserList ofView:(NSView * _Nonnull)positioningView;

- (void)showDetailUserList:(NSArray <ZMUserInfo *> * _Nonnull)userLists ofView:(NSView * _Nonnull)positioningView;

- (void)showToolTipWithUserList:(NSArray <ZMUserInfo *> * _Nonnull)userLists ofView:(NSView * _Nonnull)positioningView;

- (void)showToolTipWithUserList:(NSArray <ZMUserInfo *> * _Nonnull)userLists
                 ignoreUserList:(NSArray <ZMUserInfo *> * _Nullable)ignoreUserList
                         ofView:(NSView * _Nonnull)positioningView;

- (void)showToolTipWithUserList:(NSArray <ZMUserInfo *> * _Nonnull)userLists
                 ignoreUserList:(NSArray <ZMUserInfo *> * _Nullable)ignoreUserList
                         ofView:(NSView * _Nonnull)positioningView
                         inRect:(NSRect)positioningRect
               emptyListMessage:(NSString* _Nullable)emptyListMessage;

- (void)showToolTipWithUserList:(NSArray <ZMUserInfo *> * _Nonnull)userLists
                 ignoreUserList:(NSArray <ZMUserInfo *> * _Nullable)ignoreUserList
                         ofView:(NSView * _Nonnull)positioningView
                         inRect:(NSRect)positioningRect
               emptyListMessage:(NSString* _Nullable)emptyListMessage
                    showViewAll:(BOOL)showViewAll
                  viewMoreBlock:(nullable void(^)())viewMoreBlock;

- (void)showToolTipWithUserList:(NSArray <ZMUserInfo *> * _Nonnull)userLists
                 ignoreUserList:(NSArray <ZMUserInfo *> * _Nullable)ignoreUserList
                         ofView:(NSView * _Nonnull)positioningView
                         inRect:(NSRect)positioningRect
                          frame:(CGRect)rect
               emptyListMessage:(NSString*)emptyListMessage
                    showViewAll:(BOOL)showViewAll
                  viewMoreBlock:(nullable void(^)())viewMoreBlock;

- (void)showDetailUserList:(NSArray <ZMUserInfo *> * _Nonnull)userLists ofView:(NSView * _Nonnull)positioningView;

- (BOOL)isVisible;

- (void)close;

@end

@interface ZMMentionNameTextField : NSTextField
@property (nonatomic, copy) NSString *Jid;

@end

NS_ASSUME_NONNULL_END
