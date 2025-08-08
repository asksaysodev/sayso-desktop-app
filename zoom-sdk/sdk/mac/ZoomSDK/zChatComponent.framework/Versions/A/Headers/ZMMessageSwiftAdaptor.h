//
//  ZMMessageSwiftAdaptor.h
//  zChatComponent
//
//  Created by Cyan.Yang on 2024/6/26.
//

#import <Foundation/Foundation.h>

#import <zChatComponent/ZMIMCommonDefine.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMMessageAdapter;

NS_SWIFT_NAME(Message)
@interface ZMMessageSwiftAdaptor : NSObject

@property (nonatomic, readonly, strong) ZMMessageAdapter *message;

@property (nonatomic, readonly) NSString *messageID;
@property (nonatomic, readonly, nullable) NSString *sessionID;
@property (nonatomic, readonly, nullable) NSString *senderID;
@property (nonatomic, readonly) NSDate *serverSideTime;

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithMessage:(ZMMessageAdapter *)message NS_DESIGNATED_INITIALIZER;

- (void)requestToShareMeetingAssets:(ZMMeetingAssetsSharingCategory)categories
                          meetingID:(NSString *)meetingID NS_SWIFT_NAME(requestToShareMeetingAssets(_:meetingID:));

@end

NS_ASSUME_NONNULL_END
