

#include "auth_service_wrap_core.h"
#include "Header_include.h"
#import "preMeetingServiceDelegate.h"
#import "sdk_native_error.h"
static nativeErrorTypeHelp error;
ZAuthServiceWrap::ZAuthServiceWrap()
{
    m_pSink = 0;
}

ZAuthServiceWrap::~ZAuthServiceWrap()
{
    m_pSink = 0;
    [[[ZoomSDK sharedSDK] getAuthService] setDelegate:nil];
}

void ZAuthServiceWrap::Init()
{
    ZoomSDKAuthService *auth = [[ZoomSDK sharedSDK] getAuthService];
    if (auth) {
        [auth setDelegate:[authServiceDelegate share]];
    }
}

void ZAuthServiceWrap::Uninit()
{

}

void ZAuthServiceWrap::SetSink(ZNativeSDKAuthWrapSink *pSink)
{
    m_pSink = pSink;
}

ZNSDKError ZAuthServiceWrap::AuthSDK(ZNAuthContext& authContext)
{
    NSString *jwtToken = [NSString stringWithCString:authContext.sdk_jwt_token.c_str() encoding:NSUTF8StringEncoding];
    if (!jwtToken || jwtToken.length == 0) {
        return ZNSDKERR_INVALID_PARAMETER;
    }
    ZoomSDKAuthService *auth = [[ZoomSDK sharedSDK] getAuthService];
    if (!auth) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    ZoomSDKAuthContext *context = [[ZoomSDKAuthContext alloc] init];
    context.jwtToken = jwtToken;
    ZoomSDKError ret = [auth sdkAuth:context];
    nativeErrorTypeHelp error;
    return error.ZoomSDKErrorType(ret);
}

ZoomSTRING ZAuthServiceWrap::GenerateSSOLoginWebURL(ZoomSTRING prefix_of_vanity_url)
{
    if (prefix_of_vanity_url.empty())
        return  nil;
    ZoomSDKAuthService *auth = [[ZoomSDK sharedSDK] getAuthService];
    if (!auth)
        return nil;
    
    NSString* urlStr = [auth generateSSOLoginWebURL:[NSString stringWithUTF8String:prefix_of_vanity_url.c_str()]];
    return [urlStr UTF8String];
}

ZNSDKError ZAuthServiceWrap::SSOLoginWithWebUriProtocol(ZoomSTRING uri_protocol)
{
    if (uri_protocol.empty())
        return  ZNSDKERR_INVALID_PARAMETER;
    ZoomSDKAuthService *auth = [[ZoomSDK sharedSDK] getAuthService];
    if (!auth)
        return ZNSDKERR_SERVICE_FAILED;
    ZoomSDKError ret = [auth SSOLoginWithWebUriProtocol:[NSString stringWithUTF8String:uri_protocol.c_str()]];
    nativeErrorTypeHelp error;
    return error.ZoomSDKErrorType(ret);
}

ZNSDKError ZAuthServiceWrap::Logout()
{
    ZoomSDKAuthService *auth = [[ZoomSDK sharedSDK] getAuthService];
    if (!auth) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    ZoomSDKError ret = [auth logout];
    nativeErrorTypeHelp error;
    return error.ZoomSDKErrorType(ret);
}

ZNAuthResult ZAuthServiceWrap::GetAuthResult()
{
    return  [[authServiceDelegate share] getAuthResult];
}

ZNLOGINSTATUS ZAuthServiceWrap::GetLoginStatus()
{
    return [[authServiceDelegate share] getLoginStatus];
}

//callback
void ZAuthServiceWrap::onAuthenticationReturn(ZNAuthResult authResult)
{
    if (m_pSink) {
        m_pSink->onAuthenticationReturn(authResult);
    }
}

void ZAuthServiceWrap::onLoginReturnWithReason(ZNLOGINSTATUS ret, ZNLoginFailReason reason)
{
    if (m_pSink)
    {
        m_pSink->onLoginReturnWithReason(ret, reason);
    }
}

void ZAuthServiceWrap::onLogout()
{
    if (m_pSink) {
        m_pSink->onLogout();
    }
}


void ZAuthServiceWrap::onZoomIdentityExpired()
{
    if (m_pSink) {
        m_pSink->onZoomIdentityExpired();
    }
}

void ZAuthServiceWrap::onZoomAuthIdentityExpired()
{
    if (m_pSink) {
        m_pSink->onZoomAuthIdentityExpired();
    }
}


