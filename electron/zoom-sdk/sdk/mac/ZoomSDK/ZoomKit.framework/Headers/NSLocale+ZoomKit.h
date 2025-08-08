//
//  NSLocale+ZoomKit.h
//  ZoomKit
//
//  Created by Ares on 2024/8/14.
//  Copyright © 2024 zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSLocale (ZoomKit)

@property (class, readonly) NSLocale *appLocale;
@property (class, readonly, nullable) NSString *currentKeyboardInputSourceLangCode;

@end
