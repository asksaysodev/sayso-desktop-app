//
//  ZMDownloadObject.h
//  SaasBeePTUIModule
//
//  Created by Francis Zhuo on 4/15/21.
//  Copyright © 2021 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ZMOndemandProtocol.h"
#import "zPTApp/SaasBeePTAppInterface.h"

NS_ASSUME_NONNULL_BEGIN

static NSString *const zDownloadLatest = @"latest";

typedef NS_ENUM(NSUInteger, ZMDownloadResultReason) {
    ZMDownloadResultReasonNone,
    ZMDownloadResultReasonFetchUrlFail,
    ZMDownloadResultReasonNotNeedDownload,
};

@interface ZMDownloadObject : NSObject

#pragma mark - Public methods
@property (nonatomic, retain, nullable) NSString *url;
@property (nonatomic, retain, readonly, nullable) NSString *downloadRequestId;
@property (nonatomic, retain, readonly, nullable) NSString *fileDownloadRequestId;
@property (nonatomic, assign) ZMDownloadStage stage;
@property (nonatomic, retain, nullable) id<ZMOndemandProductInfoProtocol> productInfo;

- (BOOL)isInstalled;
- (BOOL)isInProgress;
- (id)registerWithProgress:(nullable ZMDownloadProgressBlock)progressBlock completion:(nullable ZMDownloadCompletionBlock)completionBlock;
- (id)downloadWithProgress:(nullable ZMDownloadProgressBlock)progressBlock completion:(nullable ZMDownloadCompletionBlock)completionBlock;
- (void)updateProgress:(nullable ZMDownloadProgressBlock)progressBlock
            completion:(nullable ZMDownloadCompletionBlock)completionBlock
             withToken:(id)token;
- (void)download;
- (void)updateIfNeededWithVersion:(NSString *)version downloadURL:(NSString *)downloadURL;
- (BOOL)abort;
- (BOOL)abortWithToken:(id)token;
- (nullable NSString *)bundlePath;
- (nullable NSString *)getProductDomain;

#pragma mark - Public for child classes
- (BOOL)isInstalledForIsInApp:(BOOL)isInApp;
- (void)isCodeSignValidWithCompletion:(void (^)(NSString *_Nullable path))completion;
- (void)isCodeSignValidWithFiles:(NSArray<NSString *> *)files completion:(void (^)(NSString *_Nullable path))completion;
- (NSString *)pathForIsInApp:(BOOL)isInApp;
- (BOOL)resetStageToInitialIfNeeded;
- (nullable NSString *)getMainBundleVersion;
- (nullable NSString *)currentVersion;
- (nullable NSString *)currentVersionInPath:(nullable NSString *)path;
- (BOOL)isVersionLatestForIsInApp:(BOOL)isInApp;
- (BOOL)isVersion:(nullable NSString *)versionA newerThanVersion:(nullable NSString *)versionB;
- (BOOL)recreateSymbolicLinkForLibs:(NSArray<NSString *> *)libs inPath:(NSString *)path;
- (NSString *)curDownloadVersion;
#pragma mark - Override Methods
+ (NSArray *)relatedFiles;
- (NSArray<NSString *> *)products;
+ (ZMDownloadType)downloadType;
- (ISBWebServiceAPI::ProductType)productType;
- (BOOL)useSystemUnzip;
- (BOOL)needLinkLibs;
- (nullable NSString *)destinationDirectory;
- (NSString *)authenticationPromptText;
- (BOOL)shouldValidateBundleResources;

- (void)startDownload;
- (void)pauseDownload;
- (void)downloadDidFinishedWithDownloadPath:(NSString *)downloadPath requestID:(NSString *)requestID result:(int)result;
- (void)downloadDidFinished:(NSString *)downloadPath;

#pragma mark - ZMDownloadProtocol
- (void)downloadStart;
- (void)downloadingWithProcess:(CGFloat)process;
//- (void)downloadFinishedWithResult:(ZMDownloadResult)result error:(nullable NSError *)error; // Deprecated, Please use "downloadFinishedWithResult: reason: error:" instead.
- (void)downloadFinishedWithResult:(ZMDownloadResult)result reason:(ZMDownloadResultReason)reason error:(nullable NSError *)error;
- (void)unzipStart;
- (void)unzipFinishedWithSuccess:(BOOL)success;
- (void)copyStart;
- (void)copyFinishedWithSuccess:(BOOL)success;

@end

NS_ASSUME_NONNULL_END
