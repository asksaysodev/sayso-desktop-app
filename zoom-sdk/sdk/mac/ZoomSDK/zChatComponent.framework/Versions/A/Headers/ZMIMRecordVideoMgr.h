//
//  ZMIMRecordVideoMgr.h
//  ChatUI
//
//  Created by simon shang on 2022/1/12.
//  Copyright © 2022 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "zSipCallCommon/IMediaClient.h"

@class ZMIMRecordVideoView;

NS_ASSUME_NONNULL_BEGIN

@interface ZMIMRecordVideoMgr : NSObject

- (BOOL)startVideoInView:(ZMIMRecordVideoView *)recordView;
- (void)stopVideo;

- (BOOL)startRecord:(NSString *)path;
- (BOOL)stopRecord;

- (BOOL)isVirtualBackgroundEnabled;

@end

NS_ASSUME_NONNULL_END
