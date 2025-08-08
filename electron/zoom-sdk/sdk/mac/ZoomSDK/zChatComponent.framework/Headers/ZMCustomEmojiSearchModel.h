//
//  ZMCustomEmojiSearchModel.h
//  zChatComponent
//
//  Created by Mario Rao on 2023/8/4.
//

#import <Foundation/Foundation.h>

@class ZMEmojiModel;

NS_ASSUME_NONNULL_BEGIN

@interface ZMCustomEmojiSearchModel : NSObject

@property (nonatomic, assign) BOOL isLoadingMore;
@property (nonatomic, strong) NSString *searchKey;
@property (nonatomic, strong) NSString *searchAfter;
@property (nonatomic, strong) NSMutableOrderedSet<ZMEmojiModel *> *resultEmojis;

@end

NS_ASSUME_NONNULL_END
