#include "Header_include.h"
#include "sdk_native_error.h"
#include "meeting_polling_wrap_core.h"
#include "meeting_service_wrap_core.h"
#import "meetingServiceDelegate.h"

ZMeetingPollingWrap &ZMeetingServiceWrap::GetMeetingPollingCtrl()
{
    return m_polling_ctrl;
}
ZMeetingPollingWrap::ZMeetingPollingWrap()
{
   m_pSink = 0;
}

ZMeetingPollingWrap::~ZMeetingPollingWrap()
{
    m_pSink = 0;
    [[[[ZoomSDK sharedSDK] getMeetingService] getMeetingPollingController] setDelegate:nil];
}

void ZMeetingPollingWrap::SetSink(ZNativeSDKPollingWrapSink *pSink)
{
    [[[[ZoomSDK sharedSDK] getMeetingService] getMeetingPollingController] setDelegate:[[meetingServicePollingDelegate alloc] init]];
    m_pSink= pSink;
}

void ZMeetingPollingWrap::Init()
{

}
void ZMeetingPollingWrap::Uninit()
{
    
}

bool ZMeetingPollingWrap::CanDoPolling() {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return false;
    
    ZoomSDKPollingController *pollingController = [service getMeetingPollingController];
    if (!pollingController)
        return false;

    return [pollingController canDoPolling];
}

