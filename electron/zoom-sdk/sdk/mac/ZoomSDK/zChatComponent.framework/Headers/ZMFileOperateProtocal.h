//
//  ZMFileOperateProtocal.h
//  SaasBeePTUIModule
//
//  Created by groot.ding on 09/09/2020.
//  Copyright © 2020 Zoom. All rights reserved.
//

#ifndef ZMFileOperateProtocal_h
#define ZMFileOperateProtocal_h

#import <ZoomKit/ZMCallbackImpl.h>
#import <zChatComponent/ZMFileEntity.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, ZMFileShareType) {
    ZMNewShareFileByMySelf,
    ZMNewShareFileByOther,
    ZMShareFile,
};

@class ZMFileIntegrationShareInfo;

@protocol ZMFileOperateListen <NSObject>

//upload
- (void)addListenFileUploadedByTarget:(id _Nonnull)target completion:(ZMCallbackBlock _Nonnull)completion;

- (void)addListenFileSummarizedByTarget:(id _Nonnull)target completion:(ZMCallbackBlock _Nonnull)completion;

- (void)addListenUpdateMessageFileConvertInfoByTarget:(id)target completion:(ZMCallbackBlock)completion;

- (void)addListenFilePauseResumedByTarget:(id _Nonnull)target completion:(ZMCallbackBlock _Nonnull)completion;

- (void)addListenFileCanceledByTarget:(id _Nonnull)target completion:(ZMCallbackBlock _Nonnull)completion;

//download
- (void)addListenFileStartToDownloadByTarget:(id _Nonnull)target completion:(ZMCallbackBlock _Nonnull)completion;

- (void)addListenFileDownloadByTarget:(id _Nonnull)target completion:(ZMCallbackBlock _Nonnull )completion;

- (void)addListenFileTransferProgressByTarget:(id _Nonnull)target completion:(ZMCallbackBlock _Nonnull)completion;

- (void)addListenFileDeleteByTarget:(id _Nonnull)target completion:(ZMCallbackBlock _Nonnull)completion;

- (void)addListenFileUpdateByTarget:(id _Nonnull)target completion:(ZMCallbackBlock _Nonnull)completion;

- (void)addListenFileShareByTarget:(id _Nonnull)target completion:(ZMCallbackBlock _Nonnull)completion;

- (void)addListenFileAsyncBlockByTarget:(id _Nonnull)target completion:(ZMCallbackBlock _Nonnull)completion;

/// This will cause when the local file was modified, re-open the file from message will re-download the file. As soon as the file downloaded, this callback will come and update the message.
- (void)addListenFileStateResetByTarget:(id _Nonnull)target completion:(ZMCallbackBlock _Nonnull)completion;

//whiteboard
- (void)addListenWhiteboardPreviewInfoUpdatebyTarget:(id _Nonnull)target completion:(ZMCallbackBlock _Nonnull)completion;

- (void)addListenWhiteboardPreviewDeleteByTarget:(id _Nonnull)target completion:(ZMCallbackBlock _Nonnull)completion;

- (void)removeAllListenByTarget:(id _Nonnull)target;

//SMS
- (void)addListenSMSFileCanceledByTarget:(id _Nonnull)target completion:(ZMCallbackBlock _Nonnull)completion;

- (void)addListenSMSFileInProgressByTarget:(id _Nonnull)target completion:(ZMCallbackBlock _Nonnull)completion;

- (void)addListenSMSFileDownloadedByTarget:(id _Nonnull)target completion:(ZMCallbackBlock _Nonnull)completion;

@optional

- (void)removeCallbackByKey:(NSString *)key;

@end

@class ZMFileCallbackParam;

@protocol ZMFileOperateCallback <NSObject>

- (void)didFileDelete:(unsigned int)result fileId:(NSString * _Nonnull)fileId reqId:(NSString * _Nullable)reqId;

- (void)didFileShare:(unsigned int)result fileId:(NSString * _Nonnull)fileId sessionId:(NSString * _Nonnull)sessionId messageId:(NSString * _Nonnull)messageId shareType:(ZMFileShareType)shareType reqId:(NSString * _Nullable)reqId;

- (void)didFileUnshare:(unsigned int)result fileId:(NSString * _Nonnull)fileId reqId:(NSString * _Nullable)reqId;

- (void)didFileDeleteByOnwer:(NSString *_Nullable)actionOwner fileId:(NSString * _Nonnull)fileId fileName:(NSString * _Nullable)fileName relatedSessionIds:(NSArray <NSString *>* _Nullable)relatedSessionIds;

- (void)didFileStatueUpdate:(NSString * _Nonnull)fileId;

