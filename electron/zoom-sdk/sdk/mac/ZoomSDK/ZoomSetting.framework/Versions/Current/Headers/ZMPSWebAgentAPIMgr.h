//
//  ZMPSWebAgentAPIMgr.h
//  ZoomClips
//
//  Created by Michael Lin on 2023/2/2.
//

#import <Foundation/Foundation.h>
#import "ZMPSObject.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZMPSWebAgentAPIMgr : ZMPSObject

- (nullable NSString *)shareURL:(nullable NSString *)recordingID;
- (nullable NSString *)libraryUrl;
- (nullable NSString *)domain;
- (void)setLanguageID:(nullable NSString *)languageID;
@property (nonatomic, weak, nullable, readonly) NSString *zak;

- (void)loadTelemetryBiz;

@end

NS_ASSUME_NONNULL_END
