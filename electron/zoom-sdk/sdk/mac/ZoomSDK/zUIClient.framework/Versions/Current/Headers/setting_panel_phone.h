#ifndef _SETTING_PANEL_PHONE_H_
#define _SETTING_PANEL_PHONE_H_

#include "zSipCallCommon/pbx_call_handling_settings_api.h"
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

namespace zui
{
  enum DialogType {
    kBusinessHour,
    kCheckBusinessHour,
    kAddVoicemail,
    kIVRMenu,
  };
}

void ZUIClient_API InitPhoneSettingDataModel();

class ZUIClient_API CPhoneSettingDataModel 
  : public IInitializable,
    public NS_SIPCALL_APP::ICallHandlingSettingsAPI::CallHandlingSettingsListener
{
public:
  
  static CPhoneSettingDataModel& GetInstance();
  CPhoneSettingDataModel();
  ~CPhoneSettingDataModel();

  virtual void initializeState() override;
  virtual void initializeStateHandler() override;

  // call handling
  void RequestAllCallHandlingSetting();

  // CallHandlingSettingsListener
  void OnRequestBusinessHourExDone(const NS_SIPCALL_APP::CmmBusinessHourEx& result) override;
  void OnGenerateBusinessHourExDone(const NS_SIPCALL_APP::CmmBusinessHourEx& result) override;
  void OnRequestCheckBusinessHourDone(bool has_conflict) override;
  void OnUpdateBusinessHourExDone(const NS_SIPCALL_APP::CmmBusinessHourEx& result) override;
  void OnRequestRoutingExDone(const std::vector<NS_SIPCALL_APP::CmmCallHandlingRoutingEx>& result) override;
  void OnRequestAvailableLibrariesDone(const NS_SIPCALL_APP::CmmCallHandlingVoiceInfo& libraries) override;
  // dialog
  void AttachDialogView(zui::DialogType type, std::shared_ptr<zui::ZUIWindow> window);
  void NotifyPopupDialog(zui::DialogType type, bool show);
  std::shared_ptr<zui::ZUIWindow> PopupDialog(zui::DialogType type, bool show);
  void CloseDialog(zui::DialogType type);
  // business hour
  void RegisterCallHandingListenter();
  void RequestCheckBusinessHour();
  void RequestUpdateBusinessHours();
  void UpdateBusinessHoursText();
  // route option
  void UpdateBusyData(const NS_SIPCALL_APP::CmmCallHandlingRoutingEx& routingData);
  void UpdateNotAnsweredData(const NS_SIPCALL_APP::CmmCallHandlingRoutingEx& routingData);
  void UpdateRoutingSetting(const std::vector<NS_SIPCALL_APP::CmmCallHandlingRoutingSettingEx>& setting);

  // helper
  std::string GetNextDay(const std::string& today);

private:
  // business hour
  void UnRegisterCallHandingListenter();
  void InitTimePoints();
  void InitCustomHours();
  std::string GetTimeCustomHoursList(int index);
  int GetCustomHoursListIndex(const std::string& hours);
  void GenerateBusinessHourEx();

  // routing
  void UpdatEnableVoicemailGreeting();
  void HandleRoutingOption(const NS_SIPCALL_APP::CmmCallHandlingRoutingOption& option);
  void RequestVoicemailGreetingList();

public:
  zui::State<bool> _isPbxVisble = false;;

  // my account online link,just control it's visible.
  BtnProperty _settingLink;

  zui::State<bool> _isDirectNumberVisble;
  zui::State<std::vector<zui::any>> _directNumbers;

  // extension: or company number:.
  zui::State<bool> _hasMainCompanyNumber;  //if has not mian company number will show extension:
  
  zui::State<bool> _isExtNumberVisble;
  zui::State<std::string> _displayExtNumber;  // if has company number , will show company number#ext.

  // Local dialing
  zui::State<bool> _isDialingNumberVisble;
  zui::State<std::string> _localDialingNumber;  // conntry code , country_name

  zui::State<bool> _isAreaCodeVisble;
  zui::State<std::string> _areaCode;

  // Text : Dial local calls without dialing country and area code first. 
  zui::State<bool> _dialingTipVisible;

  // address status: may be Detected, Default.
  enum class AddressStatus {
    kNull,
    kDefault,
    kDetected,
  };
  struct EmergAddrStatusProperty : public ControlBasePropertyWithVisible<std::string> {
    zui::State<AddressStatus> _status = AddressStatus::kNull;

    EmergAddrStatusProperty() {}
    EmergAddrStatusProperty(AddressStatus status) {
      this->operator=(status);
    }

    EmergAddrStatusProperty& operator=(AddressStatus status) {
      _status = status;
      _visible = AddressStatus::kNull != status;
      if (AddressStatus::kDefault == status)
        _value = "DEFAULT";
      else if (AddressStatus::kDetected == status)
        _value = "DETECTED";
      return *this;
    }
  };
  EmergAddrStatusProperty _emergAddrStatus = AddressStatus::kNull;

  // address info with text and visible control
  EmergAddrProperty _emergAddr = std::string("111 S Grand Ave, \nLos Angeles, California 90012, \nUnited States");
  zui::State<std::string> _emergAddrTip;

  // address loading control
  zui::State<bool> _emergAddrLoading = true;

  // Address action: Update / Confirm / Personal Location or Addresses
  BtnProperty _updateAddress;    // btn,
  BtnProperty _confirmAddress;  // btn 
  zui::State<bool> _updateAndConfirmHelper;
  BtnProperty _manageAddress;  // Personal Location or Addresses , is a link. 
  BtnProperty _addAddress;  // btn 
  BtnProperty _textCoordinates;  // btn 
  EmergAddrProperty _coordinates;  // just show gray text, may be use text control

  // checkbox  allow location permission
  SwitchControl _locationPermission;
  zui::State<std::string> _locationPermissionText;  // location description
  zui::State<std::string> _locationPermissionTip;  // tip text,
  zui::State<bool> _locationHelperVisible = false;  // MAC only

  zui::State<std::string> _locationHelperDesc;
  zui::State<std::string> _locationHelperDL;
  zui::State<bool> _locationHelperDLVisible = false;
  zui::State<bool> _locationHelperDLLoadingVisible = false;
  StringVisibleProperty _locationHelperHelpButton;
  zui::State<bool> _callingPreferenceVisible = false; // MAC only
  /*
  @property (nonatomic, copy) NSString *appName;
@property (nonatomic, copy) NSString *bundleID;
@property (nonatomic, retain) NSImage *icon;
@property (nonatomic, assign) BOOL isDefult;
  */
  // MAC only
  struct CallPreferenceInfo{
    std::string app_name;
    std::string boudle_id;
    std::string icon;
    bool is_default;
    bool operator==(const CallPreferenceInfo& other) const {
      return app_name == other.app_name
        && boudle_id == other.boudle_id
        && icon == other.icon
        && is_default == other.is_default
        ;
    }
  };
  zui::State<std::vector<zui::any>> _callingPreferences;

  // Transcript lange setting.
  zui::State<bool> _transcriptLangeVisible = true;
  zui::State<int> _transcriptLangeListSelectIndex;
  zui::State<std::list<std::string>> _transcriptLanges;

  // auto-answer call queue calls.
  zui::State<int> _autoAnswerCQPeriodSelectedIndex;
  zui::State<bool> _autoAnswerCQVisible;

  // Call Control
  BtnProperty _callControl;

  // Launch app or Url
  SwitchControl _launchAppForIncomingCalls;

  zui::State<bool> _editLaunchAppForIncomingCallsEnable;  // edit url  enable ?
  zui::State<std::string> _editLaunchAppForIncomingCalls;  // edit url content
  BtnProperty _browseBtn = { true,true};   // browse select file / exe.

  // hide zra = hideOutgoingCallsViaDialerFromHistory
  SwitchControl _hideZRACalls;

  // call me on panel 
  SwitchControl _callOut;

  //zui::State<bool> _callOutNumberVisible;
  zui::State<bool> _callOutNumberEnable;
  struct CallOutInfo {
    std::string label;
    std::string number;
    std::string id;  // Passed back to the UI layer
    bool operator==(const CallOutInfo& other) const{
      return label == other.label &&
        number == other.number
        && id == other.id;
    }
  };
  zui::State<std::vector<zui::any>> _callOutNumber;  // list show all call out number,  under number there are to button (edit / delete).

  zui::State<bool> _videoGreetingVisible = true;
  // greeting actions.  Record / Review / Delete
  SwitchControl _videoGreetingToggle;
  BtnProperty _videoGreetingRecord;
  BtnProperty _videoGreetingReview;
  BtnProperty _videoGreetingDelete;

  // voicemail prioritization with AI companion, voicemail topic
  zui::State<bool> _hasVoicemailPriorityTopics = true;
  SwitchControl _voicemailPrioritization;    // toggle state.

  // all in notification page.
  //// hide incoming calls while in a zoom meeting.
  //zui::State<bool> _hideIncomingCallInMeetingEnable;
  //zui::State<bool> _hideIncomingCallInMeeting;
  // live transcript setting.
  SwitchControl _autoStartLiveTranscript;
  // test function show webrtc.
  BtnProperty _showWebRTCWebview;

  // business hours
  std::list<std::string> _timePoints;
  std::vector<std::string> _weekdayNames;
  NS_SIPCALL_APP::CmmBusinessHourEx _businessHourData; // store https data
  zui::State<std::vector<zui::any>> _businessHoursText;
  zui::State<zui::any> _businessHoursGroupValue = zui::any(1); // BusinessHourMode
  struct CustomHours
  {
    std::string title;
    zui::State<zui::Checkbox::Status> status = zui::Checkbox::Status::kUnselected;
    zui::State<std::string> openHour = std::string("10:30 AM");
    zui::State<std::string> closeHour = std::string("7:30 PM");

    bool operator==(const CustomHours& other) const
    {
      return title == other.title && status == other.status && 
        openHour == other.openHour && closeHour == other.closeHour;
    }
  };
  zui::State<std::vector<zui::any>> _customHours;

  // voicemail
  zui::State<bool> _enableVoicemailGreeting = false;
  std::map<zui::DialogType, std::shared_ptr<zui::ZUIWindow>> _dialogMap;

  zui::State<int> _busyOnCallBusinessHoursIndex = 0;
  zui::State<int> _unansweredOnCallBusinessHoursIndex = 0;

  zui::State<int> _busyOnCallClosedHoursIndex = 0;
  zui::State<int> _unansweredOnCallClosedHoursIndex = 0;

  zui::State<CString> _voicemailGreetingValue;
  zui::State<std::vector<NS_SIPCALL_APP::CmmCallHandlingVoice>> _voicemailGreetingList;

  zui::State<std::string> _forwardExtension;
  BtnProperty _saveForwardExtension;

  zui::State<zui::Checkbox::Status> _allowToReachAnOperator;
  zui::State<zui::Checkbox::Status> _allowToCheckVoicemail;
  zui::State<zui::Checkbox::Status> _playCalleeVmGreeting;
};

