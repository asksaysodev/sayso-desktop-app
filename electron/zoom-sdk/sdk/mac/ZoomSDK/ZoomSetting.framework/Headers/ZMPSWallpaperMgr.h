//
//  ZMPSWallpaperMgr.h
//  ZoomClips
//
//  Created by Michael Lin on 2023/2/13.
//

#import <Foundation/Foundation.h>
#import <ZoomSetting/ZMPSObject.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMPSWallpaperItem;

@interface ZMPSWallpaperMgr : ZMPSObject

- (void)unregisterSink;
- (void)registerSink;
- (ZMPSWallpaperItem *)noneItem;
- (NSUInteger)wallpaperItemsCount;
- (nullable ZMPSWallpaperItem *)wallpaperItemAtIndex:(NSUInteger)index;
- (NSArray<ZMPSWallpaperItem *> *)wallpaperItems;
@property (nonatomic, weak) ZMPSWallpaperItem *selectedWallpaperItem;

@end

NS_ASSUME_NONNULL_END
