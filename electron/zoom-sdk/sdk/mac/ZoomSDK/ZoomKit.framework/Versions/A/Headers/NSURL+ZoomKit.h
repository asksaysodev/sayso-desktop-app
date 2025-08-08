//
//  NSURL+ZoomKit.h
//  ZoomKit
//
//  Created by Huxley Yang on 8/24/23.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSURL (ZoomKit)

@property (readonly, getter=isValidWebURL) BOOL validWebURL;

- (nullable NSURL *)appendQueryItemWithName:(NSString *)name value:(NSString *)value;

@end

NS_ASSUME_NONNULL_END
