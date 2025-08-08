#ifndef _ZUI_NAVIGATION_H_
#define _ZUI_NAVIGATION_H_

#include "vertical_stack.h"
#include "horizontal_stack.h"

namespace zui
{

	class ZUI_API Navigation : public View  //unused
	{
	public:


		template <typename... Views>
		Navigation(Views &&...views);
		virtual ~Navigation() = default;

		std::shared_ptr<ViewNode> Build();
	};
}

#endif // NAVIGATION_H