- (void)didGetFileSummarized:(ZMFileCallbackParam *)param;

- (void)didUpdateMessageFileConvertInfo:(nonnull NSString *)messageId
                                 params:(ZMFileCallbackParam *)param;

- (void)didFileAttachInfoUpdate:(NSString * _Nonnull)fileId;

- (void)didWhiteboardPreviewInfoUpdatebyTarget:(unsigned int)result fileId:(NSString * _Nonnull)fileId wbLink:(NSString * _Nullable)wbLink sessionId:(NSString * _Nullable)sessionId messageId:(NSString * _Nullable)msgId fileIndex:(NSInteger)fileIndex;

- (void)didDelWhiteboardFromMessage:(NSString *  _Nonnull)sessionId msgId:(NSString *  _Nonnull)msgId wbLink:(NSString *  _Nonnull)wbLink;

- (void)didSelectedFileIntegrationInfo:(ZMFileIntegrationShareInfo *_Nonnull )fileInfo identifier:(NSString *_Nonnull)identifier;
@end

@protocol ZMFileNotifyProtocal <NSObject>

- (void)didFileDownload:(NSString * _Nonnull)fileId param:(ZMFileCallbackParam * _Nonnull)param sourceType:(NSInteger)sourceType;

@end

@class ZMFileEntity;

@protocol ZMFileOperateProtocal <NSObject>

@optional

- (void *_Nullable)getRawDataOfFileWithFileId:(NSString *_Nullable)fileId messageId:(NSString *_Nullable)messageId sessionId:(NSString *_Nullable)sessionId fileIndex:(NSInteger)fileIndex needDestroyFile:(BOOL *_Nullable)needDestroyFile sourceType:(NSInteger)sourceType;

- (void)destroyRawDataFile:(void * _Nonnull)fileObject sourceType:(NSInteger)sourceType;;

- (__kindof ZMFileEntity * _Nullable)getFileById:(NSString *_Nonnull)fileId sourceType:(NSInteger)sourceType;

- (ZMFileEntity * _Nullable)getFileById:(NSString *_Nonnull)fileId sessionID:(NSString *)sessionID sourceType:(NSInteger)sourceType;

- (__kindof ZMFileEntity * _Nullable)getFileByMessageId:(NSString *_Nonnull)messageId sessionId:(NSString *_Nonnull)sessionId fileIndex:(NSUInteger)fileIndex sourceType:(NSInteger)sourceType;
- (void)updateFileItemByFileEntity:(ZMFileEntity *_Nonnull)fileEntity;

- (ZMFileEntity * _Nullable)getFileItemByFileEntity:(ZMFileEntity *_Nonnull)fileEntity;

- (NSUInteger)getFilePercentByFileEntity:(ZMFileEntity *_Nonnull)fileEntity;

- (void)downloadByFileEntity:(ZMFileEntity *_Nonnull)fileEntity localPath:(NSString * _Nullable)localPath needReconnect:(BOOL)needReconnect isManual:(BOOL)isManual error:(NSError *_Nullable*_Nullable)error;
/// Downloads the specified file after permission checks.
/// Shows the user a save panel in `window` to get the download location.
/// Calls `onComplete` when done. Error is non-null if downloading failed for reasons beyond the user's control (e.g. account permissions, server issue).
/// No error on succesful download or user cancel.
- (void)downloadByFileEntity:(ZMFileEntity *_Nonnull)fileEntity withSavePanelInWindow:(NSWindow*)window needReconnect:(BOOL)needReconnect isManual:(BOOL)isManual onComplete:(void(^_Nullable)(NSError* _Nullable))onComplete;

- (void)downloadByFileEntity:(ZMFileEntity *_Nonnull)fileEntity localPath:(NSString * _Nullable)localPath needReconnect:(BOOL)needReconnect isManual:(BOOL)isManual error:(NSError *_Nullable*_Nullable)error secureCheck:(void(^_Nullable)(BOOL safe))secureCheck;

- (BOOL)downloadFileWithoutPermissionCheck:(ZMFileEntity *_Nonnull)fileEntity localPath:(NSString *_Nullable)localPath needReconnect:(BOOL)needReconnect isManual:(BOOL)isManual;

- (BOOL)downloadFile:(NSString *_Nonnull)sessionID messageID:(NSString *_Nonnull)msgID fileIndex:(NSUInteger)fileIndex localPath:(NSString *_Nullable)localPath needReconnect:(BOOL)needReconnect isManual:(BOOL)isManual sourceType:(NSInteger)sourceType;

