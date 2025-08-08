//
//  ZMFileIntegrationShareInfo.h
//  zChatUI
//
//  Created by groot.ding on 12/9/19.
//  Copyright © 2019 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMFileIntegrationShareInfo : NSObject

@property (nonatomic,assign)    NSInteger   integrationType;
@property (nonatomic,copy)      NSString    *integrationFileId;
@property (nonatomic,copy)      NSString    *previewURL;
@property (nonatomic,copy)      NSString    *previewPath;
@property (nonatomic,copy)      NSString    *downloadURL;
@property (nonatomic,copy)      NSString    *thumbnailURL;
@property (nonatomic,copy)      NSString    *fileName;
@property (nonatomic,assign)    long        fileSize;

@end

NS_ASSUME_NONNULL_END
