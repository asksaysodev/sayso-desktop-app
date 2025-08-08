#ifndef _SETTING_PANEL_RECORDINGS_H_
#define _SETTING_PANEL_RECORDINGS_H_

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
#include "zui_client_data_define.h"
#include "webService/SBWebServiceTypes.h"

class ZUIClient_API CSettingRecordingsDataModel : public IInitializable
{
public:

  static CSettingRecordingsDataModel& GetInstance();
  CSettingRecordingsDataModel();
  ~CSettingRecordingsDataModel() = default;

  virtual void initializeState() override;

  virtual void initializeStateHandler() override;

public:
  void InitRecordingsDataModel();
  void UpdateLocalStorage(const CmmUInt64 freesapce);
  void UpdateLocalLocalStoragePath(const Cmm::CString& path);
  void UpdateCloudStorage(const SB_webservice::CMRStorage& storage);
  void UpdateKeepUnconvertedFiles(const bool val); //only mac,init value
  zui::State<std::string> _localStoragePath;

public:
  
  zui::State<std::string> _loclStorageDesc;
  zui::State<bool> _isShowWarning = false;
  zui::State<bool> _isShowChangePathBtn = true;

  zui::State<bool> _selectStoragePathEachMeeting = false;

  zui::State<bool> _showCloudStorage = false;
  zui::State<bool> _showCloudStorageUsed = false;
  zui::State<float> _cloudStorageUsed = 0.0f;
  zui::State<std::string> _cloudStorageDesc;
  
  zui::State<bool> _showParticipantsName;
  zui::State<bool> _includeTimestamp;
  zui::State<bool> _includeVideoInScreenSharing;
  zui::State<zui::Checkbox::Status> _showVideoAndShareSideBySide;
  zui::State<bool> _showAdvanced = false;

  zui::State<bool> _recordSeparateAudioFiles;
  RadioBtnProperty _keepUnconvertedFiles;
  zui::State<bool> _optimizeThirdPartyVideoEditors;
  zui::State<bool> _isVDIShowLocalRecording = true;
  zui::State<bool> _isVDIShowRecordingPreference = true;
  zui::State<bool> _showAdvancedButton = true;
};

class ZUIClient_API SettingPanelRecordings : public zui::View
{
public:

  SettingPanelRecordings();
  ~SettingPanelRecordings() = default;

  std::shared_ptr<zui::View> Clone() const override { return std::make_shared<SettingPanelRecordings>(*this); }
  std::shared_ptr<zui::ViewNode> Build() override;
};

#define RecordPanelDataModel CSettingRecordingsDataModel::GetInstance()
#endif
