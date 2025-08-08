#ifndef _ZUI_RADIO_BUTTON_H_
#define _ZUI_RADIO_BUTTON_H_

#include "view.h" 

#include "text.h"
#include "popover.h"
#include <iostream>

namespace zui
{

	// ====================================================
	// RadioButton Interface
	// ====================================================

	class ZUI_API RadioButton : public View
	{
	public:
		// -- Constructors and Destructor --
		explicit RadioButton();
		virtual ~RadioButton() = default;

		// -- Accessors --
		RadioButton& title(const std::string& title);
		RadioButton& value(const any& value);
		// When the radionButton is embedded into the Loop, the groupValue needs to be manually set
		RadioButton& groupValue(const Bind<any>& groupValue);
	};

	// ====================================================
	// RadioButtonGroup Interface
	// ====================================================

	class ZUI_API RadioButtonGroup : public View
	{
	public:
		static constexpr float RadioDetailTopPadding = -6.f;
#ifdef __MACOS__
		static constexpr float ContentLeadingPadding = 22.f;
#else
		static constexpr float ContentLeadingPadding = 26.f;
#endif

		// -- Constructors and Destructor --
		template<typename... Views>
		RadioButtonGroup(Views&&... views)
			: View(View::Type::RadioButtonGroup)
		{
			int dummy[] = { 0, (AddChildView(views.GetViewNode()), 0)... };
			static_cast<void>(dummy); // Prevent unused variable warning
		}

		~RadioButtonGroup() = default;

		// -- Accessors --
		RadioButtonGroup& groupValue(const Bind<any>& groupValue);

		// -- Text Auto Alignment --
		RadioButtonGroup& textAutoAlignment(bool enabled);

	private:
		void AddChildView(std::shared_ptr<ViewNode> viewNode);
	};

}

#endif
