#ifndef _ZUI_CLIENT_MGR_H_
#define _ZUI_CLIENT_MGR_H_

#ifdef _WIN32
#include "zUIClient/zUI_stdafx.h"
#else
#include "zUI_stdafx.h"
#endif
#include "zui_new_setting_event.h"

#include "cmmlib/CmmPrefBase.h"
#include "cmmlib/CmmPrefConsts.h"

class ZUIClient_API ZUIClientMgr: public zpref::IPolicyUpdateObserver
{
public:
  static ZUIClientMgr* getInstance();

  //> zpref::IPolicyUpdateObserver
  virtual void OnPolicyUpdated(const zpref::zPolicyList& policies) override;

  void InitLanguage(zoom_data::CMM_LANGUAGE_ID_VALUE languageId);
  zoom_data::CMM_LANGUAGE_ID_VALUE GetLanguageID();
  bool IsFullWidthPunctuation();

  void SetNewSettingEventHandler(CZUINewSettingEventHandler* handler);
  CZUINewSettingEventHandler* GetNewSettingEventHandler();

  static void NotifyEvent(ZUIClientSettingEventParam& event);
  static void NotifyEvent(ZUINewSettingEvent event_type);
  static void NotifyEvent(ZUINewSettingEvent event_type, zui::any& wparam);
private:
  ZUIClientMgr() = default;
  ~ZUIClientMgr() = default;

  CZUINewSettingEventHandler* _eventHandler = nullptr;
  zoom_data::CMM_LANGUAGE_ID_VALUE _languageId = zoom_data::CMM_LANGUAGE_ID_VALUE::Unknow;
};

#endif //_ZUI_CLIENT_MGR_H_
