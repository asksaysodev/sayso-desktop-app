#include "Header_include.h"
#include "sdk_native_error.h"
#include "meeting_ai_companion_wrap_core.h"
#include "meeting_service_wrap_core.h"
#import "meetingServiceDelegate.h"
ZMeetingAICompanionWrap &ZMeetingServiceWrap::GetMeetingAICompanionCtrl()
{
    return m_meeting_ai_companion_ctrl;
}
ZMeetingAICompanionWrap::ZMeetingAICompanionWrap()
{
   m_pSink = 0;
}

ZMeetingAICompanionWrap::~ZMeetingAICompanionWrap()
{
    m_pSink = 0;
    [[[[ZoomSDK sharedSDK] getMeetingService] getInMeetingAICompanionController] setDelegate:nil];
    [[[[[ZoomSDK sharedSDK] getMeetingService] getInMeetingAICompanionController] getSmartSummaryHelper] setDelegate:nil];
    [[[[[ZoomSDK sharedSDK] getMeetingService] getInMeetingAICompanionController] getQueryHelper] setDelegate:nil];
}

void ZMeetingAICompanionWrap::Init()
{

}
void ZMeetingAICompanionWrap::Uninit()
{
    
}

void ZMeetingAICompanionWrap::SetSink(ZNativeSDKMeetingAICompanionWrapSink *pSink)
{
    [[[[ZoomSDK sharedSDK] getMeetingService] getInMeetingAICompanionController] setDelegate:[meetingServiceAICompanionControllerDelegate share]];
    [[[[[ZoomSDK sharedSDK] getMeetingService] getInMeetingAICompanionController] getSmartSummaryHelper] setDelegate:[meetingServiceAICompanionSmartSummaryDelegate share]];
    [[[[[ZoomSDK sharedSDK] getMeetingService] getInMeetingAICompanionController] getQueryHelper] setDelegate:[meetingServiceAICompanionQueryDelegate share]];
    m_pSink= pSink;
}

// IMeetingAICompanionController
bool ZMeetingAICompanionWrap::IsTurnoffAllAICompanionsSupported()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return false;
    }
    
    ZoomSDKMeetingAICompanionController *aiCompanionController = [service getInMeetingAICompanionController];
    if (!aiCompanionController) {
        return false;
    }
    
    return [aiCompanionController isTurnoffAllAICompanionsSupported];
}

bool ZMeetingAICompanionWrap::IsTurnOnAllAICompanionsSupported()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return false;
    }
    
    ZoomSDKMeetingAICompanionController *aiCompanionController = [service getInMeetingAICompanionController];
    if (!aiCompanionController) {
        return false;
    }
    
    return [aiCompanionController isTurnOnAllAICompanionSupported];
}

bool ZMeetingAICompanionWrap::CanTurnOffAllAICompanions()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return false;
    }
    
    ZoomSDKMeetingAICompanionController *aiCompanionController = [service getInMeetingAICompanionController];
    if (!aiCompanionController) {
        return false;
    }
    
    return [aiCompanionController canTurnOffAllAICompanions];
}

bool ZMeetingAICompanionWrap::CanTurnOnAllAICompanions()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return false;
    }
    
    ZoomSDKMeetingAICompanionController *aiCompanionController = [service getInMeetingAICompanionController];
    if (!aiCompanionController) {
        return false;
    }
    
    return [aiCompanionController canTurnOnAllAICompanion];
}

ZNSDKError ZMeetingAICompanionWrap::TurnOffAllAICompanions(bool deleteMeetingAssets)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    ZoomSDKMeetingAICompanionController *aiCompanionController = [service getInMeetingAICompanionController];
    if (!aiCompanionController) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    ZoomSDKError ret = [aiCompanionController turnOffAllAICompanion:deleteMeetingAssets];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingAICompanionWrap::TurnOnAllAICompanions()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    ZoomSDKMeetingAICompanionController *aiCompanionController = [service getInMeetingAICompanionController];
    if (!aiCompanionController) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    ZoomSDKError ret = [aiCompanionController turnOnAllAICompanion];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

bool ZMeetingAICompanionWrap::CanRequestTurnoffAllAICompanions()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return false;
    }
    
    ZoomSDKMeetingAICompanionController *aiCompanionController = [service getInMeetingAICompanionController];
    if (!aiCompanionController) {
        return false;
    }
    
    return [aiCompanionController canRequestTurnoffAllAICompanion];
}

