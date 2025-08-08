//
//  ZMMessageCellViewDelegate.h
//  ZCommonUI
//
//  Created by groot.ding on 29/07/2020.
//  Copyright © 2020 zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <zChatComponent/ZMEmojiModel.h>

#ifndef ZMMessageCellViewDelegate_h
#define ZMMessageCellViewDelegate_h

NS_ASSUME_NONNULL_BEGIN

@class ZMBaseMessageCellView,ZMIMAvatarView,ZMChatMsgItemInfo,ZMReactionItemView,ZMFileEntity,ZMScheduleMeetingInfo;

@class ZMAddReplyMessageCellView, ZMChatAppContext, ZMChatAppShortcutInfo;

@protocol ZMMessageCellViewDelegate <NSObject>

@optional

- (ZMChatMsgItemInfo *)cellView:(ZMBaseMessageCellView *)cellView getItemByNode:(NSTreeNode *)node;

- (void)cellView:(ZMBaseMessageCellView *)cellView updateMessageHeightByItem:(ZMChatMsgItemInfo *)item;

- (void)cellView:(ZMBaseMessageCellView *)cellView updateMessageHeightWithoutReloadingByItem:(ZMChatMsgItemInfo *)item;

- (void)cellView:(ZMBaseMessageCellView *)cellView addReplyByItem:(ZMChatMsgItemInfo *)item;

- (void)cellView:(ZMBaseMessageCellView *)cellView loadMoreReplyByItem:(ZMChatMsgItemInfo *)item;

- (void)cellView:(ZMBaseMessageCellView *)cellView collapseReplyByItem:(ZMChatMsgItemInfo *)item;

- (void)cellView:(ZMBaseMessageCellView *)cellView moreButtonClick:(NSButton *)button item:(ZMChatMsgItemInfo *)item;


//reaction
- (void)cellView:(ZMBaseMessageCellView *)cellView addReactionWithButton:(ZMReactionItemView *)button item:(ZMChatMsgItemInfo *)item;

- (void)cellView:(ZMBaseMessageCellView *)cellView reactionUnicodeEmoji:(NSString *)unicodeEmoji orCustomEmoji:(ZMCustomEmojiModel *)emojiModel item:(ZMChatMsgItemInfo *)item;

- (void)cellView:(ZMBaseMessageCellView *)cellView showReactionTipWithButton:(ZMReactionItemView *)button item:(ZMChatMsgItemInfo *)item;

- (BOOL)cellView:(ZMBaseMessageCellView *)cellView checkReactionPannelVisibleWithItem:(ZMChatMsgItemInfo *)item;

//avatar
- (void)cellView:(ZMBaseMessageCellView *)cellView avatarClick:(ZMIMAvatarView *)avatarView isKeyPress:(BOOL)isKeyPress item:(ZMChatMsgItemInfo *)item;

- (void)cellView:(ZMBaseMessageCellView *)cellView avatarHover:(ZMIMAvatarView *)avatarView item:(ZMChatMsgItemInfo *)item;

//edit
- (void)cellView:(ZMBaseMessageCellView *)cellView commitEditMessageByItem:(ZMChatMsgItemInfo *)item;

- (void)cellView:(ZMBaseMessageCellView *)cellView cancelEditMessageByItem:(ZMChatMsgItemInfo *)item;

//mark
- (void)cellView:(ZMBaseMessageCellView *)cellView markActionByItem:(ZMChatMsgItemInfo *)item;

//jump
- (void)cellView:(ZMBaseMessageCellView *)cellView jumpToMessageByItem:(ZMChatMsgItemInfo *)item;

//delete
- (void)cellView:(ZMBaseMessageCellView *)cellView deleteMessageByItem:(ZMChatMsgItemInfo *)item;
- (void)cellView:(ZMBaseMessageCellView *)cellView deleteLocalMessageByItem:(ZMChatMsgItemInfo *)item;

//resend
- (void)cellView:(ZMBaseMessageCellView *)cellView resendByItem:(ZMChatMsgItemInfo *)item;

//e2e decode
- (void)cellView:(ZMBaseMessageCellView *)cellView tryDecodeE2EByItem:(ZMChatMsgItemInfo *)item;