- (BOOL)downloadWithFileId:(NSString * _Nonnull)fileId localPath:(NSString * _Nullable)localPath isManual:(BOOL)isManual sourceType:(NSInteger)sourceType;

- (nullable NSString *)downloadLinkImageWithURL:(NSString *_Nonnull)url originalURL:(NSString *_Nonnull)originalURL path:(NSString *_Nonnull)path messageId:(NSString *_Nonnull)messageId sessionId:(NSString *_Nonnull)sessionId sourceType:(NSInteger)sourceType;

- (nullable NSString *)downloadLinkFaviconWithURL:(NSString *_Nonnull)url originalURL:(NSString *_Nonnull)originalURL path:(NSString *_Nonnull)path messageId:(NSString *_Nonnull)messageId sessionId:(NSString *_Nonnull)sessionId sourceType:(NSInteger)sourceType;

- (nullable NSString *)downloadFileWithURL:(NSString *_Nonnull)url path:(NSString *_Nonnull)path messageId:(NSString *_Nonnull)messageId sessionId:(NSString *_Nonnull)sessionId image:(NSImage *_Nonnull*_Nonnull)image sourceType:(NSInteger)sourceType;

- (nullable NSString *)downloadFileForBillableHoursWithURL:(NSString *_Nonnull)url path:(NSString *_Nonnull)path sourceType:(NSInteger)sourceType;

- (nullable NSString *)downloadFileForAppCardWithURL:(NSString *_Nonnull)url path:(NSString *_Nonnull)path messageId:(NSString *_Nonnull)messageId sessionId:(NSString *_Nonnull)sessionId image:(NSImage * _Nullable * _Nullable)image sourceType:(NSInteger)sourceType;

- (nullable NSString *)downloadImageWithURL:(NSString *_Nonnull)url path:(NSString *_Nonnull)path sessionId:(NSString *_Nonnull)sessionId  sourceType:(NSInteger)sourceType;

- (BOOL)checkFileExistByURL:(NSString *_Nonnull)url path:(NSString *_Nullable *_Nullable)path sourceType:(NSInteger)sourceType;

- (BOOL)checkImageExistByURL:(NSString *_Nonnull)url path:(NSString *_Nullable *_Nullable)path sourceType:(NSInteger)sourceType;

- (BOOL)check3rdPartyAppFileExistByURL:(NSString *_Nonnull)url path:(NSString *_Nullable *_Nullable)path sourceType:(NSInteger)sourceType;

- (BOOL)checkFileDownloadingByURL:(NSString *_Nonnull)url sourceType:(NSInteger)sourceType;

- (nullable NSString *)getFilePathByURL:(NSString *_Nonnull)url sourceType:(NSInteger)sourceType;

- (nullable NSString *)getImagePathByURL:(NSString *_Nonnull)url sourceType:(NSInteger)sourceType;

- (nullable NSString *)get3rdPartyAppFilePathByURL:(NSString *_Nonnull)url sourceType:(NSInteger)sourceType;

- (nullable NSImage *)checkDownloadFileExist:(NSString *_Nonnull)url path:(NSString *_Nonnull)path;

- (nullable NSString *)getValidFilePathByURL:(NSString *_Nonnull)url sourceType:(NSInteger)sourceType;

- (BOOL)downloadPreviewByFileEntity:(ZMFileEntity *_Nonnull)fileEntity;

- (BOOL)downloadVideoPreviewByFileEntity:(ZMFileEntity *_Nonnull)fileEntity;

- (BOOL)cancelByFileEntity:(ZMFileEntity *_Nonnull)fileEntity;

- (BOOL)pauseByFileEntity:(ZMFileEntity *_Nonnull)fileEntity;

/// If local file content modified, we'd re-download the file when user tries to open the file in message.
/// Also, we'd use resetFileDownloadStateWithWebFileID: to reset the file state in cache & DB.
/// FileID is Must.
- (BOOL)resetFileDownloadStateByFileEntity:(ZMFileEntity *_Nonnull)fileEntity;

- (BOOL)resumeDownloadFileEntity:(nullable ZMFileEntity *)fileEntity needReconnect:(BOOL)needReconnect error:(NSError *_Nullable*_Nullable)error;

- (BOOL)resumeUploadFileEntity:(nullable ZMFileEntity *)fileEntity needReconnect:(BOOL)needReconnect error:(NSError *_Nullable*_Nullable)error;

- (nullable NSString*)downloadFileForEditCodeSnippetWithFileId:(nullable NSString*)fileId andLocalPath:(nullable NSString*)localPath andFileSize:(NSUInteger)fileSize sourceType:(NSInteger)sourceType;

