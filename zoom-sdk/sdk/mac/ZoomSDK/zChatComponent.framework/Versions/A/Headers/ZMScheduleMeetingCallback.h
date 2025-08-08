//
//  ZMScheduleMeetingCallback.h
//  ChatUI
//
//  Created by simon shang on 2022/6/1.
//  Copyright © 2022 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMScheduleMeetingInfo;

@interface ZMScheduleMeetingCallback : NSObject

@property (nonatomic, assign) BOOL hasMore;
@property (nonatomic, copy) NSString *sessionId;
@property (nonatomic, copy) NSString *requestId;
@property (nonatomic, assign) NSInteger errorCode;
@property (nonatomic, retain) NSArray <ZMScheduleMeetingInfo *> *meetingList;

@end

NS_ASSUME_NONNULL_END
