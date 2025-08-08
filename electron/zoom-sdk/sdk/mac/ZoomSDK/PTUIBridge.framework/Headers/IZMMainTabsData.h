//
//  IZMMainTabsData.h
//  zChatUI
//
//  Created by Kay Fu on 10/25/24.
//  Copyright © 2024 Zoom. All rights reserved.
//

#import "IZMMainTabsLifeCycle.h"
NS_ASSUME_NONNULL_BEGIN

@protocol ZMPTTabNavigationProtocol
@optional
- (void)navigationToURL:(NSString *)url;
- (void)navigationToURL:(NSString *)url isNavigateAction:(BOOL)isNavigateAction;
- (void)navigationWithExtParams:(NSDictionary *)extParams;
@end
 
@protocol ZMBaseTabsDataProtocol <NSObject,IZMMainTabsLifeCycle>

@required
@property (nonatomic, copy) NSString *tabName;
@property (nonatomic, strong, nullable) NSImage *tabIcon;
@property (nonatomic, copy) NSString *zmIdentifier;
@property (nonatomic, assign) CGFloat priority;
@property (nonatomic, strong) NSView *displayView;

@property (nonatomic, assign) BOOL isFeatureEnable;
@optional
@property (nonatomic, copy) NSString *toolTip;
@property (nonatomic, assign) BOOL isSupportHistory;
@property (nonatomic, assign) BOOL isSupportDeeplink;
@property (nonatomic, assign) BOOL isSupportReload;
@property (nonatomic, assign) BOOL isSupportPopout;
@property (nonatomic, assign) BOOL isEnabel;
@property (nonatomic, assign) BOOL isSupportOffLineMode;
@property (nonatomic, assign) BOOL showNew;
@property (nonatomic, assign) BOOL showBeta;
@property (nonatomic, assign) BOOL showFreeTrial;

@end

@protocol ZMUnifyTabsDataProtocol <ZMBaseTabsDataProtocol>

@required
@property (nonatomic, copy) NSString *zAppId;

@optional
/// Default to NO
@property (nonatomic, assign) BOOL isSupportDownload;

/// Default to NO
@property (nonatomic, assign) BOOL isNeedCustomCache;

@property (nonatomic, copy) NSString *processDisplayName;
@property (nonatomic, copy) NSString *webUrl;

@end
 

NS_ASSUME_NONNULL_END