class ZUIClient_API CSIPPhoneSettingDataModel : public IInitializable
{
public:

  static CSIPPhoneSettingDataModel& GetInstance();
  CSIPPhoneSettingDataModel();
  ~CSIPPhoneSettingDataModel() = default;

  virtual void initializeState() override;

  virtual void initializeStateHandler() override;

public:
  zui::State<bool> _isSipVisble = false;;
  zui::State<bool> _sipErrorMsgVisible;
  zui::State<std::string> _sipErrorMsg;
 
  zui::State<std::string> _sipDomain;
  zui::State<std::string> _sipRegisterServer;
  zui::State<std::string> _sipTransportProtocol;
  zui::State<std::string> _sipProxyServer;
  zui::State<std::string> _sipRegistrationExpiry;
  zui::State<std::string> _sipLastRegistration;
  zui::State<std::string> _sipUserName;
  zui::State<std::string> _sipAuthorizationName;
  zui::State<std::string> _sipUserIdentity;
  zui::State<std::string> _sipVoicemail;
  SwitchControl           _locationPermission;
};

class ZUIClient_API NewSettingPanelPhone : public zui::View
{
public:
  NewSettingPanelPhone();
  ~NewSettingPanelPhone() = default;

  std::shared_ptr<zui::View> Clone() const override { return std::make_shared<NewSettingPanelPhone>(*this); }
  std::shared_ptr<zui::ViewNode> Build() override;

private:
  void UpdateEmergencyAddresssLabel();
  void UpdateShowDirectNumberList();

