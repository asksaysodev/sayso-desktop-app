//
//  ZMPSAppHolder.h
//  ZoomSetting
//
//  Created by Michael Lin on 2024/1/11.
//

#import <Foundation/Foundation.h>
#import <ZoomSetting/ZMPSObject.h>
#import <zPSApp/app/IPSApp.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMPSAppHolder : ZMPSObject

@property (nonatomic, nullable) ps::IPSApp *app;

@end

NS_ASSUME_NONNULL_END
