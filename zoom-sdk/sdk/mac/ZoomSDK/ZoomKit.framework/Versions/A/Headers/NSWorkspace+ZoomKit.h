//
//  NSWorkspace+ZoomKit.h
//  ZCommonUI
//
//  Created by 10.14 on 2020/4/1.
//  Copyright © 2020 zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSWorkspace (ZoomKit)
-(NSArray *)getWhiteListArray;
-(NSArray *)getBlackListArray;
-(void)setServerBlackListArray:(nullable NSArray<NSString*>*)blackListArray;//ZOOM-566012
-(NSArray<NSString*>*)getServerBlackListArray;
- (BOOL)safeOpenFile:(NSString *)filePath;
- (BOOL)openNewWindowWithURL:(NSURL *)url completionHandler:(nullable void (^)(NSInteger pid, NSError *_Nullable error))completionHandler;
@end

NS_ASSUME_NONNULL_END
