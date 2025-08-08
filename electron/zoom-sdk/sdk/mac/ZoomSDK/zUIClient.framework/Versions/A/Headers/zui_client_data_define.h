#pragma once
#include <unordered_map>
#include "zUI/zui_base.h"
#include "zUI/environment.h"

class IInitializable {
public:

  void Init() {
    initializeState();
    if (!_isInitStateHandler) {
      initializeStateHandler();
      _isInitStateHandler = true;
    }
  }

  virtual void initializeState() = 0;

  virtual void initializeStateHandler() = 0;

private:
  bool _isInitStateHandler = false;
};

template <typename T>
class tagControlBaseProperty
{
public:
  zui::State<bool> _enable = true;
  zui::State<T> _value;

public:
  tagControlBaseProperty() {}
  tagControlBaseProperty(const T& value) : _value(value) {}
  tagControlBaseProperty(const T& value, bool enable) : _value(value), _enable(enable) {}

  void SetValue(const T& value) { _value = value; }
  void SetValueWithoutNotify(const T& value) { _value.SetWithoutNotify(value); }
  void SetEnable(bool enable) { _enable = enable; }
  void SetEnableWithoutNotify(bool enable) { _enable.SetWithoutNotify(enable); }

  void SetValueUpdate(const std::function<void(const T&)>& func) { _value.SetUpdate(func); }
  void SetEnableUpdate(const std::function<void(const bool&)>& func) { _enable.SetUpdate(func); }

  T const GetValue() const { return _value; }
  zui::Bind<T> GetValueBind() const { return zui::Bind<T>(_value); }
  const zui::State<T>& GetValueState() const { return _value; }

  bool GetEnable() const { return _enable; }
  zui::Bind<bool> GetEnableBind() const { return zui::Bind<T>(_enable); }
  const zui::State<T>& GetEnableState() const { return _enable; }

  tagControlBaseProperty& operator=(const T& value)
  {
    SetValue(value);
    return *this;
  }
};

template <typename T>
using ControlBaseProperty = tagControlBaseProperty<T>;

template <typename T>
class tagControlBasePropertyWithVisible : public tagControlBaseProperty<T>
{
public:
  zui::State<bool> _visible = true;

public:
  tagControlBasePropertyWithVisible() : tagControlBaseProperty<T>() {}
  tagControlBasePropertyWithVisible(const T& value) : tagControlBaseProperty<T>(value) {}
  tagControlBasePropertyWithVisible(const T& value, bool visible)
    : tagControlBaseProperty<T>(value), _visible(visible) {}
  tagControlBasePropertyWithVisible(T value, bool visible, bool enable)
    : tagControlBaseProperty<T>(value, enable), _visible(visible) {}

  void SetVisible(bool visible) { _visible = visible; }
  void SetVisibleWithoutNotify(bool visible) { _visible.SetWithoutNotify(visible); }

  void SetVisibleUpdate(const std::function<void(const bool&)>& func) { _visible.SetUpdate(func); }

  bool GetVisible() const { return _visible; }
  zui::Bind<bool> GetVisibleBind() const { return zui::Bind<bool>(_visible); }
  const zui::State<bool>& GetVisibleState() const { return _visible; }
};

template <typename T>
using ControlBasePropertyWithVisible = tagControlBasePropertyWithVisible<T>;

typedef struct tagBtnProperty
{
  zui::State<bool> _visible = true;
  zui::State<bool> _enable = true;
  zui::State<bool> _activityIndicator = false;
} BtnProperty;

typedef struct tagRadioBtnProperty
{
  zui::State<bool> _enable = true;
  zui::State<bool> _value = true;
} RadioBtnProperty;

typedef struct tagRadioBtnProperty1
{
  zui::State<bool> _visible = true;
  zui::State<bool> _enable = true;
  zui::State<bool> _value = false;
} RadioBtnProperty1;

