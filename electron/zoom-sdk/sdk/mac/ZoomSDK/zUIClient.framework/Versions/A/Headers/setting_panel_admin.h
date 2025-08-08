#pragma once

#ifdef _WIN32
#include "zUI/view.h"
#include "zUIClient/zUI_stdafx.h"
#elif defined __MACOS__
#import <zUI/view.h>
#import <zUIClient/zUI_stdafx.h>
#else
#include "view.h"
#include "zUI_stdafx.h"
#endif

class CAdminPanelDataModel
{
public:
  static CAdminPanelDataModel& GetInstance();

  void Init();

public:
  // UI Control enable
  zui::State<bool> _enableAIC_toggleEnabled = true;
  zui::State<bool> _meetingQuestionsAutoStart_checkboxEnabled = true;
  zui::State<bool> _meetingSummaryAutoStart_checkboxEnabled = true;

  zui::State<bool> _enableAIC = false;
  zui::State<bool> _meetingQuestionsAutoStart = false;
  zui::State<bool> _meetingSummaryAutoStart = false;
  zui::State<std::string> _AICFeatureDisabledInSettingTip;
  zui::State<bool> _AICFeatureDisabledInSetting = false;

protected:
  CAdminPanelDataModel() = default;
  ~CAdminPanelDataModel() = default;
};

class ZUIClient_API SettingPanelAdmin : public zui::View
{
public:
    SettingPanelAdmin();
    ~SettingPanelAdmin() = default;

    std::shared_ptr<zui::View> Clone() const override { return std::make_shared<SettingPanelAdmin>(*this); }
    std::shared_ptr<zui::ViewNode> Build() override;

protected:
    zui::State<bool> _dividerVisible = true;
    zui::State<bool> _AICBannerVisible = true;
};

#define AdminPanelDataModel CAdminPanelDataModel::GetInstance()
