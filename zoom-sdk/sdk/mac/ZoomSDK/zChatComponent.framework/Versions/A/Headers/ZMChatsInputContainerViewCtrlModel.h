//
//  ZMChatsInputContainerViewCtrlModel.h
//  zChatComponent
//
//  Created by Yong Zhou on 10/31/22.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

// Created by @Cain will be removed in v5.13.0 @cain zhou

/// UI Configs Model
/// Suggest adding UI configs instead of business params for generality
@interface ZMChatsInputContainerViewCtrlModel : NSObject

@property (nonatomic, copy) NSString *sessionID; // business id
@property (nonatomic, assign) BOOL topLineViewHidden;
@property (nonatomic, assign) BOOL fileButtonHidden;
@property (nonatomic, assign) BOOL giphyButtonHidden;
@property (nonatomic, assign) BOOL moreButtonHidden;
@property (nonatomic, assign) BOOL emojiButtonHidden;
@property (nonatomic, assign) BOOL customEmojiHidden;
@property (nonatomic, assign) BOOL screenshotButtonHidden;
@property (nonatomic, assign) BOOL appButtonHidden;
@property (nonatomic, assign) BOOL isVideoPlayable; // will remove in v5.13.0 @cain zhou
@property (nonatomic, assign) BOOL canSendHybridMsg; // will
@property (nonatomic, assign) BOOL commandMenuHidden;
@end

NS_ASSUME_NONNULL_END
