//
//  ZMSvgElement.h
//  ZMImageRes
//
//  Created by francis zhuo on 12/12/2019.
//  Copyright © 2019 zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

__attribute__((visibility("hidden")))
@interface PZMSvgElement : NSObject<NSCopying>
@property(assign) CGFloat scale;
@property(nullable,retain) NSString* elementName;
@property(nullable,retain) NSDictionary* attribute;
@property(nullable,retain) NSArray* childElements;
@end

NS_ASSUME_NONNULL_END