//code sinppet
- (void)cellView:(ZMBaseMessageCellView *)cellView installCodeSippetPluginByItem:(ZMChatMsgItemInfo *)item;


- (void)cellView:(ZMBaseMessageCellView *)cellView previewCodeSippetByItem:(ZMChatMsgItemInfo *)item;

//file
- (void)cellView:(ZMBaseMessageCellView *)cellView openFile:(ZMChatMsgItemInfo *)item;

- (void)cellView:(ZMBaseMessageCellView *)cellView downloadButtonClick:(nullable NSButton *)button item:(ZMChatMsgItemInfo *)item file:(nullable ZMFileEntity *)fileEntity;

- (void)cellView:(ZMBaseMessageCellView *)cellView summarizeButtonClicked:(nullable NSButton *)button item:(ZMChatMsgItemInfo *)item file:(nullable ZMFileEntity *)fileEntity;

- (void)cellView:(ZMBaseMessageCellView *)cellView fileMoreButtonClick:(NSButton *)button item:(ZMChatMsgItemInfo *)item file:(ZMFileEntity *)fileEntity;
- (void)cellView:(ZMBaseMessageCellView *)cellView quickDownloadButtonClicked:(NSButton * _Nullable)button item:(ZMChatMsgItemInfo *)item file:(ZMFileEntity *)fileEntity;

//view pin history

- (void)cellView:(ZMBaseMessageCellView *)cellView viewPinHistory:(ZMChatMsgItemInfo *)item;

// file view operation

- (void)cellView:(ZMBaseMessageCellView *)cellView fileViewRightMouseClickedWithInfo:(ZMFileEntity *)fileModel atPoint:(NSPoint)aPoint;

- (void)cellView:(nullable ZMBaseMessageCellView *)cellView fileViewOpenFileWithInfo:(ZMFileEntity *)fileModel;

- (void)cellView:(ZMBaseMessageCellView *)cellView fileViewRightMouseClickedWithInfo:(ZMFileEntity *)fileModel;

- (void)cellView:(ZMBaseMessageCellView *)cellView textView:(NSTextView *)textView clickAttributes:(NSDictionary *)attributes charIndex:(NSInteger)charIndex item:(ZMChatMsgItemInfo *)item;

- (void)cellView:(ZMBaseMessageCellView *)cellView textView:(NSTextView *)textView hoverAttributes:(NSDictionary *)attributes charIndex:(NSInteger)charIndex item:(ZMChatMsgItemInfo *)item;

- (void)cellView:(ZMBaseMessageCellView *)cellView textView:(NSTextView *)textView sendAddonAction:(NSDictionary *)dict item:(ZMChatMsgItemInfo *)item;

- (void)cellView:(ZMBaseMessageCellView *)cellView textView:(NSTextView *)textView hoverAttachmentCell:(id<NSTextAttachmentCell>)cell charIndex:(NSInteger)charIndex item:(ZMChatMsgItemInfo *)item;

- (void)cellView:(ZMBaseMessageCellView *)cellView textView:(NSTextView *)textView hoverMentionsName:(NSString *)jid messageItem:(ZMChatMsgItemInfo *)item relativeRect:(NSRect)rect;

- (void)cellView:(ZMBaseMessageCellView *)cellView clickAttachmentCell:(id<NSTextAttachmentCell>)cell item:(ZMChatMsgItemInfo *)item;

- (void)cellView:(ZMBaseMessageCellView *)cellView openChatApp:(ZMChatAppContext *)context referenceView:(NSView *)referenceView;

// channel invite request
- (BOOL)cellView:(ZMBaseMessageCellView *)cellView approveChannelJoinRequest:(ZMChatMsgItemInfo *)item resultCallback:(void (^_Nullable)(UInt32 xmppResult, UInt32 joinBlockReason))resultCallback;

- (BOOL)cellView:(ZMBaseMessageCellView *)cellView declineChannelJoinRequest:(ZMChatMsgItemInfo *)item resultCallback:(void (^_Nullable)(UInt32 xmppResult))resultCallback;

