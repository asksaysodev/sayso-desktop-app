//
//  ZMOndemandProtocol.h
//  SaasBeePTUIModule
//
//  Created by Francis Zhuo on 7/7/21.
//  Copyright © 2021 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef enum {
    ZMDownloadStageIdle = 0,
    ZMDownloadStageInitial,
    ZMDownloadStageRequestUrl,
    ZMDownloadStageDownloading,
    ZMDownloadStageUnzipping,
    ZMDownloadStageCopying,
    ZMDownloadStageFailed,
    ZMDownloadStageSuccess,
    ZMDownloadStageCancel,
} ZMDownloadStage;

typedef enum
{
    ZMDownloadTypeDefault,
    ZMDownloadTypeCodeSnippet,
    ZMDownloadTypeSipCall,
    ZMDownloadTypeRetinaImages,
    ZMDownloadTypeComponents,
    ZMDownloadTypeAomhost,
    ZMDownloadTypeAirhost,
    ZMDownloadTypeViperEX,
    ZMDownloadTypeMail,
    ZMDownloadTypeCalendar,
}ZMDownloadType;

typedef enum
{
    ZMDownloadResultFailed,
    ZMDownloadResultSuccess,
    ZMDownloadResultCancel,
}ZMDownloadResult;

typedef void(^ZMDownloadProgressBlock)(ZMDownloadStage stage, CGFloat process);
typedef void(^ZMDownloadCompletionBlock)(ZMDownloadResult result, NSError * _Nullable error);

@protocol ZMOndemandProtocol <ZMRoutableObject>
@optional

- (BOOL)isInProgress;

- (void)downloadForType:(ZMDownloadType)type;

///return token
- (id)downloadForType:(ZMDownloadType)type progress:(nullable ZMDownloadProgressBlock)progress completion:(nullable ZMDownloadCompletionBlock)completion;

///return token
- (id)registerForType:(ZMDownloadType)type progress:(nullable ZMDownloadProgressBlock)progress completion:(nullable ZMDownloadCompletionBlock)completion;

- (void)updateProgress:(nullable ZMDownloadProgressBlock)progress completion:(nullable ZMDownloadCompletionBlock)completion withType:(ZMDownloadType)type token:(id)token;

- (nullable NSString *)downloadUrlForType:(ZMDownloadType)type;

- (BOOL)cancelDownloadForType:(ZMDownloadType)type withToken:(id)token;
- (BOOL)cancelDownloadForType:(ZMDownloadType)type;

- (void)checkUpdateReturn:(NSString*)inLatestVersion
              downloadURL:(NSString*)inURLString
              releaseNote:(NSString*)inReleaseNote
                requestId:(NSString*)inRequestId
                   result:(NSInteger)inResult;
- (BOOL)isInstalledForType:(ZMDownloadType)type;
- (nullable NSString *)bundlePathForType:(ZMDownloadType)type;
@end

NS_ASSUME_NONNULL_END
