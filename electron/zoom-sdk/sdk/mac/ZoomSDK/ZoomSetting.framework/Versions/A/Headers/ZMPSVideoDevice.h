//
//  ZMPSVideoDevice.h
//  ZoomSetting
//
//  Created by Michael Lin on 2024/10/31.
//

#import <Foundation/Foundation.h>
#ifdef __cplusplus
#import <zPSApp/define/PSDefine.h>
#endif
#import <ZoomSetting/ZMPSDevice.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_OPTIONS(NSInteger, ZMPSBioFaceStatusType) {
    ZMPSBioFaceStatusTypeNone = 0,
    ZMPSBioFaceStatusTypeSingleFace = 1 << 0,
    ZMPSBioFaceStatusTypeFaceInRect = 1 << 1,
    ZMPSBioFaceStatusTypeGoodFacePose = 1 << 2,
    ZMPSBioFaceStatusTypeNoFaceObstruction = 1 << 3,
};

typedef NS_ENUM(NSInteger, ZMPSBioVerifyStatus) {
    ZMPSBioVerifyStatusVerifying = 0,
    ZMPSBioVerifyStatusSuccess,
    ZMPSBioVerifyStatusFailure,
};

typedef NS_ENUM(NSInteger, ZMPSBioInfoMode) {
    ZMPSBioInfoModeInvalid = -1,
    ZMPSBioInfoModeNone = 0,
    ZMPSBioInfoModeBlinkEyes,
    ZMPSBioInfoModeOpenMouth,
    ZMPSBioInfoModeTurnHeadLeft,
    ZMPSBioInfoModeTurnHeadRight,
    ZMPSBioInfoModeNodHead,
    ZMPSBioInfoModeCompliance,
};

@interface ZMPSVideoDevice: ZMPSDevice

#ifdef __cplusplus
- (nullable instancetype)initWithVideoDevice:(const ps::PSVideoDevice&)device scene:(ZMPSScene)scene;
#endif
// FIXME: Test code, fix me later - Michael.l. useless, delete later
- (void)checkFaceCountAndAreaPercentage:(nullable void (^)(NSUInteger faceCount, NSEdgeInsets areaPercentage))completion;

@property (nonatomic, assign) NSRect bioInfoFaceAreaPercentage;
- (void)enableBioInfoVerify:(ZMPSBioInfoMode)mode;
- (void)updateBioInfoVerify:(ZMPSBioInfoMode)mode;
- (void)disableBioInfoVerify;
- (BOOL)isBioInfoVerifying;

@end

NS_ASSUME_NONNULL_END
