//
//  ZMMainTabsLifeCycleProtocol.h
//  ChatUI
//
//  Created by Kay Fu on 10/25/24.
//  Copyright © 2024 Zoom. All rights reserved.
//

NS_ASSUME_NONNULL_BEGIN

@protocol IZMMainTabsLifeCycle 

@optional
- (void)onTabRegister;
- (void)onTabItemInited:(ZMTabItem *)tabItem;
- (void)onTabSelected;
- (void)reloadTab;
- (void)onTabUnregister;
- (void)onTabReset;

@end

NS_ASSUME_NONNULL_END
