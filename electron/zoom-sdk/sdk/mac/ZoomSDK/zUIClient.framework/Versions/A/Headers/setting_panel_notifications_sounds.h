#ifndef _SETTING_PANEL_NOTIFICATIONS_SOUNDS_H_
#define _SETTING_PANEL_NOTIFICATIONS_SOUNDS_H_

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
#include "zVideoApp/Confdef.h"
#include "cmmlib/IZMRingtoneMgr.h"
#include <vector>
#include "zui_client_data_define.h"

class ZUIClient_API CNotifSoundsSettingDataModel : public IInitializable
{
public:

  //enum class RING_TONE_TYPE
  //{
  //  RINGTONE_DEFAULT = 0,
  //  RINGTONE_MEETING,
  //  RINGTONE_PHONE,
  //  RINGTONE_CONTACT_CENTER,
  //  RINGTONE_CONTACT,
  //};

  enum class RingToneType
  {
    kDefault,
    kVideoCalls,
    kPhone,
    kShareLineGroup,
    kCallQueues,
    kShareLineAppearance,
    kContactCenter,
    kCustomContactRingtones,
  };

  struct CustomRingtones
  {
    std::string _name;
    int _ringType;
    bool operator==(const CustomRingtones& other) const
    {
      return _name == other._name && _ringType == other._ringType;
    }
  };

  zui::State<std::vector<zui::any>> _customRingtones;
  zui::State<bool> _isShowEmpyCustomRingTones = true;
  zui::State<std::string> _ringToneResourcePathForMac;   // only for mac

  static CNotifSoundsSettingDataModel& GetInstance();
  CNotifSoundsSettingDataModel();
  ~CNotifSoundsSettingDataModel() = default;

  virtual void initializeState() override;

  virtual void initializeStateHandler() override;

  ZoomRingtone::IZoomRingtoneMgr* GetRingtoneMgr();
  void GetRingToneInfo(std::vector<ZoomRingtone::RingtoneData*>& ringtone_list);
  CString GetRingtonesFromCache(CNotifSoundsSettingDataModel::RingToneType type, const CString& phone_number = _T(""));

  void UpdateRingtoneList();
  void UpdateRingtoneVolume();
  void UpdateCallWaitingVolume();
  void RefreshActivityCenterSetting();
  void UpdateIMTeamChatFeatures();

  CmmBool OnAudioDeviceStatusChanged(DEVICE_CMD cmd, CmmUInt32 ret, CmmUInt32 handle);
  void OnStartPlayRingtone(const CString& filePath);
  void OnStopPlayRingtone();
  void OnAudioChanged(CmmUInt32 ret, CmmUInt32 handle);

  CmmBool SyncRingtones2db(RingToneType type, const CString& sel_ringtone, const CString& phone_number = _T(""));
  int GetSelectRingtoneIndex(RingToneType type, const std::string& phone_number = "");
  void UpdateCCIInfo();
  std::string CreateDNDTimeStr(int hour, int minutes);
  std::list<std::string> CreateTimeList(bool is24Format);
  int GetTimerIndex(const std::string& value);
  void GetDNDTimerParam(int& startHour, int& startMinute, int& endHour, int& endMinute);

  void DownloadRingtone();

private:
  void InitCCINotificationItem();
  void InitNotiticationItem();
  void InitRemindUncomingMeeting();
  void InitNewRecordingNotify();
  void InitIMNotification();
  void InitRingTone();
  void InitSilenceNotificationWhenSharing();
  
  void InitLoadRingtoneConfig();

  void GetAccountUserID(CString& user_id);


public:
  SliderControl _ringtoneVolume;
  SliderControl _callWaitingVolume = {false,true,0.0f};

  CmmUInt32 _ringtoneHandle = NULL_AUDIO_FILE_HANDLE;

  struct RingtoneData {
    zui::State<int> _selectIndex = 0;
    zui::State<bool> _deviceChange = false;
    std::string _label;
    std::string _phoneNumber;

    bool operator==(const RingtoneData& other) const {
      return _selectIndex == other._selectIndex &&
        _deviceChange == other._deviceChange && 
        _label == other._label &&
        _phoneNumber == other._phoneNumber;
    }
  };
  CString _downloadRingtonesReqId;
  ZoomRingtone::RingtoneData _defaultRingtone;
  zui::State<bool> _needDownload = false;
  zui::State<std::vector<ZoomRingtone::RingtoneData*>> _ringtoneList;
  zui::State<std::string> _currentPlayLabel;

  RingtoneData _videoCallsRingtone;
  
