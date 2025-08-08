#ifndef _SETTING_PANEL_STATISTICS_H_
#define _SETTING_PANEL_STATISTICS_H_

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

class ZUIClient_API CStatisticsDataModel
{
public:

  static CStatisticsDataModel& GetInstance();
    
  void ToggleTimer(bool toggleOn);
  void UpateConnectionP2PStatus();
  void UpdateNetWorkDataCenter();
  void Reset();

  void UpdateBandLimitInfo();

#ifdef BUILD_FOR_VDI
  void UpdateIsShowVDICpuMemory();
  void UpdateVDIBandLimitInfo();
#endif

  CStatisticsDataModel();
  ~CStatisticsDataModel() = default;
  // overall page
  zui::State<int> _index = 0;
  zui::State<float> _cpuZoom = 0;
  zui::State<float> _cpuOverall = 0;

  zui::State<std::string> _cpuZoomPer;
  zui::State<std::string> _cpuOverallPer;

  zui::State<std::string> _cpuConfig;
  zui::State<std::string> _memConfig;

  zui::State<float> _memoryZoom;
  zui::State<float> _memoryOverall;

  zui::State<std::string> _memoryZoomSize;
  zui::State<std::string> _memoryOverallSize;

  zui::State<std::string> _bandWidth;
  zui::State<std::string> _networkType;
  zui::State<std::string> _connectionType;
  zui::State<std::string> _dataCenter;
  zui::State<std::string> _encryptionAlgorithm;
  zui::State<std::string> _proxy;
  zui::State<std::string> _clientVersion;
  zui::State<bool>        _wifiStatsVisible = false;
  zui::State<std::string> _wifiStatsDesc;

  // vdi page
  zui::State<float> _vdiPluginCpuZoom = 0;
  zui::State<float> _vdiPluginCpuOverall = 0;

  zui::State<std::string> _vdiPluginCpuZoomPer;
  zui::State<std::string> _vdiPluginCpuOverallPer;

  zui::State<std::string> _vdiPluginCpuConfig;
  zui::State<std::string> _vdiPluginMemoryConfig;

  zui::State<float> _vdiPluginMemoryZoom;
  zui::State<float> _vdiPluginMemoryOverall;

  zui::State<std::string> _vdiPluginMemoryZoomSize;
  zui::State<std::string> _vdiPluginMemoryOverallSize;

  zui::State<std::string> _vdiThinClientOSVer;
  zui::State<std::string> _vdiPluginVer;
  zui::State<std::string> _vdiPluginStatus;
  zui::State<std::string> _vdiConnectionError;
  zui::State<std::string> _vdiPluginConnectionMode;
  zui::State<std::string> _vdiShareOffloadMode;
  zui::State<std::string> _vdiPluginProxy;
  zui::State<std::string> _vdiPingServerMode;
  zui::State<std::string> _vdiLogLevel;
  zui::State<std::string> _vdiPluginMgmtVer;
  zui::State<std::string> _vdiVMDeviceName;

  // audio page
  zui::State<std::string> _sendAudFrequency = std::string("--");
  zui::State<std::string> _sendAudLatency = std::string("--");
  zui::State<std::string> _sendAudJitter = std::string("--");
  zui::State<std::string> _sendAudPackLoss = std::string("--");
  zui::State<std::string> _recvAudFrequency = std::string("--");
  zui::State<std::string> _recvAudLatency = std::string("--");
  zui::State<std::string> _recvAudJitter = std::string("--");
  zui::State<std::string> _recvAudPackLoss = std::string("--");

  //video page
  zui::State<std::string> _sendVideoLatency = std::string("--");
  zui::State<std::string> _sendVideoJitter = std::string("--");
  zui::State<std::string> _sendVideoPackLoss = std::string("--");
  zui::State<std::string> _sendVideoResolution = std::string("--");
  zui::State<std::string> _sendVideoFramesPer = std::string("--");

  zui::State<std::string> _recvVideoLatency = std::string("--");
  zui::State<std::string> _recvVideoJitter = std::string("--");
  zui::State<std::string> _recvVideoPackLoss = std::string("--");
  zui::State<std::string> _recvVideoResolution = std::string("--");
  zui::State<std::string> _recvVideoFramesPer = std::string("--");

  //share page
  zui::State<std::string> _sendShareLatency = std::string("--");
  zui::State<std::string> _sendShareJitter = std::string("--");
  zui::State<std::string> _sendSharePackLoss = std::string("--");
  zui::State<std::string> _sendShareResolution = std::string("--");
  zui::State<std::string> _sendShareFramesPer = std::string("--");

  zui::State<std::string> _recvShareLatency = std::string("--");
  zui::State<std::string> _recvShareJitter = std::string("--");
  zui::State<std::string> _recvSharePackLoss = std::string("--");
  zui::State<std::string> _recvShareResolution = std::string("--");
  zui::State<std::string> _recvShareFramesPer = std::string("--");

  // phone page
  zui::State<bool> _isShowPhoneTab = false;
  zui::State<bool> _isShowPhoneVDIDetail = false;
  zui::State<bool> _isShowPhoneLocalSurvivability = false;
  zui::State<bool> _isShowPhoneNodeDetail = false;
  zui::State<bool> _isShowPhoneTxStatus = false;

