//
//  ZMMTVideoTheme.h
//  ZoomSetting
//
//  Created by Francis Zhuo on 2024/2/4.
//

#import <Foundation/Foundation.h>
#import <ZoomKit/ZoomKit.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, ZMMTVideoCornerType){
    ZMMTVideoCornerType_Default,
    ZMMTVideoCornerType_NoCorner,
    ZMMTVideoCornerType_Outside,
};

@interface ZMMTVideoTheme : NSObject
@property(strong) ZMPigment* backsplash;
@property CGFloat videoTileGap;
@property ZMMTVideoCornerType cornerType;
@property CGFloat cornerRadius;

- (void)setTheme:(ZMMTVideoTheme*)theme;
@end

@interface ZMMTFloatPanelTheme : NSObject
@property(strong) ZMPigment* frame;

- (void)setTheme:(ZMMTFloatPanelTheme*)theme;
@end

@interface ZMMTTitlebarTheme : NSObject
@property(strong) ZMPigment* titlebar;
@property(strong) ZMPigment* headerInfo;

- (void)setTheme:(ZMMTTitlebarTheme*)theme;
@end
NS_ASSUME_NONNULL_END