  zui::State<std::vector<zui::any>> _extensionRingtones;

  zui::State<bool> _isShareLineGroupVisble = false;
  zui::State<bool> _isCallQueuesVisble = false;
  zui::State<bool> _isShareLineAppearanceVisble = false;
  zui::State<bool> _isCciRingtoneVisble = false;
  zui::State<bool> _isVDIShowRingtones = true;

  RingtoneData _shareLineGroupRingtone;
  RingtoneData _callQueuesRingtone;
  RingtoneData _shareLineAppearanceRingtone;
  RingtoneData _cciRingtone;
  //team Chat

  SwitchControl _palyNewChatMsgSound;
  SwitchControl _palyNewEmailSound;

  zui::State<bool> _is24TimerFormat = true;
  SwitchControl _setDoNotDisturbHours;
  std::list<std::string> _doNotDisturbTime;
  zui::State<int> _doNotDisturbFromIndex = 0;
  zui::State<int> _doNotDisturbToIndex = 0;

  enum HidePhoneCallType
  {
    BlockActivity,
    AlertButContinueActivity,
  };

  DropDownProperty _hidePhoneCallType = { true,true,static_cast<int>(AlertButContinueActivity) };
  SwitchControl _hidePhoneCallsDuringMeetings;
  SwitchControl _hideIncomingCallsWhileOnCall;
  SwitchControl _muteChatDuringMeetings;
  SwitchControl _openChatOnPushNotification;
  SwitchControl _showMessagePreview;
  SwitchControl _showNotificationsUntilDismissed;
  SwitchControl _silenceNotificationsWhenSharing;
   
  //Send desktop notifications for:
  enum UnComingMeetingTimeType
  {
    UnComingMeetingAtEventTime,
    UnComingMeeting1Minutes,
    UnComingMeeting2Minutes,
    UnComingMeeting5Minutes,
    UnComingMeeting10Minutes,
    UnComingMeeting15Minutes,
  };

  zui::State<int> _upcomingMeetingsTimeType = static_cast<int>(UnComingMeeting5Minutes);
  SwitchControl _upcomingMeetings;
  SwitchControl _newRecordingsAvaiable;
  SwitchControl _newChatMessage;
  enum class SystemNotifForChatMessageType {
    kAllMessages = 0,
    kDirectMessages
  };
  zui::State<zui::any> _systemNotifForChatMessageType = zui::any(SystemNotifForChatMessageType::kAllMessages);
  zui::State<unsigned int> _exceptChatChannelCount = 0;
  zui::State<unsigned int> _keywordsMentionedCount = 0;
  zui::State<unsigned int> _contactsMentionedCount = 0;

  SwitchControl _newEmails;

  zui::State<bool> _showActiveCenterPanel = true;
  zui::State<bool> _activityNotiForMeetingRecordingReady;
  zui::State<bool> _activityNotiForMeetingTransReady;
  zui::State<bool> _activityNotiForMeetingMissedCalls;

  zui::State<bool> _isShowIncomingCallopts;

  enum CCINotificationType
  {
    AlwaysPlay,
    MuteDuringMeetingCalls,
    AlwaysMute,
  };
  RadioGroupProperty _contacCenterNotiGroup;

  zui::State<bool> _teamChatEnbaled = true;

  //huddle
  SwitchControl _showNotificationForStarredActiveHuddle;
  SwitchControl _remindJoinLastHuddleWhenIdle;
  zui::State<int> _remindMinute;
  SwitchControl _PlayJoinSoundWhenJoinHuddle;
  zui::State<bool> _huddlesVisible = true;
};


class ZUIClient_API SettingPanelNotifications : public zui::View
{
public:

  SettingPanelNotifications() = default;
  ~SettingPanelNotifications() = default;

  std::shared_ptr<zui::View> Clone() const override { return std::make_shared<SettingPanelNotifications>(*this); }
  std::shared_ptr<zui::ViewNode> Build() override;

  zui::View GetSoundsView();

  zui::View GetNotificationsView();

  zui::View GetActivityCenterView();

  zui::View GetOtherView();

  zui::View GetHuddleView();

  void UpdateDoNotDisturb();

private:
  zui::Bind<bool> _showCustomRingtones = false;
  zui::Bind<bool> _is24TimerFormat = true;
  zui::State<std::list<std::string>> _doNotDistrubTime;
  zui::State<std::string> _bkColor_notification = std::string(zui::fillDefault);
};


#define NotiPanelDataModel CNotifSoundsSettingDataModel::GetInstance()

#endif
