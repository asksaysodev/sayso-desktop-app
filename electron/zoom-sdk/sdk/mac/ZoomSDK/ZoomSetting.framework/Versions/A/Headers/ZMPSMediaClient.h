//
//  ZMPSMediaClient.h
//  ZoomClips
//
//  Created by Michael Lin on 2023/1/11.
//

#import <Foundation/Foundation.h>
#import <ZoomSetting/ZMPSObject.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, ZMPSResolutionType) {
    ZMPSResolutionTypeNone = 0,
    ZMPSResolutionType90P = 1 << 0,
    ZMPSResolutionType180P = 1 << 1,
    ZMPSResolutionType270P = 1 << 2,
    ZMPSResolutionType360P = 1 << 3,
    ZMPSResolutionType720P = 1 << 4,
    ZMPSResolutionType1080P = 1 << 5,
    
    ZMPSResolutionTypeAuto = 1 << 30,
};

@interface ZMPSMediaClient : ZMPSObject

- (NSUInteger)supportResolutions;
- (ZMPSResolutionType)getRecommendedRecordResolution;
- (void)setupProxy;
- (BOOL)supportSmartVirtualBackground;
- (BOOL)supportSmartVideoVirtualBackground;
- (BOOL)support3DAvatarEffect;
- (void)notifyAomhostReady;
- (void)notifyViperExReady;

@end

NS_ASSUME_NONNULL_END
