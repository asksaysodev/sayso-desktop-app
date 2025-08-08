//
//  ZMNetUtilsHelper.h
//  ZoomSetting
//
//  Created by Michael Lin on 2024/12/30.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMNetUtilsHelper : NSObject

+ (BOOL)isUrlMatchCurrentEnv:(nullable NSString *)inUrl;

@end

NS_ASSUME_NONNULL_END
