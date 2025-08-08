//
//  ZMIMMessageLayoutConfig.h
//  zChatComponent
//
//  Created by groot.ding on 9/28/23.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol ZMEditMessageViewControllerProtocol;

@interface ZMIMMessageLayoutConfig : NSObject

@property (nonatomic, assign) BOOL needHideEmoji;
@property (nonatomic, assign) BOOL needHideReaction;
@property (nonatomic, assign) BOOL needHideCustomEmoji;
@property (nonatomic, assign) BOOL needHideGiphy;
@property (nonatomic, assign) BOOL needHideReply;
@property (nonatomic, assign) BOOL needForceCaptureWindow;
@property (nonatomic, assign) BOOL needHideAtList;
@property (nonatomic, assign) BOOL isVideoPlayable;
// event (a trick for long long way communication)
@property (nonatomic, weak, nullable) id<ZMEditMessageViewControllerProtocol> editDelegate;

@end

NS_ASSUME_NONNULL_END