ZNList<ZNSDKPollingQuestionItem> ZMeetingPollingWrap::GetPollingQuestionItemList(ZoomSTRING zn_pollingID) {
    ZNList<ZNSDKPollingQuestionItem> zn_polling_question_item_list;
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return zn_polling_question_item_list;

    ZoomSDKPollingController *pollingController = [service getMeetingPollingController];
    if (!pollingController)
        return zn_polling_question_item_list;
    
    NSString *pollingID = [NSString stringWithUTF8String:zn_pollingID.c_str()];

    NSArray<ZoomSDKPollingQuestionItem *> *arr = [pollingController getPollingQuestionItemList:pollingID];

    for (ZoomSDKPollingQuestionItem *pollQuestionItem in arr) {
        if (pollQuestionItem) {
            ZNSDKPollingQuestionItem zn_pollQuestionItem;

            // pollingID
            ZoomSTRING zn_p_ID;
            if ([pollQuestionItem pollingID])
                zn_p_ID = [[pollQuestionItem pollingID] UTF8String];                 
            zn_pollQuestionItem.pollingID = zn_p_ID;

            // pollingQuestionID
            ZoomSTRING zn_pollingQuestionID;
            if ([pollQuestionItem pollingQuestionID]) {
                zn_pollingQuestionID = [[pollQuestionItem pollingQuestionID] UTF8String];
            }
            zn_pollQuestionItem.pollingQuestionID = zn_pollingQuestionID;

            // pollingQuestionName
             ZoomSTRING zn_pollingQuestionName;
            if ([pollQuestionItem pollingQuestionName]) {
                zn_pollingQuestionName = [[pollQuestionItem pollingQuestionName] UTF8String];
            }
            zn_pollQuestionItem.pollingQuestionName = zn_pollingQuestionName;

            // pollingQuestionType
            nativeErrorTypeHelp Help_type;

            ZoomSDKPollingQuestionType pollingQuestionType = [pollQuestionItem pollingQuestionType];
            ZNSDKPollingQuestionType zn_pollingQuestionType = Help_type.ZNSDKPollingQuestionTypeMap(pollingQuestionType);
            zn_pollQuestionItem.pollingQuestionType = zn_pollingQuestionType;

            // answeredCount
            zn_pollQuestionItem.answeredCount = [pollQuestionItem answeredCount];

            // isRequired
            zn_pollQuestionItem.isRequired = [pollQuestionItem answeredCount];

            // pollingSubQuestionItemList
            ZNList<ZNSDKPollingQuestionItem> zn_pollingSubQuestionItemList; 
            NSArray<ZoomSDKPollingQuestionItem *> *pollingSubQuestionItemList = [pollQuestionItem pollingSubQuestionItemList];
            for (ZoomSDKPollingQuestionItem *subQuestionItem in pollingSubQuestionItemList) {
                if (subQuestionItem) {
                    ZNSDKPollingQuestionItem zn_subQuestionItem;

                    // sub-question's pollingID
                    ZoomSTRING zn_p_ID_2;
                    NSString *pollingID = [subQuestionItem pollingID];
                    if (pollingID)
                        zn_p_ID_2 = [pollingID UTF8String];                 
                    zn_subQuestionItem.pollingID = zn_p_ID_2;

                    // sub-question's pollingQuestionID
                    ZoomSTRING zn_pollingQuestionID_2;
                    if ([subQuestionItem pollingQuestionID]) {
                        zn_pollingQuestionID_2 = [[subQuestionItem pollingQuestionID] UTF8String];
                    }
                    zn_subQuestionItem.pollingQuestionID = zn_pollingQuestionID_2;

                    // sub-question's pollingQuestionName
                    ZoomSTRING zn_pollingQuestionName_2;
                    if ([subQuestionItem pollingQuestionName]) {
                        zn_pollingQuestionName_2 = [[subQuestionItem pollingQuestionName] UTF8String];
                    }
                    zn_subQuestionItem.pollingQuestionName = zn_pollingQuestionName_2;

                    // sub-question's pollingQuestionType
                    ZoomSDKPollingQuestionType subQuestionItemType = [subQuestionItem pollingQuestionType];
                    ZNSDKPollingQuestionType zn_subQuestionItemType = Help_type.ZNSDKPollingQuestionTypeMap(subQuestionItemType);
                    zn_subQuestionItem.pollingQuestionType = zn_subQuestionItemType;

                    // sub-question's answeredCount
                    zn_subQuestionItem.answeredCount = [subQuestionItem answeredCount];

                    // sub-question's isRequired
                    zn_subQuestionItem.isRequired = [subQuestionItem answeredCount];

                    // sub-question's sub-question list: this list is set to be empty
                    ZNList<ZNSDKPollingQuestionItem> zn_subs_pollingSubQuestionItemList;
                    zn_subQuestionItem.pollingSubQuestionItemList = zn_subs_pollingSubQuestionItemList;

                    // pollingAnswerItemList for current sub-question 
                    ZNList<ZNSDKPollingAnswerItem> zn_sub_answer_item_list;
                    NSArray<ZoomSDKPollingAnswerItem *> *sub_arr = [subQuestionItem pollingAnswerItemList];
                    for (ZoomSDKPollingAnswerItem *sub_pollAnswerItem in sub_arr) {
                        if (sub_pollAnswerItem) {
                            ZNSDKPollingAnswerItem zn_sub_pollAnswerItem;

                            // pollingID
                            ZoomSTRING zn_sub_answer_p_ID;
                            if ([sub_pollAnswerItem pollingID]) {
                                zn_sub_answer_p_ID = [[sub_pollAnswerItem pollingID] UTF8String];
                            }
                            zn_sub_pollAnswerItem.pollingID = zn_sub_answer_p_ID;

                            // pollingQuestionID
                            ZoomSTRING zn_sub_answer_pollingQuestionID;
                            if ([sub_pollAnswerItem pollingQuestionID]) {
                                zn_sub_answer_pollingQuestionID = [[sub_pollAnswerItem pollingQuestionID] UTF8String];
                            }
                            zn_sub_pollAnswerItem.pollingQuestionID = zn_sub_answer_pollingQuestionID;

                            // pollingSubQuestionID
                            ZoomSTRING zn_sub_answer_pollingSubQuestionID;
                            if ([sub_pollAnswerItem pollingSubQuestionID]) {
                                zn_sub_answer_pollingSubQuestionID = [[sub_pollAnswerItem pollingSubQuestionID] UTF8String];
                            }
                            zn_sub_pollAnswerItem.pollingSubQuestionID = zn_sub_answer_pollingSubQuestionID;

                            // pollingAnswerID
                            ZoomSTRING zn_sub_answer_pollingAnswerID;
                            if ([sub_pollAnswerItem pollingAnswerID]) {
                                zn_sub_answer_pollingAnswerID = [[sub_pollAnswerItem pollingAnswerID] UTF8String];
                            }
                            zn_sub_pollAnswerItem.pollingAnswerID = zn_sub_answer_pollingAnswerID;

                            // pollingAnswerName
                            ZoomSTRING zn_sub_answer_pollingAnswerName;
                            if ([sub_pollAnswerItem pollingAnswerName]) {
                                zn_sub_answer_pollingAnswerName = [[sub_pollAnswerItem pollingAnswerName] UTF8String];
                            }
                            zn_sub_pollAnswerItem.pollingAnswerName = zn_sub_answer_pollingAnswerName;
                            
                            // pollingAnsweredContent
                            ZoomSTRING zn_sub_answer_pollingAnsweredContent;
                            if ([sub_pollAnswerItem pollingAnsweredContent]) {
                                zn_sub_answer_pollingAnsweredContent = [[sub_pollAnswerItem pollingAnsweredContent] UTF8String];
                            }
                            zn_sub_pollAnswerItem.pollingAnsweredContent = zn_sub_answer_pollingAnsweredContent;

                            // isChecked
                            zn_sub_pollAnswerItem.isChecked = [sub_pollAnswerItem isChecked];
                            
                            zn_sub_answer_item_list.push_back(zn_sub_pollAnswerItem);
                        }

                    }
                    zn_subQuestionItem.pollingAnswerItemList = zn_sub_answer_item_list;
                    zn_pollingSubQuestionItemList.push_back(zn_subQuestionItem);
                }
            }
            zn_pollQuestionItem.pollingSubQuestionItemList = zn_pollingSubQuestionItemList;

            // pollingAnswerItemList
            ZNList<ZNSDKPollingAnswerItem> zn_polling_answer_item_list;
            NSArray<ZoomSDKPollingAnswerItem *> *arr = [pollQuestionItem pollingAnswerItemList];
            for (ZoomSDKPollingAnswerItem *pollAnswerItem in arr) {
                if (pollAnswerItem) {
                    ZNSDKPollingAnswerItem zn_pollAnswerItem;

                    // pollingID
                    ZoomSTRING zn_answer_p_ID;
                    if ([pollAnswerItem pollingID]) {
                        zn_answer_p_ID = [[pollAnswerItem pollingID] UTF8String];
                    }
                    zn_pollAnswerItem.pollingID = zn_answer_p_ID;

                    // pollingQuestionID
                    ZoomSTRING zn_answer_pollingQuestionID;
                    if ([pollAnswerItem pollingQuestionID]) {
                        zn_answer_pollingQuestionID = [[pollAnswerItem pollingQuestionID] UTF8String];
                    }
                    zn_pollAnswerItem.pollingQuestionID = zn_answer_pollingQuestionID;

                    // pollingSubQuestionID
                    ZoomSTRING zn_pollingSubQuestionID;
                    if ([pollAnswerItem pollingSubQuestionID]) {
                        zn_pollingSubQuestionID = [[pollAnswerItem pollingSubQuestionID] UTF8String];
                    }
                    zn_pollAnswerItem.pollingSubQuestionID = zn_pollingSubQuestionID;

                    // pollingAnswerID
                    ZoomSTRING zn_pollingAnswerID;
                    if ([pollAnswerItem pollingAnswerID]) {
                        zn_pollingAnswerID = [[pollAnswerItem pollingAnswerID] UTF8String];
                    }
                    zn_pollAnswerItem.pollingAnswerID = zn_pollingAnswerID;

                    // pollingAnswerName
                    ZoomSTRING zn_pollingAnswerName;
                    if ([pollAnswerItem pollingAnswerName]) {
                        zn_pollingAnswerName = [[pollAnswerItem pollingAnswerName] UTF8String];
                    }
                    zn_pollAnswerItem.pollingAnswerName = zn_pollingAnswerName;
                    
                    // pollingAnsweredContent
                    ZoomSTRING zn_pollingAnsweredContent;
                    if ([pollAnswerItem pollingAnsweredContent]) {
                        zn_pollingAnsweredContent = [[pollAnswerItem pollingAnsweredContent] UTF8String];
                    }
                    zn_pollAnswerItem.pollingAnsweredContent = zn_pollingAnsweredContent;

                    // isChecked
                    zn_pollAnswerItem.isChecked = [pollAnswerItem isChecked];
                    
                    zn_polling_answer_item_list.push_back(zn_pollAnswerItem);
                }
            }
            zn_pollQuestionItem.pollingAnswerItemList = zn_polling_answer_item_list;
            
            zn_polling_question_item_list.push_back(zn_pollQuestionItem);
        }
    }
    return zn_polling_question_item_list;
}

