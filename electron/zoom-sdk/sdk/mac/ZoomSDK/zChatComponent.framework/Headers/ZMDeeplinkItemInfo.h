//
//  ZMDeeplinkItemInfo.h
//  zChatComponent
//
//  Created by groot.ding on 5/30/24.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMDeepLinkEntity.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMDeeplinkItemInfo : NSObject

@property (nonatomic,retain, nullable) ZMDeepLinkEntity *deeplinkEntity;

@property (nonatomic,assign) NSSize size;

@property (nonatomic,assign) NSSize descSize;

@property (nonatomic,assign) BOOL isPreviewExpanded;

@property (nonatomic,assign) BOOL isReply;

- (NSSize)calculateSizeWithMaxWidth:(NSUInteger)maximumAllowableWidth;

- (NSDictionary *_Nonnull)titleAttributes;

- (NSDictionary *_Nullable)timestampAttributes;

- (NSDictionary *_Nonnull)descriptionAttributes;

@end

NS_ASSUME_NONNULL_END
