#ifndef _ZUI_VERTICAL_STACK_H_
#define _ZUI_VERTICAL_STACK_H_

#include "container.h"

namespace zui
{
	class ZUI_API VerticalStack : public Container
	{
	public:
		template <typename... Views>
		VerticalStack(Views &&...views)
			: Container(View::Type::VerticalStack, std::forward<Views>(views)...)
		{
		}

		virtual ~VerticalStack() = default;

		std::shared_ptr<ViewNode> Build();
	};
}

#endif // VERTICAL_STACK_H

