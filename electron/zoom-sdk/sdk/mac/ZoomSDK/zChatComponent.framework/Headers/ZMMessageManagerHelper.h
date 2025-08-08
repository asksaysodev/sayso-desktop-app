//
//  ZMMessageManagerHelper.h
//  zChatComponent
//
//  Created by Groot Ding on 2022/7/21.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMMessageAdapter;
@class ZMFileEntity;

@interface ZMMessageManagerHelper : NSObject

+ (void)setNeedDisableShortCutsCmdF:(BOOL)isNeed;
+ (BOOL)needDisableShortCutsCmdF;

+ (BOOL)canStartRecordVoiceMessage:(NSView*)fromView;

+ (BOOL)canStartRecordVideoMessage:(NSView *)fromView;

+ (NSURL*)localDownloadPathForFileName:(NSString*)fileName;

+ (void)showPopoverInSession:(NSString *)sessionID title:(NSString *)title relativeView:(NSView *)relativeView delayTime:(CGFloat)delayTime autoClose:(BOOL)autoClose;

+ (void)resendMessage:(ZMMessageAdapter*)inMsgInfo fromView:(NSView *)fromView;

+ (void)deleteMessage:(ZMMessageAdapter*)inMsgInfo fromView:(NSView *)fromView;

+ (void)removeMessage:(ZMMessageAdapter *)inMsgInfo fromView:(NSView *)fromView;

+ (BOOL)canPostMessageInSession:(NSString *)sessionID;

@end

NS_ASSUME_NONNULL_END
