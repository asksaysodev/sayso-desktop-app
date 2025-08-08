#ifndef _SETTING_PANEL_GENERAL_H_
#define _SETTING_PANEL_GENERAL_H_

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

#ifdef ENABLE_ZAPP
#include "zApp/ZappBase.h"
#endif // ENABLE_ZAPP

#include "cmmlib/CmmEmojiReactionDefine.h"
#include "zui_client_data_define.h"
#include "common_def.h"

class ZUIClient_API CGeneralSettingDataModel : public IInitializable
{
public:

  static CGeneralSettingDataModel& GetInstance();
  CGeneralSettingDataModel();
  ~CGeneralSettingDataModel() = default;

  virtual void initializeState() override;

  virtual void initializeStateHandler() override;

  void OnPTLogout();
  void OnPTLogin();
  void UpdateThemeColor();
  void UpdateThemePanelVisibility();
  void UpdateTopNavigationUI();
  void UpdateZoomPresenceWithOutlook();
  void UpdateShowPairRoomFeautre();
  void UpdateZoomRoomSettings();
  void UpdateZoomAssistant(bool need_update_phone = true);
  void OnUpdatesPolicyUpdated(const zpref::zPolicyList& policies);
  void OnUpdatesPolicyUpdated(zpref::zPolicyId policeId);
  void ResetIntegrationZoomWithOutlook();
  void RefreshEmojiSkinTone();
#ifdef ENABLE_ZAPP
  void UpdateAppsItemShowState(NS_ZOOM_APPS::ZappEnableState pt_state);
#endif // ENABLE_ZAPP
private:
  void UpdateGeneralUpdates();
  void UpdateChannels();
  void UpdateCurrentChannel();
  void EnableShowUpdateChannel();
  void HandleSingleUpdatesPolicyUpdate(zpref::zPolicyId policeId);
  // overall page
public:
    
  zui::State<bool> _appearanceVisible = true;
  enum AppearanceType {
    kAppearanceLight = 1,
    kAppearanceDark,
    kAppearanceSystem
  };
  zui::State<zui::any> _appearanceType = zui::any(AppearanceType::kAppearanceLight);
    
  enum ThemeType {
    kThemeClassic = 0,
    kThemeBloom,
    kThemeAgace = 3,
    kThemeRose = 2
  };

  zui::State<bool> _themeVisible = true;
  zui::State<bool> _themeEnable = true;

  zui::State<zui::any> _themeType = zui::any(ThemeType::kThemeClassic);
  zui::State<zui::any> _skinToneType = zui::any(CmmEmojiReactionSkinTone_Unknown);
    
  // reset button
  SwitchControl _updateZoomAuto;
  struct AutoUpdateSpeed
  {
    RadioGroupProperty _group;
    zui::State<std::vector<zui::any>> _data;
  };
  AutoUpdateSpeed _autoUpdateSpeed;
    
  SwitchControl _autoStartZoom;
  CheckBoxProperty _autoStartZoomSilently;
  SwitchControl _minimizeZoom;
  SwitchControl _syncZoomPresenceWithOutlook;
  SwitchControl _pinZoomToMacMenuBar;
  zui::State<bool> _enableDiagnostic = false;
  zui::State<bool> _showIconMethod = false;
  SwitchControl _showZRPairIcon;
  RadioGroupProperty _ShowPairMethod;
  zui::State<bool> _useZRAutoDetection = false;
  zui::State<bool> _useZRAutoDetectionWarningVisible = false;
  SwitchControl _AutoEnableCompanionMic;
  zui::State<bool> _isShowZRPanel = true;
    
  SwitchControl _enableDiagnosticDataViewer;

  zui::State<bool> _isShowResetNavigationBar = true;
    
  enum ZRJoinMethod
  {
    kSelectMethodEveryTime,
    kJoinFromRoomAndAsCompanion,
    kJoinFromComputerOnly,
    kJoinFromRoomOnly
  } ZRJoinMethod;

  DropDownProperty _zrJoinMethod;

  zui::State<bool> _showClearZoomAppsCache = false;
  SwitchControl  _callThisDeviceForScheduleMeeting;
  SwitchControl _useZoomAssistantForMeetings = { false,false,false };
  SwitchControl _useZoomAssistantForPhone = {false,false,false};
  zui::State<bool> _IsUseZoomAssistantVisible = false;
  CheckBoxProperty _hideZoomAssistantWhenNoCall;
  zui::State<std::string> _zoomappsLastClearCacheTime;
  zui::State<bool> _zoomappsLastClearCacheTimeVisible = false;

  //huddles
  SwitchControl _fadeMusic;
  SwitchControl _keepStatusWhenInHuddle;
  zui::State<bool> _huddlesVisible = true;

#if (defined ZOOM_DEBUG) && (defined WIN32)
  zui::State<bool> _runtime_test = false;
  zui::State<bool> _debug_switch = false;
  zui::State<bool> _automation = false;
#endif

private:
  CmmBool is_init_with_au2_value_ = CmmFalse;
};


class ZUIClient_API SettingPanelGeneral : public zui::View
{
public:

  SettingPanelGeneral();
  ~SettingPanelGeneral() = default;

  std::shared_ptr<zui::View> Clone() const override { return std::make_shared<SettingPanelGeneral>(*this); }
  std::shared_ptr<zui::ViewNode> Build() override;
  
private:
  void UpdateAppearanceResource();
  void UpdateAutoUpdateSpeedVisible();
  void UpdateAutoCallTips();
  void UpdateAutoCallTipVisible();

  void OnClickLearnMoreAboutUpdate();

  zui::View GetAppearanceView();

  zui::View GetNavigationBarView();

  zui::View GetZoomWorkPlaceView();

  zui::View GetPresenceStatusView();

  zui::View GetZoomRoomsView();

  zui::View GetScheduleMeetingView();

  zui::View GetZoomAppsView();

  zui::View GetHuddleView();

  zui::View GetZoomAssistantView();

#if (defined ZOOM_DEBUG) && (defined WIN32)
  zui::View GetDebugView();
#endif


private:
  zui::State<std::string> _appearance_light_bk;
  zui::State<std::string> _appearance_dark_bk;
  zui::State<std::string> _appearance_system_bk;
  zui::State<bool> _autoUpdateSpeedVisible = false;
  zui::Bind<zui::any> _themeType = zui::any(CGeneralSettingDataModel::ThemeType::kThemeClassic);
  zui::Bind<std::vector<zui::any>> _autoUpdateSpeedData;
  zui::Bind<bool> _updateZoomAutoValue = true;
  
  zui::State<std::string> _bk_pair_with_room = std::string(zui::fillDefault);

  zui::Bind<bool> _autoCallValue = false;
  zui::Bind<bool> _autoCallEnable = true;
  zui::State<bool> _autoCallTipsVisible = false;
  zui::State<std::string> _autoCallTips;
};

#define GeneralPanelDataModel CGeneralSettingDataModel::GetInstance()

#endif
