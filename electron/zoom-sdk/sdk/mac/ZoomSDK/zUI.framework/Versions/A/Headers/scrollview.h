#pragma once

#include "view.h"
#include "vertical_stack.h"
#include "horizontal_stack.h"

namespace zui
{

	/**
	 * @class ScrollView
	 * @brief A view representing a ScrollView.
	 *
	 * A Scroll View provides a scrollable area for content that exceeds the visible display area.
	 * It allows users to view and interact with content by scrolling vertically or horizontally,
	 * making it suitable for displaying longer or larger content within a limited space.
	 *
	 */
	class ZUI_API ScrollView : public View
	{
	public:
		template <typename ViewT,
			typename = typename std::enable_if<
			std::is_convertible<typename std::decay<ViewT>::type, VerticalStack>::value ||
			std::is_convertible<typename std::decay<ViewT>::type, HorizontalStack>::value>::type>

		ScrollView(ViewT&& view)
			: View(View::Type::ScrollView)
		{
			AddChildView(std::forward<ViewT>(view).GetViewNode());
		}

		virtual ~ScrollView() = default;

		ScrollView& horizontal(bool enabled);
		ScrollView& vertical(bool enabled);

		std::shared_ptr<ViewNode> Build() override;

	private:
		void AddChildView(std::shared_ptr<ViewNode> viewNode);
	};

}
