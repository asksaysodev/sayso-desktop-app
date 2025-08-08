//
//  ZMStickerInfo.h
//  ZoomMsgUI
//
//  Created by Groot Ding on 2022/4/29.
//

#import <Foundation/Foundation.h>
#include <zPTApp/SaasBeePTAppInterface.h>
#include <zChatApp/StickerInterface.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMStickerInfo : NSObject

- (instancetype)initWithStickerInfo:(ns_zoom_messager::zStickerInfo)stickerInfo;
- (instancetype)initWithStickerFileId:(NSString*)fileId;
- (NSString*)fileId;
- (ns_zoom_messager::zStickerInfo) stickerInfo;
- (void)setStickerInfo:(ns_zoom_messager::zStickerInfo)inStickerInfo;
@end

NS_ASSUME_NONNULL_END
