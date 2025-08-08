#ifndef _SETTING_PANEL_SHARE_SCREEN_H_
#define _SETTING_PANEL_SHARE_SCREEN_H_

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
#include "zUI/zui_base.h"
#include "zui_client_data_define.h"

class ICmmShareConfig;

class ZUIClient_API CSettingShareDataModel : public IInitializable
{
public:

  static CSettingShareDataModel& GetInstance();
  CSettingShareDataModel();
  ~CSettingShareDataModel() = default;

  virtual void initializeState() override;

  virtual void initializeStateHandler() override;

  // interface start
  ICmmShareConfig* GetShareConfigIt();

  void SetSideBySideControlsVisible(bool val);
  void UpdateSideBySideControls();
  void UpdateShareFilterOption();
  void UpdateOptimizeVideoclip();
  void SetUseSystemPickerVisible(bool val);  // mac only
  RadioBtnProperty1 _eableRemoteControlInAllApps;  // win only

  // interface end

public:
  enum {
    windowSizeCurrentSize = 0,
    windowSizeFullScreen = 1,
  };
  RadioGroupProperty _winSizeOfViewingShare;
  RadioBtnProperty _scaleToFitContent;
  RadioBtnProperty1 _sideBySide;

  enum {
    shareWindow = 0,
    shareApp = 1
  };
  RadioGroupProperty _shareContentOption;

  RadioBtnProperty _showGreenBorder;
  RadioBtnProperty1 _showZoomWindowWhenSharing;
  CheckBoxProperty _shareToolbarAndNoti;

  enum {
    previewContent = 0,
    shareDesktop = 1
  };
  RadioGroupProperty _shareButtonBehavior;
  RadioGroupProperty _shareButtonBehaviorInZR;

  enum {
    captureAuto = 0,
    captureWithWindowFilter,
    captureWithWindowFilterAdvanced,
    captureWithoutWindowFilter,
#ifdef WIN32
    captureSecureShareWithWindowFilter,
#endif
    captureLegacyOperationgSystems
  };

  DropDownProperty _captureMode;

  RadioBtnProperty _shareSelectedAppWindowOnly;//mac only
  RadioBtnProperty1 _useSystemPicker;  //mac only

  RadioBtnProperty _useTCP;

  enum {
    k1080p_30fps = 0,
    k720p_30fps,
    k1080p_60fps,
    k720p_60fps
  };
  zui::State<bool> _isShowPanelAutoOptimizeVideoSharing = true;
  RadioBtnProperty _autoOptimizeVideoSharing;
  DropDownProperty _preferredParam;
  zui::State<bool> _isShownPreferredParamWarning = false;

  
  enum {
    limitShareFPS_15,
    limitShareFPS_10,
    limitShareFPS_8,
    limitShareFPS_6,
    limitShareFPS_4,
    limitShareFPS_2,
    limitShareFPS_1
  };
  SwitchControl    _shareFPSLimit;
  DropDownProperty _limitShareFPSValue;

  RadioBtnProperty _accelerationShareScreen;
  SwitchControl   _accelerationVideoScreen;
  RadioBtnProperty _accelerationAnnoScreen;

  DropDownProperty _screenCaptureModeOnWayland;
  
  zui::State<bool> _showAdvanced = false;

  // only linux need
  zui::State<bool> _isAutoGrabRemoteControlEnabled = false;
  zui::State<int> _remoteControllerIdleIndex = 0;
  
  zui::State<bool> _showAdvancedButton = true;
#ifdef BUILD_FOR_VDI
  zui::State<std::list<std::string>> _vdiShareFps;
#endif
};

class ZUIClient_API SettingPanelShareScreen : public zui::View
{
public:

  SettingPanelShareScreen();
  ~SettingPanelShareScreen() = default;

  std::shared_ptr<zui::View> Clone() const override { return std::make_shared<SettingPanelShareScreen>(*this); }
  std::shared_ptr<zui::ViewNode> Build() override;

};

#define SharePanelDataModel CSettingShareDataModel::GetInstance()

#endif
