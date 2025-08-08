#ifndef _SETTING_PANEL_AUDIO_H_
#define _SETTING_PANEL_AUDIO_H_

#include "ssb_media_client_def.h"
#include "cmmlib/CmmTimer.h"
#include "zVideoApp/CmmConfAPI.h"

#include "zui_client_data_define.h"

#ifdef _WIN32
#include "zUI/view.h"
#include "zUIClient/zUI_stdafx.h"
#elif defined __MACOS__
#import <zUI/view.h>
#import <zUI/checkbox.h>
#import <zUIClient/zUI_stdafx.h>
#else
#include "view.h"
#include "zUI_stdafx.h"
#endif

#ifdef BUILD_FOR_VDI
#include "vdi/vdi_interface.h"
#endif

#define MIC_MENU        1
#define SPK_MENU        2
#define RING_MENU       3

#define RECORD_USER_VOICE_MAX_TIME  300 //30S

typedef enum
{
  enuNoneNew = 1,
  enuRecordInfoNew,
  enuPlayInfoNew,
  enuSpkInfoNew,
  enuMicerrNew,
  enuRingInfoNew,
  enuSpkerrNew,
  enuRingerrNew
} NEW_TEST_AUDIO_INFO;

class ZUIClient_API CAudioPanelDataModel
  : public Cmm::CTimerProc
  , public ICmmDeviceUIAPI
  , public IInitializable
{
public:
  static CAudioPanelDataModel& GetInstance();


  //virtual ICmmConfUIAPI* GetZmInterface(ICmmConfUIAPI*);


  virtual void initializeState() override;

  virtual void initializeStateHandler() override;

  void OnSelect();
  void OnUnSelect();

  void InitAudioSettingInfo();
  void InitAudioDevice();
  void InitSpks();
  void InitMics();
  void InitRingers();
  void UpdateVolumeUI();
  void UpdateSpatialAudioControl();
  void UpdateRingVolumeUI();
  void RefreshAudioSettingUI();
  void InitAudioAdvancedSettings();
  void RefreshAudioProfileUI();
  void RefreshOptimizedAudioSubOption();
  void UpdateManualRecordVoicePrintBtns();
  void OnPkgDownloadSuccess();
  void RefreshAudioInputChannels();
  void RefreshSelChannelNum();
  void OnManulRecordedWndClose();
  void UpdateWindowsSystemAudioEnhancement();
  void SetIsInMeeting(CmmBool is_in_meeting);

  CmmBool OnAudioDeviceStatusChanged(DEVICE_CMD cmd, CmmUInt32 ret, CmmUInt32 handle);

  //ICmmDeviceUIAPI
  virtual CmmBool OnDeviceStatusChanged(DEVICE_CMD cmd, CmmUInt32 ret = 0, CmmUInt32 handle = 0) override;
  virtual CmmBool ShowMyAudioLevel(CmmUInt32 level, CmmUInt32 handle, CmmBool isMic = CmmTrue) override;
  virtual CmmBool OnVolumeChange(CmmUInt32 vol, CmmBool isMic) override;

#ifdef BUILD_FOR_VDI
  ns_vdi::IVdiPluginCapacity* GetVDIPluginCapacity();
  CmmBool IsVDISupportSetAudioCapturePlaybackAPI();
#endif

private:
   bool GetChannelsNumAndState(CmmUInt32& channel_num, std::bitset<32>& channel_states, CmmUInt32* sel_channel_num = nullptr);

public:
  zui::State<bool> _showAdvanced = false;
  zui::State<bool> _isPAIEnable = false;
  zui::State<bool> _isPTLogin = false;
  zui::State<bool> _isPAIOPValue = false;
  zui::State<bool> _isVDISupportPlaybackAPI = true;
  zui::State<bool> _isVDIShowAudioProfile = true;
  zui::State<bool> _isVDIEnableNoiseSuppression = true;
  zui::State<bool> _isVDIShowSpeakerMicVolumeSlider = true;
  zui::State<bool> _isVDIShowAutoAdjustMicVolume = true;
  zui::State<bool> _isVDIShowSpecificAudioChannels = true;
  zui::State<bool> _isVDIShowRingtone = true;

  // Speaker
  ToggleProperty _spatialAudio = false;
  ToggleProperty _ringtonesFromAdditionalDevice = false;
  DynDropdownProperty _speakerDropdown = 0;
  zui::State<float> _speakerLevel = 0; // float 0 - 1
  AudioVolumeProperty _speakerVolume = 0; // float 0 - 1
  BtnClickedProperty _speakerTesting = false;
  DynDropdownProperty _ringerDropdown = 0;
  AudioVolumeProperty _ringerVolume = 0; // float 0 - 1

  // Microphone
  ToggleProperty _microphoneVolumeAutoAdjust = { false, false };
  DynDropdownProperty _microphoneDropdown = 0;
  zui::State<float> _microphoneLevel = 0; // float 0 - 1
  AudioVolumeProperty _microphoneVolume = 0; // float 0 - 1
  zui::State<int> _microphoneRecordingTime = -1;
  enum MicTestStatus {
    kReady,
    kRecording,
    kPlaying,
  };
  BtnAnyClickedProperty _microphoneTesting = (zui::any)(int)MicTestStatus::kReady;
  SpecificAudioChannelsProperty _specificAudioChannels = 0;

  // Audio profile
  RadioButtonGroupProperty _audioProfile = (zui::any)(int)(CMM_AUDIO_PROFILE::kAudioProfileBackGroundNoise);

  // background noise removal
  DropDownBaseProperty _bknoiseRemovalLevel = 0;

  // original sound for musicians
  CheckBoxBoolProperty _highFidelityMusicMode = false;
  CheckBoxBoolProperty _echoCancellation = true;
  CheckBoxBoolProperty _stereoAudio = false;

  // personalized audio isolation
  enum PAIStatus {
    kNull,
    kCreateVoiceprints,
    kDeleteAutomaticVoiceprints,
    kDeleteAllVoiceprints,
    kUpdateVoiceprints,
  };
  zui::State<zui::any> _PAIStatus = (zui::any)(int)kNull;
  PAIStatusProperty _PAIAutomaticRecording = false;
  PAIStatusProperty _PAIManualRecording = false;

  // Advanced
  DropDownBaseProperty _echoCancellationAdv = 0;
#ifdef _WIN32
  DropDownBaseProperty _windowsSystemAudioEnhancementAdv = 0;
  DropDownBaseProperty _signalProcessingByWindowsAudioDeviceAdv = 0;
  DropDownBaseProperty _audioCaptureAndPlaybackAPI = 0;
#endif

  SwitchControl _syncHeadsetButtons;
  CheckBoxProperty _syncAirPodsButtons;
    
  void StopMic();
  void StopSpk();
  void TestMic(CmmBool bDummy);
    
private:
  void SwitchAudioDevice(const CString& deviceid, int menu_type);
  bool OnSelectMic(const CString& deviceid);
  bool OnSelectSpeaker(const CString& deviceid);
  bool OnSelectRinger(const CString& deviceid);

  void OnAdjustMicVolume(float volume);
  void OnAdjustSpkVolume(float volume);
  void OnAdjustRingVolume(float volume);

  void OnTestMicClicked();

  void SetTestMicStatus(CAudioPanelDataModel::MicTestStatus status);
  CmmBool PlayRecordedMic();
  void ResetTestMicStatus();
  void OnTestSpkClicked();
  void TestSpk();


  void OnSpkLevelChanged(CmmUInt32 level, CmmUInt32 handle);
  void OnMicLevelChanged(CmmUInt32 level, CmmUInt32 handle);
  void OnAudioChanged(CmmUInt32 ret, CmmUInt32 handle);

  void OnMicStatusChanged(CmmUInt32 ret);
  void OnSpkStatusChanged(CmmUInt32 ret);
  void OnRingStatusChanged(CmmUInt32 ret);

private:
  //CTimerProc
  virtual void OnTimer(void*, CmmUInt, CmmHTimer idEvent, CmmULong dwTime) override;

private:
  CString _selectSpkId;
  SSB_MC_AUDIO_DEVICE_API_TYPE selectSpkType = SSB_MC_AUDIO_DEVICE_API_TYPE_SYSTEM_DEFAULT;

  CString _selectMicId;
  SSB_MC_AUDIO_DEVICE_API_TYPE selectMicType = SSB_MC_AUDIO_DEVICE_API_TYPE_SYSTEM_DEFAULT;

  // Show Info
  NEW_TEST_AUDIO_INFO          m_last_SpkInfo_ = enuNoneNew;
  NEW_TEST_AUDIO_INFO          m_last_MicInfo_ = enuNoneNew;
  NEW_TEST_AUDIO_INFO          last_ring_info_ = enuNoneNew;

  BOOL                     m_bStartTestMic = FALSE;
  BOOL                     m_bMaunalTestMic = FALSE;
  BOOL                     m_bStartTestSpk = FALSE;
  BOOL                     need_replay_spk_handle_now_ = FALSE;
  BOOL                     need_replay_ring_handle_now_ = FALSE;
  CmmUInt32                m_SpkLevel = 0;
  CmmUInt32                test_spk_handle_ = NULL_AUDIO_FILE_HANDLE;
  CmmUInt32		          m_TestMicHandle = NULL_AUDIO_FILE_HANDLE;

  CmmUInt32             ring_level_ = 0;
  CmmUInt32             test_ring_handle_ = NULL_AUDIO_FILE_HANDLE;

  CmmUInt32		          m_MicLevel = 0;

  CmmUInt32		          m_MaunalTestTime = 0;
  CmmBool              is_manual_record_wnd_show_ = CmmFalse;
  CmmBool              is_in_meeting_ = CmmFalse;
  


  Cmm::CTimerID audio_setting_timer_;
  Cmm::CTimerID audio_setting_ring_timer_;

protected:
  CAudioPanelDataModel() = default;
  ~CAudioPanelDataModel() = default;
};

class ZUIClient_API SettingPanelAudio : public zui::View
{
public:

  SettingPanelAudio();
  ~SettingPanelAudio() = default;

  std::shared_ptr<zui::View> Clone() const override { return std::make_shared<SettingPanelAudio>(*this); }
  std::shared_ptr<zui::ViewNode> Build() override;

};

#define AudioPanelDataModel CAudioPanelDataModel::GetInstance()

#endif
