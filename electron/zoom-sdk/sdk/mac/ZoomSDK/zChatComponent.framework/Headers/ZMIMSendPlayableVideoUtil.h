//
//  ZMIMSendPlayableVideoUtil.h
//  SaasBeePTUIModule
//
//  Created by simon shang on 2021/4/12.
//  Copyright © 2021 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMIMSendPlayableVideoUtil : NSObject

+ (BOOL)isVideoFile:(NSString *)localFilePath sessionId:(NSString *)sessionId;

+ (BOOL)isVideoSizeMorethan30MB:(NSString *)videoPath;

+ (NSString *)getVideoPreviewPathWithSeesionId:(NSString *)sessionId;

+ (BOOL)writeVideoPreviewImageWithVideoPath:(NSString *)videoPath videoPreviewPath:(NSString *)videoPreviewPath;

@end

NS_ASSUME_NONNULL_END
