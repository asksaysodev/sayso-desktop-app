//
//  ZMChatFileItem.h
//  SaasBeePTUIModule
//
//  Created by ryan on 4/19/16.
//  Copyright © 2016 Zipow. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMFileEntity.h>
#import <zChatComponent/ZMIMCommonDefine.h>

#ifdef __cplusplus
#include <zPTApp/SaasBeePTAppInterface.h>
#include <zChatApp/FileContentInterface.h>
#import <zChatApp/search/SearchMgr.h>
#endif

@class ZMChatFileShareAction;

@interface ZMChatFileItem : ZMFileEntity

@property (copy) NSString* key;
@property (assign) BOOL isPreviewDownloaded;
@property (nonatomic, assign) BOOL isFileDownloaded;
@property (retain) NSArray* matchInfo;
@property (assign) long long timestamp;
@property (assign) long long modifyTimestamp;
#ifdef __cplusplus
@property (assign) NS_ZOOM_DATA::FileTransferState fileTransferState;
#endif
@property (assign) long long minFilterTimeStamp;
@property (assign) long long maxFilterTimeStamp;
@property (retain) AVPlayer *avPlayer;
@property (nonatomic,assign) ZMSearchFileSourceType  searchFileSourceType;
@property (copy) NSString* pbxInternalFileId;
@property (copy) NSString* pbxSessionId;
@property (copy) NSString* pbxPhoneNumber;

@property (nonatomic, assign) BOOL isMailZipFile;
@property (nonatomic, readonly) BOOL isCustomEmoji;

@property (nonatomic, assign) BOOL isUnshared;
@property (nonatomic, assign) BOOL isFromFilesTab;

@property (nonatomic, readonly) BOOL canShareFile;

#ifdef __cplusplus
- (id)initWithFileInfo:(NS_ZOOM_MESSAGER::IZoomFileInterface*)zoomFile;
#endif
- (void)updateGiphyFileInfo:(NSString*)sessionId msgId:(NSString*)msgId;
#ifdef __cplusplus
- (void)updateWithFileInfo:(NS_ZOOM_MESSAGER::IZoomFileInterface*)zoomFile;
#endif
- (id)initWithSMSFileInfo:(void *)smsFile;
- (void)update;
- (void)updateSMSFileItem;

- (NSArray <ZMChatFileShareAction *>*)shareActions;
- (BOOL)isImage;
- (BOOL)isVideo;
- (BOOL)isAudio;
- (BOOL)isFileDownloaded;
- (BOOL)isFileDownloading;
- (double)transferredSize;
- (BOOL)removeAction:(NSString*)session;

- (void)updateTimestampWithSessionID:(NSString*)sessionID;
- (long long)compareEarliestTimeStampWithSession:(NSString*)sessionID;
- (BOOL)isDeletePending;
- (BOOL)isUnsharePending;
- (BOOL)isRenamePending;
- (BOOL)isImagePathExtension;

- (NSString*)getLocalLink;
- (NSString*)getParrentID;

- (BOOL)isFileIntegrationType; //zoom-68763
- (BOOL)isExternalFile;

- (BOOL)canDeleteFile;

#ifdef __cplusplus
- (const NS_ZOOM_MESSAGER::FileIntegrationShareInfo* )getFileIntegrationShareInfo; //zoom-68763
- (void)initFileIntegrationShareInfo:(NS_ZOOM_MESSAGER::FileIntegrationShareInfo)info; // set share info: maybe it can change to expose the property (ZOOM-401850-v5.12.0-meeting)
#endif

#ifdef __cplusplus
- (ns_zoom_messager::FileIntegrationType)getFileIntegrationType;
#endif
- (BOOL)integrationFileIsValid;

- (BOOL)couldShareWithNote;

@end

