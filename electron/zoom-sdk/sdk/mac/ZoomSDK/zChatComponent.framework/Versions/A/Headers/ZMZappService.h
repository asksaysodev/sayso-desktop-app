//
//  ZMZappService.h
//  zChatComponent
//
//  Created by groot.ding on 9/26/23.
//

#import <Foundation/Foundation.h>
#import <ZoomUnit/ZPZAppAdapter.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMZappService : NSObject

+ (void)getZappHeadById:(NSString *)appId completion:(void (^)(ZPZAppHeader *header, NSError *error))completion;

+ (void)onGetZappHeaderInPT:(NSString *)key header:(ZPZAppHeader *)header;

+ (NSString *)getInvitationThumbnailPath:(NSString *)msgId;

+ (void)downloadInvitationThumbnail:(NSString *)msgId downloadParam:(ZMZappDownloadParam *)downloadParam;

@end

NS_ASSUME_NONNULL_END
