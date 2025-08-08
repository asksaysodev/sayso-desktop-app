//
//  NSView+ZMSettingRouter.h
//
//  Created by kay.fu on 3/26/24.
//  Copyright © 2024 zoom.us. All rights reserved.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

//setting router
@interface NSView (ZMSettingRouter)

- (void)zm_setSettingRouterAnchor:(NSString *)anchor;
- (NSString *)zm_getSettingRouterAnchor;

@end

NS_ASSUME_NONNULL_END
