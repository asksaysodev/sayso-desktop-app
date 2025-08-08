//
//  ZMMessageFontCache.h
//  zChatComponent
//
//  Created by Yong Zhou on 11/27/24.
//

/*
 '- (NSFont *)convertFont:(NSFont *)fontObj toHaveTrait:(NSFontTraitMask)trait' is time consuming API.
 Cache font to fontCache for Performance.
 Since types of message-font are almostly the same, so, number of font cache will be limited.
 Should never worry about the memory that ZMMessageFontCache takes up.
 */

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMMessageFontCache : NSObject

+ (instancetype)sharedInstance;

+ (void)releaseInstance;

- (void)setFont:(NSFont*)font toKey:(NSString*)key;
- (NSFont*)fontFromKey:(NSString*)key;

@end

NS_ASSUME_NONNULL_END