ZNList<ZNSDKPollingAnswerItem> ZMeetingPollingWrap::GetPollingRightAnswerItemList(ZoomSTRING zn_pollingID) {
    ZNList<ZNSDKPollingAnswerItem> zn_polling_answer_item_list;
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return zn_polling_answer_item_list;

    ZoomSDKPollingController *pollingController = [service getMeetingPollingController];
    if (!pollingController)
        return zn_polling_answer_item_list;

    NSString *pollingID = [NSString stringWithUTF8String:zn_pollingID.c_str()];
    
    NSArray<ZoomSDKPollingAnswerItem *> *arr = [pollingController getPollingRightAnswerItemList:pollingID];

    for (ZoomSDKPollingAnswerItem *pollAnswerItem in arr) {
        if (pollAnswerItem) {
            ZNSDKPollingAnswerItem zn_pollAnswerItem;

            // pollingID
            ZoomSTRING zn_p_ID;
            if ([pollAnswerItem pollingID]) {
                zn_p_ID = [[pollAnswerItem pollingID] UTF8String];
            }
            zn_pollAnswerItem.pollingID = zn_p_ID;

            // pollingQuestionID
            ZoomSTRING zn_pollingQuestionID;
            if ([pollAnswerItem pollingQuestionID]) {
                zn_pollingQuestionID = [[pollAnswerItem pollingQuestionID] UTF8String];
            }
            zn_pollAnswerItem.pollingQuestionID = zn_pollingQuestionID;

            // pollingSubQuestionID
            ZoomSTRING zn_pollingSubQuestionID;
            if ([pollAnswerItem pollingSubQuestionID]) {
                zn_pollingSubQuestionID = [[pollAnswerItem pollingSubQuestionID] UTF8String];
            }
            zn_pollAnswerItem.pollingSubQuestionID = zn_pollingSubQuestionID;

            // pollingAnswerID
            ZoomSTRING zn_pollingAnswerID;
            if ([pollAnswerItem pollingAnswerID]) {
                zn_pollingAnswerID = [[pollAnswerItem pollingAnswerID] UTF8String];
            }
            zn_pollAnswerItem.pollingAnswerID = zn_pollingAnswerID;

            // pollingAnswerName
            ZoomSTRING zn_pollingAnswerName;
            if ([pollAnswerItem pollingAnswerName]) {
                zn_pollingAnswerName = [[pollAnswerItem pollingAnswerName] UTF8String];
            }
            zn_pollAnswerItem.pollingAnswerName = zn_pollingAnswerName;
            
            // pollingAnsweredContent
            ZoomSTRING zn_pollingAnsweredContent;
            if ([pollAnswerItem pollingAnsweredContent]) {
                zn_pollingAnsweredContent = [[pollAnswerItem pollingAnsweredContent] UTF8String];
            }
            zn_pollAnswerItem.pollingAnsweredContent = zn_pollingAnsweredContent;

            // isChecked
            zn_pollAnswerItem.isChecked = [pollAnswerItem isChecked];
            
            zn_polling_answer_item_list.push_back(zn_pollAnswerItem);
        }
    }
    return zn_polling_answer_item_list;
}

