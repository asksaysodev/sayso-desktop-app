#pragma once

#include "view.h"
#include "basic_button.h"
#include "tooltip.h"
#include "zui_data_define.h"

namespace zui
{
	/**
	 * @class IconButton
	 * @brief A view representing an IconButton.
	 *
	 * Allows users to take action, uses an icon on them that elaborates on the button’s function.
	 *
	 */
	class ZUI_API IconButton
		: public View
		, public enable_tooltip<IconButton>
		, public enable_popover<IconButton>
	{
	public:
		enum class Style
		{
			kPrimary,
			kSecondary,
			kTetiary,
			kOverlay,
		};

		enum class Size
		{
			kSmall,
			kMedium,
			kLarge,
			kXLarge
		};
	public:
		/// @brief Construct an IconButton object.
		explicit IconButton();

		/// @brief Virtual destructor of the IconButton class.
		virtual ~IconButton() = default;

		std::shared_ptr<ViewNode> Build() override;

		/**
		 * @brief Set the style of the IconButton.
		 * @param[in] style The style of the IconButton.
		 * @todo If we are reusing the style from BasicButton, why not merge the two classes? BasicButton can have an optional icon.
		 */
		IconButton& style(const Bind<Style>& style);
		IconButton& stylePrimary();
		IconButton& styleSecondary();
		IconButton& styleTetiary();
		IconButton& styleOverlay();

		IconButton& size(const Bind<Size>& size);
		IconButton& sizeSmall();
		IconButton& sizeMedium();
		IconButton& sizeLarge();
		IconButton& sizeXLarge();

		IconButton& icon(const Bind<std::string>& licon);
		IconButton& activityIndicator(const Bind<bool>& show);
		IconButton& onClick(std::function<void()> callback);
	};
}

