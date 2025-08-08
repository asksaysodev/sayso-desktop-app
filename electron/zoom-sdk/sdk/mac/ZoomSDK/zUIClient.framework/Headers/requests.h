#ifndef _REQUESTLIST_H_
#define _REQUESTLIST_H_

#ifdef _WIN32
#include "zUI/view.h"
#include "zUIClient/zUI_stdafx.h"
#include "zNet/IRequestLifecycleMonitor.h"
#include "cmmlib/win/WinTimer.h"
#elif defined __MACOS__
#import <zUI/view.h>
#import <zUIClient/zUI_stdafx.h>
#include "CmmTimer_Mac.h"
#import <zPTApp/PTService/IPTAppWebService.h>
#elif defined __LINUX_CLIENT__
#include "view.h"
#include "zUI_stdafx.h"
#include "linux/CmmTimer_Linux.h"
#include "zNet/IRequestLifecycleMonitor.h"
#else
#include "view.h"
#include "zUI_stdafx.h"
#endif


using namespace zui;


class ZUIClient_API IRequestSink
{
public:
    virtual void viewButtonDidClick(const void* aRequestEntity) = 0;//Triggered from RequestList Page
    virtual bool isInListPage() = 0;//Triggered from RequestList Page
    virtual void closeButtonDidClick() = 0;//Triggered from RequestDetail Page
    virtual void copyButtonDidClick(const std::string& stringToCopy) = 0;//Triggered from RequestDetail Page
};


class ZUIClient_API RequestList : 
public zui::View,
public IRequestLifecycleSink,
public Cmm::CTimerProc
{
public:
    
    //IRequestLifecycleSink
    void OnRequestLifecycleDataCollected(const RequestLifecycleData& data, CmmInt64 callId) override;
    void OnRequestLifecycleDataExported(const RequestLifecycleExportPath& paths, CmmInt64 callId) override;
    
    RequestList(IRequestSink* sink);
    ~RequestList();
    
    std::shared_ptr<zui::View> Clone() const override;
    std::shared_ptr<zui::View> Clone() override;
    std::shared_ptr<zui::ViewNode> Build() override;
    
    RequestList(const RequestList&);
    
private:
    
    Cmm::CTimerID _timer;
    
    void OnTimer(void*, CmmUInt, CmmHTimer idEvent, CmmULong dwTime) override;
    
    void UpdateRequestEntities(const std::vector<zui::any>& allRequestEntities);
    
    bool CanRefresh();
    
    std::string _textInSearchField;
    
    void Refresh(bool forced = false);
    
    //Title
    State<std::string> _textOnTitle = std::string("HTTP Request Details");
    
    //Loading
    bool _loading = false;
    State<std::string> _textOnRefreshButton = std::string("Refresh");
    State<bool> _refreshButtonEnabled = true;
    void SetLoading(bool loading);
    
    //Searching
    State<bool> _searching = false;
    
    State<std::vector<zui::any>> _allVisibleRequestEntities;
    std::vector<zui::any> _allRequestEntities;
    
    IRequestSink* _sink;
    
    CmmInt64 _requestID;
    CmmInt64 _responseID;
};

class ZUIClient_API RequestDetail : public zui::View
{
public:
    RequestDetail(IRequestSink* sink);
    ~RequestDetail() = default;
    
    std::shared_ptr<zui::View> Clone() const override;
    std::shared_ptr<zui::View> Clone() override;
    std::shared_ptr<zui::ViewNode> Build() override;
    
    void Update(const void* aRequestEntity);
    
private:
    
    IRequestSink* _sink;
    
    //Section 1: Request Information
    State<bool> _copyURLHidden = true;
    State<std::string> _stringURL = std::string("N/A");
    State<std::string> _stringMethod = std::string("N/A");
    State<std::string> _stringStatus = std::string("N/A");
    State<std::string> _stringThisID = std::string("N/A");
    State<std::string> _stringLocalIP = std::string("N/A");
    State<std::string> _stringCreateTime = std::string("N/A");
    State<std::string> _stringSendTime = std::string("N/A");
    
    //Section 2: Performance Information
    State<std::string> _stringDNSTime = std::string("N/A");
    State<std::string> _stringTCPTime = std::string("N/A");
    State<std::string> _stringTLSTime = std::string("N/A");
    State<std::string> _stringTotalTime = std::string("N/A");
    
    //Section 3: Status Information
    State<std::string> _stringOvertime = std::string("N/A");
    State<std::string> _stringOSError = std::string("N/A");
    State<std::string> _stringCURLError = std::string("N/A");
    State<std::string> _stringIsProxy = std::string("N/A");
    State<std::string> _stringProxyAddress = std::string("N/A");
    State<std::string> _stringIsPK = std::string("N/A");
    State<std::string> _stringIgnoreProxy = std::string("N/A");
};

#endif
