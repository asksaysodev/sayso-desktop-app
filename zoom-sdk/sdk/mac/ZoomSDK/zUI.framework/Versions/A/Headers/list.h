#pragma once

#include "zui_data_define.h"
#include "view.h"
#include <vector>

namespace zui
{

	class ZUI_API List : public View
	{
	public:
		template<typename... T,
			typename = typename std::enable_if<zui::conjunction<std::is_base_of<View, T>...>::value>::type>
			List(T&&... views)
			: View(View::Type::List)
		{
			int dummy[] = { 0, (AddChildView(std::forward<T>(views)), 0)... };
			static_cast<void>(dummy); // Prevent unused variable warning
		}

		List(const Bind<std::vector<zui::any>>& data);
		virtual ~List() = default;

		List& cell(const std::function<std::shared_ptr<View>(zui::any& cellData, size_t index)>& cellFunc);
		List& cellHeight(float cellHeight);
		List& cellPadding(const Layout::EdgeInsets& cellPadding);


	private:
		void AddChildView(View& viewNode);
	};

}
