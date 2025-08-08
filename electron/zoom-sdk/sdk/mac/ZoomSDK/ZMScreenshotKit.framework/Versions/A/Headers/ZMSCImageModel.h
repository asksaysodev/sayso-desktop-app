//
//  ZMCaptureModel.h
//  ZMScreenCapture
//
//  Created by javenlee on 2018/2/11.
//  Copyright © 2018 ZOOM. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ZMSCImageModel : NSObject{
    CGImageRef _cgImage;
    NSImage *_image;
}
@property (assign) CGFloat              scaleFactor;
@property (assign) NSInteger            windowLayer;
@property (assign) NSRect               frame;
@property (assign) NSInteger            windowID;
@property (nonatomic ,strong) NSImage   *image;

- (void)setCGImage:(CGImageRef)cgImage;

@end
