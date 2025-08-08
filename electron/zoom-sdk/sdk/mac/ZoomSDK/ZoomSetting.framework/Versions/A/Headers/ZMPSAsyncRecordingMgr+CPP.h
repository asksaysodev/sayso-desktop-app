//
//  ZMPSAsyncRecordingMgr+CPP.h
//  ZoomSetting
//
//  Created by Michael Lin on 2024/1/11.
//

#import <Foundation/Foundation.h>
#include <commonDef/PSCommonStruct.h>
#import <zPSApp/product/IPSAsyncRecording.h>
#import <ZoomSetting/ZMPSAsyncRecordingMgr.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMPSAsyncRecordingMgr (CPP)

- (nullable ps::IPSAsyncRecording *)asyncRecordingWithID:(NSUInteger)recordingID;
- (void)sendUploadResultToPT:(ps::PSAsyncRecordingUploadResultToPT&)result;
- (void)responseToTerm:(ps::PSTerminateResponse)response;

@end

NS_ASSUME_NONNULL_END
