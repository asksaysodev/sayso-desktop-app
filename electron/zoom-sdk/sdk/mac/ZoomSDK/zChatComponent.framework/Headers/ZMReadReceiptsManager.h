//
//  ZMReadReceiptsManager.h
//  zChatComponent
//
//  Created by Mario Rao on 2023/11/1.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMReadReceiptSession.h>

NS_ASSUME_NONNULL_BEGIN

extern NSString *ZMReadReceiptsAccountSettingChangeNotification;
extern NSString *ZMReadReceiptsUserSettingChangeNotification;

@interface ZMReadReceiptsManager : NSObject

//MARK: Public

+ (instancetype)shared;

- (BOOL)getReadReceiptsAccountOpt;

- (BOOL)getReadReceiptsUserSetting;

- (void)setReadReceiptsUserSetting:(BOOL)flag;

- (BOOL)isReadReceiptsOnForSession:(NSString *)sessionID;

- (NSInteger)getReadReceiptChannelMemberLimit;

- (NSString *)lastMyMessageIDBySessionID:(NSString *)sessionID;

- (NSString *)currentSubscribedMessageIDForSession:(NSString *)sessionID;

- (NSString *)getMyLastMessageReadReceiptCountWithSessionID:(NSString *)sessionID updateBlock:(ReadReceiptsUpdateBlock)resultBlock forceUpdate:(BOOL)forceUpdate;

- (void)getReadReceiptCountWithSessionID:(NSString *)sessionID messageID:(NSString *)messageID updateBlock:(ReadReceiptsUpdateBlock)resultBlock forceUpdate:(BOOL)forceUpdate;

- (void)updateReadReceiptsWithSessionID:(NSString *)sessionID;

- (NSArray<NSString *> *)getReadReceiptJidListForSessionID:(NSString *)sessionID messageID:(NSString *)messageID;

- (void)removeCallbackForSession:(NSString *)sessionID;

- (void)removeAllCallbacks;

- (void)showPopoverForSessionID:(NSString *)sessionID messageID:(NSString *)messageID ofView:(NSView *)view;

- (void)dismissPopover;

//MARK: CallBack

- (void)onGetReadReceiptWithReqID:(NSString *)reqID totalCount:(NSInteger)totalCount readCount:(NSInteger)readCount;

- (void)onUpdateReadReceiptWithSessionID:(NSString *)sessionID messageID:(NSString *)messageID totalCount:(NSInteger)totalCount readCount:(NSInteger)readCount;

- (void)onReadReceiptAccountSettingUpdate;

- (void)onReadReceiptSettingUpdate;

- (void)onReadReceiptShouldRefresh:(NSString *)reqID;

@end

NS_ASSUME_NONNULL_END
