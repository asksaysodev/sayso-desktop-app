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

class ZUIClient_API SettingPanelMyAccount : public zui::View
{
public:
    SettingPanelMyAccount();
    ~SettingPanelMyAccount() = default;

    std::shared_ptr<zui::View> Clone() const override { return std::make_shared<SettingPanelMyAccount>(*this); }
    std::shared_ptr<zui::ViewNode> Build() override;

private:
    std::string _myName = std::string("Mengyuan Feng");
    std::string _signOnStatus = std::string("Single add-on");
};
