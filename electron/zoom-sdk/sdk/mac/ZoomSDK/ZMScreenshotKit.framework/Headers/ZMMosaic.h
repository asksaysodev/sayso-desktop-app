//
//  ZMMosaic.h
//  ZMScreenshotKit
//
//  Created by Javenlee.Li on 2025/4/11.
//  Copyright © 2025 zoom. All rights reserved.
//

#import "ZMGraphic.h"

@interface ZMMosaic : ZMGraphic

@property (nonatomic, strong) NSImage *mosaicImage;
@property (nonatomic, assign) CGImageRef cachedMosaicImage;

+ (ZMMosaic *)mosaicClass;

@end

