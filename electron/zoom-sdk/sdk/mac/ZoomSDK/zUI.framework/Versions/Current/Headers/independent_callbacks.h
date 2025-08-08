//
//  independent_callbacks.h
//  zUI
//
//  Created by Rob Christenson on 9/23/24.
//

#ifndef independent_callbacks_h
#define independent_callbacks_h

#include <functional>

namespace zui
{
class ViewNode;

using OnSizeUpdateFunc = std::function<void(const float& width, const float& height, std::shared_ptr<ViewNode> viewNode)>;
using OnSizeUpdateFuncSimple = std::function<void(const float& width, const float& height)>;
using OnSizeUpdateFuncWithContext = std::function<void(const float& width, const float& height, void* context)>;
}

#endif /* independent_callbacks_h */
