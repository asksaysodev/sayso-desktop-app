//
//  ZMChatSearchMessageInfo.h
//  zChatUI
//
//  Created by groot.ding on 2019/7/4.
//  Copyright © 2019 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <zPTApp/SaasBeePTAppInterface.h>
#include <zChatApp/FileContentInterface.h>
#import <zChatApp/search/SearchMgr.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMIMMessageDraft;

@interface ZMChatSearchMessageInfo : NSObject
{
    NSString*                       _msgID;
    NSString*                       _sessionID;
    NSString*                       _senderID;
    NSString*                       _senderName;
    NSString*                       _key;
    NSDate*                         _date;
    unsigned long long              _searchedSvrSideTime;
}

@property (nonatomic,copy) NSString* msgID;
@property (nonatomic,copy) NSString* sessionID;
@property (nonatomic,copy) NSString* senderID;
@property (nonatomic,copy) NSString* senderName;
/*! The message content.
 @note SDK search results are sometimes truncated (i.e. they DON'T contain the entire message).
 See @c cutType , @c cutPrefix , @c cutSuffix
 */
@property (nonatomic, readonly, strong) NSString* content;
@property (nonatomic,copy) NSString* key;
@property (nonatomic,copy) NSDate* date;
@property (nonatomic,assign) unsigned long long searchedSvrSideTime;
@property (nonatomic,assign) unsigned long long sendTime;
@property (nonatomic,assign) NSInteger cutType;
@property (nonatomic,copy) NSString*  cutPrefix;
@property (nonatomic,copy) NSString*  cutSuffix;
@property (assign, getter = isLocalSearchMessage) BOOL localSearchMessage;

@property (nonatomic,assign) BOOL isComment;
@property (nonatomic,copy) NSString *threadId;
@property (nonatomic,copy) NSString *commentID;
@property (nonatomic,assign) unsigned long long commentSvrSideTime;
@property (nonatomic,assign) unsigned long long threadSvrSideTime;
@property (nonatomic,assign) NSInteger expLoadCommentCount;
@property (nonatomic,assign) BOOL isSMSMessage;
@property (nonatomic,assign) BOOL fromWebSearch;
@property (nonatomic,assign) ns_zoom_messager::SMSMessageType smsMsgType;
@property (nonatomic,assign) NSInteger smsFilesCount;

@property (assign) BOOL isNeedHighlightBubble;
@property (assign) BOOL isNeedHighlightBackground;
@property (nonatomic,assign) BOOL shouldCleanUnreadFront;

@property (nonatomic,retain) ZMIMMessageDraft *draftInfo;

- (instancetype)initWithMessageContent:(NSString *)content;
- (instancetype)initWithSearchAppParams:(NSDictionary *)params;
- (id)initWithMessageSearchResult:(ns_zoom_messager::MessageSearchResult)message;
- (id)initWithLocalMessageSearchResult:(ns_zoom_messager::MessageSearchResult)message;

/*!
 Returns the NSRanges that should be highlighted when displaying self.content
 */
@property (nullable, nonatomic, readonly, strong) NSArray<NSValue*>* searchMatches;
/*!
 Returns the NSRanges that should be highlighted when displaying the entire message
 @note Returns nil if the message isn't loaded
 */
- (NSArray<NSValue*>* _Nullable)searchMatchesForMessageBody;

/// if _content already set a value, this call will be ignore and return immediately.
- (void)setContent:(NSString *)content highlightRange:(NSRange)range encodeType:(NSInteger)type;

@end

NS_ASSUME_NONNULL_END
