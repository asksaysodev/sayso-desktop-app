//
//  ZMIntegrationProtocal.h
//  SaasBeePTUIModule
//
//  Created by Jed Zheng on 13/7/2021.
//  Copyright © 2021 Zoom. All rights reserved.
//

#ifndef ZMIntegrationProtocal_h
#define ZMIntegrationProtocal_h

#import <ZoomKit/ZMCallbackImpl.h>

typedef NS_ENUM(NSInteger, SharePointErrorType) {
    SharePointErrorType_Success = 0,
    SharePointErrorType_CreateFile,
    SharePointErrorType_RenameFile,
    SharePointErrorType_MoveFile,
    SharePointErrorType_DeleteFile,
    SharePointErrorType_CreateFileLink,
    SharePointErrorType_CreateFolder,
    SharePointErrorType_RenameFolder,
    SharePointErrorType_MoveFolder,
    SharePointErrorType_DeleteFolder,
    SharePointErrorType_CreateDownloadLink,
    SharePointErrorType_DownlaodFile,
    SharePointErrorType_UploadFile,
    SharePointErrorType_GetRootNode,
};

typedef NS_ENUM(NSUInteger, ZMSharepointFileType) {
    ZMSharepointFileTypeNone = 0,
    ZMSharepointFileTypeWord,
    ZMSharepointFileTypeExcel,
    ZMSharepointFileTypePPT,
    ZMSharepointFileTypeOneNote,
    ZMSharepointFileTypeBoxNode,
    ZMSharepointFileTypeBoxNodeFromTemplate,
    ZMSharepointFileTypeGoogleDocs,
    ZMSharepointFileTypeGoogleSlides,
    ZMSharepointFileTypeGoogleSheet,
};

@class ZMFileEntity, ZMIntegrationEvent;

@protocol ZMIntegrationActionProtocal <NSObject>

- (NSString *)getRootNodeInfoWithSessionId:(NSString *)sessionId;

- (NSString *)getRootNodeInfoWithSessionId:(NSString *)sessionId needShowAuthenticate:(BOOL)bShow;

- (NSString *)renameFileWithSessionId:(NSString *)sessionId nodeId:(NSString *)nodeId rename:(NSString *)rename;
- (NSString *)moveFileWithSessionId:(NSString *)sessionId nodeId:(NSString *)nodeId sourceNodeId:(NSString *)sourceNodeId targetNodeId:(NSString *)targetNodeId;

- (NSString *)deleteFileWithSessionId:(NSString *)sessionId nodeId:(NSString *)nodeId parentId:(NSString *)parentId;

- (NSString *)createFolderWithSessionId:(NSString *)sessionId parentNodeId:(NSString *)parentNodeId name:(NSString *)name;

- (NSString *)renameFolderWithSessionId:(NSString *)sessionId nodeId:(NSString *)nodeId rename:(NSString *)rename;
- (NSString *)moveFolderWithSessionId:(NSString *)sessionId nodeId:(NSString *)nodeId sourceNodeId:(NSString *)sourceNodeId targetNodeId:(NSString *)targetNodeId;
- (NSString *)deleteFolderWithSessionId:(NSString *)sessionId nodeId:(NSString *)nodeId;

- (NSString*)createLinkWithSessionID:(NSString*)sessionID nodeID:(NSString*)nodeID;

- (NSString*)createSharePointFileWithSessionId:(NSString *)sessionId name:(NSString *)name parentId:(NSString *)parentId type:(ZMSharepointFileType)type;

- (NSString*)download3rdStorageFile:(ZMFileEntity *)fileInfo;
- (NSString*)download3rdStorageFile:(ZMFileEntity *)fileInfo isFromMessage:(BOOL)isFromMessage;

- (NSString*)uploadFileTo3rdFileStorageWithPath:(NSString*)path sessionID:(NSString*)sessionID nodeID:(NSString*)nodeID;
- (NSString*)retryToUpladWithRequestID:(NSString*)requestID sessionID:(NSString*)sessionID;

- (NSString*)download3rdStorageFileWithRequestID:(NSString*)requestID link:(NSString*)link result:(NSInteger)result;

- (BOOL)cancelDownload3rdStorageFileWithRequesteID:(NSString*)requestID;

- (BOOL)cancelDownload3rdStorageFile:(NSString *)fileID;

- (NSMutableDictionary*)getDownloadInfo;
- (ZMFileEntity*)getFileItemBySessionID:(NSString*)sessionID messageID:(NSString*)messageID fileID:(NSString*)fileID;

- (BOOL)hasFailUploadFilesInSession:(NSString *)sessionId;
- (NSArray*)getAllUploadFilesWithSessionID:(NSString*)sessionID;
- (BOOL)removeUploadFileWith:(NSString*)requestID sessionId:(NSString *)sessionId;

- (void)showErrorToastWithResult:(NSInteger)result type:(NSInteger)type;

- (void)showErrorToastWithResult:(NSInteger)result type:(NSInteger)type sessionID:(NSString*)sessionID;

- (void)authenticateWithUrl:(NSString *)url requestID:(NSString*)requestID sessionID:(NSString*)sessionID;

- (void)showAuthenticateWithUrl:(NSString *)url requestID:(NSString*)requestID sessionID:(NSString*)sessionID;

@end

@protocol ZMOperateIntegrationCallbackProtocal <NSObject>

