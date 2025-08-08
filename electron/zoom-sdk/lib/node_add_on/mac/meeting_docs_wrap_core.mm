#include "Header_include.h"
#include "sdk_native_error.h"
#include "meeting_docs_wrap_core.h"
#include "meeting_service_wrap_core.h"
#import "meetingServiceDelegate.h"
ZMeetingDocsWrap &ZMeetingServiceWrap::GetMeetingDocsCtrl()
{
    return m_meeting_docs_ctrl;
}

ZMeetingDocsWrap::ZMeetingDocsWrap()
{
   m_pSink = 0;
}

ZMeetingDocsWrap::~ZMeetingDocsWrap()
{
    m_pSink = 0;
    [[[[ZoomSDK sharedSDK] getMeetingService] getMeetingDocsController] setDelegate:nil];
}

void ZMeetingDocsWrap::Init()
{

}

void ZMeetingDocsWrap::Uninit()
{

}

void ZMeetingDocsWrap::SetSink(ZNativeSDKMeetingDocsWrapSink *pSink)
{
    [[[[ZoomSDK sharedSDK] getMeetingService] getMeetingDocsController] setDelegate:[[meetingServiceMeetingDocsControllerDelegate alloc] init]];
    m_pSink = pSink;
}

bool ZMeetingDocsWrap::IsSupportDocs() {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) 
        return false;

    ZoomSDKMeetingDocsController *docsController = [service getMeetingDocsController];
    if (!docsController) 
        return false;

    return [docsController isSupportDocs];
}

bool ZMeetingDocsWrap::CanStartShareDocs(ZNCannotShareReasonType& zn_reason) {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) 
        return false;

    ZoomSDKMeetingDocsController *docsController = [service getMeetingDocsController];
    if (!docsController) 
        return false;

    ZoomSDKCannotShareReasonType reason;
    BOOL bCan = [docsController canStartShareDocs:&reason];

    nativeErrorTypeHelp Help_type;
    zn_reason = Help_type.ZNCannotShareReasonTypeMap(reason);
    return bCan;
}

bool ZMeetingDocsWrap::CanSetDocsOption() {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) 
        return false;

    ZoomSDKMeetingDocsController *docsController = [service getMeetingDocsController];
    if (!docsController) 
        return false;

    return [docsController canSetDocsOption];
}

ZNSDKError ZMeetingDocsWrap::SetDocsShareOption(ZNSDKDocsShareOption zn_option) {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) 
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKMeetingDocsController *docsController = [service getMeetingDocsController];
    if (!docsController) 
        return ZNSDKERR_SERVICE_FAILED;

    nativeErrorTypeHelp Help_type;
    ZoomSDKDocsShareOption option = Help_type.ZoomSDKDocsShareOptionMap(zn_option);
    ZoomSDKError ret = [docsController setDocsShareOption:option];
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingDocsWrap::GetDocsShareOption(ZNSDKDocsShareOption& zn_option) {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) 
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKMeetingDocsController *docsController = [service getMeetingDocsController];
    if (!docsController) 
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKDocsShareOption option = [docsController getDocsShareOption];
    nativeErrorTypeHelp Help_type;
    zn_option = Help_type.ZNSDKDocsShareOptionMap(option);
    return ZNSDKERR_SUCCESS;
}

ZNSDKError ZMeetingDocsWrap::SetDocsCreateOption(ZNSDKDocsCreateOption zn_option) {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) 
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKMeetingDocsController *docsController = [service getMeetingDocsController];
    if (!docsController) 
        return ZNSDKERR_SERVICE_FAILED;

    nativeErrorTypeHelp Help_type;
    ZoomSDKDocsCreateOption option = Help_type.ZoomSDKDocsCreateOptionMap(zn_option);
    ZoomSDKError ret = [docsController setDocsCreateOption:option];
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingDocsWrap::GetDocsCreateOption(ZNSDKDocsCreateOption& zn_option) {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) 
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKMeetingDocsController *docsController = [service getMeetingDocsController];
    if (!docsController) 
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKDocsCreateOption option = [docsController getDocsCreateOption];
    nativeErrorTypeHelp Help_type;
    zn_option = Help_type.ZNSDKDocsCreateOptionMap(option);
    return ZNSDKERR_SUCCESS;
}

bool ZMeetingDocsWrap::IsOtherSharingDocs() {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) 
        return false;

    ZoomSDKMeetingDocsController *docsController = [service getMeetingDocsController];
    if (!docsController) 
        return false;

    return [docsController isOtherSharingDocs];
}

