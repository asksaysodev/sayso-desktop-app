//
//  ZMDisasterModeChangeProtocol.h
//  ChatUI
//
//  Created by fistice on 2024/8/13.
//  Copyright © 2024 Zoom. All rights reserved.
//

#ifndef ZMDisasterModeChangeProtocol_h
#define ZMDisasterModeChangeProtocol_h
#import <Cocoa/Cocoa.h>

@protocol ZMDisasterModeChangeProtocol <NSObject>

- (void)onDisasterModeChanged:(BOOL)enabled;
@optional
- (void)onProxyModeCheckCallback:(BOOL)result;
- (void)onMockLoginFinish;
@end

#endif /* ZMDisasterModeChangeProtocol_h */