- (void)cellView:(ZMBaseMessageCellView *)cellView openNewWebView:(NSString *)htmlPath item:(ZMChatMsgItemInfo *)item;

- (void)cellView:(ZMAddReplyMessageCellView *)cellView addReplyMessage:(NSAttributedString *)msg fileListArr:(NSArray<ZMFileEntity *> *)fileListArr item:(ZMChatMsgItemInfo *)item dismissLinks:(NSArray<NSString *> *)dismissLinks previewField:(NSMutableDictionary *)previewField;

- (void)cellView:(ZMAddReplyMessageCellView *)cellView updateFirstResponse:(BOOL)firstResponse item:(ZMChatMsgItemInfo *)item;
- (void)cellView:(ZMAddReplyMessageCellView *)cellView willAddToReuseCacheWithItem:(ZMChatMsgItemInfo *)item;
- (void)cellView:(ZMAddReplyMessageCellView *)cellView messageDidUpdated:(ZMChatMsgItemInfo *)item;

- (void)cellView:(ZMAddReplyMessageCellView *)cellView selectFileIntegration:(NSInteger)fileType;

// schedule meeting card

- (void)cellView:(ZMBaseMessageCellView *)cellView joinMeeting:(NSString *)sessionId meetingNumber:(long)meetingNumber password:(NSString *)password;

- (void)cellView:(ZMBaseMessageCellView *)cellView fromSession:(NSString *)sessionId openCMCchat:(NSString *)subCMCSessionId;

- (void)cellView:(ZMBaseMessageCellView *)cellView viewInCalendar:(NSString *)calendarLink;

- (void)cellView:(ZMBaseMessageCellView *)cellView copyInvitation:(NSString *)joinURL;

- (void)cellView:(ZMBaseMessageCellView *)cellView editMeeting:(NSString *)sessionId meetingNumber:(long)meetingNumber occurrenceTime:(long)occurrenceTime meetingMasterEventId:(NSString *)meetingMasterEventId;

- (void)cellView:(ZMBaseMessageCellView *)cellView deleteMeeting:(NSString *)sessionId meetingNumber:(long)meetingNumber occurrenceTime:(long)occurrenceTime meetingMasterEventId:(NSString *)meetingMasterEventId;

- (void)cellView:(ZMBaseMessageCellView *)cellView joinFromARoom:(NSString *)sessionId meetingNumber:(long)meetingNumber;

- (void)cellView:(ZMBaseMessageCellView *)cellView moreParticipantsClicked:(ZMScheduleMeetingInfo *)meetingInfo;

- (BOOL)cellView:(ZMBaseMessageCellView *)cellView isInMeeting:(long)meetingNumber;

//zapp meeting cell
- (void)cellView:(ZMBaseMessageCellView *_Nonnull)cellView  openZappByItem:(ZMChatMsgItemInfo *_Nonnull)item;
- (NSString *_Nonnull)cellView:(ZMBaseMessageCellView *_Nonnull)cellView getAppInvitationInfoUrl:(NSString *_Nonnull)appId linkType:(NSUInteger)type;

- (BOOL)cellView:(ZMBaseMessageCellView *_Nonnull)cellView canRespondToItem:(ZMChatMsgItemInfo *_Nonnull)item;

- (NSArray<ZMChatAppShortcutInfo *> *)chatAppThreadShortcutsForCellView:(ZMBaseMessageCellView *_Nonnull)cellView;
- (void)cellView:(ZMBaseMessageCellView *_Nonnull)cellView clickChatApp:(ZMChatAppShortcutInfo *)chatAppInfo sender:(NSButton *)button;

- (void)cellViewClickSummarizeNewMessages:(ZMBaseMessageCellView *_Nonnull)cellView;
- (void)cellViewCloseSummarizeNewMessages:(ZMBaseMessageCellView *_Nonnull)cellView;

- (void)cellView:(ZMBaseMessageCellView *)cellView forwardedMessageMoreButtonClick:(NSButton *)button item:(ZMChatMsgItemInfo *)item;

@end

NS_ASSUME_NONNULL_END

#endif /* ZMMessageDelegate_h */
