#pragma once

// Include the definition of Bind<T>
#include "any.h"

namespace zui {

template <typename T>
class Bind;

// First overload: comparing T with any
template <typename T>
bool compareValues(const T& lhs, const any& rhsAny) {
  const T& rhs = zui::any_cast<T>(rhsAny);
  return lhs == rhs;
}

// Second overload: comparing Bind<T> with any
template <typename T>
bool compareValues(const Bind<T>& lhs, const any& rhsAny) {
  const T& rhs = zui::any_cast<T>(rhsAny);
  return lhs.Get() == rhs;
}

} // namespace zui

