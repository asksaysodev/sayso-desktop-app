#ifndef _ZUI_BUTTON_H_
#define _ZUI_BUTTON_H_

#include <functional>
#include "view.h" 

#include "text.h"
#include "image.h"
#include "zui_data_define.h"

/// @todo Delete

namespace zui
{
	class ZUI_API Button
		: public View
	{
	public:

		Button();
		virtual ~Button() = default;

		std::shared_ptr<ViewNode> Build() override;
		Button& onClick(std::function<void()> callback);

		Button& label(const std::string& label);
		Button& normalImage(const Bind<std::string>& image);
		Button& hoverImage(const Bind<std::string>& image);
		Button& pressedImage(const Bind<std::string>& image);
		Button& disabledImage(const Bind<std::string>& image);

	};
}
#endif
