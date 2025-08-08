//
//  ZMVideoEventProtocol.h
//  ChatUI
//
//  Created by Vinson Wang on 2024/2/24.
//  Copyright © 2024 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMConfSession;

@protocol ZMVideoEventProtocol <ZMRoutableObject>

@optional
- (void)onCameraStatusChanged:(unsigned int)value;

- (void)onCameraSelectedWillChange:(NSString *)deviceId deviceName:(NSString *)deviceName;

- (void)onCameraSelectedChanged:(nullable ZMConfSession *)session;

- (void)onChangeMirror;

- (void)startPreview;

- (void)stopPreview;

- (void)updatePreview;

- (void)onVirtualBackgroundSettingStatusChanged;

@end

NS_ASSUME_NONNULL_END
