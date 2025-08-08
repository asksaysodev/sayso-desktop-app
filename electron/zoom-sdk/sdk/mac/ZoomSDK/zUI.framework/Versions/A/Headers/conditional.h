#ifndef _ZUI_CONDITIONAL_H_
#define _ZUI_CONDITIONAL_H_

#include "container.h"
#include "zui_data_define.h"

// forward declaration
namespace zui {
  class ZUI_API Conditional : public View
  {
  public:
    Conditional(const Bind<bool> &condition);

    virtual ~Conditional() = default;

    std::shared_ptr<ViewNode> Build();

    /**
    * @brief Set child views on true conditional.
    * @param[in] views
    */
    template<typename... Views>
    Conditional& IfTrue(Views&&... views)
    {
      int dummy[] = { 0, (AddIfTrueView(views.GetViewNode()), 0)... };
      static_cast<void>(dummy); // Prevent unused variable warning

      return *this;
    }

    /**
    * @brief Set child views on false conditional.
    * @param[in] views
    */
    template<typename... Views>
    Conditional &IfFalse(Views&&... views)
    {
      int dummy[] = { 0, (AddIfFalseView(views.GetViewNode()), 0)... };
      static_cast<void>(dummy); // Prevent unused variable warning

      return *this;
    }

  private:
    void AddIfTrueView(std::shared_ptr<ViewNode> viewNode);
    void AddIfFalseView(std::shared_ptr<ViewNode> viewNode);
    void SetCondition(const Bind<bool>& condition);
  };

  class ZUI_API MultiConditional : public View
  {
  public:
    MultiConditional(const Bind<int>& multiCondition);

    virtual ~MultiConditional() = default;

    std::shared_ptr<ViewNode> Build();

    /**
    * @brief Set Child views in different valid conditions
    * @param[in] val
    * @param[in] views
    */
    template<typename... Views>
    MultiConditional& ConditionValid(int val, Views&&... views)
    {
      int dummy[] = { 0, (AddConditionValidView(val, views.GetViewNode()), 0 )... };
      static_cast<void>(dummy);

      return *this;
    }

    /**
    * @brief Set default Child views if all conditions are false
    * @param[in] val
    * @param[in] views
    */
    template<typename... Views>
    MultiConditional& ConditionInvalid(Views&&... views)
    {
      int dummy[] = { 0, (AddConditionInvalidView(views.GetViewNode()), 0)... };
      static_cast<void>(dummy);

      return *this;
    }

  private:
    void AddConditionValidView(int val, std::shared_ptr<ViewNode> viewNode);
    void AddConditionInvalidView(std::shared_ptr<ViewNode> viewNode);
  };
}
#endif // _ZUI_CONDITIONAL_H_
