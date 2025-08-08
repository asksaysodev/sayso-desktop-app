//
//  ZMPublicRoomInfo.h
//  ZoomMsgUI
//
//  Created by Groot Ding on 2022/5/18.
//

#import <Foundation/Foundation.h>
#if defined(__cplusplus)
#import <zPTApp/SaasBeePTAppInterface.h>
#endif

NS_ASSUME_NONNULL_BEGIN

@interface ZMPublicRoomInfo : NSObject

@property (nonatomic, copy) NSString* groupName;
@property (nonatomic, copy) NSString* ownerName;
@property (nonatomic, copy) NSString* jid;
@property (nonatomic, assign) long long memberCount;
@property (nonatomic, assign) BOOL isSelected;
#if defined(__cplusplus)
@property (nonatomic, assign) NS_SSB_XMPP::kMUCFlag flag;
#endif
@property (nonatomic, assign) BOOL amIInGroup;

@property (nonatomic, copy) NSString *channelIcon;
@property (nonatomic, assign) NSInteger iconType;

#if defined(__cplusplus)
- (id)initWithZoomRoom:(NS_SSB_XMPP::ZoomXmppRoom)room;
#endif

@end

NS_ASSUME_NONNULL_END
