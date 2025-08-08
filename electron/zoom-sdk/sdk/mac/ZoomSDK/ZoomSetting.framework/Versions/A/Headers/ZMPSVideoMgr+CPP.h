//
//  ZMPSVideoMgr+CPP.h
//  ZoomSetting
//
//  Created by Michael Lin on 2024/1/11.
//

#import <Foundation/Foundation.h>
#import <zPSApp/media/IPSVideo.h>
#import <ZoomSetting/ZMPSVideoMgr.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMPSVideoMgr (CPP)

- (nullable ps::IPSVideoConfig *)videoConfig;
- (ps::PSRecordContentType)getRecordContentType;
- (void)setRecordContentType:(ps::PSRecordContentType)type;

@end

NS_ASSUME_NONNULL_END
