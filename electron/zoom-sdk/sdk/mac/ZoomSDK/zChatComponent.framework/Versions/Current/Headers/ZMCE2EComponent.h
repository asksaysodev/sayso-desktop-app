//
//  ZMCE2EComponent.h
//  zChatComponent
//
//  Created by Huxley Yang on 11/12/24.
//

#import <Cocoa/Cocoa.h>
#import <zChatComponent/ZMCBaseComponent.h>
#include <zPTApp/SaasBeePTAppInterface.h>
#include <zChatApp/E2EManager/IZoomMessengerE2E.h>
#import <ZoomKit/ZoomKit.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMToast;

@protocol ZMCE2EAPI <NSObject> //!IZoomMessengerE2E
@optional

//MARK: APIs
@property (readonly) E2EChatOption myOption;
@property (readonly) BOOL isE2ESelectiveModeEnabled;
@property (readonly) BOOL isACEEnabled;//! Whether I enabled ACE mode

//! Get E2E session type
- (BOOL)isE2ESession:(NSString *)sessionID;
- (NS_ZOOM_MESSAGER::E2ESessionType)getE2ESessionType:(NSString *)sessionID;

//MARK: CSE
//! Whether I enabled CSE mode
@property (readonly) BOOL isCSEEnabled;
//! Wether need login CSE key connector
- (BOOL)isNeedLoginCSEKeyConnector:(NSString *)sessionID;
//! Whether I authed CSE key connector
@property (readonly) BOOL isCSEKeyConnectorLogin;
//! Try to auth CSE key connector, callback is IZoomMessengerE2EUI::E2E_OnLoginCSEKeyConnectorResult
- (BOOL)loginCSEKeyConector;

@end

@protocol ZMCE2ECallback <NSObject> //!IZoomMessengerE2EUI
@optional

//MARK: Common
- (void)myStateUpdate:(MyE2EState)state;

- (void)sessionStateUpdate:(NSString *)sessionID tid:(NSString *)tid state:(E2ESessionState)state error:(NS_SSB_XMPP::E2EError)err;

- (void)messageStateUpdate:(NSString *)sessionID messageID:(NSString *)messageID newState:(MessageState)newState;

- (void)notifyAutoLogoff;

- (void)notifyKeyBindFailed:(NSString *)sessionID msgs:(NSArray <NSString *>*)msgs;

//MARK: CSE
//! Notify ui to login the CSE key connector.
- (void)notifyToLoginCSEKeyConnector:(CmmInt32)reason;

//! E2E_LoginCSEKeyConector callback.
- (void)onLoginCSEKeyConnectorResult:(const NS_ZOOM_MESSAGER::LoginCSEKeyConnectorResult)result;

//! Notify that create CSE data key failed. The key is for encrypting the sending message.
- (void)onCreateCSEDataKeyFailed:(const NS_ZOOM_MESSAGER::CreateCSEDataKeyResult)result;

//! Notify that fetch CSE data key failed. The key is for decrypting the received message.
- (void)onFetchCSEDataKeyFailed:(const NS_ZOOM_MESSAGER::FetchCSEDataKeyResult)result;

@end

typedef NS_OPTIONS(NSUInteger, ZMCE2EErrorToastType) {
    ZMCE2EErrorToastTypeSend = 1 << 0,
    ZMCE2EErrorToastTypeReceive = 1 << 1,
    ZMCE2EErrorToastTypeForward = 1 << 2,
};

@protocol ZMCE2EUIProvider <NSObject>
@optional

@property (readonly, nonatomic) CGFloat preferredWidthOfAuthenticateBannerView;
- (NSView *)authenticateBannerView;

- (ZMToast *)errorToastWithType:(ZMCE2EErrorToastType)type needConnector:(BOOL)needConnector errorCode:(NSUInteger)errorCode;

@end

@class ZMCE2EAPI, ZMCE2ECallback, ZMCE2EUIProvider;
@interface ZMCE2EComponent : ZMCBaseComponent <ZMCE2EAPI, ZMCE2EUIProvider>

@property (nullable, strong, readonly) ZMCE2EAPI *api;
@property (nullable, strong, readonly) ZMCE2ECallback *sink;
@property (nullable, strong, readonly) ZMCE2EUIProvider *UIProvider;

- (void)addSinkTarget:(id <ZMCE2ECallback>)target;
@property (readonly, nonatomic) NSArray <ZMCE2ECallback>*allTargets;

- (ns_zoom_messager::IZoomMessengerE2E *)pMessengerE2E;

@end

@interface ZMCE2EComponentImp : ZMCBaseComponentImp

@property (weak, readonly, nonatomic, nullable) ZMCE2EComponent *component;

@end

NS_ASSUME_NONNULL_END
