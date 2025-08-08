//
//  IZPMainMenuMgr.h
//  PTUIBridge
//
//  Created by fistice on 3/13/25.
//

//#import <PTUIBridge/ZMMainMenuConstants.h>

//#ifndef ZMMainMenuConstants_h
//#define ZMMainMenuConstants_h

// items' tags in system menu bar (move from ZPPTMainMenuMgr.h)
// twice move from ZMIMCommonDefine.h
enum{
    TAG_Memu_Item_Copy = 1001,
    TAG_Memu_Item_Select_All = 1002,
//    TAG_Menu_Item_Previous_Chat = 1003,
//    TAG_Menu_Item_Next_Chat = 1004,
//    TAG_Menu_Item_Callapse_All = 1005,
};

typedef NS_ENUM(NSUInteger, ZMSystemMenuItemType) {
    ZMSystemMenuItemTypeAbout,
    ZMSystemMenuItemTypeSettings,
    ZMSystemMenuItemTypeCheckUpdate,
    ZMSystemMenuItemTypeUninstall,
    ZMSystemMenuItemTypeJoinMeeting,
    ZMSystemMenuItemTypeStartMeeting,
    ZMSystemMenuItemTypeScheduleMeeting,
    ZMSystemMenuItemTypeScreenShare,
    ZMSystemMenuItemTypeHideZoom,
    ZMSystemMenuItemTypeHideOthers,
    ZMSystemMenuItemTypeShowAll,
    ZMSystemMenuItemTypeSwitchAccount,
    ZMSystemMenuItemTypeLogout,
    ZMSystemMenuItemTypeQuitZoom,
    
    ZMSystemMenuItemTypeEditFontBigger,
    ZMSystemMenuItemTypeEditFontSmaller,

    ZMSystemMenuItemTypeEditFindMenu,
    ZMSystemMenuItemTypeEditScreenshot,
    ZMSystemMenuItemTypeEditBack,
    ZMSystemMenuItemTypeEditForward,

    ZMSystemMenuItemTypeWindowMinimizeMenu,
    ZMSystemMenuItemTypeWindowCloseMenu,
    ZMSystemMenuItemTypeWindowZoomMenu,
    ZMSystemMenuItemTypeWindowSwitchMenu,
    ZMSystemMenuItemTypeWindowSwitchWideMenu,
    ZMSystemMenuItemTypeSwitchTabMenuItem,
    ZMSystemMenuItemTypeCloseSessionMenu,
    ZMSystemMenuItemTypeMenuItemMeetingControls,
//    ZMSystemMenuItemTypeRecommendToFriends,
    ZMSystemMenuItemTypeHelpHelp,
    ZMSystemMenuItemTypeWorkvivoHelp,
    ZMSystemMenuItemTypeZoomCommunity,
    ZMSystemMenuItemTypeProvideFeedback,
    ZMSystemMenuItemTypeReportProblem,
};
//#endif  ZMMainMenuConstants_h

/*
 
 <ZPMainMenuItemDelegate>
 
 [ZMSharedFor(IZPMainMenuMgr) registerDelegate:self forSystemMenuItem:xxx];

 #pragma mark ZPMainMenuItemDelegate
 - (BOOL)isValidForMenuItem:(ZMSystemMenuItemType)menuType menuItem:(NSMenuItem *)menuItem;// validateMenuItem
 {
    if(xxx == menuType) {
        return YES;
    }
    return NO;
 }
 
 - (void)menuItemClicked:(ZMSystemMenuItemType)menuType menuItem:(NSMenuItem *)menuItem;
 {
     if(xxx == menuType) {
         [self clickXxx];
     }
 }
 */


@protocol ZPMainMenuItemDelegate <NSObject>
@optional
- (void)menuItemClicked:(ZMSystemMenuItemType)menuType menuItem:(NSMenuItem *)menuItem;
- (BOOL)isValidForMenuItem:(ZMSystemMenuItemType)menuType menuItem:(NSMenuItem *)menuItem;// validateMenuItem
@end


@protocol IZPMainMenuMgr <NSObject>

- (void)registerDelegate:(id<ZPMainMenuItemDelegate>)delegate forSystemMenuItem:(ZMSystemMenuItemType)menuType;

- (__kindof NSMenuItem *)getMenuItemByType:(ZMSystemMenuItemType)itemType;
@end
