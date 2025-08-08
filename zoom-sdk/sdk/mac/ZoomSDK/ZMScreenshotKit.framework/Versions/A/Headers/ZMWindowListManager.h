//
//  ZMWindowListManager.h
//  ZMScreenCapture
//
//  Created by javenlee on 2018/2/6.
//  Copyright © 2018 ZOOM. All rights reserved.
//

@import Cocoa;

@class ZMSCImageModel;
@interface ZMWindowListManager : NSObject

+ (ZMSCImageModel *)getScreenshot:(NSScreen *)screen;
+ (NSMutableArray <ZMSCImageModel *>*)getWindowInfo;
+ (CGImageRef)getImageForWindow:(NSInteger)windowID;


@end
