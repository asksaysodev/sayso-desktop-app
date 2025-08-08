//
//  ZMMessageMenuImpl.h
//  zChatUI
//
//  Created by groot.ding on 2019/7/17.
//  Copyright © 2019 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMReminderMessageMgrProtocol.h>

@class ZMChatMsgItemInfo;
@class ZMHMenuItem;
@class ZMChatAppContext;

typedef NS_ENUM(NSInteger, ZMIMTextViewMenuTag) {
    ZMIMTextViewMenuTag_copy = 9990,
    ZMIMTextViewMenuTag_shareMessage,
    ZMIMTextViewMenuTag_openFile,
    ZMIMTextViewMenuTag_showInFinder,
    ZMIMTextViewMenuTag_Expand,
    ZMIMTextViewMenuTag_saveAs,
    ZMIMTextViewMenuTag_shareFile,
    ZMIMTextViewMenuTag_delete,
    ZMIMTextViewMenuTag_removeChannelMsg,
    ZMIMTextViewMenuTag_addEmoji,
    ZMIMTextViewMenuTag_markUnread,
    ZMIMTextViewMenuTag_edit,
    ZMIMTextViewMenuTag_joinMeeting,
    ZMIMTextViewMenuTag_download,
    ZMIMTextViewMenuTag_cancel,
    ZMIMTextViewMenuTag_openByIntegrationTool,
    ZMIMTextViewMenuTag_followThread,   //follow or unfollow
    ZMIMTextViewMenuTag_starredMessage, //star or unstar
    ZMIMTextViewMenuTag_numberJoinMeeting,
    ZMIMTextViewMenuTag_numberCall,
    ZMIMTextViewMenuTag_numberCopy,
    ZMIMTextViewMenuTag_CopyLink,
    ZMIMTextViewMenuTag_OpenLink,
    ZMIMTextViewMenuTag_OpenInBrowser,
    ZMIMTextViewMenuTag_TopPin,
    ZMIMTextViewMenuTag_UnTopPin,
    ZMIMTextViewMenuTag_JumpToMessage,
    ZMIMTextViewMenuTag_UnStar,
    ZMIMTextViewMenuTag_RemoveFromPinHistory,
    ZMIMTextViewMenuTag_sendSMS,
	ZMIMTextViewMenuTag_ReportUser,
	ZMIMTextViewMenuTag_RemoveChatMsg,
	ZMIMTextViewMenuTag_ReportRemoveUserAndMsg,
    ZMIMTextViewMenuTag_Quote,
    ZMIMTextViewMenuTag_CopyDeepLink,
    ZMIMTextViewMenuTag_Reminder_None,
    ZMIMTextViewMenuTag_Reminder_Set,
    ZMIMTextViewMenuTag_Reminder_Cancel,
    ZMIMTextViewMenuTag_Reminder_ExpiresIn_30Minutes,
    ZMIMTextViewMenuTag_Reminder_ExpiresIn_1Hour,
    ZMIMTextViewMenuTag_Reminder_ExpiresIn_3Hours,
    ZMIMTextViewMenuTag_Reminder_ExpiresIn_Tomorrow,
    ZMIMTextViewMenuTag_Reminder_ExpiresIn_NextMonday,
    ZMIMTextViewMenuTag_Reminder_ExpiresIn_Custom,
    ZMIMTextViewMenuTag_ChatApp,
    ZMIMTextViewMenuTag_CopyHuddleLink,
    ZMIMTextViewMenuTag_AddAsTab,
    ZMIMTextViewMenuTag_Reload,
    ZMIMTextViewMenuTag_CreateTask,
    ZMIMTextViewMenuTag_saveToIntegration = 19000,
    ZMIMTextViewMenuTag_openInIntegration = 29000
};

@interface ZMMessageMenuImpl : NSObject

@property (nonatomic,copy) void(^menuWillShowOrClose)(BOOL show);

@property (nonatomic,copy) void(^menuClick)(ZMHMenuItem *item);

@property (nonatomic,copy) BOOL(^canShareMsg)(ZMHMenuItem *item);

- (void)showMenuWithMessageItem:(ZMChatMsgItemInfo *)msgItem
                      sessionId:(NSString *)sessionId
                         inView:(NSView *)inView
                     localPoint:(CGPoint)localPoint;

- (void)showMenuInFilePreviewByMessageItem:(ZMChatMsgItemInfo *)msgItem
                                 sessionId:(NSString *)sessionId
                                    inView:(NSView *)inView
                                localPoint:(CGPoint)localPoint;

- (void)showReminderSetTimeoutMenuByMessage:(ZMChatMsgItemInfo *)msgItem
                                  sessionId:(NSString *)sessionId
                                     inView:(NSView *)inView
                                 localPoint:(CGPoint)localPoint
                                    context:(ZMReminderMessageContext)context;

- (void)showAppViewerMenuByAppContext:(ZMChatAppContext *)appContext
                               inView:(NSView *)inView
                           localPoint:(CGPoint)localPoint;

- (void)showMenuWithNumber:(NSString*)numberString
                 sessionId:(NSString *)sessionId
                    inView:(NSView *)inView
                localPoint:(CGPoint)localPoint
                     style:(NSInteger)style;

- (void)closeMenu;


//MARK: - Override by Meeting
- (void)onMenuItemClicked:(ZMHMenuItem *)menuItem;
- (NSArray<ZMHMenuItem*> *)getMenuItemsByMessageItem:(ZMChatMsgItemInfo *)msgItem;
- (NSArray<ZMHMenuItem*> *)getDeleteMenuByMessageItem:(ZMChatMsgItemInfo *)msgItem needsSeparator:(BOOL)needsSeparator;

@end
