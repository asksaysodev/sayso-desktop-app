//
//  ZMBannerItem.h
//  zChatUI
//
//  Created by Huxley on 2018/5/22.
//  Copyright © 2018 Zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "ZMTabDefineHeader.h"
#import <ZoomKit/ZoomKit.h>

typedef NS_ENUM(NSUInteger, ZMBannerItemPriority) {
    ZMBannerItemPriorityLowest = 0,//!< RESERVED
    ZMBannerItemPriorityInClientBanner = 10,//!< InClientBanner
    ZMBannerItemPriorityStatusMessage = 10,//!< statusMessage
    ZMBannerItemPriorityLow = 30,//!< "A new version is available"
    ZMBannerItemPriorityNormal = 60,//!< Upcoming meetings,// bi-sync(only in meeting list tab)
    ZMBannerItemPriorityCCI = 70,//!< CCI Call
    ZMBannerItemPriorityHigh = 80,//!< Sip phone calling, transfer meetings, showing at the same time
    ZMBannerItemPriorityService = 90,//!< Service Banner: net work status
    ZMBannerItemPriorityRequired = 100,//!< RESERVED
    ZMBannerItemPriorityAlwaysShownOnBottom = 101,
};
typedef NS_ENUM(NSUInteger, ZMBannerItemFloatingLevel) {
    ZMBannerItemFloatingLevelDefault = 0,//!< Normal
    ZMBannerItemFloatingLevelLow = 30,
    ZMBannerItemFloatingLevelHigh = 80,//!< Sip phone calling
    ZMBannerItemFloatingLevelMax = 100//!< RESERVED
};

typedef BOOL(^ZMBannerItemAvailable)(ZMMainTabsType type, BOOL compressed);//config how the banner item shows

@interface ZMBannerItem : NSObject

@property (assign, getter=isActive) BOOL active;

@property (readonly, nonatomic) NSView *contentView;//!< horizontal risizeable, height is constant
@property (assign) NSUInteger priority;
@property (assign) NSUInteger floatingLevel;//!< used when show with other items

@property (assign) BOOL nonexclusive;//!< will not affect other items when this is set to `YES`

+ (instancetype)itemWithContentView:(NSView *)contentView priority:(NSUInteger)priority;//!< All tabs type

+ (instancetype)itemWithContentView:(NSView *)contentView priority:(NSUInteger)priority available:(ZMBannerItemAvailable)available;

@property (assign, nonatomic) NSInteger height;

@property (copy, nonatomic) ZMBannerItemAvailable available;

@end
