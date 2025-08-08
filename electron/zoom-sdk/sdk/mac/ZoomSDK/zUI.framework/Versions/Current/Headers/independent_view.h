/*****************************************************************************
*
* Copyright (C) 2024, Zoom Video Communications, Inc
*
*****************************************************************************/
#pragma once

#include <memory>
#ifdef _WIN32
#include "zUI/helper.h"
#include "zUI/view.h"
#else
#include "helper.h"
#include "view.h"
#endif

namespace zui {

  class ZUI_API Independent : public View
  {
  public:
    template<typename T>
    Independent(T&& view)
      : View(View::Type::Independent)
    {
      AddChildView(view.GetViewNode());
    }

    Independent()
      : View(View::Type::Independent)
    {
    }

    virtual ~Independent();

    std::shared_ptr<View> Clone() const override
    {
      return std::make_shared<Independent>(*this);
    }

    virtual std::shared_ptr<ViewNode> Build() override;

  private:
    friend class IndependentWrapper;
    void RemoveAllDescendants();
    void AddChildView(std::shared_ptr<ViewNode> viewNode);
  };
}
