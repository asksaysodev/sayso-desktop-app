//
//  ZMBotModule.h
//  ChatUI
//
//  Created by Roy Lovejoy on 8/13/21.
//  Copyright © 2021 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMSearchResultItem.h>

/////////////////////////////////////////////Xmpp Bot Response Error Codes
#define XmppError_NoBotPermission               4011
#define XmppError_BotUninstall                  4041
#define XmppError_BotSystemError                5000
#define XmppError_BotExists                     5002
#define XmppError_BotNotInChannel               5003

typedef void (^ZMBotSearchCompletion)(NSArray <ZMSearchResultItem *> * items, NSDictionary* response);
typedef void (^ZMBotAddCompletion)(NSString* channel, NSDictionary* response);
typedef void (^ZMBotRemoveCompletion)(NSString* channel, NSDictionary* response);

@interface ZMBotModule : NSObject

+ (instancetype)shareInstance;

- (void)searchBot:(NSString *)key callBack:(ZMBotSearchCompletion)callBack;

- (void)addBots:(NSArray<NSString*>*)botJIDs channel:(NSString*)channelID callback:(ZMBotAddCompletion)callback;
- (void)removeBots:(NSArray<NSString*>*)botJIDs channel:(NSString*)channelID callback:(ZMBotRemoveCompletion)callback;

@end
