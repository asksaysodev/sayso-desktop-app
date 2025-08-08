#pragma once
#include "zui_client_data_define.h"
#include "zVideoApp/Confdef.h"

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

class ZUIClient_API IFarEndAudioControlSink {
public:
  virtual void OnDeviceUpdateInit(CMM_USERID user_id) = 0;
  virtual void OnGiveupAudioControl(CMM_USERID user_id) = 0;
};

class ZUIClient_API FarEndAudioControlViewModel : public IInitializable {
private:
  FarEndAudioControlViewModel();
  ~FarEndAudioControlViewModel() = default;

public:
  FarEndAudioControlViewModel(const FarEndAudioControlViewModel&) = delete;
  FarEndAudioControlViewModel& operator=(const FarEndAudioControlViewModel&) = delete;

  static FarEndAudioControlViewModel& Get();

  virtual void initializeState() override {}

  virtual void initializeStateHandler() override;

  void UpdateMicButton(bool newValue);

  void OnRemoteAudioDeviceUpdate(const RemoteAudioDeviceInfo& deviceInfo,
    DeviceUpdateType updateType, int data_from);

  void SetSink(IFarEndAudioControlSink* sink) { _sink = sink; }
  void GiveupAudioControl();
  CMM_USERID GetUserID() const{ return user_id_; }
  void UpdateAccNameByUserID(CMM_USERID id);

private:
  void OnDeviceUpdateInit(const RemoteAudioDeviceInfo& deviceInfo);
  void OnMicMuteStateChange(const RemoteAudioDeviceInfo& deviceInfo);
  void OnMicVolumeChange(const RemoteAudioDeviceInfo& deviceInfo);
  void OnSpkVolumeChange(const RemoteAudioDeviceInfo& deviceInfo);
  void OnCurMicSwitch(const RemoteAudioDeviceInfo& deviceInfo);
  void OnCurSpkSwitch(const RemoteAudioDeviceInfo& deviceInfo);
  void OnSpkListChange(const RemoteAudioDeviceInfo& deviceInfo);
  void OnMicListChange(const RemoteAudioDeviceInfo& deviceInfo);

public:
  DynDropdownProperty _microphoneDropdown = 0;
  AudioVolumeProperty _microphoneVolume = 0;
  DynDropdownProperty _speakerDropdown = 0;
  AudioVolumeProperty _speakerVolume = 0;

  CMM_USERID user_id_ = 0;
  std::string _accMicrophoneDropdown;
  std::string _accMicrophoneSlider;
  std::string _accSpeakerDropdown;
  std::string _accSpeakerSlider;

  CString _selectMicId;
  CString _selectSpkId;

  zui::State<bool> _isMicMuted = false;
  zui::State<std::string> _micButtonName;
  zui::State<std::string> _iconMicButton;

  std::string _lbMicMuteButtonName;
  std::string _lbMicUnMuteButtonName;
  std::string _leftIconMicOn;
  std::string _leftIconMicOff;

  IFarEndAudioControlSink* _sink = nullptr;
};

class ZUIClient_API FarEndAudioControlView : public zui::View
{
public:
  FarEndAudioControlView() = default;
  ~FarEndAudioControlView() = default;

  virtual std::shared_ptr<zui::View> Clone() const override { return std::make_shared<FarEndAudioControlView>(*this); }
  virtual std::shared_ptr<zui::ViewNode> Build() override;
};