bool ZMeetingAICompanionWrap::CanRequestTurnOnAllAICompanions()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return false;
    }
    
    ZoomSDKMeetingAICompanionController *aiCompanionController = [service getInMeetingAICompanionController];
    if (!aiCompanionController) {
        return false;
    }
    
    return [aiCompanionController canRequestTurnOnAllAICompanion];
}

ZNSDKError ZMeetingAICompanionWrap::RequestTurnoffAllAICompanions()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    ZoomSDKMeetingAICompanionController *aiCompanionController = [service getInMeetingAICompanionController];
    if (!aiCompanionController) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    ZoomSDKError ret = [aiCompanionController requestTurnoffAllAICompanion];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingAICompanionWrap::RequestTurnOnAllAICompanions()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    ZoomSDKMeetingAICompanionController *aiCompanionController = [service getInMeetingAICompanionController];
    if (!aiCompanionController) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    ZoomSDKError ret = [aiCompanionController requestTurnOnAllAICompanion];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

//callback
void ZMeetingAICompanionWrap::onAICompanionFeatureTurnOffByParticipant()
{
    if (m_pSink) {
        m_pSink->onAICompanionFeatureTurnOffByParticipant();
    }
}

ZNList<ZNAICompanionFeature> ZMeetingAICompanionWrap::GetFeatureList()
{
    ZNList<ZNAICompanionFeature> zn_list;
    ZoomSDKAICompanionFeatureTurnOnAgainHandler *handler = [meetingServiceAICompanionControllerDelegate share].aiCompanionFeatureTurnOnAgainHandler;
    if (!handler) {
        return zn_list;
    }

    NSArray *arr = handler.featuresList;
    nativeErrorTypeHelp help;
    if (arr && arr.count > 0) {
        for (NSNumber *item in arr) {
            ZoomSDKAICompanionFeature featureValue = (ZoomSDKAICompanionFeature)[item unsignedIntValue];
            ZNAICompanionFeature zn_feature = help.ZNAICompanionFeatureMap(featureValue);
            zn_list.push_back(zn_feature);
        }
    }
    return zn_list;
}

ZNList<ZNAICompanionFeature> ZMeetingAICompanionWrap::GetAssetsDeletedFeatureList()
{
    ZNList<ZNAICompanionFeature> zn_list;
    ZoomSDKAICompanionFeatureTurnOnAgainHandler *handler = [meetingServiceAICompanionControllerDelegate share].aiCompanionFeatureTurnOnAgainHandler;
    if (!handler) {
        return zn_list;
    }

    NSArray *arr = handler.assetsDeletedFeatureList;
    nativeErrorTypeHelp help;
    if (arr && arr.count > 0) {
        for (NSNumber *item in arr) {
            ZoomSDKAICompanionFeature featureValue = (ZoomSDKAICompanionFeature)[item unsignedIntValue];
            ZNAICompanionFeature zn_feature = help.ZNAICompanionFeatureMap(featureValue);
            zn_list.push_back(zn_feature);
        }
    }
    return zn_list;
}

