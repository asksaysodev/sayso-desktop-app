#ifndef __ZUI_ZVECTOR_H__
#define __ZUI_ZVECTOR_H__

#include <vector>
#include "model.h"

namespace zui 
{
template<typename T>
class ZVector
{
public:
  ZVector() :_stateSize(0){}
  ~ZVector() {}

  const State<int>& GetStateSize() const;
  void PushBack(const T& val);
  void PopBack();
  void SetUpdateCallback(const std::function<void(const int&)>& cb);

private:
  void UpdateDataSize();

private:
  std::vector<T> _data;
  State<int> _stateSize;
};

template<typename T>
const State<int>& ZVector<T>::GetStateSize() const
{
  return _stateSize;
}

template<typename T>
void ZVector<T>::PushBack(const T& val)
{
  _data.emplace_back(val);
  UpdateDataSize();
}

template<typename T>
void ZVector<T>::PopBack()
{
  if (!_data.empty()) 
  {
    _data.pop_back();
    UpdateDataSize();
  }
}

template<typename T>
void ZVector<T>::SetUpdateCallback(const std::function<void(const int&)>& cb)
{
  _stateSize.SetUpdate(cb);
}

template<typename T>
void ZVector<T>::UpdateDataSize()
{
  _stateSize = static_cast<int>(_data.size());
}

}
#endif