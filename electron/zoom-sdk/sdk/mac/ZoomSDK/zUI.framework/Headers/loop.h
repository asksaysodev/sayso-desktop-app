#ifndef _ZUI_LOOP_H_
#define _ZUI_LOOP_H_

#include <vector>
#include "container.h"

namespace zui
{
	class ZUI_API Loop : public View
	{
	public:
		explicit Loop(const Bind<std::vector<zui::any>>& range);

		virtual ~Loop() = default;

		std::shared_ptr<ViewNode> Build();

		/**
		* @brief Set up a view for loop creation.
		* @param[in] view
		*/
		Loop& body(const std::function<std::shared_ptr<View>(zui::any& val)>& func);

	};
}

#endif
