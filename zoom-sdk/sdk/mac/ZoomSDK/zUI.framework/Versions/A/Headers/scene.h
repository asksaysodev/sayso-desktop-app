#ifndef _ZUI_SCENE_H_
#define _ZUI_SCENE_H_

#include <memory>
#include "helper.h"
#include "view.h"


// forward declaration

namespace zui {

  class SceneNode;

  class ZUI_API Scene : public View
  {
    enum class SceneState
    {
      kActive,
      kInactive,
      kBackground
    };

  public:
    template<typename T>
    Scene(T&& view)
      : View(View::Type::Scene)
    {
      AddChildView(view.GetViewNode());
    }

    virtual ~Scene();

    virtual std::shared_ptr<ViewNode> Build() override;

    Scene& onClose(const std::function<void()>& func);
    Scene& onResize(const std::function<void(int width, int height)>& func);

    Scene& fixedSize(float width, float height);
    Scene& idealSize(float width, float height);
    Scene& title(const State<std::string>& title);

  private:
    std::shared_ptr<SceneNode> CreateSceneNode();
    void AddChildView(std::shared_ptr<ViewNode> viewNode);

  private:
    SceneState _state = SceneState::kBackground;
  };
}
#endif

