//
//  ZMReminderMessageMgrProtocol.h
//  zChatComponent
//
//  Created by Kris Smith on 8/23/22.
//

#ifndef ZMReminderMessageProtocol_h
#define ZMReminderMessageProtocol_h

#import <ZoomKit/ZMCallbackImpl.h>
#import <ZoomKit/ZMGradientLabelView.h>
#import <ZoomKit/ZMHMenuItem.h>

@class ZMMessageAdapter;
@class ZMReminderMessageInfo;
@class ZMChatMsgItemInfo;
@class ZMReminderContainerView;

//==== Reminder Message Types

//reminder new badge expires in 1 day
static NSTimeInterval kReminderMessageBadgeExpiration = 86400;

typedef NS_ENUM(NSUInteger, ZMReminderMessageFilter)
{
    ZMReminderMessageFilter_None,
    ZMReminderMessageFilter_All,
    ZMReminderMessageFilter_Upcoming,
    ZMReminderMessageFilter_Past,
};

typedef NS_ENUM(NSUInteger, ZMReminderMessageContext)
{
    ZMReminderMessageContext_None,
    ZMReminderMessageContext_Custom,
    ZMReminderMessageContext_Set,
    ZMReminderMessageContext_Reschedule,
    ZMReminderMessageContext_Edit,
    ZMReminderMessageContext_Cancel,
    ZMReminderMessageContext_Remove,
    ZMReminderMessageContext_Expire
};

//==== Reminder Message Callbacks

typedef NSArray<ZMReminderMessageInfo *> ZMReminderMessageInfoList;

typedef void(^ZMReminderListCallbackBlock)(ZMReminderMessageInfoList * _Nullable info, NSError * _Nullable error);

typedef void(^ZMReminderItemCallbackBlock)(id _Nullable param, NSError * _Nullable error);

//==== Reminder Message Protocol

@protocol ZMReminderMessageMgrProtocol <NSObject>

@optional

//== reminder cache
- (ZMReminderMessageInfo* _Nullable)reminderForMessage:(NSString* _Nonnull)messageId serverTime:(long long)serverTime;

//== option flags and limits
- (BOOL)isReminderMessage:(ZMMessageAdapter* _Nonnull)message;
- (BOOL)isReminderMessageEnabled;

- (BOOL)canSetReminderForMessage:(ZMMessageAdapter* _Nonnull)message;

//== reminder sync
- (NSString* _Nonnull)syncReminderMessages;

//== reminder actions
- (void)setReminderMessage:(ZMMessageAdapter* _Nonnull)message
                   timeout:(NSInteger)timeout
                      note:(NSString* _Nullable)note
                   context:(ZMReminderMessageContext)context;

- (void)setReminderMessage:(ZMMessageAdapter* _Nonnull)message
                   timeout:(NSInteger)timeout
                      note:(NSString* _Nullable)note
                   context:(ZMReminderMessageContext)context
                completion:(ZMCallbackBlock _Nullable)block;

- (void)removeReminderMessage:(ZMMessageAdapter* _Nonnull)message
                      context:(ZMReminderMessageContext)context;

- (void)removeReminderMessage:(ZMMessageAdapter* _Nonnull)message
                      context:(ZMReminderMessageContext)context
                   completion:(ZMCallbackBlock _Nullable)block;

- (void)removeReminderMessageByServerTime:(long long)serverTime
                                  session:(NSString* _Nonnull)sessionId
                                  context:(ZMReminderMessageContext)context;

- (void)removeReminderMessageByServerTime:(long long)serverTime
                                  session:(NSString* _Nonnull)sessionId
                                  context:(ZMReminderMessageContext)context
                               completion:(ZMCallbackBlock _Nullable)block;

//== reminder query
- (NSInteger)reminderMessageUnreadCount;
- (NSInteger)reminderMessageUnreadCountReset:(ZMMessageAdapter* _Nullable)message;

//== reminder UI
- (void)showSetReminderPanelForMessage:(ZMMessageAdapter* _Nonnull)message
                               context:(ZMReminderMessageContext)context;

- (ZMReminderContainerView* _Nonnull)reminderMessageBubbleForMessageInfo:(ZMChatMsgItemInfo* _Nonnull)info;

- (BOOL)shouldDisplayReminderMessageBadge;

- (ZMGradientLabelView* _Nullable)reminderMessageBadgeForToolbar;

- (ZMGradientLabelView* _Nullable)reminderMessageBadgeForGroupCell;

- (ZMHMenuItem* _Nonnull)reminderMessageBadgeForMenu:(ZMHMenuItem* _Nonnull)menuItem;

- (void)setReminderMessageBadgeExpiry:(NSTimeInterval)expiry;

//== notifications
- (void)addListenReminderSetByTarget:(id _Nonnull)target
                          completion:(ZMCallbackBlock _Nonnull)block;

- (void)addListenReminderRemoveByTarget:(id _Nonnull)target
                             completion:(ZMCallbackBlock _Nonnull)block;

- (void)addListenReminderExpireByTarget:(id _Nonnull)target
                             completion:(ZMCallbackBlock _Nonnull)block;

- (void)addListenReminderUnreadCountByTarget:(id _Nonnull)target
                                  completion:(ZMCallbackBlock _Nonnull)block;

- (void)removeAllListenByTarget:(id  _Nonnull)target;

- (void)addListenReminderListMergeByTarget:(id _Nonnull)target
                                completion:(ZMReminderListCallbackBlock _Nonnull)block;

- (void)addListenReminderListUpdateByTarget:(id _Nonnull)target
                                 completion:(ZMReminderListCallbackBlock _Nonnull)block;

- (void)addListenReminderListRemoveByTarget:(id _Nonnull)target
                                 completion:(ZMReminderListCallbackBlock _Nonnull)block;

- (void)callbackListenByType:(NSString* _Nonnull)typeKey
                       param:(id _Nonnull)param
                       error:(NSError * _Nullable)error;

@end

#ifndef ZMShareReminderMessageProtocol
#define ZMShareReminderMessageProtocol (id <ZMReminderMessageMgrProtocol>)ZMSharedFor(ZMReminderMessageMgrProtocol)
#endif

#endif /* ZMReminderMessageProtocol_h */