- (void)didNeedAuthenticateWithUrl:(NSString *)url requestID:(NSString*)requestID sessionID:(NSString*)sessionID;

- (void)didNeedShowAuthenticateWithUrl:(NSString *)url requestID:(NSString*)requestID sessionID:(NSString*)sessionID;

- (void)didGetRootNodeInfoWithSessionID:(NSString*)sessionID rootNodeID:(NSString*)nodeID requestID:(NSString*)requestID result:(NSInteger)result;
- (void)didGetFileStorageSupportedTypesWithTypes:(NSArray *)types requestID:(NSString*)requestID;
//Files
- (void)didCreateFileWithFileURL:(NSString *)fileURL name:(NSString*)name requestID:(NSString*)requestID nodeID:(NSString*)nodeID sessionID:(NSString *)sessionID parentNodeID:(NSString*)parentNode result:(NSInteger)result nodeType:(NSUInteger)nodeType;
- (void)didRenameFileWithSessionId:(NSString *)sessionId name:(NSString*)name nodeId:(NSString*)nodeId requestID:(NSString*)requestID result:(NSInteger)result;
- (void)didMoveFileWithSessionId:(NSString *)sessionId nodeId:(NSString*)nodeId requestID:(NSString*)requestID result:(NSInteger)result;
- (void)didDeleteFileWithSessionId:(NSString *)sessionId nodeId:(NSString*)nodeId requestID:(NSString*)requestID result:(NSInteger)result;
- (void)didCreateFileLinkWithSessionId:(NSString *)sessionId link:(NSString*)link requestID:(NSString*)requestID result:(NSInteger)result;
//Folders
- (void)didCreateFolderWithSessionId:(NSString *)sessionId nodeId:(NSString*)nodeId requestID:(NSString*)requestID result:(NSInteger)result;
- (void)didRenameFolderWithSessionId:(NSString *)sessionId nodeId:(NSString*)nodeId name:(NSString*)name requestID:(NSString*)requestID result:(NSInteger)result;
- (void)didMoveFolderWithSessionId:(NSString *)sessionId nodeId:(NSString*)nodeId requestID:(NSString*)requestID result:(NSInteger)result;
- (void)didDeleteFolderWithSessionId:(NSString *)sessionId nodeId:(NSString*)nodeId requestID:(NSString*)requestID result:(NSInteger)result;
- (void)didCreateDownloadLinkWithSessionID:(NSString *)sessionID nodeID:(NSString*)nodeID link:(NSString *)link requestID:(NSString*)requestID result:(NSInteger)result;
- (void)didDownloaded3rdStorageFile:(NSString *)requestID result:(NSInteger)result nodeId:(NSString*)nodeId;
//- (void)onDownload3rdStorageFileProgress:(NSString *)requestID radio:(NSInteger)radio completeSize:(NSInteger)completeSize bitPerSecond:(NSInteger)bitPerSecond;
//- (void)onDownload3rdStorageFileTimeOut:(NSString *)requestID;

- (void)didUpload3rdStorageFile:(NSString *)requestID nodeId:(NSString*)nodeId result:(NSInteger)result;
- (void)onUpload3rdStorageFileProgress:(NSString *)requestID radio:(NSInteger)radio completeSize:(NSInteger)completeSize bitPerSecond:(NSInteger)bitPerSecond;
- (void)onUpload3rdStorageFileTimeOut:(NSString *)requestID;
- (void)onThirdPartyFileStorageEvent:(ZMIntegrationEvent *)event;

- (void)didAuthenticate;

- (void)didCancelUpload3rdStorageFile:(NSString *)requestID sessionId:(NSString*)sessionId;
- (void)didStartUpload3rdStorageFile:(NSString *)requestID sessionId:(NSString*)sessionId;
- (void)didRetryUpload3rdStorageFile:(NSString *)requestID sessionId:(NSString*)sessionId;
@end

@protocol ZMListenIntegrationProtocal <NSObject>
- (void)removeAllListenByTarget:(id)target;
- (void)addListenIntegrationGetRootNodeByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenIntegrationCreateFileByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenIntegrationStartDownloadByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenIntegrationStartCreateDownloadLinkFailedByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenIntegrationRenameFileByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenIntegrationMoveFileByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenIntegrationDeleteFileByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenIntegrationCreateFolderByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenIntegrationRenameFolderByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenIntegrationMoveFolderByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenIntegrationDeleteFolderByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenIntegrationCreateFileLinkByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenIntegrationDidAuthenticateByTarget:(id)target completion:(ZMCallbackBlock)completion;
//- (void)addListenIntegrationDownload3rdStorageFileProgressByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenIntegrationDownload3rdStorageFileDownloadedTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenIntegrationUpload3rdStorageFileProgressByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenIntegrationUpload3rdStorageFileUploadedTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenIntegrationUpload3rdStorageFileStartUploadTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenIntegrationUpload3rdStorageFileCancelTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenIntegrationUpload3rdStorageFileRetryTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenIntegrationThirdPartyFileStorageEventByTarget:(id)target completion:(ZMCallbackBlock)completion;


@end

@protocol ZMIntegrationProtocal <ZMOperateIntegrationCallbackProtocal,ZMListenIntegrationProtocal,ZMIntegrationActionProtocal>

@end

#endif /* ZMIntegrationProtocal_h */
