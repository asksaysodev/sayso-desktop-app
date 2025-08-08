//
//  IZMMainTabsMgr.h
//  PTUIBridge
//
//  Created by fistice on 2/28/25.
//

#import <PTUIBridge/ZMPTRouterCenter.h>
#import <PTUIBridge/ZMTabDefineHeader.h>
#import <ZoomKit/ZMBaseKit.h>
#import <PTUIBridge/ZMBannerItem.h>

@class ZMBannerItem;

@protocol IZMMainTabsMgr <ZMPTRoutableObject, ZMViewProviding>

@property (nonatomic, readonly) ZMMainTabsType currentType;
@property (strong, readwrite, nullable) NSTouchBar *touchBar;


@property (readonly, getter=isViewLoaded) BOOL viewLoaded;
@property (readonly, getter=isViewAppeared) BOOL viewAppeared;
@property (retain, nonatomic, nullable) IBOutlet NSView *headerView;

- (void)onMenuSelectNextTab;
- (void)updateSidebarButtons;
- (void)selectTabAt:(NSUInteger)index;
- (void)updateStatusMessageActiveTimeBanner;
- (void)updateAICButton;
//header view
- (void)layoutHeader;
- (void)updateUserButton;

- (void)updateTabs;

//Psl custom logo
- (BOOL)isCoBrandEnable;
- (void)updateCobrandingLogo;
- (NSImage * _Nullable)getLightCobrandingLogo;
- (NSImage * _Nullable)getDarkCobrandingLogo;
- (NSImage * _Nullable)getLightCobrandingDialogLogo;
- (NSImage * _Nullable)getDarkCobrandingDialogLogo;
@end
