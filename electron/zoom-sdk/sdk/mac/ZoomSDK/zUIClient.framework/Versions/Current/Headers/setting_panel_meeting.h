#ifndef _SETTING_PANEL_MEETING_H_
#define _SETTING_PANEL_MEETING_H_

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

#ifdef BUILD_FOR_VDI
#include "vdi/util/VdiHelper.h"
#endif
#include "zUIClient/zui_client_data_define.h"
#include "cmmlib/CmmEmojiReactionDefine.h"

class ZUIClient_API CMeetingSettingDataModel : public IInitializable
{
public:

  static CMeetingSettingDataModel& GetInstance();
  CMeetingSettingDataModel();
  ~CMeetingSettingDataModel() = default;
  // overall page

  virtual void initializeState() override;

  virtual void initializeStateHandler() override;

public:
#ifdef ENABLE_ZAPP
  bool IsZoomAppSelected();
  void SwitchToZoomAppItem();
  void UpdateAppsItemShowState(NS_ZOOM_APPS::ZappEnableState pt_state);
  void OnManageApps();
#endif // ENABLE_ZAPP
  void UpdateDisplayPreviewWhenJoinMeeting();
  void UpdateAutoJoinVOIP();
  void UpdateWindowModeControls();
  void UpdateDualModeSettingValue(bool is_enable_dual_stream);
  void UpdateShowMeetingTimers();
  void UpdateAlwaysShowToolbarUI();
  void UpdateAlwayDisplayParticipantName();
  void UpdateShowNoVideoParticipants();
  void UpdateOpenSelectedZoomApps();

  void GetOpenSelectedZoomApps(int& selected, int& count);
  void UpdateAppItemChecked(const std::string& app_id, bool checked);
  void OnIconDownloaded(const CString& app_id, const CString& icon_path);

  void OnPTLogin();
  void OnPTLogout();

  void RefreshStopIncomingVideo();
  void RefreshHandGestureRecognitionSetting();
  void RefreshAnimateEmojisSetting();
  void RefreshRaiseHandChimeSetting();
  void UpdateResetDisclaimerUI();
  void UpdatePersonalWallpaperVisibility();
  void UpdateAllowedUnmuteAudiolistPanel();
  void UpdatePerformancePanel();

#ifdef BUILD_FOR_VDI
  bool IsVDISupportDualMonitor();
#endif
public:
  SwitchControl _showVideoPreview;
  SwitchControl _keepMyCameraOff;
  SwitchControl _muteMyMicro;
  SwitchControl _autoJoinAudio;
  SwitchControl _enterFullScreenAuto;

  zui::State<bool> _showManageSelectedApps = false;
  zui::State<bool> _showSelectedZappOverview = true;

  enum class MeetingWindowsView {
    kSingleWindowWithTbas,
    kDualWindow
  };
  RadioGroupProperty _meetingWindowView;

  SwitchControl _useDualMonitors;
  SwitchControl _keepParticipantNamesVisible;
  SwitchControl _showMeetingTimer;
  SwitchControl _showParticipantProfilePicInChat;
  SwitchControl _showPMIOnMeetingTab;
  
  SwitchControl _keepMeetingControlsVisible;
  SwitchControl _useSpaceBarToMute;
  SwitchControl _playSoundForMuteUnmute;
  SwitchControl _copyInviteLinkToClipboard;

  BtnProperty _whoCanUnmuteYou;

  SwitchControl _hideSelfView;
  SwitchControl _showMeAsActiveSpeaker;
  SwitchControl _stopVideoWhenComputerLocks;

  SwitchControl _showNoVideoParticipants;

  zui::State<bool> _wallpaperVisible = true;
  zui::State<bool> _showPersonalWallpaper = false;
  zui::State<bool> _showAdvancedButton = true;
  zui::State<std::string> _personalWallpaper;
  
  enum class MaxParticipantsPerScreen
  {
    k25Participants,
    k49Participants
  };
  RadioGroupProperty _maxParticipantsPerScreen;

  SwitchControl _showReactionsAboveToolbar;
  SwitchControl _animateEmojis;
  SwitchControl _useHandGestures;
  SwitchControl _palyChimeWithHandRaise;
  
  SwitchControl _showLeaveConfConfirmation;
  SwitchControl _keepMyZAppsOpen;

  SwitchControl _showStopIncomingVideo;
  SwitchControl _skipAudioPromptsInThirdPartMeetings;
  BtnProperty _resetMeetingDisclaimer;
  zui::State<bool> _advancedMeetingVisible = true;

  SwitchControl _optimzieIncomingVideoQuality;
  SwitchControl _optimizeOutgoingVideoQuality;

  DropDownProperty _videoRenderingMethod;
  DropDownProperty _videoRenderingPostProcessing;
  DropDownProperty _videoCaputringMethod;

  SwitchControl _hardwareAcceForRecvVideo;
  SwitchControl _hardwareAcceForSendVideo;
  SwitchControl _hardwareAcceForVideoProcessing;
  SwitchControl _hardwareAcceForVB;

  zui::State<bool> _showAdvanced = false;
  zui::State<std::string> _animateEmojisClap;
  zui::State<std::string> _animateEmojisThumbsUp;

  struct AppData
  {
    std::string icon;
    std::string name;
    std::string app_id;
    bool open = false;

    bool operator==(const AppData& other) const{
      return icon == other.icon && name == other.name && open == other.open;
    }
  };
  zui::State<std::vector<zui::any>> _selectedZoomApps;
  zui::State<std::string> _selectedZoomAppsText = std::string("Selected(0/0)");
  zui::State<bool> _showSelectZoomAppsLimit = false;

  zui::State<bool> _showOpenSelectedZoomApps = false;

  zui::State<std::string> _strUseSpaceBarToMute;

#ifdef _WIN32
  SwitchControl _turnOnEfficiencyMode;
  zui::State<bool> _turnOnEfficiencyModeTipVisible = false;
#endif

private:
  bool _ignore_update_limit_text = false;
};

class ZUIClient_API SettingPanelMeetings : public zui::View
{
public:
    SettingPanelMeetings();
    ~SettingPanelMeetings() = default;

    std::shared_ptr<zui::View> Clone() const override { return std::make_shared<SettingPanelMeetings>(*this); }
    std::shared_ptr<zui::ViewNode> Build() override;
};

#define MeetingPanelDataModel CMeetingSettingDataModel::GetInstance()

#endif //_SETTING_PANEL_MEETING_H_
