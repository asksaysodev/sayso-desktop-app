//
//  ZMReducedRoomEditInfo.h
//  zChatComponent
//
//  Created by Simon Shang on 2024/4/29.
//

#import <Foundation/Foundation.h>
#import <zPTApp/SaasBeePTAppInterface.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMReducedRoomEditInfo : NSObject

@property (nonatomic, copy) NSString *sessionId;
@property (nonatomic, assign) BOOL everyOneCanReply;
@property (nonatomic, assign) BOOL isArchived;
@property (nonatomic, assign) ns_zoom_messager::AtAllOption atAllOptionType;
@property (nonatomic, assign) ns_zoom_messager::GroupAnnouncerType announceType;
@property (nonatomic, copy) NSArray *subGroupIds;

@end

NS_ASSUME_NONNULL_END
