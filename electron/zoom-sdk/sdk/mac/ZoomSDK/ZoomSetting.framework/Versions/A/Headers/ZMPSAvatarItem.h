//
//  ZMPSAvatarItem.h
//  ZoomClips
//
//  Created by Michael Lin on 2023/1/6.
//

#import <Foundation/Foundation.h>
#import <ZoomSetting/ZMPSObject.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMPSAvatarItem : ZMPSObject

@property (nullable, nonatomic, strong) NSString *name;

/// Prefer to use `-fetchIconCompletion:` which uses completion block without main thread freeze.
@property (nonatomic, strong, nullable) NSImage *icon;

- (NSImage *)generateIcon;

@property (nonatomic, nullable, strong) NSString *thumbPath;

@property (nonatomic, assign) BOOL isNone;
@property (nonatomic, assign) BOOL isCustom;
@property (nonatomic, assign) NSInteger type;
@property (nonatomic, assign) NSInteger index;

- (BOOL)downloadIfNeeded;

@end

NS_ASSUME_NONNULL_END
