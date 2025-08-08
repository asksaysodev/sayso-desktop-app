//
//  ZMWebViewBaseView.h
//  zChatUI
//
//  Created by Jed Zheng on 29/11/2022.
//  Copyright © 2022 Zoom. All rights reserved.
//

#import <ZoomKit/ZoomKit.h>
#import "zoom_client_conf.h"
#ifdef ENABLE_ZAPP
#import <zUnifyWebView/ZMUnifyWebViewProtocol.h>
#import <zUnifyWebView/ZMUnifyWebViewDefine.h>
#import <zUnifyWebView/ZMUnifyWebView.h>
#endif

@interface ZMWebViewBaseView : ZMBaseView
@property (nonatomic, retain) NSString *moduleIdentifier;
@property (nonatomic, retain) NSString *identifierValue;
@property (nonatomic, retain) ZMProfileConfig *floatConfig;
@property (nonatomic, assign) NSInteger resourceType;
@property (nonatomic, assign) NSSize zm_intrinsicContentSize;

#ifdef ENABLE_ZAPP
@property(nonatomic, retain)id<ZMUnifyWebViewInstance> zAppWebView;
#endif

@end
