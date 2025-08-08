#ifndef __ZUI_OBSERVABLE_OBJECT_H__
#define __ZUI_OBSERVABLE_OBJECT_H__

#include <set>
#include <string>
#include "zui_data_define.h"

namespace zui {

#define OBSERVABLE_ATTRIBUTE(type, name) \
public: \
  void Set##name(const type& val) { \
    _##name = val; \
    Insert(_##name.GetBindId()); \
  } \
  Published<type> Get##name() { \
    return _##name; \
  } \
private: \
  Published<type> _##name;

#define OBSERVABLE_ATTRIBUTE_WITH_DEFAULT(type, name, defVal) \
public: \
  void Set##name(const type& val) { \
    _##name = val; \
    Insert(_##name.GetBindId()); \
  } \
  Published<type> Get##name() { \
    return _##name; \
  } \
private: \
  Published<type> _##name = defVal;

class ZUI_API ObservableObject
{
public:
  ObservableObject() = default;
  virtual ~ObservableObject() = default;

  virtual void GetAllAttributeBindIds(std::set<int>& bindIds) {}

  void Clear() { _updateAttributeIds.clear(); }
  void Insert(int bindId) { _updateAttributeIds.emplace(bindId); }
  void SwapUpdateAttributeIds(std::set<int>& bindIds) { _updateAttributeIds.swap(bindIds); }
private:
  std::set<int> _updateAttributeIds;
};

class ZUI_API ObservableObjectHelper
{
public:
  explicit ObservableObjectHelper(ObservableObject& object, bool isCopied = false);
  ~ObservableObjectHelper();

private:
  void NotifyUpdateState(const std::set<int>& updateBindIds);

private:
  ObservableObject& _object;
  bool _isCopied;
};



}
#endif