typedef struct tagRadioBtnProperty2
{
  zui::State<bool> _visible = true;
  zui::State<bool> _enable = true;
  zui::State<zui::any> _value;
  zui::State<std::string> _title;

  bool operator==(const tagRadioBtnProperty2& other) const {
    return _visible == other._visible && 
      _enable == other._enable &&
      _value.Get() == other._value.Get() &&
      _title.Get() == other._title.Get();
  };
} RadioBtnProperty2;


typedef struct tagOptionsProperty
{
  zui::State<bool> _visible = true;
  zui::State<bool> _enable = true;
  zui::State<int> _value = false;
} OptionsProperty;

typedef struct tagRadioGroupProperty
{
  zui::State<bool> _enable = true;
  zui::State<bool> _visible = true;
  zui::State<zui::any> _value;
} RadioGroupProperty;

typedef struct tagCheckBoxProperty
{
  zui::State<bool> _enable = true;
  zui::State<zui::Checkbox::Status> _value = zui::Checkbox::Status::kUnselected;
} CheckBoxProperty;

typedef struct tagDropDownProperty
{
  zui::State<bool> _enable = true;
  zui::State<bool> _visible = true;
  zui::State<int> _value = 0;
} DropDownProperty;

/*
 * basic dropdown with dynamic options and user data
 * std::list<std::string>
 */
typedef struct tagDynDropdownProperty
  : public ControlBasePropertyWithVisible<int>
{
  zui::State<std::list<std::string>> _options;
  zui::State<std::list<std::string>> _userData;

  tagDynDropdownProperty() {}
  tagDynDropdownProperty(int value)
    : ControlBasePropertyWithVisible<int>(value) {}

  tagDynDropdownProperty& operator=(int value) {
    ControlBasePropertyWithVisible<int>::operator=(value);
    return *this;
  }
} DynDropdownProperty;

/*
 * std::string data property with override visible control
 */
typedef struct tagStringVisibleProperty
  : public ControlBasePropertyWithVisible<std::string>
{
  tagStringVisibleProperty() {}
  tagStringVisibleProperty(std::string addr)
    : ControlBasePropertyWithVisible<std::string>(addr) {}

  tagStringVisibleProperty& operator=(const std::string addr) {
    _value = addr;
    _visible = !addr.empty();
    return *this;
  }
} StringVisibleProperty;

/*
* emergency address data property
*/
using EmergAddrProperty = StringVisibleProperty;

/*
 * float control value, for example, output level/volume
 * value 0 ~ 1
 */
using AudioVolumeProperty = ControlBaseProperty<float>;

/*
 * bool control value for button controls
 * toggle, checkbox without group
 */
using ToggleProperty = ControlBaseProperty<bool>;
using CheckBoxBoolProperty = ControlBaseProperty<bool>;

/*
 * bool status value for button clicked or not
 */
using BtnClickedProperty = ControlBaseProperty<bool>;
using PAIStatusProperty = ControlBaseProperty<bool>;

/*
 * any status value for button clicked or not
 */
using BtnAnyClickedProperty = ControlBaseProperty<zui::any>;
/*
 * any status value for radio button group
 */
using RadioButtonGroupProperty = ControlBaseProperty<zui::any>;

/*
 * int control value for dropdown selected item index
 */
using DropDownBaseProperty = ControlBaseProperty<int>;

using DropDownTextProperty = ControlBaseProperty<std::string>;

typedef struct tagSwitchControl {
  zui::State<bool> _visible = true;
  zui::State<bool> _enable = true;
  zui::State<bool> _value = true;


  void setValue(bool value) {
    _value = value;
  }

  void setValueWithoutNotify(bool value) {
    _value.SetWithoutNotify(value);
  }

  void setVisibility(bool visible) {
    _visible = visible;
  }

  void setEnabled(bool enabled) {
    _enable = enabled;
  }
} SwitchControl;

typedef struct tagMeetingZappCellProperty
{
  zui::State<std::string> _name;
  zui::State<std::string> _icon;
  zui::State<bool> _selected = false;

  bool operator==(const tagMeetingZappCellProperty& other) const {
    return _name.Get() == other._name.Get() &&
           _icon.Get() == other._icon.Get() &&
           _selected.Get() == other._selected.Get();
  };
} MeetingZAppCellProperty;