ZNList<ZNSDKPollingAnswerResultItem> ZMeetingPollingWrap::GetPollingAnswerResultItemList(ZoomSTRING zn_pollingID) {
    ZNList<ZNSDKPollingAnswerResultItem> zn_polling_answer_result_item_list;
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return zn_polling_answer_result_item_list;

    ZoomSDKPollingController *pollingController = [service getMeetingPollingController];
    if (!pollingController)
        return zn_polling_answer_result_item_list;

    NSString *pollingID = [NSString stringWithUTF8String:zn_pollingID.c_str()];
    
    NSArray<ZoomSDKPollingAnswerResultItem *> *arr = [pollingController getPollingAnswerResultItemList:pollingID];

    for (ZoomSDKPollingAnswerResultItem *pollAnswerResultItem in arr) {
        if (pollAnswerResultItem) {
            ZNSDKPollingAnswerResultItem zn_pollAnswerResultItem;

            // pollingID
            ZoomSTRING zn_p_ID;
            if ([pollAnswerResultItem pollingID]) {
                zn_p_ID = [[pollAnswerResultItem pollingID] UTF8String];
            }
            zn_pollAnswerResultItem.pollingID = zn_p_ID;

            // pollingQuestionID
            ZoomSTRING zn_pollingQuestionID;
            if ([pollAnswerResultItem pollingQuestionID]) {
                zn_pollingQuestionID = [[pollAnswerResultItem pollingQuestionID] UTF8String];
            }
            zn_pollAnswerResultItem.pollingQuestionID = zn_pollingQuestionID;

            // pollingSubQuestionID
            ZoomSTRING zn_pollingSubQuestionID;
            if ([pollAnswerResultItem pollingSubQuestionID]) {
                zn_pollingSubQuestionID = [[pollAnswerResultItem pollingSubQuestionID] UTF8String];
            }
            zn_pollAnswerResultItem.pollingSubQuestionID = zn_pollingSubQuestionID;

            // pollingAnswerID
            ZoomSTRING zn_pollingAnswerID;
            if ([pollAnswerResultItem pollingAnswerID]) {
                zn_pollingAnswerID = [[pollAnswerResultItem pollingAnswerID] UTF8String];
            }
            zn_pollAnswerResultItem.pollingAnswerID = zn_pollingAnswerID;

            // pollingAnswerName
            ZoomSTRING zn_pollingAnswerName;
            if ([pollAnswerResultItem pollingAnswerName]) {
                zn_pollingAnswerName = [[pollAnswerResultItem pollingAnswerName] UTF8String];
            }
            zn_pollAnswerResultItem.pollingAnswerName = zn_pollingAnswerName;
            
            // selectedCount
            zn_pollAnswerResultItem.selectedCount = [pollAnswerResultItem selectedCount];
            
            zn_polling_answer_result_item_list.push_back(zn_pollAnswerResultItem);
        }
    }
    return zn_polling_answer_result_item_list;
}

