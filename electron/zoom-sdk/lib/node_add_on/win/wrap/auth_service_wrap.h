#pragma once
#include "common_include.h"
#if defined(WIN32)
#include "directshare_helper_wrap.h"
#endif
BEGIN_ZOOM_SDK_NAMESPACE
ZOOM_SDK_NAMESPACE::IAuthService* InitIAuthServiceFunc(ZOOM_SDK_NAMESPACE::IAuthServiceEvent* pEvent);
void UninitIAuthServiceFunc(ZOOM_SDK_NAMESPACE::IAuthService* obj);

BEGIN_CLASS_DEFINE_WITHCALLBACK(IAuthService, IAuthServiceEvent)
STAITC_CLASS(IAuthService)
INIT_UNINIT_WITHEVENT(IAuthService)
//virtual SDKError SetEvent(IAuthServiceEvent* pEvent) = 0;
//DEFINE_FUNC_1(SetEvent, SDKError, IAuthServiceEvent*, pEvent)
virtual SDKError SetEvent(IAuthServiceEvent* pEvent)
{
	external_cb = pEvent;
	return SDKERR_SUCCESS;
}

//virtual SDKError SDKAuth(AuthContext& authContext) = 0;
DEFINE_FUNC_1(SDKAuth, SDKError, AuthContext&, authContext)

//virtual AuthResult GetAuthResult() = 0;
DEFINE_FUNC_0(GetAuthResult, AuthResult)

//virtual const zchar_t* GetSDKIdentity() = 0;
DEFINE_FUNC_0(GetSDKIdentity, const zchar_t*)

//virtual const zchar_t* GenerateSSOLoginWebURL(const zchar_t* prefix_of_vanity_url) = 0;
DEFINE_FUNC_1(GenerateSSOLoginWebURL, const zchar_t*, const zchar_t*, prefix_of_vanity_url)

//virtual SDKError SSOLoginWithWebUriProtocol(const zchar_t* uri_protocol) = 0;
DEFINE_FUNC_1(SSOLoginWithWebUriProtocol, SDKError, const zchar_t*, uri_protocol)

//virtual SDKError LogOut() = 0;
DEFINE_FUNC_0(LogOut, SDKError)

//virtual IAccountInfo* GetAccountInfo() = 0;
DEFINE_FUNC_0(GetAccountInfo, IAccountInfo*)

//virtual LOGINSTATUS GetLoginStatus() = 0;
DEFINE_FUNC_0(GetLoginStatus, LOGINSTATUS)
#if (defined UserInterfaceClass)
private:
	//virtual IDirectShareServiceHelper* GetDirectShareServiceHelper() = 0;
	DEFINE_FUNC_0(GetDirectShareServiceHelper, IDirectShareServiceHelper*)
public:
#define T_GetDirectShareServiceHelper _GetDirectShareServiceHelper
#else
#define T_GetDirectShareServiceHelper GetDirectShareServiceHelper
#endif

//virtual IDirectShareServiceHelper* GetDirectShareServiceHelper() = 0;
#if defined(WIN32)
DEFINE_FUNC_AND_MEMBER(T_GetDirectShareServiceHelper, IDirectShareServiceHelperWrap)
#endif

//virtual void onAuthenticationReturn(AuthResult ret)
CallBack_FUNC_1(onAuthenticationReturn, AuthResult,ret)
//virtual void onLogout() = 0;
CallBack_FUNC_0(onLogout)
//virtual void onZoomIdentityExpired() = 0;
CallBack_FUNC_0(onZoomIdentityExpired)
//virtual void onZoomAuthIdentityExpired() = 0;
CallBack_FUNC_0(onZoomAuthIdentityExpired)
//virtual void onLoginReturnWithReason(LOGINSTATUS ret, IAccountInfo* pAccountInfo, LoginFailReason reason) = 0;
CallBack_FUNC_3(onLoginReturnWithReason, LOGINSTATUS, ret, IAccountInfo*,pAccountInfo, LoginFailReason, reason)
#if defined(WIN32)
//virtual void onNotificationServiceStatus(SDKNotificationServiceStatus status, SDKNotificationServiceError error) = 0;
CallBack_FUNC_Override_2(onNotificationServiceStatus, SDKNotificationServiceStatus, status, SDKNotificationServiceError, error)
#endif

END_CLASS_DEFINE(IAuthService)
END_ZOOM_SDK_NAMESPACE