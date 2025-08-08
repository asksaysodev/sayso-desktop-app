//
//  ZMPSWallpaperItem.h
//  ZoomClips
//
//  Created by Michael Lin on 2023/1/6.
//

#import <Foundation/Foundation.h>
#import <ZoomSetting/ZMPSObject.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMPSWallpaperItem : ZMPSObject

@property (nullable, nonatomic, strong) NSString *name;
@property (nullable, nonatomic, strong) NSString *path;

/// Prefer to use `-fetchIconCompletion:` which uses completion block without main thread freeze.
@property (nonatomic, strong, nullable) NSImage *icon;

@property (nonatomic, assign) BOOL isNone;

@end

NS_ASSUME_NONNULL_END