ZNSDKPollingItem ZMeetingPollingWrap::GetPollingItemByID(ZoomSTRING zn_pollingID) {
    ZNSDKPollingItem zn_polling_item;

    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return zn_polling_item;

    ZoomSDKPollingController *pollingController = [service getMeetingPollingController];
    if (!pollingController)
        return zn_polling_item;
    
    if (zn_pollingID.empty()) {
        return zn_polling_item;
    }

    NSString *pollingID = [NSString stringWithUTF8String:zn_pollingID.c_str()];
    ZoomSDKPollingItem* poll = [pollingController getPollingItemByID:pollingID];

    if (poll) {
        ZoomSTRING zn_p_ID;
        if ([poll pollingID]) {
            zn_p_ID = [[poll pollingID] UTF8String];
        }
        zn_polling_item.pollingID = zn_p_ID;

        ZoomSTRING zn_pollingName;
        if ([poll pollingName]) {
            zn_pollingName = [[poll pollingName] UTF8String];
        }
        zn_polling_item.pollingName = zn_pollingName;

        nativeErrorTypeHelp Help_type;

        ZoomSDKPollingType pollingType = [poll pollingType];
        ZNSDKPollingType zn_pollingType = Help_type.ZNSDKPollingTypeMap(pollingType);
        zn_polling_item.pollingType = zn_pollingType;

        ZoomSDKPollingStatus pollingStatus = [poll pollingStatus];
        ZNSDKPollingStatus zn_pollingStatus = Help_type.ZNSDKPollingStatusMap(pollingStatus);
        zn_polling_item.pollingStatus = zn_pollingStatus;

        zn_polling_item.pollingQuestionCount = [poll pollingQuestionCount];
        zn_polling_item.totalVotedUserCount = [poll totalVotedUserCount];

        zn_polling_item.isLibraryPolling = [poll isLibraryPolling];
    }

    return zn_polling_item;
}

