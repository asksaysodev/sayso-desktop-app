//
//  ZMChatClassificationInfo.h
//  SaasBeePTUIModule
//
//  Created by groot.ding on 4/8/21.
//  Copyright © 2021 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMChatClassificationInfo : NSObject

@property (nonatomic,copy) NSString *ID;
@property (nonatomic,copy) NSString *name;
@property (nonatomic,copy) NSString *desc;
@property (nonatomic,assign) NSInteger color;
@property (nonatomic,assign) NSInteger type; //type=1  default classification, type=0 normal classification.
@property (nonatomic,readonly) BOOL isDefault;

- (NSColor *)getColor;

@end

NS_ASSUME_NONNULL_END
