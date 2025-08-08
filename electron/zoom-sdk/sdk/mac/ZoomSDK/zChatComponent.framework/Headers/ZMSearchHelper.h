//
//  ZMSearchHelper.h
//  zChatComponent
//
//  Created by groot.ding on 8/7/23.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMSearchResultItem;
@class ZMBuddyAdapter;

@interface ZMSearchHelper : NSObject

+ (NSArray <ZMSearchResultItem *> *)sortContactResultByKeyAndMsgTimeWithItems:(NSArray <ZMSearchResultItem *> *)items;

+ (NSArray <ZMSearchResultItem *> *)sortContactResultByKeyAndMsgTimeWithItemsV2:(NSArray <NSString *> *)results key:(NSString *)key;

+ (NSArray <ZMSearchResultItem *> *)getResultItemsBySessionIds:(NSArray <NSString *> *)sessionIds;

+ (NSArray <NSString *> *)getResultItemsIds:(NSArray <ZMSearchResultItem *> *)items;

+ (NSArray <ZMSearchResultItem *> *)sortContactResultByKeyAndMsgTimeWithSessionIds:(NSArray <NSString *> *)results;

+ (NSArray <NSString *> *)sortContactSearchResultWithResults:(NSArray *)resluts withFilter:(NSInteger)filterValue;

+ (void)sortSearchResultItemsByName:(NSMutableArray <ZMSearchResultItem *> *)items;

+ (BOOL)match:(NSString *)key buddy:(ZMBuddyAdapter *)buddy;

+ (BOOL)isPassed:(ZMBuddyAdapter*)buddyAdapter withFilter:(NSInteger)filterValue;

@end

NS_ASSUME_NONNULL_END
