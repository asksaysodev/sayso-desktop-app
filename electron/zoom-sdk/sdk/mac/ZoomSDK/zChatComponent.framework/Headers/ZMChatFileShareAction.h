//
//  ZMChatFileShareAction.h
//  ZoomMsgUI
//
//  Created by Groot Ding on 2022/4/29.
//

#import <Foundation/Foundation.h>
#import <zPTApp/SaasBeePTAppInterface.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMChatFileShareAction : NSObject

@property (nonatomic, copy) NSString* webFileID;
@property (nonatomic, copy) NSString* sharee;
@property (nonatomic, copy) NSDate* date;
@property (nonatomic, assign) NSInteger index;
@property (nonatomic, copy) NSString *msgId;
@property (nonatomic, copy) NSString *threadId;
@property (nonatomic, assign) BOOL isComment;
@property (nonatomic, assign) unsigned long long msgServerTime;
@property (nonatomic, assign) unsigned long long threadServerTime;
@property (nonatomic, assign) BOOL isForwardedMsg;

/// when ZMFileDLActSource_BrowseFileList, there is mutiple shareAction, only jump if zm_shouldJumpToMessage is true.
@property (nonatomic, assign) BOOL zm_shouldJumpToMessage;

- (id)initWithShareAction:(NS_ZOOM_MESSAGER::IZoomShareAction*)zoomShareAction;

- (NSString *)getJumpMessageID;
- (NSString *)getJumpSessionID:(NSString *)senderJid;

@end

NS_ASSUME_NONNULL_END
