//
//  ZMCustomEmojiView.h
//  zChatComponent
//
//  Created by Mario Rao on 2023/6/30.
//

#import <ZoomKit/ZoomKit.h>
#import <zChatComponent/ZMEmojiModel.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, ZMCustomEmojiShowMode) {
    ZMCustomEmojiShowModeDownloadAndShow,
    ZMCustomEmojiShowModeOnlyLoading,
    ZMCustomEmojiShowModeShowWithoutDownload,
};

@interface ZMCustomEmojiView : ZMBaseView <ZMTextAttachmentContentViewProtocol>

@property (nonatomic, strong, nullable) ZMCustomEmojiModel *emojiModel;
@property (nonatomic, assign) NSEdgeInsets emojiInsets;
@property (nonatomic, assign) BOOL disableUserInteraction;
@property (nonatomic, assign) BOOL forceDarkMode;
@property (nonatomic, copy, nullable) dispatch_block_t notifyEmojiDeleted;
@property (nonatomic, copy, nullable) dispatch_block_t notifyEmojiShowed;

- (instancetype)initWithEmojiInsets:(NSEdgeInsets)insets;

- (BOOL)isEmojiDeleted;

- (void)setCustomEmojiWidth:(CGFloat)width;

- (void)setErrorContentWidth:(CGFloat)width;

- (void)updateEmojiModel:(ZMCustomEmojiModel *)emojiModel showMode:(ZMCustomEmojiShowMode)showMode;

- (void)downloadAndShowEmojiIfNeeded;

@end

NS_ASSUME_NONNULL_END
