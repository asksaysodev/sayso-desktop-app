//
//  ZMSendMessageParams.h
//  zChatComponent
//
//  Created by Yong Zhou on 1/6/24.
//

#import <Foundation/Foundation.h>

@class ZMFileEntity;

/*
 Send Message Params
 Design pattern: adapter for avoid too many input params and version incompatibility for `sendText` method
 */

@protocol ZMSendMessageUIProtocol <NSObject>

- (void)checkShowUserGuideInE2EChannelAfterSendHybridMsg;

@end

@interface ZMSendMessageParams : NSObject

// General chat
@property (nonatomic, copy) NSString *sessionID;
@property (nonatomic, copy) NSString *threadID;
@property (nonatomic, assign) NSUInteger messageType;
@property (nonatomic, assign) BOOL isScreenShot;
@property (nonatomic, retain) ZMFileEntity *fileEntity;
@property (nonatomic, retain) NSString *body; // will be deprecated later
@property (nonatomic, retain) NSAttributedString *attrString;
@property (nonatomic, retain) NSArray *fileList;
@property (nonatomic, retain) NSArray *independentSendFileList;
@property (nonatomic, retain) NSArray *urlArray;
@property (nonatomic, retain) NSArray *appPreviewsInfoArray;
@property (nonatomic, retain) NSArray<NSString *> *dismissLinks;
@property (nonatomic, strong) NSMutableDictionary *appMessagePreviewField;
@property (nonatomic, assign) BOOL isRecordVideoMessage;
@property (nonatomic, assign) BOOL isSharedMessage;

// meeting chat
@property (nonatomic, assign) BOOL isMeetMessage;
@property (nonatomic, assign) NSInteger meetMsgType;
@property (nonatomic, copy) NSString *meetDMReceiverId;
@property (nonatomic, assign) NSInteger meetDMReceiverNodeId;

@property (nonatomic, retain, nullable) NSString *draftID;
@property (nonatomic, assign) BOOL isThreadedDraft;

@property (nonatomic, weak, nullable) id <ZMSendMessageUIProtocol> UIDelegate;

@property (nonatomic, assign, readonly, getter=isValid) BOOL valid;

@property (nonatomic, retain) NSArray <NSString *> *multipleShareSessionIds;

@property (nonatomic, assign) BOOL isFromMultipleForward;

@end


@interface ZMSendMessageSessionInfo : NSObject

@property (nonatomic, copy, nonnull) NSString *sessionID;
@property (nonatomic, assign, readonly) BOOL isE2E;
@property (nonatomic, assign, readonly) BOOL isEnabled3rdPartStorage;
@property (nonatomic, assign, readonly) BOOL isZoomMeetChannel;
@property (nonatomic, assign, readonly) BOOL isCMC;

@end

