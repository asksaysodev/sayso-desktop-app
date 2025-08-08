//
//  ZMFileEntity+FTC.h
//  zChatComponent
//
//  Created by Dikey.Jin on 2025/1/23.
//

#import <zChatComponent/ZMFileEntity.h>
#import <zChatComponent/ZMFileModule.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMFileTransferControlFlagParam;

@interface ZMFileEntity (FTC)

#pragma mark - FTC Unify

- (BOOL)isMeetingChat;

// Check if the file is a special internal file
// screenshot/giphy/audio/recordvideo/codeSnippet
// Can used in team chat only
- (BOOL)isInternalSpecialFile;

// Check if the file has share access
// Can used in team chat only
- (BOOL)isFileHaveShareAccess;

// Check if the file has view access
// Can used in team chat only
- (BOOL)isFileHaveViewAccess;

// Can used in team chat only
- (ZMFTPermissionType)getFileViewPermissionType;

// Can used in team chat only
// Check if the file can be shared to a session
- (ZMFTPermissionType)isFileCanShareToSession:(NSString *)shareToSessionId;

@end

NS_ASSUME_NONNULL_END
