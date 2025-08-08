#pragma once

#include "helper.h"
#include "view.h"

namespace zui
{
	// forward declaration
	class ContainerNode;

	class ZUI_API Container : public View
	{
	public:
		template<typename... Views>
		Container(View::Type viewType, Views&&... views)
			: View(viewType)
		{
			int dummy[] = { 0, (AddChildView(views.GetViewNode()), 0)... };
			static_cast<void>(dummy); // Prevent unused variable warning
		}

		~Container() = default;

		Container& spacing(float spacing);
		Container& alignItems(Layout::Alignment alignment);
		Container& alignUndefined();
		Container& alignCenter();
		Container& alignStart();
		Container& alignEnd();
		Container& alignFill();

		Container& onHover(std::function<void(bool)> callback);

		Container& accessibilityGroupName(const Bind<std::string>& groupName);

	private:
		void AddChildView(std::shared_ptr<ViewNode> viewNode);

		// move back once margin is supported correctly in layout engine
		Container& margin(const Layout::EdgeInsets& insets);
		Container& margin(const Layout::EdgeSet& edges, float length);
		Container& margin(float length);
	};
}

