/*****************************************************************************
*
* Copyright (C) 2024, Zoom Video Communications, Inc
*
*****************************************************************************/
#pragma once

#include <memory>
#include "view.h"

namespace zui {

  // forward declaration
  class ExternalViewNode;
  class ViewNode;

  class ZUI_API External : public View
  {
  public:
    External(void* externalViewData)
      : View(zui::reinterpret_pointer_cast<ViewNode>(CreateExternalViewNode(externalViewData)))
    {
      _externalViewData = externalViewData;
    }

    External()
      : View(zui::reinterpret_pointer_cast<ViewNode>(CreateExternalViewNode(nullptr)))
    {
    }

    virtual ~External();

    std::shared_ptr<View> Clone() const override
    {
      return std::make_shared<External>(*this);
    }

    virtual std::shared_ptr<ViewNode> Build() override;
      
    void SetExternalViewData(void* externalViewData) { _externalViewData = externalViewData; };

  private:
    std::shared_ptr<ExternalViewNode> CreateExternalViewNode(void* externalViewData);
    void* _externalViewData = nullptr;
  };
}
