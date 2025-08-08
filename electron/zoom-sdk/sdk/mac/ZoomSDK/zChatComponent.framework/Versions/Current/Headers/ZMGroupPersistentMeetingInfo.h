//
//  ZMGroupPersistentMeetingInfo.h
//  zChatComponent
//
//  Created by Simon Shang on 2022/10/9.
//

#import <Foundation/Foundation.h>
#ifdef __cplusplus
#import <zPTApp/SaasBeePTAppInterface.h>
#endif

NS_ASSUME_NONNULL_BEGIN

@interface ZMGroupPersistentMeetingInfo : NSObject

@property (nonatomic, copy, readonly) NSString *hostId;
@property (nonatomic, copy, readonly) NSString *topic;
@property (nonatomic, copy, readonly) NSString *parentId;
@property (nonatomic, assign, readonly) long meetingNumber;
@property (nonatomic, assign, readonly) BOOL isMeetingOnProgress;
@property (nonatomic, assign, readonly) BOOL isRecurring;
@property (nonatomic, copy, readonly) NSString *meetingPwd;
@property (nonatomic, assign, readonly) long startTime;
@property (nonatomic, assign, readonly) long endTime;
@property (nonatomic, assign, readonly) BOOL isIncludeDetail; // if false, only "meetingNum" and "isRecurring" is valid
@property (nonatomic, assign, readonly) BOOL isNoFixedTime; // a recurring meeting ("isRecurring" is true), maybe without fixed time

#ifdef __cplusplus
- (void)updateInfoWithGroup:(NS_ZOOM_MESSAGER::IZoomGroup*)buddyGroup isRequestFromWeb:(BOOL)bRequestFromWeb;
#endif

@end

NS_ASSUME_NONNULL_END