ZNList<ZNSDKPollingItem> ZMeetingPollingWrap::GetPollingItemList() {
    ZNList<ZNSDKPollingItem> zn_polling_item_list;
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return zn_polling_item_list;

    ZoomSDKPollingController *pollingController = [service getMeetingPollingController];
    if (!pollingController)
        return zn_polling_item_list;
    
    NSArray<ZoomSDKPollingItem *> *arr = [pollingController getPollingItemList];
    for (ZoomSDKPollingItem *poll in arr) {
        if (poll) {
            ZNSDKPollingItem zn_polling_item;
            
            ZoomSTRING zn_p_ID;
            if (poll.pollingID) {
                zn_p_ID = [poll.pollingID UTF8String];
            }
            zn_polling_item.pollingID = zn_p_ID;

            ZoomSTRING zn_pollingName;
            if ([poll pollingName]) {
                zn_pollingName = [[poll pollingName] UTF8String];
            }
            zn_polling_item.pollingName = zn_pollingName;

            nativeErrorTypeHelp Help_type;

            ZoomSDKPollingType pollingType = [poll pollingType];
            ZNSDKPollingType zn_pollingType = Help_type.ZNSDKPollingTypeMap(pollingType);
            zn_polling_item.pollingType = zn_pollingType;

            ZoomSDKPollingStatus pollingStatus = [poll pollingStatus];
            ZNSDKPollingStatus zn_pollingStatus = Help_type.ZNSDKPollingStatusMap(pollingStatus);
            zn_polling_item.pollingStatus = zn_pollingStatus;

            zn_polling_item.pollingQuestionCount = [poll pollingQuestionCount];
            zn_polling_item.totalVotedUserCount = [poll totalVotedUserCount];

            zn_polling_item.isLibraryPolling = [poll isLibraryPolling];

            zn_polling_item_list.push_back(zn_polling_item);
        }
    }
    return zn_polling_item_list;
}

