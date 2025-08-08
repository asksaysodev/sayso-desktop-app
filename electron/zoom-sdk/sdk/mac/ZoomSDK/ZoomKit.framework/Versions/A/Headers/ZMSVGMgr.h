//
//  ZMSVGMgr.h
//  ZMImageRes
//
//  Created by francis zhuo on 11/12/2019.
//  Copyright © 2019 zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMSVGMgr : NSObject

+ (nonnull instancetype)sharedMgr;
- (nonnull instancetype)parserWithFileURL:(NSURL*_Nonnull)fileURL;
- (nullable NSImage*)imageName:(NSString*_Nonnull)imageName;
- (nullable NSImage*)imageName:(NSString*_Nonnull)imageName bundle:(NSString *_Nonnull)bundle;
- (nullable NSImage*)imageName:(NSString*_Nonnull)imageName scale:(CGFloat)scale;
- (nullable NSImage*)imageName:(NSString*_Nonnull)imageName scale:(CGFloat)scale bundle:(NSString *_Nonnull)bundle;
- (BOOL)isImageExist:(NSString* _Nonnull)imageName;
@end

NS_ASSUME_NONNULL_END
