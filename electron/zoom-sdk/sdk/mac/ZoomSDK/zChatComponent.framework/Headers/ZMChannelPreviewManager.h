//
//  ZMChannelPreviewManager.h
//  ChatUI
//
//  Created by Felipe Bastos on 3/29/23.
//  Copyright © 2023 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <zChatComponent/ZMIMDataInfo.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMChannelPreviewManager : NSObject

+ (nonnull instancetype)sharedInstance;

- (nonnull NSString *)requestPreviewInfo:(nonnull NSString *)channelID
                              withLinkId:(NSString* _Nullable)linkId
                       completionHandler:(void(^ _Nonnull)(NSError *_Nullable,
                                                           ZMPreviewGroupInfo *_Nullable))completionHandler
                          timeoutHandler:(void(^ _Nullable)())timeoutHandler;


- (void)handlePreviewReceipt:(ZMPreviewGroupInfo *)groupInfo;

@end

NS_ASSUME_NONNULL_END
