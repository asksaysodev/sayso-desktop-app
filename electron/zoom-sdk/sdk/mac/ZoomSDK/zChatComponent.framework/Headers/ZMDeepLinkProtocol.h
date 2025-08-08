//
//  ZMDeepLinkProtocol.h
//  zChatComponent
//
//  Created by Groot Ding on 2022/7/19.
//

#ifndef ZMDeepLinkProtocol_h
#define ZMDeepLinkProtocol_h

#import <zChatComponent/ZMIMCommonDefine.h>
#include <zChatComponent/ZMDeepLinkEntity.h>

#if defined(__cplusplus)
#include <zPTApp/SaasBeePTAppInterface.h>
#endif

@class ZMMessageAdapter;

@interface ZMUniversalDeepLinkInfo : NSObject

@property (nonatomic) ZMUniversalDeepLinkType linkType;
@property (nonatomic, nullable, retain) NSString *messageID;
@property (nonatomic, nullable, retain) NSString *sessionID;

@end

@protocol ZMDeepLinkProtocol <NSObject>

@optional

- (BOOL)isDeepLink:(NSString *_Nonnull)url sessionId:(NSString *_Nullable)sessionId;
- (BOOL)shouldShowDeepLinkFormatForURL:(NSString *_Nonnull)url sessionID:(NSString *_Nullable)sessionID;

- (void)openLink:(NSString* _Nonnull)url sessionId:(NSString *_Nullable)sessionId;

- (ZMUniversalDeepLinkType)getDeepLinkType:(NSString *_Nonnull)url sessionId:(NSString *_Nullable)sessionId;
- (ZMUniversalDeepLinkInfo *_Nullable)getLinkInfo:(NSString *_Nonnull)forLink sessionId:(NSString *_Nullable)sessionId;

/// FeaturePhase_1
- (BOOL)isDeepLinkEnabled;
#if defined(__cplusplus)
- (BOOL)isDeepLinkEnabledForPhase:(NS_ZOOM_MESSAGER::IDeepLinkV2Manager::FeaturePhase)phase;
#endif
- (BOOL)isDeepLinkEnabledForSessionID:(NSString *_Nonnull)sessionID messageID:(NSString *_Nullable)messageID serverTime:(long long)serverTime;
/// V4 channel invite links
- (BOOL)isDeeplinkV4Enabled;

- (ZMDeepLinkEntity *_Nullable)getEntity:(NSString *_Nonnull)forLink sessionId:(NSString *_Nullable)sessionId;

- (void)makeLinkFromMessageAdapter:(ZMMessageAdapter *_Nonnull)adapter
                          callback:(void(^_Nullable)(NSString *_Nonnull, NSUInteger))callback;

- (void)makeLinkFromSessionID:(NSString *_Nonnull)session_id
                    messageID:(NSString *_Nullable)message_id
             messageTimestamp:(NSUInteger)message_timestamp
                     callback:(void(^_Nullable)(NSString *_Nonnull, NSUInteger))callback;

- (BOOL)approveJoinRequest:(ZMMessageAdapter *_Nonnull)message
            resultCallback:(void (^_Nullable)(UInt32 xmppResult,
                                              UInt32 joinBlockReason))resultCallback;

- (BOOL)declineJoinRequest:(ZMMessageAdapter *_Nonnull)message
            resultCallback:(void (^_Nullable)(UInt32 xmppResult))resultCallback;

- (void)openMessage:(nonnull NSString *)messageID sessionID:(nonnull NSString *)sessionID serverTime:(unsigned long long)serverTime;
- (void)openSession:(nonnull NSString *)sessionID;

- (void)directJoinSubCMC:(nonnull NSString *)subCMCSessionId;

- (BOOL)openEmail:(nonnull NSString *)email;
- (void)getInviteLinkForSessionId:(NSString* _Nonnull)sessionId callback:(void(^ _Nonnull)(NSString* _Nullable))callback;
- (void)resetInviteLinkForChannel:(NSString* _Nonnull)sessionId;

- (BOOL)shouldShowShareLinkModalOnboardingForChannel:(NSString* _Nonnull)sessionId;
- (void)didShowShareLinkModalOnboarding;
@end

#ifndef ZMShareDeepLinkProtocol
#define ZMShareDeepLinkProtocol (id <ZMDeepLinkProtocol>)ZMSharedFor(ZMDeepLinkProtocol)
#endif


#endif /* ZMDeepLinkProtocol_h */