typedef struct tagSliderControl {
  zui::State<bool> _visible = true;
  zui::State<bool> _enable = true;
  zui::State<float> _value = 0.0;


  void setValue(float value) {
    _value = value;
  }

  void setValueWithoutNotify(float value) {
    _value.SetWithoutNotify(value);
  }

  void setVisibility(bool visible) {
    _visible = visible;
  }

  void setEnabled(bool enabled) {
    _enable = enabled;
  }
} SliderControl;

typedef struct tagSpecificAudioChannelsProperty
  : public ControlBasePropertyWithVisible<bool> {
  zui::State<unsigned int> _channelNums = 0;

  tagSpecificAudioChannelsProperty(int channelNums)
    : _channelNums(channelNums) {}

  tagSpecificAudioChannelsProperty& operator=(int channelNums) {
    _channelNums = channelNums;
    return *this;
  }
  tagSpecificAudioChannelsProperty& operator=(bool value) {
    _value = value;
    return *this;
  }
} SpecificAudioChannelsProperty;

namespace zui {
  const char* const assistantView = "general.assistantView";
  const char* const dndView = "noti.dndView";
  const char* const ringtoneView = "phone.ringtoneView";
  const char* const messageTranslationView = "teamChat.messageTranslationView";
  const char* const generalScrollView = "general.scrollView";
  const char* const audioScrollView = "audio.scrollView";
  const char* const notificationScrollView = "notification.scrollView";
  const char* const meetingScrollView = "meeting.scrollView";
  const char* const recordingScrollView = "recording.scrollView";
  const char* const shareScreenScrollView = "shareScreen.scrollView";
  const char* const phoneScrollView = "phone.scrollView";
  const char* const teamChatScrollView = "teamChat.scrollView";
  const char* const accessibilityScrollView = "accessibility.scrollView";
}

class JumpViewHelper
{
public:
  enum class ViewID {
    // control
    kAssistantView,
    kDndToggleView,
    kRingtoneView,
    kMessageTranslationView,
    // scrollView,
    kGeneralScrollView,
    kAudioScrollView,
    kNotificationScrollView,
    kMeetingScrollView,
    kRecordingScrollView,
    kShareScreenScrollView,
    kPhoneScrollView,
    kTeamChatScrollView,
    kAccessibilityScrollView,
  };

  static void JumpToView(ViewID id, const std::string& highlighColor = zui::fillSubtlerPrimary)
  {
    static const std::unordered_map<ViewID, std::string> viewMap = {
      {ViewID::kAssistantView, zui::assistantView},
      {ViewID::kDndToggleView, zui::dndView},
      {ViewID::kRingtoneView, zui::ringtoneView},
      {ViewID::kMessageTranslationView, zui::messageTranslationView},
    };

    auto iter = viewMap.find(id);
    if (iter == viewMap.end())
      return;
    zui::gEnvironment.ScrollToView(iter->second, highlighColor);
  }

  static void JumpToTop(ViewID id)
  {
    const std::unordered_map<ViewID, const char*> viewIdToStringMap = {
      {ViewID::kGeneralScrollView, zui::generalScrollView},
      {ViewID::kAudioScrollView, zui::audioScrollView},
      {ViewID::kNotificationScrollView, zui::notificationScrollView},
      {ViewID::kMeetingScrollView, zui::meetingScrollView},
      {ViewID::kRecordingScrollView, zui::recordingScrollView},
      {ViewID::kShareScreenScrollView, zui::shareScreenScrollView},
      {ViewID::kPhoneScrollView, zui::phoneScrollView},
      {ViewID::kTeamChatScrollView, zui::teamChatScrollView},
      {ViewID::kAccessibilityScrollView, zui::accessibilityScrollView},
    };

    auto iter = viewIdToStringMap.find(id);
    if (iter == viewIdToStringMap.end())
      return;
    zui::gEnvironment.ScrollToTop(iter->second);
  }
};
