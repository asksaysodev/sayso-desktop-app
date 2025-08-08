#ifndef _ZUI_MODEL_DEFINE_H_
#define _ZUI_MODEL_DEFINE_H_

#include <map>
#include <set>
#include <memory>
#ifdef __MACOS__
#include <zUI/any.h>
#import <zUI/zui_data_define.h>
#else
#include "any.h"
#include "zui_data_define.h"
#endif

namespace zui {
#define INVALID_BINDING_ID -1

enum class ModelDataType {
  kUnknown,
  // kChar,
  // kUChar,
  // kShort,
  // kUShort,
  kInt,
  kUInt,
  // kLong,
  // kULong,
  // kLongLong,
  // kULongLong,
  kFloat,
  kDouble,
  kBool,
  kString
};

class ZUI_API IStateObserver {
public:
  enum class Type { kModel, kState, kBind, kPublish, kBridge };

  explicit IStateObserver(Type type);
  virtual ~IStateObserver();

  virtual void OnUpdate(const zui::any &val, bool needNotifySwift) {}
  virtual void OnViewUpdate(const zui::any &val, bool needNotifySwift) {}

  int GetBindId() const { return _bindId; }
  void SetBindId(int bindId) { _bindId = bindId; }

  void SetViewId(const std::string &viewId);
  std::string GetViewId() { return _viewId; }

  void SetNotifyUI(bool notify) { _notifyUpdate = notify; }
  bool IsNotifyUI() { return _notifyUpdate; }

  bool IsState() { return _type == Type::kState; }
  bool IsBridge() { return _type == Type::kBridge; }
  
  // for swift
public:
  std::string GetNotifyName() { return _stateName; }
  ModelDataType GetDataType() { return _dataType; }
  void registerStateName(const std::string &stateName, ModelDataType type);
  void unregisterStateName();

protected:
  void RemoveObserver();
  void AddObserver();
  void Move(int targetId);
  void Update(const any &val, bool notify = true);
  void Insert(const any &val, bool notify = true);
  void Bridge(const zui::any &defValue, int targetBindId);

protected:
  int _bindId;
  std::string _viewId;
  Type _type;
  bool _notifyUpdate;

  // for swift bind
  std::string _stateName;
  ModelDataType _dataType;

  // for debug
  any _debugVal;
};

enum class ActionType {
  kUnknow,
  kAdd,
  kAddObserver,
  kUpdate,
  kMove,
  kRemove,
  // bind with swift
  kRegisterToSwift,
  kUpdateFromSwift,
  // state bridge
  kBridge,
};

struct Action {
  zui::any val;
  ActionType type = ActionType::kUnknow;
  IStateObserver *observer = nullptr;
  int bindId = INVALID_BINDING_ID;
  std::string viewId;
  bool notify = true;
};

struct AddAction : public Action {
  explicit AddAction(const zui::any &data, IStateObserver *obs, bool noti) {
    type = ActionType::kAdd;
    val = data;
    observer = obs;
    notify = noti;
  }

  ~AddAction() = default;
};

struct AddObserverAction : public Action {
  explicit AddObserverAction(IStateObserver *obs) {
    type = ActionType::kAddObserver;
    observer = obs;
  }

  ~AddObserverAction() = default;
};

struct UpdateAction : public Action {
  explicit UpdateAction(const zui::any &data, IStateObserver *obs, bool noti) {
    type = ActionType::kUpdate;
    val = data;
    observer = obs;
    notify = noti;
  }
  ~UpdateAction() = default;
};

struct MoveAction : public Action {
  explicit MoveAction(IStateObserver *obs, int targetId) {
    type = ActionType::kMove;
    observer = obs;
    bindId = targetId;
  }

  ~MoveAction() = default;
};

struct RemoveAction : public Action {
  explicit RemoveAction(IStateObserver *obs) {
    type = ActionType::kRemove;
    observer = obs;
  }

  ~RemoveAction() = default;
};

struct BindWithSwiftAction : public Action {
  explicit BindWithSwiftAction(IStateObserver *obs) {
    type = ActionType::kRegisterToSwift;
    observer = obs;
  }

  ~BindWithSwiftAction() = default;
};

struct UpdateFromSwiftAction : public Action {
  explicit UpdateFromSwiftAction(int targetBindId, const zui::any &value) {
    type = ActionType::kUpdateFromSwift;
    bindId = targetBindId;
    val = value;
  }

  ~UpdateFromSwiftAction() = default;
};

struct BridgeAction : public Action {
  explicit BridgeAction(IStateObserver *bridge, const zui::any &defValue, int targetBindId = INVALID_BINDING_ID) {
    type = ActionType::kBridge;
    observer = bridge;
    val = defValue;
    bindId = targetBindId;
  }
};

class ZUI_API ModelStore {
public:
  ModelStore() = default;
  ~ModelStore();

  static std::shared_ptr<ModelStore> Instance();

  void Dispatch(const Action &action);
  std::set<IStateObserver *> GetObservers(int bindId);
  bool GetViewIds(int bindId, std::set<std::string> &viewIds);
  void ZUIStateDidChange(const std::string &notifyName, ModelDataType type,
                         void *data);
                         
    template <typename T>
    typename std::enable_if<std::is_same<T, any>::value, T>::type
    Get(int bindId) {
      if (_data.find(bindId) == _data.end()) {
          return T();
      }
      return _data[bindId].val;
    }

    template <typename T>
    typename std::enable_if<!std::is_same<T, any>::value, T>::type
    Get(int bindId) {
      if (_data.find(bindId) == _data.end()) {
          return T();
      }
      return any_cast<T>(_data[bindId].val);
    }

private:
  void Insert(const zui::any &val, IStateObserver *observer, bool notify);
  void InsertObserver(IStateObserver *observer);
  void Update(const zui::any &var, IStateObserver *observer, bool notify);
  void Update(int bindId, const zui::any &var, bool notify, bool needNotifySwift);
  void Move(IStateObserver *observer, int targetBindId);
  void Remove(IStateObserver *observer);
  void RegisterStateToSwift(IStateObserver *observer);
  void UpdateFromSwift(int bindId, const zui::any &val);
  void Bridge(IStateObserver *bridge, const zui::any &defValue, int targetBindId);
  void RemoveBridge(IStateObserver* observer);

private:
  struct StoreData {
    zui::any val;
    std::set<IStateObserver *> observers;
  };
  std::map<int, StoreData> _data;
  std::map<int, int> _bridgeEntries;
  int _currentId = 0;

public:
  static bool _isDestroy;
};
} // namespace zui
#endif