  zui::State<std::string> _regID = std::string("--");
  zui::State<std::string> _regNetAddr = std::string("--");
  zui::State<std::string> _netSwitch = std::string("--");
  zui::State<std::string> _localNetIt = std::string("--");
  zui::State<std::string> _vdiConnectStatus = std::string("--"); // vdi status
  zui::State<std::string> _mediaMode = std::string("--"); // vdi status

  zui::State<std::string> _localSurvivability = std::string("--");
  zui::State<std::vector<std::string>> _IPAddressVector;
  zui::State<std::string> _IPAddress = std::string("--");

  zui::State<std::string> _peerNum = std::string("--");
  zui::State<std::string> _localNetAddr = std::string("--"); // conflict with vdi status
  zui::State<std::string> _vdiHostNetAddr = std::string("--"); // vdi status
  zui::State<std::string> _thinClientNetAddr = std::string("--"); // vdi status
  zui::State<std::string> _remoteNetAddr = std::string("--");
  zui::State<std::string> _networkDelay = std::string("--");

  zui::State<std::string> _sendPhonePacket = std::string("--");
  zui::State<std::string> _sendPhoneFrequency = std::string("--");
  zui::State<std::string> _sendPhonePacketLoss = std::string("--");
  zui::State<std::string> _sendPhoneJitter = std::string("--");
  zui::State<std::string> _sendPhoneBandwidth = std::string("--");
  zui::State<std::string> _sendPhoneCodec = std::string("--");

  zui::State<std::string> _recvPhonePacket = std::string("--");
  zui::State<std::string> _recvPhoneFrequency = std::string("--");
  zui::State<std::string> _recvPhonePacketLoss = std::string("--");
  zui::State<std::string> _recvPhoneJitter = std::string("--");
  zui::State<std::string> _recvPhoneBandwidth = std::string("--");
  zui::State<std::string> _recvPhoneCodec = std::string("--");

  struct PhoneNodeData {
    std::string _callerID;

    std::string _peerNum = std::string("--");
    std::string _localNetAddr = std::string("--"); // conflict with vdi status
    std::string _vdiHostNetAddr = std::string("--"); // vdi status
    std::string _thinClientNetAddr = std::string("--"); // vdi status
    std::string _networkDelay = std::string("--");

    std::string _sendPhonePacket = std::string("--");
    std::string _sendPhoneFrequency = std::string("--");
    std::string _sendPhonePacketLoss = std::string("--");
    std::string _sendPhoneJitter = std::string("--");
    std::string _sendPhoneBandwidth = std::string("--");
    std::string _sendPhoneCodec = std::string("--");

    std::string _recvPhonePacket = std::string("--");
    std::string _recvPhoneFrequency = std::string("--");
    std::string _recvPhonePacketLoss = std::string("--");
    std::string _recvPhoneJitter = std::string("--");
    std::string _recvPhoneBandwidth = std::string("--");
    std::string _recvPhoneCodec = std::string("--");

    bool operator==(const PhoneNodeData& other) const {
      return _peerNum == other._peerNum && _localNetAddr == other._localNetAddr && _vdiHostNetAddr == other._vdiHostNetAddr &&
             _thinClientNetAddr == other._thinClientNetAddr && _networkDelay == other._networkDelay &&
             _sendPhonePacket == other._sendPhonePacket && _sendPhoneFrequency == other._sendPhoneFrequency && _sendPhonePacketLoss == other._sendPhonePacketLoss &&
             _recvPhonePacket == other._recvPhonePacket && _recvPhoneFrequency == other._recvPhoneFrequency && _recvPhonePacketLoss == other._recvPhonePacketLoss &&
             _sendPhoneJitter == other._sendPhoneJitter && _sendPhoneBandwidth == other._sendPhoneBandwidth && _sendPhoneCodec == other._sendPhoneCodec &&
             _recvPhoneJitter == other._recvPhoneJitter && _recvPhoneBandwidth == other._recvPhoneBandwidth && _recvPhoneCodec == other._recvPhoneCodec;
    }
  };
  zui::State<std::vector<PhoneNodeData>> _phoneNodeData;

  zui::State<std::string> _bandWidthInfo;
  zui::State<bool>        _bandWidthInfoVisible = false;

  zui::State<bool> _reportPanelVisible = true;
  zui::State<bool> _reportProblemVisible = true;
  zui::State<bool> _runTestMeetingVisible = true;

  // vdi page
  zui::State<std::string> _vdiBandWidthInfo;
  zui::State<bool> _vdiBandWidthInfoVisible = false;
  zui::State<bool> _vdiCpuMemoryVisible = true;
};

class ZUIClient_API SettingPanelStatistics : public zui::View
{
public:
  SettingPanelStatistics();
    ~SettingPanelStatistics() = default;


    std::shared_ptr<zui::View> Clone() const override { return std::make_shared<SettingPanelStatistics>(*this); }
    std::shared_ptr<zui::ViewNode> Build() override;
};

#define StatPanelDataModel CStatisticsDataModel::GetInstance()

#endif //_SETTING_PANEL_MEETING_H_
