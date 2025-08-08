#ifndef __ZUI_MODEL_V2_H__
#define __ZUI_MODEL_V2_H__

#include <functional>
#include <map>
#include <set>
#ifdef __MACOS__
#import <zUI/model_define.h>
#import <zUI/zui_data_define.h>
#else
#include "model_define.h"
#include "zui_data_define.h"
#endif

namespace zui {

#define MODEL_OPERATION_FUNC(model, op, op_detail)                             \
  model &operator op(const T &val) {                                           \
    T curVal = Get();                                                          \
    Set(curVal op_detail val);                                                 \
    return static_cast<model &>(*this);                                        \
  }

#define MODEL_OPERATION_INCREMENT(model)                                       \
  model &operator++() {                                                        \
    T val = Get();                                                             \
    Set(++val);                                                                \
    return *this;                                                              \
  }                                                                            \
  model operator++(int) {                                                      \
    model temp = *this;                                                        \
    ++(*this);                                                                 \
    return temp;                                                               \
  }

#define MODEL_OPERATION_DECREMENT(model)                                       \
  model &operator--() {                                                        \
    T val = Get();                                                             \
    Set(--val);                                                                \
    return *this;                                                              \
  }                                                                            \
  model operator--(int) {                                                      \
    model temp = *this;                                                        \
    --(*this);                                                                 \
    return temp;                                                               \
  }

#define MODEL_OPERATION_EQUAL(model)                                           \
  bool operator==(const model& other) const{                                   \
    return Get() == other.Get();                                               \
  }                                                                            

#define MODEL_OPERATION_EQUAL2()                                           \
  bool operator==(const T& other) const{                                   \
    return Get() == other;                                               \
  }   

#define MODEL_OPERATION(model)                                                 \
  MODEL_OPERATION_FUNC(model, +=, +)                                           \
  MODEL_OPERATION_FUNC(model, -=, -)                                           \
  MODEL_OPERATION_FUNC(model, |=, |)                                           \
  MODEL_OPERATION_FUNC(model, &=, &)                                           \
  MODEL_OPERATION_INCREMENT(model)                                             \
  MODEL_OPERATION_DECREMENT(model)                                             \
  MODEL_OPERATION_EQUAL(model) \
  MODEL_OPERATION_EQUAL2()
  
template <typename T> class State : public IStateObserver {
public:
  State();
  State(const T &init);
  State(T &&init);
  State(const State<T> &state);
  ~State() = default;

  void Set(const T &val);
  void SetWithoutNotify(const T& val);
  T const Get() const;
  operator T() const;

  State<T> &operator=(const State<T> &state);
  State<T> &operator=(const T &val);
  MODEL_OPERATION(State<T>)

  void SetUpdate(const std::function<void(const T &)> &func);
  void OnUpdate(const zui::any &val, bool needNotifySwift) override;
  void SetViewUpdate(const std::function<void(const T &)> &func);
  void OnViewUpdate(const zui::any &val, bool needNotifySwift) override;

protected:
  std::function<void(const T &)> _update;
  std::function<void(const T &)> _viewUpdate;
};

template <typename T> class StateBridge : public IStateObserver {
public:
  StateBridge();
  StateBridge(const T& init);
  StateBridge(T&& init);
  StateBridge(const State<T>& state);
  StateBridge(const T& init, const State<T>& state);
  StateBridge(T&& init, const State<T>& state);
  ~StateBridge() = default;

  void Set(const T &val);
  void SetWithoutNotify(const T& val);
  void SetDef(const T &val);
  void SetDefWithoutNotify(const T& val);
  T const Get() const;
  operator T() const;

  void NotifyDef();
  bool IsBridged() const;
  void OutBridge(bool defValue = true);

  StateBridge<T> &operator=(const State<T> &state);
  StateBridge<T> &operator=(const T &val);
  MODEL_OPERATION(StateBridge<T>)

  void SetUpdate(const std::function<void(const T &)> &func);
  void OnUpdate(const zui::any &val, bool needNotifySwift) override;

protected:
  std::function<void(const T &)> _update;
  T _default;
  int _entryBindId;
};

template <typename T> class Published : public IStateObserver {
public:
  Published(const T &init = T{});
  Published(const Published& published);
  ~Published() = default;

  void Set(const T &val);
  void SetWithoutNotify(const T& val);
  T const Get() const;
  operator T() const;

  Published<T> &operator=(const T &val);
  Published<T>& operator=(const Published<T>& published);
  MODEL_OPERATION(Published<T>)

  void SetUpdate(const std::function<void(const T &)> &func);
  void OnUpdate(const zui::any &val, bool needNotifySwift);

protected:
  std::function<void(const T &)> _update;
};

template <typename T> class Bind : public IStateObserver {
public:
  Bind();
  Bind(const T &init);
  Bind(const State<T> &state);
  Bind(const Bind<T> &bind);
  Bind(const Published<T> &publish);
  Bind(const StateBridge<T> &bridge);
  ~Bind() = default;
    
  // Allows for Text("string") syntax for all components using Bind
  // Implicitly convert const char* to std::string
  // Specialized ctor that can accept string literal (const char*) only when T is std::String
  template <typename U = T>
  explicit Bind(const char* value,
       typename std::enable_if<std::is_same<U, std::string>::value>::type* = nullptr)
      : IStateObserver(IStateObserver::Type::kBind), _default(value) {}

  T const Get() const;
  void Set(const T &val);
  void SetWithoutNotify(const T& val);
  operator T() const;

  Bind<T> &operator=(const State<T> &val); // rebind
  Bind<T> &operator=(const Bind<T> &val);
  Bind<T> &operator=(const T &val);
  Bind<T> &operator=(const Published<T> &publish);
  Bind<T> &operator=(const StateBridge<T> &bridge);
  MODEL_OPERATION(Bind<T>)

  void SetUpdate(const std::function<void(const T &)> &func);
  void OnUpdate(const zui::any &val, bool needNotifySwift) override;
  void SetViewUpdate(const std::function<void(const T &)> &func);
  void OnViewUpdate(const zui::any &val, bool needNotifySwift) override;

private:
  std::function<void(const T &)> _update;
  std::function<void(const T &)> _viewUpdate;
  T _default;
};

template <typename T>
State<T>::State() : IStateObserver(IStateObserver::Type::kState) {
  Insert(T{});
}

template <typename T>
State<T>::State(const T &init) : IStateObserver(IStateObserver::Type::kState) {
  Insert(init);
}

template <typename T>
State<T>::State(T &&init) : IStateObserver(IStateObserver::Type::kState) {
  Insert(init);
}

template <typename T> State<T>::State(const State<T> &state) : IStateObserver(IStateObserver::Type::kState) {
  Move(state.GetBindId());
}

template <typename T> void State<T>::Set(const T &val) {
  if (_bindId == INVALID_BINDING_ID)
    Insert(val);
  else if (Get() != val)
    Update(val);
}

template <typename T> void State<T>::SetWithoutNotify(const T& val) {
  if (_bindId == INVALID_BINDING_ID)
    Insert(val, false);
  else if (Get() != val)
    Update(val, false);
}

template <typename T> T const State<T>::Get() const {
  return ModelStore::Instance()->Get<T>(_bindId);
}

template <typename T> State<T>::operator T() const { return Get(); }

template <typename T> State<T> &State<T>::operator=(const State<T> &state) {
  Set(state.Get());
  return *this;
}

template <typename T> State<T> &State<T>::operator=(const T &val) {
  Set(val);
  return *this;
}

template <typename T>
void State<T>::SetUpdate(const std::function<void(const T &)> &func) {
  if (_update) {
    // Chain call
    _update = [prev_f = _update, func](const T &val) {
      prev_f(val);
      func(val);
    };
  } else {
    _update = func;
  }
}

template <typename T>
void State<T>::SetViewUpdate(const std::function<void(const T &)> &func) {
  if (_viewUpdate) {
    _viewUpdate = [prev_f = _viewUpdate, func](const T& val) {
      prev_f(val);
      func(val);
    };
  } else {
    _viewUpdate = func;
  }
}

template <typename T>
void State<T>::OnUpdate(const zui::any &val, bool needNotifySwift) {
  zui::any copy = val;
  T newValue = zui::any_cast<T>(copy);

  if (_viewUpdate)
    _viewUpdate(newValue);
  
  if (_update)
    _update(newValue);

  if (needNotifySwift && !_stateName.empty()) {
    ModelStore::Instance()->ZUIStateDidChange(_stateName, _dataType,
                                              (void *)&newValue);
  }
}

template <typename T>
void State<T>::OnViewUpdate(const zui::any& val, bool needNotifySwift) {
  zui::any copy = val;
  T newValue = zui::any_cast<T>(copy);

  if (_viewUpdate)
    _viewUpdate(newValue);

  if (needNotifySwift && !_stateName.empty()) {
    ModelStore::Instance()->ZUIStateDidChange(_stateName, _dataType,
                                              (void *)&newValue);
  }
}

template <typename T>
StateBridge<T>::StateBridge() : IStateObserver(IStateObserver::Type::kBridge), _default(T{}) {
  Insert(_default);
}

template <typename T>
StateBridge<T>::StateBridge(const T& init) : IStateObserver(IStateObserver::Type::kBridge), _default(init) {
  Insert(_default);
}

template <typename T>
StateBridge<T>::StateBridge(T&& init) : IStateObserver(IStateObserver::Type::kBridge), _default(init) {
  Insert(_default);
}

template <typename T>
StateBridge<T>::StateBridge(const State<T>& state) : IStateObserver(IStateObserver::Type::kBridge), _default(state.Get()), _entryBindId(state.GetBindId()) {
  Bridge(_default, _entryBindId);
}

template <typename T>
StateBridge<T>::StateBridge(const T& init, const State<T>& state) : IStateObserver(IStateObserver::Type::kBridge), _default(init), _entryBindId(state.GetBindId()) {
  Bridge(_default, _entryBindId);
}

template <typename T>
StateBridge<T>::StateBridge(T&& init, const State<T>& state) : IStateObserver(IStateObserver::Type::kBridge), _default(init), _entryBindId(state.GetBindId()) {
  Bridge(_default, _entryBindId);
}

template <typename T> void StateBridge<T>::Set(const T& val) {
  if (_bindId == INVALID_BINDING_ID)
    Insert(val);
  else if (Get() != val)
    Update(val);
}

template <typename T> void StateBridge<T>::SetWithoutNotify(const T& val) {
  if (_bindId == INVALID_BINDING_ID)
    Insert(val, false);
  else if (Get() != val)
    Update(val, false);
}

template <typename T> void StateBridge<T>::SetDef(const T& val) {
  if (_bindId == INVALID_BINDING_ID)
    Insert(val);
  else if (_default != val)
    Update(_default);

  _default = val;
}

template <typename T> void StateBridge<T>::SetDefWithoutNotify(const T& val) {
  if (_bindId == INVALID_BINDING_ID)
    Insert(val, false);
  else if (_default != val)
    Update(val, false);

  _default = val;
}

template <typename T> void StateBridge<T>::OutBridge(bool defValue) {
  _entryBindId = INVALID_BINDING_ID;
  if (defValue)
    Update(_default);
}

template <typename T> T const StateBridge<T>::Get() const {
  return ModelStore::Instance()->Get<T>(_bindId);
}

template <typename T> StateBridge<T>::operator T() const { return Get(); }

template <typename T> bool StateBridge<T>::IsBridged() const { return _entryBindId != INVALID_BINDING_ID; }

template <typename T> void StateBridge<T>::NotifyDef() {
  if (_bindId == INVALID_BINDING_ID)
    Insert(_default);
  else
    Update(_default);
}

template <typename T> StateBridge<T>& StateBridge<T>::operator=(const T& val) {
  Set(val);
  if (_entryBindId == INVALID_BINDING_ID)
    _default = val;
  return *this;
}

template <typename T> StateBridge<T>& StateBridge<T>::operator=(const State<T>& state) {
  if (_entryBindId != state.GetBindId()) {
    _entryBindId = state.GetBindId();
    Bridge(_default, _entryBindId);
  }
  return *this;
}

template <typename T>
void zui::StateBridge<T>::SetUpdate(const std::function<void(const T&)>& func) {
  _update = func;
}

template <typename T>
void zui::StateBridge<T>::OnUpdate(const zui::any& val, bool needNotifySwift) {
  zui::any copy = val;
  T newValue = zui::any_cast<T>(copy);

  if (_update)
    _update(newValue);

  if (needNotifySwift && !_stateName.empty()) {
    ModelStore::Instance()->ZUIStateDidChange(_stateName, _dataType,
                                              (void*)&newValue);
  }
}

template <typename T>
Bind<T>::Bind()
  : IStateObserver(IStateObserver::Type::kBind), _default(T{}) {}

template <typename T>
Bind<T>::Bind(const T &init)
    : IStateObserver(IStateObserver::Type::kBind), _default(init) {}

template <typename T>
Bind<T>::Bind(const State<T> &state)
    : IStateObserver(IStateObserver::Type::kBind) {
  *this = state;
}

template <typename T>
Bind<T>::Bind(const Bind<T> &bind)
    : IStateObserver(IStateObserver::Type::kBind) {
  SetBindId(bind.GetBindId());
  _update = bind._update;
  _default = bind._default;
  AddObserver();
}

template <typename T>
Bind<T>::Bind(const Published<T> &publish)
    : IStateObserver(IStateObserver::Type::kBind) {
  *this = publish;
}

template <typename T>
Bind<T>::Bind(const StateBridge<T>& bridge)
    : IStateObserver(IStateObserver::Type::kBind) {
  *this = bridge;
}

template <typename T> T const Bind<T>::Get() const {
  if (_bindId == INVALID_BINDING_ID)
    return _default;
  return ModelStore::Instance()->Get<T>(_bindId);
}

template <typename T> void Bind<T>::Set(const T &val) {
  if (_bindId == INVALID_BINDING_ID)
    _default = val;
  else if (Get() != val)
    Update(val);
}

template <typename T> void Bind<T>::SetWithoutNotify(const T& val) {
  if (_bindId == INVALID_BINDING_ID)
    _default = val;
  else if (Get() != val)
    Update(val, false);
}

template <typename T> Bind<T>::operator T() const { return Get(); }

template <typename T> Bind<T> &Bind<T>::operator=(const State<T> &state) {
  Move(state.GetBindId());
  return *this;
}

template <typename T> Bind<T> &Bind<T>::operator=(const Bind<T> &bind) {
  // bind new state
  if (_bindId == INVALID_BINDING_ID) {
    _default = bind._default;
    SetBindId(bind.GetBindId());
    AddObserver();
  } else {
    Set(bind.Get());
  }
  return *this;
}

template <typename T> Bind<T> &Bind<T>::operator=(const T &val) {
  this->Set(val);
  return *this;
}

template <typename T> Bind<T> &Bind<T>::operator=(const Published<T> &publish) {
  Move(publish.GetBindId());
  return *this;
}

template <typename T> Bind<T>& Bind<T>::operator=(const StateBridge<T>& bridge) {
  Move(bridge.GetBindId());
  return *this;
}

template <typename T>
void Bind<T>::SetUpdate(const std::function<void(const T &)> &func) {
  if (_update) {
    // Chain call
    _update = [prev_f = _update, func](const T &val) {
      prev_f(val);
      func(val);
    };
  } else {
    _update = func;
  }
}

template <typename T>
void Bind<T>::OnUpdate(const zui::any &val, bool needNotifySwift) {
  zui::any newValue = val;
  T value = zui::any_cast<T>(newValue);

  if (_viewUpdate)
    _viewUpdate(value);
  if (_update)
    _update(value);
  if (needNotifySwift && !_stateName.empty())
    ModelStore::Instance()->ZUIStateDidChange(_stateName, _dataType,
                                              (void *)&value);
}

template <typename T>
void Bind<T>::SetViewUpdate(const std::function<void(const T &)> &func) {
  if (_viewUpdate) {
    _viewUpdate = [prev_f = _viewUpdate, func](const T& val) {
      prev_f(val);
      func(val);
    };
  } else {
    _viewUpdate = func;
  }
}

template <typename T>
void Bind<T>::OnViewUpdate(const zui::any& val, bool needNotifySwift) {
  zui::any newValue = val;
  T value = zui::any_cast<T>(newValue);

  if (_viewUpdate)
    _viewUpdate(value);
  if (needNotifySwift && !_stateName.empty())
    ModelStore::Instance()->ZUIStateDidChange(_stateName, _dataType,
                                              (void *)&value);
}

template <typename T> Published<T>::Published(const T &init)
    : IStateObserver(IStateObserver::Type::kPublish)
{
  Insert(init);
}

template <typename T> Published<T>::Published(const Published& published)
    : IStateObserver(IStateObserver::Type::kPublish)
{
  Move(published.GetBindId());
}

template <typename T> void Published<T>::Set(const T &val) {
  if (_bindId == INVALID_BINDING_ID)
    Insert(val);
  else if (Get() != val)
    Update(val);
}

template <typename T> void Published<T>::SetWithoutNotify(const T& val) {
  if (_bindId == INVALID_BINDING_ID)
    Insert(val, false);
  else if (Get() != val)
    Update(val, false);
}

template <typename T> T const Published<T>::Get() const {
  return ModelStore::Instance()->Get<T>(_bindId);
}

template <typename T> Published<T>::operator T() const { return Get(); }

template <typename T> Published<T> &Published<T>::operator=(const T &val) {
  Set(val);
  return *this;
}

template <typename T> Published<T>& Published<T>::operator=(const Published<T>& published)
{
  Set(published.Get());
  return *this;
}

template <typename T>
void Published<T>::SetUpdate(const std::function<void(const T &)> &func) {
  if (_update) {
    // Chain call
    _update = [prev_f = _update, func](const T &val) {
      prev_f(val);
      func(val);
    };
  } else {
    _update = func;
  }
}

template <typename T>
void Published<T>::OnUpdate(const zui::any &val, bool needNotifySwift) {
  zui::any newValue = val;
  if (_update)
    _update(zui::any_cast<T>(newValue));
}

} // namespace zui

#include "compare_values.h"

#endif
