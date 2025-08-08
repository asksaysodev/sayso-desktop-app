//
//  ZMPinMessageInfo.h
//  ZCommonUI
//
//  Created by groot.ding on 28/09/2020.
//  Copyright © 2020 zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMMessageAdapter;

@interface ZMPinMessageInfo : NSObject

@property (nonatomic,retain) ZMMessageAdapter *messageEntity;

@property (nonatomic,copy) NSString *pinner;
@property (nonatomic,assign) long long pinnerTime;
@property (nonatomic,assign) BOOL isTopPin;

@end

NS_ASSUME_NONNULL_END