bool ZMeetingDocsWrap::IsSharingDocsOut() {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) 
        return false;

    ZoomSDKMeetingDocsController *docsController = [service getMeetingDocsController];
    if (!docsController) 
        return false;

    return [docsController isSharingDocsOut];
}

ZNSDKError ZMeetingDocsWrap::SetOwnerWindow(ZoomSTRING hOwner) {
    return ZNSDKERR_NO_IMPL;
}

ZNSDKError ZMeetingDocsWrap::ShowDocsDashboard(int x, int y) {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) 
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKMeetingDocsController *docsController = [service getMeetingDocsController];
    if (!docsController) 
        return ZNSDKERR_SERVICE_FAILED;

    NSPoint point = NSMakePoint(x, y);
    ZoomSDKError ret = [docsController showDashboardWindow:point];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingDocsWrap::SetDocDashboardSize(int width, int height) {
    return ZNSDKERR_NO_IMPL;
}

ZNSDKError ZMeetingDocsWrap::SetDocsViewPos(int x, int y) {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) 
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKMeetingDocsController *docsController = [service getMeetingDocsController];
    if (!docsController) 
        return ZNSDKERR_SERVICE_FAILED;

    NSPoint point = NSMakePoint(x, y);
    ZoomSDKError ret = [docsController setDocsWindowPos:point];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingDocsWrap::SetDocsViewSize(int width, int height) {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) 
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKMeetingDocsController *docsController = [service getMeetingDocsController];
    if (!docsController) 
        return ZNSDKERR_SERVICE_FAILED;

    NSSize size = NSMakeSize(width, height);
    ZoomSDKError ret = [docsController setDocsWindowSize:size];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingDocsWrap::ShowDocsViewWindow(unsigned int shareSourceID) {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) 
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKMeetingDocsController *docsController = [service getMeetingDocsController];
    if (!docsController) 
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKError ret = [docsController showDocsWindow:shareSourceID];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingDocsWrap::CloseDocsViewWindow() {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) 
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKMeetingDocsController *docsController = [service getMeetingDocsController];
    if (!docsController) 
        return ZNSDKERR_SERVICE_FAILED;

    ZoomSDKError ret = [docsController closeDocsWindow];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNList<ZNZoomSDKDocSharingSourceInfo> ZMeetingDocsWrap::GetDocsSharingSourceInfoList(unsigned int userID)
{
    ZNList<ZNZoomSDKDocSharingSourceInfo> zn_list;
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service){
        return zn_list;
    }
    ZoomSDKMeetingDocsController *docsController = [service getMeetingDocsController];
    if (!docsController) {
        return zn_list;
    }
    NSArray *arr = [docsController getSharingSourceInfoList:userID];
    if (!arr || arr.count == 0) {
        return zn_list;
    }
    for (ZoomSDKDocsSharingSourceInfo *info in arr) {
        nativeErrorTypeHelp Help_type;
        ZNZoomSDKDocSharingSourceInfo zn_info = Help_type.ConvertSDKDocsSharingSourceInfoToZNDocsSharingSourceInfo(info);
        zn_list.push_back(zn_info);
    }
    return zn_list;
}

ZNList<unsigned int> ZMeetingDocsWrap::GetViewableDocsSharingUserList()
{
    ZNList<unsigned int> zn_list;
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return zn_list;
    
    ZoomSDKMeetingDocsController *docsController = [service getMeetingDocsController];
    if (!docsController)
        return zn_list;

    NSArray *arr = [docsController getViewableSharingUserList];
    if (!arr || arr.count == 0) {
        return zn_list;
    }
    for (NSNumber* number in arr) {
        if(!number)
            continue;
        unsigned int userid = [number unsignedIntValue];
        zn_list.push_back(userid);
    }
    return zn_list;
}

//callback
void ZMeetingDocsWrap::onDocsSharingSourceInfoChanged(ZNZoomSDKDocSharingSourceInfo info) {
    if (m_pSink) {
        m_pSink->onDocsSharingSourceInfoChanged(info);
    }
}

void ZMeetingDocsWrap::onDocsPermissionChanged(ZNSDKDocsCreateOption createOption, ZNSDKDocsShareOption shareOption) {
    if (m_pSink) {
        m_pSink->onDocsPermissionChanged(createOption, shareOption);
    }
}