//
//  ZMMessageUIHelper.h
//  zChatComponent
//
//  Created by groot.ding on 7/26/23.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMMessageAdapter;
@class ZMFileEntity;
@class ZMCTemplateEditCommandParameters;

@interface ZMMessageUIHelper : NSObject

+ (void)topPinMessage:(nonnull ZMMessageAdapter *)message inView:(nullable NSView *)view;
+ (void)unTopPinMessage:(nonnull ZMMessageAdapter *)message inView:(nullable NSView *)view;;
+ (void)hideTopPinMessage:(nonnull ZMMessageAdapter *)message inView:(nullable NSView *)view;;
+ (void)removePinMessage:(nonnull ZMMessageAdapter *)message inView:(nullable NSView *)view;;

+ (void)deleteHybridItem:(ZMFileEntity *)fileItem fromView:(NSView *)fromView;

+ (void)previewPublicChanelWithChannelId:(nonnull NSString *)channelId channelName:(nullable NSString*) channelName attachedWindow:(nullable NSWindow*)inWindow;

+ (void)beginEditCommonAppMessageWithInfo:(nonnull ZMCTemplateEditCommandParameters *)info onWindow:(nullable NSWindow *)window;
+ (void)beginEditCommonAppMessageByPopup:(nonnull ZMCTemplateEditCommandParameters *)info;
+ (void)dismissEditCommonAppPopupWindow;

+ (void)showMCCMembersPanel:(nonnull NSString *)meetingID sessionID:(nonnull NSString*)sessionID messageID:(nonnull NSString*)messageID onWindow:(nullable NSWindow*)inWindow;

+ (void)showMentionGroupMembersListPopover:(nonnull NSString *)mentionGroupId channelId:(nonnull NSString *)channelId point:(NSPoint)point view:(nonnull NSView *)view;

+ (void)showMentionGroupMembersListPopover:(nonnull NSString *)mentionGroupId channelId:(nonnull NSString *)channelId point:(NSPoint)point view:(nonnull NSView *)view frame:(CGRect)rect;

@end

NS_ASSUME_NONNULL_END
