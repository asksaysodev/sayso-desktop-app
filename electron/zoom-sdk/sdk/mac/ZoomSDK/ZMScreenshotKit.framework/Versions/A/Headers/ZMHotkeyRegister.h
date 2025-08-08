//
//  ZMHotkeyRegister.h
//  ScreenCapture
//
//  Created by javenlee on 2018/4/4.
//  Copyright © 2018 ZOOM. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ZMHotkeyRegister : NSObject

@property (nonatomic, assign) BOOL supportHotKey;

+ (instancetype)shareRegister;

- (void)registerHotkeys;
- (void)unRegisterHostKeys;

- (void)registerEnterESCRef;
- (void)unregisterEnterESCRef;

@end
