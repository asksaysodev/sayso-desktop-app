#pragma once

#include "container.h"
#include "zui_data_define.h"

// forward declaration
namespace zui
{
	/**
	* @class HorizontalStack
	* @brief A container that lays out its children in a horizontal line.
	*
	*/
	class ZUI_API HorizontalStack : public Container
	{
	public:
		template <typename... Views>
		HorizontalStack(Views &&...views)
			: Container(View::Type::HorizontalStack, std::forward<Views>(views)...)
		{
		}
	public:
		/// @brief Virtual destructor of the HorizontalStack class.
		virtual ~HorizontalStack() = default;

		std::shared_ptr<ViewNode> Build();

	};
}