ZNSDKError ZMeetingAICompanionWrap::TurnOnAgain()
{
    ZoomSDKAICompanionFeatureTurnOnAgainHandler *handler = [meetingServiceAICompanionControllerDelegate share].aiCompanionFeatureTurnOnAgainHandler;
    if (!handler) {
        return ZNSDKERR_NO_PERMISSION;
    }

    ZoomSDKError ret = [handler turnOnAgain];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingAICompanionWrap::AgreeTurnOff()
{
    ZoomSDKAICompanionFeatureTurnOnAgainHandler *handler = [meetingServiceAICompanionControllerDelegate share].aiCompanionFeatureTurnOnAgainHandler;
    if (!handler) {
        return ZNSDKERR_NO_PERMISSION;
    }

    ZoomSDKError ret = [handler agreeTurnOff];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

void ZMeetingAICompanionWrap::onAICompanionFeatureSwitchRequested()
{
    if (m_pSink) {
        m_pSink->onAICompanionFeatureSwitchRequested();
    }
}

unsigned int ZMeetingAICompanionWrap::GetRequestUserID()
{
    unsigned int request_user_id = 0;
    ZoomSDKAICompanionFeatureSwitchHandler *handler = [meetingServiceAICompanionControllerDelegate share].aiCompanionFeatureSwitchHandler;
    if (!handler) {
        return request_user_id;
    }
    return handler.requestUserID;
}

bool ZMeetingAICompanionWrap::IsTurnOn()
{
    ZoomSDKAICompanionFeatureSwitchHandler *handler = [meetingServiceAICompanionControllerDelegate share].aiCompanionFeatureSwitchHandler;
    if (!handler) {
        return false;
    }
    return handler.isTurnOn;
}

ZNSDKError ZMeetingAICompanionWrap::Agree(bool deleteAssets)
{
    ZoomSDKAICompanionFeatureSwitchHandler *handler = [meetingServiceAICompanionControllerDelegate share].aiCompanionFeatureSwitchHandler;
    if (!handler) {
        return ZNSDKERR_NO_PERMISSION;
    }

    ZoomSDKError ret = [handler agree:deleteAssets];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingAICompanionWrap::Decline()
{
    ZoomSDKAICompanionFeatureSwitchHandler *handler = [meetingServiceAICompanionControllerDelegate share].aiCompanionFeatureSwitchHandler;
    if (!handler) {
        return ZNSDKERR_NO_PERMISSION;
    }

    ZoomSDKError ret = [handler decline];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

// IMeetingAICompanionSmartSummaryHelper
//callback
void ZMeetingAICompanionWrap::onSmartSummaryStateNotSupported()
{
    if (m_pSink) {
        m_pSink->onSmartSummaryStateNotSupported();
    }
}

void ZMeetingAICompanionWrap::onSmartSummaryStateSupportedButDisabled()
{
    if (m_pSink) {
        m_pSink->onSmartSummaryStateSupportedButDisabled();
    }
}

ZNSDKError ZMeetingAICompanionWrap::EnableSmartSummary()
{
    ZoomSDKEnableSmartSummaryHandler *handler = [meetingServiceAICompanionSmartSummaryDelegate share].enableSmartSummaryHandler;
    if (!handler) {
        return ZNSDKERR_NO_PERMISSION;
    }

    ZoomSDKError ret = [handler enableSmartSummary];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

bool ZMeetingAICompanionWrap::IsForRequestEnableSmartSummary()
{
    ZoomSDKEnableSmartSummaryHandler *handler = [meetingServiceAICompanionSmartSummaryDelegate share].enableSmartSummaryHandler;
    if (!handler) {
        return false;
    }

    return [handler isForRequest];
}

void ZMeetingAICompanionWrap::onSmartSummaryStateEnabledButNotStarted()
{
    if (m_pSink) {
        m_pSink->onSmartSummaryStateEnabledButNotStarted();
    }
}

ZNSDKError ZMeetingAICompanionWrap::StartSmartSummary()
{
    ZoomSDKStartSmartSummaryHandler *handler = [meetingServiceAICompanionSmartSummaryDelegate share].startSmartSummaryHandler;
    if (!handler) {
        return ZNSDKERR_NO_PERMISSION;
    }

    ZoomSDKError ret = [handler startSmartSummary];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

bool ZMeetingAICompanionWrap::IsForRequestStartSmartSummary()
{
    ZoomSDKStartSmartSummaryHandler *handler = [meetingServiceAICompanionSmartSummaryDelegate share].startSmartSummaryHandler;
    if (!handler) {
        return false;
    }

    return [handler isForRequest];
}

void ZMeetingAICompanionWrap::onSmartSummaryStateStarted()
{
    if (m_pSink) {
        m_pSink->onSmartSummaryStateStarted();
    }
}

ZNSDKError ZMeetingAICompanionWrap::StopSmartSummary()
{
    ZoomSDKStopSmartSummaryHandler *handler = [meetingServiceAICompanionSmartSummaryDelegate share].stopSmartSummaryHandler;
    if (!handler) {
        return ZNSDKERR_NO_PERMISSION;
    }

    ZoomSDKError ret = [handler stopSmartSummary];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

void ZMeetingAICompanionWrap::onFailedToStartSmartSummary(bool timeout)
{
    if (m_pSink) {
        m_pSink->onFailedToStartSmartSummary(timeout);
    }
}

void ZMeetingAICompanionWrap::onSmartSummaryEnableRequestReceived()
{
    if (m_pSink) {
        m_pSink->onSmartSummaryEnableRequestReceived();
    }
}

unsigned int ZMeetingAICompanionWrap::GetSenderEnableSmartSummaryUserID()
{
    unsigned int sender_user_id = 0;
    ZoomSDKApproveEnableSmartSummaryHandler *handler = [meetingServiceAICompanionSmartSummaryDelegate share].approveEnableSmartSummaryHandler;
    if (!handler) {
        return sender_user_id;
    }
        
    return handler.senderUserID;
}

ZNSDKError ZMeetingAICompanionWrap::ContinueApproveEnableSmartSummary()
{
    ZoomSDKApproveEnableSmartSummaryHandler *handler = [meetingServiceAICompanionSmartSummaryDelegate share].approveEnableSmartSummaryHandler;
    if (!handler) {
        return ZNSDKERR_NO_PERMISSION;
    }

    ZoomSDKError ret = [handler continueApprove];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

void ZMeetingAICompanionWrap::onSmartSummaryStartRequestReceived()
{
    if (m_pSink) {
        m_pSink->onSmartSummaryStartRequestReceived();
    }
}

unsigned int ZMeetingAICompanionWrap::GetSenderStartSmartSummaryUserID()
{
    unsigned int sender_user_id = 0;
    ZoomSDKApproveStartSmartSummaryHandler *handler = [meetingServiceAICompanionSmartSummaryDelegate share].approveStartSmartSummaryHandler;
    if (!handler) {
        return sender_user_id;
    }
        
    return handler.senderUserID;
}

ZNSDKError ZMeetingAICompanionWrap::ApproveStartSmartSummary()
{
    ZoomSDKApproveStartSmartSummaryHandler *handler = [meetingServiceAICompanionSmartSummaryDelegate share].approveStartSmartSummaryHandler;
    if (!handler) {
        return ZNSDKERR_NO_PERMISSION;
    }

    ZoomSDKError ret = [handler approve];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingAICompanionWrap::DeclineStartSmartSummary()
{
    ZoomSDKApproveStartSmartSummaryHandler *handler = [meetingServiceAICompanionSmartSummaryDelegate share].approveStartSmartSummaryHandler;
    if (!handler) {
        return ZNSDKERR_NO_PERMISSION;
    }

    ZoomSDKError ret = [handler decline];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

void ZMeetingAICompanionWrap::onSmartSummaryEnableActionCallback()
{
    if (m_pSink) {
        m_pSink->onSmartSummaryEnableActionCallback();
    }
}

ZoomSTRING ZMeetingAICompanionWrap::GetEnableSmartSummaryActionTipTitle()
{
    ZoomSDKEnableSmartSummaryActionHandler *handler = [meetingServiceAICompanionSmartSummaryDelegate share].enableSmartSummaryActionHandler;
    if (!handler) {
        return "";
    }
        
    NSString *tip_title_string = handler.tipTitle;
    if (!tip_title_string) {
        return "";
    }
    return [tip_title_string UTF8String];
}

ZoomSTRING ZMeetingAICompanionWrap::GetEnableSmartSummaryActionTipString()
{
    ZoomSDKEnableSmartSummaryActionHandler *handler = [meetingServiceAICompanionSmartSummaryDelegate share].enableSmartSummaryActionHandler;
    if (!handler) {
        return "";
    }
        
    NSString *tip_string = handler.tipString;
    if (!tip_string) {
        return "";
    }
    return [tip_string UTF8String];
}

ZNSDKError ZMeetingAICompanionWrap::ConfirmEnableSmartSummaryAction()
{
    ZoomSDKEnableSmartSummaryActionHandler *handler = [meetingServiceAICompanionSmartSummaryDelegate share].enableSmartSummaryActionHandler;
    if (!handler) {
        return ZNSDKERR_NO_PERMISSION;
    }

    ZoomSDKError ret = [handler confirm];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingAICompanionWrap::CancelEnableSmartSummaryAction()
{
    ZoomSDKEnableSmartSummaryActionHandler *handler = [meetingServiceAICompanionSmartSummaryDelegate share].enableSmartSummaryActionHandler;
    if (!handler) {
        return ZNSDKERR_NO_PERMISSION;
    }

    ZoomSDKError ret = [handler cancel];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

// IMeetingAICompanionQueryHelper
ZNSDKError ZMeetingAICompanionWrap::CanChangeQuerySetting(bool& bCan)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }
  
    ZoomSDKMeetingAICompanionController *aiCompanionController = [service getInMeetingAICompanionController];
    if (!aiCompanionController) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    ZoomSDKAICompanionQueryHelper *queryHelper = [aiCompanionController getQueryHelper];
    if (!queryHelper) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    bCan = [queryHelper canChangeQuerySetting];
    return ZNSDKERR_SUCCESS;
}

ZNMeetingAICompanionQuerySettingOptions ZMeetingAICompanionWrap::GetSelectedQuerySetting()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return ZNMeetingAICompanionQuerySettingOptions_None;
    }
  
    ZoomSDKMeetingAICompanionController *aiCompanionController = [service getInMeetingAICompanionController];
    if (!aiCompanionController) {
        return ZNMeetingAICompanionQuerySettingOptions_None;
    }

    ZoomSDKAICompanionQueryHelper *queryHelper = [aiCompanionController getQueryHelper];
    if (!queryHelper) {
        return ZNMeetingAICompanionQuerySettingOptions_None;
    }
    
    ZoomSDKAICompanionQuerySettingOptions setting = [queryHelper getSelectedQuerySetting];
    
    nativeErrorTypeHelp Help_type;
    return Help_type.ZNMeetingAICompanionQuerySettingOptionsMap(setting);
}

ZNSDKError ZMeetingAICompanionWrap::ChangeQuerySettings(ZNMeetingAICompanionQuerySettingOptions zn_setting)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }
  
    ZoomSDKMeetingAICompanionController *aiCompanionController = [service getInMeetingAICompanionController];
    if (!aiCompanionController) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    ZoomSDKAICompanionQueryHelper *queryHelper = [aiCompanionController getQueryHelper];
    if (!queryHelper) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    nativeErrorTypeHelp Help_type;
    ZoomSDKAICompanionQuerySettingOptions setting = Help_type.ZoomSDKMeetingAICompanionQuerySettingOptionsMap(zn_setting);
    ZoomSDKError ret = [queryHelper changeQuerySettings:setting];
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingAICompanionWrap::IsAICompanionQueryLegalNoticeAvailable(bool& bAvailable)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }
  
    ZoomSDKMeetingAICompanionController *aiCompanionController = [service getInMeetingAICompanionController];
    if (!aiCompanionController) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    ZoomSDKAICompanionQueryHelper *queryHelper = [aiCompanionController getQueryHelper];
    if (!queryHelper) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    bAvailable = [queryHelper isAICompanionQueryLegalNoticeAvailable];
    return ZNSDKERR_SUCCESS;
}

ZoomSTRING ZMeetingAICompanionWrap::GetAICompanionQueryLegalNoticesPrompt()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return "";
    }
  
    ZoomSDKMeetingAICompanionController *aiCompanionController = [service getInMeetingAICompanionController];
    if (!aiCompanionController) {
        return "";
    }

    ZoomSDKAICompanionQueryHelper *queryHelper = [aiCompanionController getQueryHelper];
    if (!queryHelper) {
        return "";
    }

    NSString *noticesPrompt = [queryHelper getAICompanionQueryLegalNoticesPrompt];
    if (!noticesPrompt)
        return "";

    return [noticesPrompt UTF8String];
}

ZoomSTRING ZMeetingAICompanionWrap::GetAICompanionQueryLegalNoticesExplained()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return "";
    }
  
    ZoomSDKMeetingAICompanionController *aiCompanionController = [service getInMeetingAICompanionController];
    if (!aiCompanionController) {
        return "";
    }

    ZoomSDKAICompanionQueryHelper *queryHelper = [aiCompanionController getQueryHelper];
    if (!queryHelper) {
        return "";
    }

    NSString *noticesExplained = [queryHelper getAICompanionQueryLegalNoticesExplained];
    if (!noticesExplained)
        return "";
        
    return [noticesExplained UTF8String];
}

ZNSDKError ZMeetingAICompanionWrap::IsAICompanionQueryPrivacyLegalNoticeAvailable(bool& bAvailable)
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }
  
    ZoomSDKMeetingAICompanionController *aiCompanionController = [service getInMeetingAICompanionController];
    if (!aiCompanionController) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    ZoomSDKAICompanionQueryHelper *queryHelper = [aiCompanionController getQueryHelper];
    if (!queryHelper) {
        return ZNSDKERR_SERVICE_FAILED;
    }

    bAvailable = [queryHelper isAICompanionQueryPrivacyLegalNoticeAvailable];
    return ZNSDKERR_SUCCESS;
}

