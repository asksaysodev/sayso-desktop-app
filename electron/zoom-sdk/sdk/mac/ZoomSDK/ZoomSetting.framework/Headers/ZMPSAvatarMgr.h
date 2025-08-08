//
//  ZMPSAvatarMgr.h
//  ZoomClips
//
//  Created by Michael Lin on 2023/2/13.
//

#import <Foundation/Foundation.h>
#import <ZoomSetting/ZMPSObject.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMPSAvatarItem;

@interface ZMPSAvatarMgr : ZMPSObject

- (void)unregisterSink;
- (void)registerSink;
- (BOOL)needPromptBiometricDisclaimer;
- (void)agreeBiometricDisclaimer;
- (void)disagreeBiometricDisclaimer;
- (NSUInteger)customAvatarItemsCount;
- (NSUInteger)animalAvatarItemsCount;
- (nullable ZMPSAvatarItem *)customAvatarItemAtIndex:(NSUInteger)index;
- (NSArray<ZMPSAvatarItem *> *)customItems;
- (nullable ZMPSAvatarItem *)animalAvatarItemAtIndex:(NSUInteger)index;
- (NSArray<ZMPSAvatarItem *> *)animalItems;

- (BOOL)is3DAvatarDataReady:(ZMPSAvatarItem *)avatarItem;
- (BOOL)isDownloading3DAvatar:(ZMPSAvatarItem *)avatarItem;
- (void)download3DAvatar:(ZMPSAvatarItem *)avatarItem;

@property (nonatomic, weak) ZMPSAvatarItem *selectedAvatarItem;
@property (nonatomic, strong) ZMPSAvatarItem *noneItem;

@end

NS_ASSUME_NONNULL_END