  void OnClickZoomTechnicalSupport();

private:
    zui::State<bool> _showDirectNumberList = true;

    zui::State<std::string> _locationHelperVersion = std::string("Location helper (version 5.16.0.134) installed");
    
    zui::State<std::string> _emergencyAddressRoad = std::string("888 Monument Village Cir");
    zui::State<std::string> _emergencyAddressCityState = std::string("Grand Junction, Colorado 81507");
    zui::State<std::string> _emergencyAddressCountry = std::string("United States");
  
    zui::State<bool> _launchExternalApp = false;
    zui::State<bool> _recordVideoGreeting = false;
    zui::State<bool> _manageVoiceMail = false;

    zui::Bind<std::vector<zui::any>> _directNumbers;
};

class ZUIClient_API SettingPanelPhoneCallControl : public zui::View
{
public:
    SettingPanelPhoneCallControl() = default;
  ~SettingPanelPhoneCallControl() = default;

  std::shared_ptr<zui::View> Clone() const override { return std::make_shared<SettingPanelPhoneCallControl>(*this); }
  std::shared_ptr<zui::ViewNode> Build() override;

};

#define PhoneDataModel CPhoneSettingDataModel::GetInstance()
#define SIPPhoneDataModel CSIPPhoneSettingDataModel::GetInstance()

#endif
