//
//  ZMAddCustomEmojiViewController.h
//  zChatComponent
//
//  Created by Mario Rao on 2023/5/15.
//

#import <zChatComponent/ZMEmojiModel.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMAddCustomEmojiViewController : ZMBaseViewController

@property (nonatomic, assign) ZMCustomEmojiSource source;
@property (nonatomic, assign) BOOL isFromEidt;

@end

NS_ASSUME_NONNULL_END
