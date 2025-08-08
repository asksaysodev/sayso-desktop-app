#ifndef _ZUI_APP_H_
#define _ZUI_APP_H_

#include <vector>
#include <map>
#include <memory>
#include "scene.h"
#include "zui_data_define.h"

namespace zui
{
	// forward declaration
	class SceneNode;

	class ZUI_API App
	{
	public:
		template<typename ...Scene>
		App(Scene&& ...scenes)
		{
			int dummy[] = { 0, (Init(scenes.GetViewNode()), 0)... };
			static_cast<void>(dummy); // Prevent unused variable warning
		}

		~App() = default;

		void OpenWindow(const std::string& id);
		void CloseWindow(const std::string& id);
		void Destory();

		void OpenUrlWithDefaultBrowser(const std::string& url);

    void OpenSystemNotifications();
    void ScrollToView(const std::string& viewId, const std::string& highlightColor);
  private:
    void Init(std::shared_ptr<ViewNode> sceneNode);
    void SetWindowHidden(const std::string& id, const Bind<bool>& hidden);

	private:

	};
}

#endif
