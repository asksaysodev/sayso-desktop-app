//
//  IZMCheckUpdateMgr.h
//  PTUIBridge
//
//  Created by davies zhao on 2024/10/30.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol IZMCheckUpdateMgr <NSObject>

- (void)forceReCheckUpdate;
- (void)checkUpdate;

- (BOOL)downloadDidFinish;
- (BOOL)isSelfCheck;
- (BOOL)isEnableAutoUpdate;
- (BOOL)isNewVersionAvailable;

- (BOOL)canShowUpdateBanner;
- (BOOL)canShowGlobalUpdateButton;
- (BOOL)isVersionOutdated;
- (BOOL)isGlobalUpdateButtonDisabled;

- (BOOL)isAllowedUpdateWithVersion:(NSString *)version;

@end

@protocol ZMUpdateEventProtocol <ZMRoutableObject>
@optional
- (void)newVersionReady;

@end

NS_ASSUME_NONNULL_END
// ZMSharedFor(IZMCheckUpdateMgr)