ZoomSTRING ZMeetingAICompanionWrap::GetAICompanionQueryPrivacyLegalNoticesPrompt()
{
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service) {
        return "";
    }
  
    ZoomSDKMeetingAICompanionController *aiCompanionController = [service getInMeetingAICompanionController];
    if (!aiCompanionController) {
        return "";
    }

    ZoomSDKAICompanionQueryHelper *queryHelper = [aiCompanionController getQueryHelper];
    if (!queryHelper) {
        return "";
    }

    NSString *noticesExplained = [queryHelper getAICompanionQueryPrivacyLegalNoticesPrompt];
    if (!noticesExplained)
        return "";
        
    return [noticesExplained UTF8String];
}

void ZMeetingAICompanionWrap::onQueryStateNotSupported()
{
    if (m_pSink) {
        m_pSink->onQueryStateNotSupported();
    }
}

void ZMeetingAICompanionWrap::onQueryStateSupportedButDisabled()
{
    if (m_pSink) {
        m_pSink->onQueryStateSupportedButDisabled();
    }
}

ZNSDKError ZMeetingAICompanionWrap::EnableQuery()
{
    ZoomSDKEnableQueryHandler *handler = [meetingServiceAICompanionQueryDelegate share].enableQueryHandler;
    if (!handler) {
        return ZNSDKERR_NO_PERMISSION;
    }

    ZoomSDKError ret = [handler enableQuery];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

bool ZMeetingAICompanionWrap::IsForRequestEnableQuery()
{
    ZoomSDKEnableQueryHandler *handler = [meetingServiceAICompanionQueryDelegate share].enableQueryHandler;
    if (!handler) {
        return false;
    }

    return [handler isForRequest];
}

void ZMeetingAICompanionWrap::onQueryStateEnabledButNotStarted()
{
    if (m_pSink) {
        m_pSink->onQueryStateEnabledButNotStarted();
    }
}

ZNSDKError ZMeetingAICompanionWrap::StartMeetingQuery()
{
    ZoomSDKStartQueryHandler *handler = [meetingServiceAICompanionQueryDelegate share].startQueryHandler;
    if (!handler) {
        return ZNSDKERR_NO_PERMISSION;
    }

    ZoomSDKError ret = [handler startMeetingQuery];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

bool ZMeetingAICompanionWrap::IsForRequestStartMeetingQuery()
{
    ZoomSDKStartQueryHandler *handler = [meetingServiceAICompanionQueryDelegate share].startQueryHandler;
    if (!handler) {
        return false;
    }

    return [handler isForRequest];
}

void ZMeetingAICompanionWrap::onQueryStateStarted()
{
    if (m_pSink) {
        m_pSink->onQueryStateStarted();
    }
}

ZNList<ZoomSTRING> ZMeetingAICompanionWrap::GetDefaultQueryQuestions()
{
    ZNList<ZoomSTRING> zn_list;
    ZoomSDKSendQueryHandler *handler = [meetingServiceAICompanionQueryDelegate share].sendQueryHandler;
    if (!handler) {
        return zn_list;
    }

    NSArray *arr = [handler getDefaultQueryQuestions];
    if (arr && arr.count > 0) {
        for (NSString *question in arr) {
            zn_list.push_back(question.UTF8String);
        }
    }
    return zn_list;
}

ZNSDKError ZMeetingAICompanionWrap::SendQueryQuestion(ZoomSTRING zn_question)
{
    ZoomSDKSendQueryHandler *handler = [meetingServiceAICompanionQueryDelegate share].sendQueryHandler;
    if (!handler) {
        return ZNSDKERR_NO_PERMISSION;
    }

    if (zn_question.empty())
        return ZNSDKERR_INVALID_PARAMETER;

    NSString *question = [NSString stringWithUTF8String:zn_question.c_str()];
    ZoomSDKError ret = [handler sendQueryQuestion:question];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingAICompanionWrap::StopMeetingQuery()
{
    ZoomSDKSendQueryHandler *handler = [meetingServiceAICompanionQueryDelegate share].sendQueryHandler;
    if (!handler) {
        return ZNSDKERR_NO_PERMISSION;
    }

    ZoomSDKError ret = [handler stopMeetingQuery];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

bool ZMeetingAICompanionWrap::CanSendQuery()
{
    ZoomSDKSendQueryHandler *handler = [meetingServiceAICompanionQueryDelegate share].sendQueryHandler;
    if (!handler) {
        return false;
    }

    return [handler canSendQuery];
}

ZNSDKError ZMeetingAICompanionWrap::RequestSendQueryPrivilege()
{
    ZoomSDKSendQueryHandler *handler = [meetingServiceAICompanionQueryDelegate share].sendQueryHandler;
    if (!handler) {
        return ZNSDKERR_NO_PERMISSION;
    }

    ZoomSDKError ret = [handler requestSendQueryPrivilege];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

void ZMeetingAICompanionWrap::onQuerySettingChanged(ZNMeetingAICompanionQuerySettingOptions eSetting)
{
    if (m_pSink) {
        m_pSink->onQuerySettingChanged(eSetting);
    }
}

void ZMeetingAICompanionWrap::onFailedToStartQuery(bool bTimeout)
{
    if (m_pSink) {
        m_pSink->onFailedToStartQuery(bTimeout);
    }
}

void ZMeetingAICompanionWrap::onReceiveRequestToEnableQuery()
{
    if (m_pSink) {
        m_pSink->onReceiveRequestToEnableQuery();
    }
}

unsigned int ZMeetingAICompanionWrap::GetSenderEnableQueryUserID()
{
    unsigned int sender_user_id = 0;
    ZoomSDKApproveEnableQueryHandler *handler = [meetingServiceAICompanionQueryDelegate share].approveEnableQueryHandler;
    if (!handler) {
        return sender_user_id;
    }
        
    return handler.senderUserID;
}

ZNSDKError ZMeetingAICompanionWrap::ContinueApproveEnableQuery()
{
    ZoomSDKApproveEnableQueryHandler *handler = [meetingServiceAICompanionQueryDelegate share].approveEnableQueryHandler;
    if (!handler) {
        return ZNSDKERR_NO_PERMISSION;
    }

    ZoomSDKError ret = [handler continueApprove];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

void ZMeetingAICompanionWrap::onReceiveRequestToStartQuery()
{
    if (m_pSink) {
        m_pSink->onReceiveRequestToStartQuery();
    }
}

unsigned int ZMeetingAICompanionWrap::GetSenderStartQueryUserID()
{
    unsigned int sender_user_id = 0;
    ZoomSDKApproveStartQueryHandler *handler = [meetingServiceAICompanionQueryDelegate share].approveStartQueryHandler;
    if (!handler) {
        return sender_user_id;
    }
        
    return handler.senderUserID;
}

ZNSDKError ZMeetingAICompanionWrap::ApproveStartQuery()
{
    ZoomSDKApproveStartQueryHandler *handler = [meetingServiceAICompanionQueryDelegate share].approveStartQueryHandler;
    if (!handler) {
        return ZNSDKERR_NO_PERMISSION;
    }

    ZoomSDKError ret = [handler approve];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingAICompanionWrap::DeclineStartQuery()
{
    ZoomSDKApproveStartQueryHandler *handler = [meetingServiceAICompanionQueryDelegate share].approveStartQueryHandler;
    if (!handler) {
        return ZNSDKERR_NO_PERMISSION;
    }

    ZoomSDKError ret = [handler decline];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

void ZMeetingAICompanionWrap::onReceiveQueryAnswer(ZNMeetingAICompanionQueryItem queryItem)
{
    if (m_pSink) {
        m_pSink->onReceiveQueryAnswer(queryItem);
    }
}

ZNSDKError ZMeetingAICompanionWrap::AICompanionQueryFeedback(ZNMeetingAICompanionQueryFeedbackType zn_type)
{
    ZoomSDKAICompanionQueryItem *handler = [meetingServiceAICompanionQueryDelegate share].aiCompanionQueryItem;
    if (!handler) {
        return ZNSDKERR_NO_PERMISSION;
    }

    nativeErrorTypeHelp Help_type;
    ZoomSDKAICompanionQueryFeedbackType feedbackType = Help_type.ZoomSDKAICompanionQueryFeedbackTypeMap(zn_type);
    ZoomSDKError ret = [handler feedback:feedbackType];
    return Help_type.ZoomSDKErrorType(ret);
}

void ZMeetingAICompanionWrap::onQueryEnableActionCallback()
{
    if (m_pSink) {
        m_pSink->onQueryEnableActionCallback();
    }
}

ZoomSTRING ZMeetingAICompanionWrap::GetEnableQueryActionTipTitle()
{
    ZoomSDKEnableQueryActionHandler *handler = [meetingServiceAICompanionQueryDelegate share].enableQueryActionHandler;
    if (!handler) {
        return "";
    }

    NSString *tipTitle = [handler tipTitle];
    if (!tipTitle) {
        return "";
    }

    return [tipTitle UTF8String];
}

ZoomSTRING ZMeetingAICompanionWrap::GetEnableQueryActionTipString()
{
    ZoomSDKEnableQueryActionHandler *handler = [meetingServiceAICompanionQueryDelegate share].enableQueryActionHandler;
    if (!handler) {
        return "";
    }

    NSString *tipString = [handler tipString];
    if (!tipString) {
        return "";
    }

    return [tipString UTF8String];
}

ZNSDKError ZMeetingAICompanionWrap::ConfirmEnableQueryAction()
{
    ZoomSDKEnableQueryActionHandler *handler = [meetingServiceAICompanionQueryDelegate share].enableQueryActionHandler;
    if (!handler) {
        return ZNSDKERR_NO_PERMISSION;
    }

    ZoomSDKError ret = [handler confirm];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingAICompanionWrap::CancelEnableQueryAction()
{
    ZoomSDKEnableQueryActionHandler *handler = [meetingServiceAICompanionQueryDelegate share].enableQueryActionHandler;
    if (!handler) {
        return ZNSDKERR_NO_PERMISSION;
    }

    ZoomSDKError ret = [handler cancel];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

void ZMeetingAICompanionWrap::onSendQueryPrivilegeChanged(bool canSendQuery)
{
    if (m_pSink) {
        m_pSink->onSendQueryPrivilegeChanged(canSendQuery);
    }
}

void ZMeetingAICompanionWrap::onFailedToRequestSendQuery(bool bTimeout)
{
    if (m_pSink) {
        m_pSink->onFailedToRequestSendQuery(bTimeout);
    }
}

void ZMeetingAICompanionWrap::onReceiveRequestToSendQuery()
{
    if (m_pSink) {
        m_pSink->onReceiveRequestToSendQuery();
    }
}

unsigned int ZMeetingAICompanionWrap::GetSenderQueryUserID()
{
    unsigned int sender_user_id = 0;
    ZoomSDKApproveSendQueryHandler *handler = [meetingServiceAICompanionQueryDelegate share].approveSendQueryHandler;
    if (!handler) {
        return sender_user_id;
    }
        
    return handler.senderUserID;
}

ZNSDKError ZMeetingAICompanionWrap::ApproveSendQuery()
{
    ZoomSDKApproveSendQueryHandler *handler = [meetingServiceAICompanionQueryDelegate share].approveSendQueryHandler;
    if (!handler) {
        return ZNSDKERR_NO_PERMISSION;
    }

    ZoomSDKError ret = [handler approve];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingAICompanionWrap::DeclineSendQuery(bool declineAll)
{
    ZoomSDKApproveSendQueryHandler *handler = [meetingServiceAICompanionQueryDelegate share].approveSendQueryHandler;
    if (!handler) {
        return ZNSDKERR_NO_PERMISSION;
    }

    ZoomSDKError ret = [handler decline:declineAll];
    nativeErrorTypeHelp Help_type;
    return Help_type.ZoomSDKErrorType(ret);
}