//
//  ZMSmartReplyOptionView.h
//  zChatComponent
//
//  Created by Zoro.Fu on 2023/7/18.
//

#import <ZoomKit/ZoomKit.h>

NS_ASSUME_NONNULL_BEGIN

typedef void(^ZMSmartReplySelectedCallback)(NSString *sessionid, NSString * _Nullable threadId, NSString *reply);

@interface ZMSmartReplyOptionView : ZMBaseView

@property (nonatomic, assign) CGFloat maxWidth;

@property (nonatomic, assign) CGFloat buttonSpacing;

@property (nonatomic, assign) NSEdgeInsets contentInsets;

@property (nonatomic, readonly) CGSize contentSize;

@property (nonatomic, assign) BOOL multiLines;

/// for compose input
@property (nonatomic, weak) id associatedNode;
/// for thread add reply
@property (nonatomic, weak) NSView *associatedCell;

@property (nonatomic, copy) NSString *mentionStr;

@property (nonatomic, readonly) ZMPureWindow *popupWindow;

+ (instancetype)sharedView;

- (void)showReplys:(NSArray<NSString *> *)replyArr
         inSession:(NSString *)sessionId
            thread:(nullable NSString *)threadId
  selectedCallback:(ZMSmartReplySelectedCallback)callback;

- (void)hideForSession:(NSString *)sessionId thread:(nullable NSString *)threadId;

- (void)forceHide;

- (BOOL)isShowInSession:(NSString *)sessionId thread:(nullable NSString *)threadId;

- (BOOL)isShowInSession:(NSString *)sessionId;

@end

NS_ASSUME_NONNULL_END
