

#import <Foundation/Foundation.h>
#import "Header_include.h"
NS_ASSUME_NONNULL_BEGIN
@interface meetingServiceDelegate : NSObject<ZoomSDKMeetingServiceDelegate,ZoomSDKMeetingActionControllerDelegate>
@property(nonatomic,retain)ZoomSDKJoinMeetingHelper *joinMeetingHelper;
@property(nonatomic,retain)ZoomSDKMeetingInputUserInfoHandler *meetingInputUserInfoHandler;
@property(nonatomic,retain)ZoomSDKMeetingArchiveConfirmHandler *meetingArchiveConfirmHandler;

+(meetingServiceDelegate *)share;

@end

@interface meetingServiceChatDelegate : NSObject<ZoomSDKMeetingChatControllerDelegate>
+(meetingServiceChatDelegate *)share;
@property(nonatomic,retain)ZoomSDKFileSender *fileSender;
@property(nonatomic,retain)ZoomSDKFileReceiver *fileReceiver;
@end

@interface meetingServiceRecordDelegate : NSObject<ZoomSDKMeetingRecordDelegate>
+(meetingServiceRecordDelegate *)share;
@property(nonatomic,retain)ZoomSDKRequestStartCloudRecordingHandler *cloudRecordingHandler;
@property(nonatomic,retain)ZoomSDKRequestEnableAndStartSmartRecordingHandler *enableAndStartSmartRecordingHandler;
@property(nonatomic,retain)ZoomSDKSmartRecordingEnableActionHandler *smartRecordingEnableActionHandler;
@end

@interface meetingServiceWaitingRoomDelegate : NSObject<ZoomSDKWaitingRoomDelegate>
+(meetingServiceWaitingRoomDelegate *)share;
@property(nonatomic,retain)ZoomSDKWaitingRoomDataDownloadHandler *waitingRoomHandler;
@end

@interface meetingUIControllerDelegate : NSObject<ZoomSDKMeetingUIControllerDelegate>

@end

@interface meetingH323HelperDelegate : NSObject<ZoomSDKH323HelperDelegate>

@end

@interface meetingASControllerDelegate : NSObject<ZoomSDKASControllerDelegate>
+(meetingASControllerDelegate *)share;
@end

@interface realNameAuthenticationDelegate : NSObject<ZoomSDKRealNameAuthenticationDelegate>

@property(nonatomic,retain)ZoomSDKRetrieveSMSVerificationCodeController *retriveController;
@property(nonatomic,retain)ZoomSDKVerifySMSVerificationCodeController *verifyCintroller;

+(realNameAuthenticationDelegate *)share;
-(BOOL)getResendRetriveController;
-(BOOL)getReVerifyCintroller;
@end

@interface WebinarControllerDelegate : NSObject<ZoomSDKWebinarControllerDelegate>
@property(nonatomic,retain)ZoomSDKWebinarRegisterHelper *webinarRegisterHelper;
@property(nonatomic,retain)ZoomSDKWebinarInputScreenNameHandler *webinarInputScreenNameHandler;
+(WebinarControllerDelegate *)share;
@end

@interface meetingServiceAICompanionControllerDelegate : NSObject<ZoomSDKMeetingAICompanionControllerDelegate>
+(meetingServiceAICompanionControllerDelegate *)share;
@property(nonatomic,retain)ZoomSDKAICompanionFeatureTurnOnAgainHandler *aiCompanionFeatureTurnOnAgainHandler;
@property(nonatomic,retain)ZoomSDKAICompanionFeatureSwitchHandler *aiCompanionFeatureSwitchHandler;
@end

@interface meetingServiceQADelegate : NSObject<ZoomSDKQAControllerDelegate>

@end

@interface meetingServiceReactionDelegate : NSObject<ZoomSDKReactionControllerDelegate>

@end

@interface meetingServiceCloseCaptionDelegate : NSObject<ZoomSDKCloseCaptionControllerDelegate>

@end

@interface meetingServicePollingDelegate : NSObject<ZoomSDKPollingControllerDelegate>

@end


@interface meetingServiceAICompanionSmartSummaryDelegate : NSObject<ZoomSDKAICompanionSmartSummaryHelperDelegate>
+(meetingServiceAICompanionSmartSummaryDelegate *)share;
@property(nonatomic,retain)ZoomSDKEnableSmartSummaryHandler *enableSmartSummaryHandler;
@property(nonatomic,retain)ZoomSDKStartSmartSummaryHandler *startSmartSummaryHandler;
@property(nonatomic,retain)ZoomSDKStopSmartSummaryHandler *stopSmartSummaryHandler;
@property(nonatomic,retain)ZoomSDKApproveEnableSmartSummaryHandler *approveEnableSmartSummaryHandler;
@property(nonatomic,retain)ZoomSDKApproveStartSmartSummaryHandler *approveStartSmartSummaryHandler;
@property(nonatomic,retain)ZoomSDKEnableSmartSummaryActionHandler *enableSmartSummaryActionHandler;
@end

@interface meetingServiceAICompanionQueryDelegate : NSObject<ZoomSDKAICompanionQueryHelperDelegate>
+(meetingServiceAICompanionQueryDelegate *)share;
@property(nonatomic,retain)ZoomSDKEnableQueryHandler *enableQueryHandler;
@property(nonatomic,retain)ZoomSDKStartQueryHandler *startQueryHandler;
@property(nonatomic,retain)ZoomSDKSendQueryHandler *sendQueryHandler;
@property(nonatomic,retain)ZoomSDKApproveEnableQueryHandler *approveEnableQueryHandler;
@property(nonatomic,retain)ZoomSDKApproveStartQueryHandler *approveStartQueryHandler;
@property(nonatomic,retain)ZoomSDKAICompanionQueryItem *aiCompanionQueryItem;
@property(nonatomic,retain)ZoomSDKEnableQueryActionHandler *enableQueryActionHandler;
@property(nonatomic,retain)ZoomSDKApproveSendQueryHandler *approveSendQueryHandler;
@end

@interface meetingServiceIndicatorControllerDelegate : NSObject<ZoomSDKMeetingIndicatorControllerDelegate>
+(meetingServiceIndicatorControllerDelegate *)share;
@property(nonatomic,retain)ZoomSDKMeetingIndicatorHandle *indicatorHandle;
@end

@interface meetingServiceWhiteboardControllerDelegate : NSObject<ZoomSDKMeetingWhiteboardControllerDelegate>

@end

@interface meetingServiceMeetingDocsControllerDelegate : NSObject<ZoomSDKMeetingDocsControllerDelegate>

@end

NS_ASSUME_NONNULL_END
