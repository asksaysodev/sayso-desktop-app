#ifndef _CONF_CAPTION_SETTINGS_H_
#define _CONF_CAPTION_SETTINGS_H_

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
#include "zUIClient/zui_client_data_define.h"

class ZUIClient_API CConfCaptionSettingView : public zui::View
{
public:
  CConfCaptionSettingView();
  ~CConfCaptionSettingView() = default;

  std::shared_ptr<zui::View> Clone() const override { return std::make_shared<CConfCaptionSettingView>(*this); }
  std::shared_ptr<zui::ViewNode> Build() override;
};

#endif