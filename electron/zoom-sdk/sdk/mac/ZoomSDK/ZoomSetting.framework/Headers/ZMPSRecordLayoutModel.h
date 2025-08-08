//
//  ZMPSRecordLayoutModel.h
//  ZoomSetting
//
//  Created by Michael Lin on 2024/10/9.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, ZMPSLayoutType) {
    ZMPSLayoutTypeDefault = 0,
    ZMPSLayoutTypeVideoInLeft,
    ZMPSLayoutTypeVideoInRight,
    ZMPSLayoutTypeVideoInTop,
    ZMPSLayoutTypeVideoInBottom,
};

@interface ZMPSRecordLayoutModel : NSObject

- (instancetype)initWithLayoutType:(ZMPSLayoutType)layoutType;
@property (nonatomic, assign) BOOL isFlipped;
@property (nonatomic, assign) CGFloat videoSidePercentage;
@property (nonatomic, assign, readonly) CGFloat videoPercentage;
@property (nonatomic, assign, readonly) CGFloat sharePercentage;
@property (nonatomic, assign, readonly) ZMPSLayoutType layoutType;

@end

NS_ASSUME_NONNULL_END
