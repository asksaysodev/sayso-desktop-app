//
//  PTUIBridge.h
//  PTUIBridge
//
//  Created by Molay.Yu on 2024/6/26.
//

#import <Foundation/Foundation.h>
//! Project version number for PTUIBridge.
FOUNDATION_EXPORT double PTUIBridgeVersionNumber;

//! Project version string for PTUIBridge.
FOUNDATION_EXPORT const unsigned char PTUIBridgeVersionString[];

// In this header, you should import all the public headers of your framework using statements like #import <PTUIBridge/PublicHeader.h>

#import <PTUIBridge/ZMPTTheme.h>
#import <PTUIBridge/ZMTabItem.h>
#import <PTUIBridge/ZMTabWidgets.h>

#ifdef __OBJC__

#import <PTUIBridge/ZMPTRouterCenter.h>
#import <PTUIBridge/IZMCheckUpdateMgr.h>
#import <PTUIBridge/IZMPTAlertMgr.h>
#import <PTUIBridge/ZMClipsProtocol.h>
#import <PTUIBridge/ZMPTUIEventCenterProtocol.h>
#import <PTUIBridge/ZPMainWindowEventProtocol.h>
#import <PTUIBridge/ZPMainWindowProtocol.h>
#import <PTUIBridge/ZMSidebarLeftTabProtcol.h>
#import <PTUIBridge/IZMPTSidePanelMgr.h>
#import <PTUIBridge/IZMPTUIMgr.h>
#import <PTUIBridge/IZPMainMenuMgr.h>
#import <PTUIBridge/IZMPTBannerMgr.h>
#import <PTUIBridge/ZMPTUIHelper.h>
#import <PTUIBridge/IZMPTMeetingUpdate.h>
#import <PTUIBridge/IZMSupportMgr.h>

//nav tabs
#import <PTUIBridge/IZMMainTabsData.h>
#import <PTUIBridge/IZMMainTabsLifeCycle.h>
#import <PTUIBridge/IZMMainTabsMgr.h>

//Session&helper
#ifdef __cplusplus
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wignored-attributes"
#import <PTUIBridge/ZMPTHelper.h>
#import <PTUIBridge/IZMCCIHelper.h>
#pragma clang diagnostic pop
#endif

#endif

/*
@routerable(ZMPTMgr,IZMPTMgr)
 */

