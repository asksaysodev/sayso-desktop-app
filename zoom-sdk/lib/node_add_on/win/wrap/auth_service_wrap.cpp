#pragma once
#include "auth_service_wrap.h"
BEGIN_ZOOM_SDK_NAMESPACE
ZOOM_SDK_NAMESPACE::IAuthService* InitIAuthServiceFunc(ZOOM_SDK_NAMESPACE::IAuthServiceEvent* pEvent)
{
	ZOOM_SDK_NAMESPACE::IAuthService* pObj(NULL);
	CSDKImpl::GetInst().CreateAuthService(&pObj);
	if (pObj)
	{
		pObj->SetEvent(pEvent);
	}
	return pObj;
}

void UninitIAuthServiceFunc(ZOOM_SDK_NAMESPACE::IAuthService* obj)
{
	CSDKImpl::GetInst().DestroyAuthService(obj);
}
	//IMPL_FUNC_1(IAuthService, SetEvent, SDKError, IAuthServiceEvent*, pEvent, SDKERR_UNINITIALIZE)
	IMPL_FUNC_1(IAuthService, SDKAuth, SDKError, AuthContext&, authContext, SDKERR_UNINITIALIZE)
	IMPL_FUNC_0(IAuthService, GetAuthResult, AuthResult, AUTHRET_NONE)
	IMPL_FUNC_0(IAuthService, GetSDKIdentity, const zchar_t*, NULL)
	IMPL_FUNC_1(IAuthService, GenerateSSOLoginWebURL, const zchar_t*, const zchar_t*, prefix_of_vanity_url, NULL)
	IMPL_FUNC_1(IAuthService, SSOLoginWithWebUriProtocol, SDKError, const zchar_t*, uri_protocol, SDKERR_UNINITIALIZE)
	IMPL_FUNC_0(IAuthService, LogOut, SDKError, SDKERR_UNINITIALIZE)
	IMPL_FUNC_0(IAuthService, GetAccountInfo, IAccountInfo*, NULL)
	IMPL_FUNC_0(IAuthService, GetLoginStatus, LOGINSTATUS, LOGIN_IDLE)

#if (defined UserInterfaceClass)
	//virtual IDirectShareServiceHelper* GetDirectShareServiceHelper() = 0;
	IMPL_FUNC_0(IAuthService, GetDirectShareServiceHelper, IDirectShareServiceHelper*, NULL)
#endif
#if defined(WIN32)
	IMPL_FUNC_AND_MEMBER(IAuthService, T_GetDirectShareServiceHelper, IDirectShareServiceHelperWrap)
#endif

END_ZOOM_SDK_NAMESPACE
