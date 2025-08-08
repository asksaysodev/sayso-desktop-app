//
//  ZMSkinToneButton.h
//  ZoomUnit
//
//  Created by Mario Rao on 2023/7/20.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@protocol ZMSkinToneButtonDelegate <NSObject>

- (void)skinToneButtonDidSetSkinTone:(NSUInteger)skinTone;

@end

@interface ZMSkinToneButton : NSView
@property (nonatomic, assign) BOOL forceDarkMode;
@property (nonatomic, weak) id<ZMSkinToneButtonDelegate> delegate;

+ (NSInteger)expWidth;
+ (BOOL)isSupportSkinTone;
- (void)cleanUp;
@end


NS_ASSUME_NONNULL_END