- (BOOL)downloadCodeFileForSessionId:(nonnull NSString*)sessionId message:(nonnull NSString*)msgID filePath:(nullable NSString*)path sourceType:(NSInteger)sourceType;

//delete
- (BOOL)deleteFile:(NSString * _Nonnull)fileId requestId:(NSString *_Nullable*_Nullable)requestId sourceType:(NSInteger)sourceType;

- (BOOL)unshareFile:(NSString * _Nonnull)fileId toSessionId:(NSString * _Nonnull)sessionId sourceType:(NSInteger)sourceType;

- (BOOL)deleteNoAccessWhiteBoardPreview:(ZMFileEntity *_Nonnull)fileEntity;

- (NSInteger)fileAllowedDownloadOption:(void * _Nonnull)zoomFile sourceType:(NSInteger)sourceType sessionId:(NSString *)sessionId;
- (NSInteger)getFileViewOption:(void * _Nonnull)zoomFile sourceType:(NSInteger)sourceType sessionId:(NSString *)sessionId;
- (NSInteger)getFileViewOption:(NSString *)fileSuffix sourceType:(NSInteger)sourceType sessionId:(NSString * _Nonnull)sessionId senderJid:(NSString * _Nonnull)senderJid;

- (void)selectFileIntegrationWithType:(NSInteger)fileType sessionId:(nonnull NSString *)sessionId threadId:(nullable NSString * )threadId integrationWithURL:(nullable NSString *)fileItergrationUrl nonce:(nullable NSString *)nonce completion:(ZMCallbackBlock _Nonnull)completion;

- (BOOL)deleteGiphyInMessageAccessory:(ZMFileEntity *)fileEntity;

- (BOOL)isFileSupportedForDocSummary:(ZMFileEntity *)fileEntity;
- (ZMChatFileSummaryStatus)queryChatSummaryFromContainer:(ZMFileEntity *)fileEntity;

@end

@class ZMFileTransferControlFlagParam;

@protocol ZMFileOPFlagProtocol <NSObject>

@optional

// FTC Unify Start

// Check if the file is a special internal file
- (BOOL)isInternalSpecialFile:(ZMFileTransferControlFlagParam *)flagParam;

// Check if the file has share access
- (BOOL)isFileHaveShareAccess:(ZMFileTransferControlFlagParam *)flagParam;

// Check if the file has view access
- (BOOL)isFileHaveViewAccess:(ZMFileTransferControlFlagParam *)flagParam;

// Check if the file has view access
- (ZMFTPermissionType)getFileViewPermissionType:(ZMFileTransferControlFlagParam *)flagParam;

// Check if the file can be shared to a session
- (ZMFTPermissionType)isFileCanShareToSession:(ZMFileTransferControlFlagParam *)flagParam
                                    sessionId:(NSString *)shareToSessionId;

// FTC Unify End

- (BOOL)isFileTransferResumeEnabled:(NSString * _Nonnull)sessionId sourceType:(NSInteger)sourceType;

- (BOOL)isFileTransferDisabled:(NSString * _Nonnull)sessionId sourceType:(NSInteger)sourceType;

- (BOOL)fileTransferAndReceiverOptionDisabled:(NSString * _Nonnull)sessionId sourceType:(NSInteger)sourceType;

- (BOOL)isFileViewDisabled:(NSString * _Nonnull)sessionId sourceType:(NSInteger)sourceType;

- (BOOL)fileViewAndReceiverOptionDisabled:(NSString * _Nonnull)sessionId sourceType:(NSInteger)sourceType;

@end


@protocol ZMFileDownladCallback <NSObject>

/**
 if "sourceType" is ZMFileSourceType_IM, pass nil to fileID;
 if "sourceType" is ZMFileSourceType_SMS, pass 0 to fileIndex;
 */
- (void)didFileUploaded:(unsigned int)result sessionId:(nullable NSString *)sessionId messageId:(nullable NSString *)messageId fileID:(nullable NSString *)fileID fileIndex:(NSUInteger)fileIndex sourceType:(NSUInteger)sourceType;

- (void)didFilePauseResumed:(nullable NSString *)sessionId messageId:(nullable NSString *)messageId fileIndex:(NSUInteger)fileIndex sourceType:(NSUInteger)sourceType;

- (void)didFileCanceledWithFileId:(NSString *_Nonnull)fileID sessionId:(NSString * _Nonnull)sessionId messageId:(NSString * _Nonnull)messageId fileIndex:(NSUInteger)fileIndex sourceType:(NSUInteger)sourceType;

