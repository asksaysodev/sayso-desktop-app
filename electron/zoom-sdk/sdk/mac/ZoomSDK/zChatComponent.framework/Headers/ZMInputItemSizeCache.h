//
//  ZMInputItemSizeCache.h
//  zChatUI
//
//  Created by Yong Zhou on 4/1/20.
//  Copyright © 2020 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ZMInputItemSizeCache : NSObject

+ (instancetype)sharedInstance;

- (NSSize)getItemSizeWithPath:(NSString *)path;

- (NSSize)convertImageSize:(NSSize)imageSize;

@end


