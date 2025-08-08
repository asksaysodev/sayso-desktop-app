#ifndef _ZUI_HELPER_H_
#define _ZUI_HELPER_H_

#include <memory>

namespace zui {
  template <class _Ty1, class _Ty2>
  std::shared_ptr<_Ty1> reinterpret_pointer_cast(const std::shared_ptr<_Ty2>& _Other) noexcept {
    const auto _Ptr = reinterpret_cast<typename std::shared_ptr<_Ty1>::element_type*>(_Other.get());
    return std::shared_ptr<_Ty1>(_Other, _Ptr);
  }
};

#endif
