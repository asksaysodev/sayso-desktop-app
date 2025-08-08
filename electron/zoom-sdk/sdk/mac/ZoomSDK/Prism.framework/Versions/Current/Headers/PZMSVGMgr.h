//
//  ZMSVGMgr.h
//  ZMImageRes
//
//  Created by francis zhuo on 11/12/2019.
//  Copyright © 2019 zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AppKit/NSBezierPath.h>
#import <Prism/PZMSvgElement.h>

NS_ASSUME_NONNULL_BEGIN

__attribute__((visibility("hidden")))
@interface PZMSVGMgr : NSObject

+ (nonnull instancetype)sharedMgr;
- (nullable NSImage*)imageForIdentifier:(NSString*_Nonnull)imageName;
- (nullable NSImage*)imageForIdentifier:(NSString*_Nonnull)imageName scale:(CGFloat)scale;
- (nullable PZMSvgElement *)svgElementForIdentifier:(nonnull NSString *)identifier;
- (nullable NSBezierPath *)firstPathForIdentifier:(NSString *)imageName scale:(CGFloat)scale;

@end

NS_ASSUME_NONNULL_END