ZNSDKError ZMeetingPollingWrap::StartPolling(ZoomSTRING zn_pollingID) {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKPollingController *pollingController = [service getMeetingPollingController];
    if (!pollingController)
        return ZNSDKERR_SERVICE_FAILED;

    nativeErrorTypeHelp Help_type;
    NSString *pollingID = [NSString stringWithUTF8String:zn_pollingID.c_str()];
    ZoomSDKError ret = [pollingController startPolling:pollingID];
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingPollingWrap::StopPolling(ZoomSTRING zn_pollingID) {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKPollingController *pollingController = [service getMeetingPollingController];
    if (!pollingController)
        return ZNSDKERR_SERVICE_FAILED;

    nativeErrorTypeHelp Help_type;
    NSString *pollingID = [NSString stringWithUTF8String:zn_pollingID.c_str()];
    ZoomSDKError ret = [pollingController stopPolling:pollingID];
    return Help_type.ZoomSDKErrorType(ret);
}

ZNSDKError ZMeetingPollingWrap::RestartPolling(ZoomSTRING zn_pollingID) {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return ZNSDKERR_SERVICE_FAILED;
    
    ZoomSDKPollingController *pollingController = [service getMeetingPollingController];
    if (!pollingController)
        return ZNSDKERR_SERVICE_FAILED;

    nativeErrorTypeHelp Help_type;
    NSString *pollingID = [NSString stringWithUTF8String:zn_pollingID.c_str()];
    ZoomSDKError ret = [pollingController restartPolling:pollingID];
    return Help_type.ZoomSDKErrorType(ret);
}

bool ZMeetingPollingWrap::IsQuestionCaseSensitive(ZoomSTRING zn_pollingID, ZoomSTRING zn_questionID) {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return false;
    
    ZoomSDKPollingController *pollingController = [service getMeetingPollingController];
    if (!pollingController)
        return false;

    NSString *pollingID = [NSString stringWithUTF8String:zn_pollingID.c_str()];
    NSString *questionID = [NSString stringWithUTF8String:zn_questionID.c_str()];

    bool ret = [pollingController isQuestionCaseSensitive:pollingID questionID:questionID];
    return ret;
}

ZoomSTRING ZMeetingPollingWrap::GetPollingQuestionImagePath(ZoomSTRING zn_pollingID, ZoomSTRING zn_questionID) {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return "";
    
    ZoomSDKPollingController *pollingController = [service getMeetingPollingController];
    if (!pollingController)
        return "";

    NSString *pollingID = [NSString stringWithUTF8String:zn_pollingID.c_str()];
    NSString *questionID = [NSString stringWithUTF8String:zn_questionID.c_str()];

    NSString *ret = [pollingController getPollingQuestionImagePath:pollingID questionID:questionID];
    
    if (!ret) {
        return "";
    }
    ZoomSTRING zn_ret = [ret UTF8String];
    return zn_ret;
}

bool ZMeetingPollingWrap::CanGetRightAnswerItemList(ZoomSTRING zn_pollingID) {
    ZoomSDKMeetingService *service = [[ZoomSDK sharedSDK] getMeetingService];
    if (!service)
        return false;
    
    ZoomSDKPollingController *pollingController = [service getMeetingPollingController];
    if (!pollingController)
        return false;

    NSString *pollingID = [NSString stringWithUTF8String:zn_pollingID.c_str()];

    return [pollingController canGetRightAnswerItemList:pollingID];
}

void ZMeetingPollingWrap::onPollingStatusChanged(ZoomSTRING pollingID, ZNSDKPollingStatus status)
{
    if (m_pSink)
    {
        m_pSink->onPollingStatusChanged(pollingID, status);
    }
}

void ZMeetingPollingWrap::onPollingResultUpdated(ZoomSTRING pollingID)
{
    if (m_pSink)
    {
        m_pSink->onPollingResultUpdated(pollingID);
    }
}

void ZMeetingPollingWrap::onPollingListUpdated()
{
    if (m_pSink)
    {
        m_pSink->onPollingListUpdated();
    }
}

void ZMeetingPollingWrap::onPollingActionResult(ZNSDKPollingActionType actionType, ZoomSTRING pollingID, bool bSuccess, ZoomSTRING errorMsg) 
{
    if (m_pSink)
    {
        m_pSink->onPollingActionResult(actionType, pollingID, bSuccess, errorMsg);
    }
}