- (void)didSMSFileCanceledWithFileId:(NSString *_Nonnull)fileID sessionId:(NSString * _Nonnull)sessionId messageId:(NSString * _Nonnull)messageId;

- (void)didSMSFileDownloadedWithFileId:(NSString *_Nonnull)fileID sessionId:(NSString * _Nonnull)sessionId messageId:(NSString * _Nonnull)messageId;

- (void)didSMSFileInProgressWithFileId:(NSString *_Nonnull)fileID sessionId:(NSString * _Nonnull)sessionId messageId:(NSString * _Nonnull)messageId ratio:(NSInteger)ratio completeSize:(NSInteger)completeSize bitPerSecond:(NSInteger)bitPerSecond;

- (void)didFileStarToDownloadWithFileId:(nullable NSString*)fileId sessionId:(nullable NSString *)sessionId messageId:(nullable NSString *)messageId fileIndex:(NSUInteger)fileIndex sourceType:(NSUInteger)sourceType;

- (void)didFileDownloaded:(unsigned int)result fileId:(nullable NSString *)fileId sessionId:(nullable NSString *)sessionId messageId:(nullable NSString *)messageId fileIndex:(NSUInteger)fileIndex sourceType:(NSUInteger)sourceType isPreview:(BOOL)isPreview;

- (void)didDownloadedFileByURL:(unsigned int)result reqId:(NSString * _Nonnull)reqId sourceType:(NSUInteger)sourceType;

- (void)didFileInProgress:(nullable NSString *)fileId sessionId:(nullable NSString *)sessionId messageId:(nullable NSString *)messageId fileIndex:(NSUInteger)fileIndex sourceType:(NSUInteger)sourceType ratio:(NSInteger)ratio completeSize:(NSInteger)completeSize bitPerSecond:(NSInteger)bitPerSecond;

- (void)didFileAsyncBlocked:(unsigned int)result fileId:(nullable NSString*)fileId sessionId:(nullable NSString *)sessionId messageId:(nullable NSString *)messageId fileIndex:(NSUInteger)fileIndex sourceType:(NSUInteger)sourceType;

- (void)didFileStateResetWitiFileEneity:(nonnull ZMFileEntity *)fileEntity;

@end

@protocol ZMImageCompressProtolcal <NSObject>

@optional

- (nonnull NSString *)checkImageNeedCompress:(nonnull ZMFileEntity *)fileEntity maxSize:(NSSize)maxSize compressSize:(NSSize)compressSize;

- (nonnull NSString *)getImageCompressFile:(nonnull ZMFileEntity *)fileEntity compressSize:(NSSize)compressSize;

- (nonnull NSString *)compressImage:(nonnull ZMFileEntity *)fileEntity compressSize:(NSSize)compressSize identifier:(NSUInteger)identifier completion:(nullable ZMCallbackBlock)completion;

- (void)removeCompressByFile:(nonnull ZMFileEntity *)fileEntity compressSize:(NSSize)compressSize identifier:(NSUInteger)identifier;

@end

@protocol ZMFileErrorProtocal <NSObject>

- (void)downloadFailedWithFileEntity:(ZMFileEntity *_Nonnull)fileEntity errorCode:(NSUInteger)errorCode;

@end

@class ZMChatMsgItemInfo;

@protocol ZMFileShareProtocal <NSObject>

- (void)showFileSharePannelByFileItem:(nullable ZMFileEntity *)fileItem sessionID:(nullable NSString*)sessionID onWindow:(nullable NSWindow*)inWindow;

- (void)showFileSharePannelBySessionID:(nullable NSString*)sessionID andMsgID:(nullable NSString*)MsgId onWindow:(nullable NSWindow*)inWindow  sourceType:(NSUInteger)sourceType;

- (void)showShareMesasgePannelByMessage:(nullable ZMChatMsgItemInfo *)msgItem onWindow:(nullable NSWindow*)inWindow  sourceType:(NSUInteger)sourceType;

- (void)showZappSharePannelByZappInfo:(nullable NSDictionary *)zappItem onWindow:(nullable NSWindow *)inWindow sourceType:(NSUInteger)sourceType;

- (void)showDocsSharePanelWithDocsInfo:(nonnull NSDictionary *)docsInfo inWindow:(nullable NSWindow *)inWindow sourceType:(NSUInteger)sourceType;

@end

@protocol ZMFileImplProtocal <ZMFileOperateProtocal,ZMFileOPFlagProtocol,ZMFileNotifyProtocal,ZMImageCompressProtolcal,ZMFileErrorProtocal, ZMFileShareProtocal>
@end

NS_ASSUME_NONNULL_END

#endif /* ZMFileOperateProtocal_h */
