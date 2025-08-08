#pragma once

#include <functional>
#include "view.h"
#include "text.h"
#include "tooltip.h"
#include "popover.h"
#include "zui_data_define.h"

namespace zui
{
	/**
	 * @class BasicButton
	 * @brief A view representing a BasicButton.
	 *
	 * Allows users to take action, has a text on them that elaborates on the button’s function.
	 *
	 */
	class ZUI_API BasicButton
		: public View
		, public enable_tooltip<BasicButton>
		, public enable_popover<BasicButton>
	{
	public:
		/**
	* @enum Style
	* @brief Defines the style of a BasicButton.
	* @todo What's the Overlay style? I counldn't find any information about it in figma.
	* @todo Should we rename the enum to BasicButtonStyle?
	*/
		enum class Style
		{
			kPrimary,
			kSecondary,
			kTetiary,
			kOverlay,
			kSelected,
			kDangerPrimary,
			kDangerSecondary,
			kDangerTetiary
		};

		/**
	* @enum Size
	* @brief Defines the size of a BasicButton.
	* @todo Should we rename the enum to BasicButtonSize?
	*/
		enum class Size
		{
			kSmall,
			kMedium,
			kLarge,
			kXLarge
		};

	public:
		/**
		 * @brief Construct a BasicButton object with the given text.
		 * @param[in] text The text displayed on the button. The text is required for BasicButton.
		 */
		explicit BasicButton(const Bind<std::string>& text);

		/// @brief Virtual destructor of the BasicButton class.
		virtual ~BasicButton() = default;

		std::shared_ptr<ViewNode> Build() override;

		/**
		 * @brief Set the style of the BasicButton.
		 * @param[in] style The style of the BasicButton.
		 */
		BasicButton& style(const Bind<Style>& style);

		BasicButton& stylePrimary();
		BasicButton& styleSecondary();
		BasicButton& styleTetiary();
		BasicButton& styleOverlay();
		BasicButton& styleSelected();
		BasicButton& styleDangerPrimary();
		BasicButton& styleDangerSecondary();
		BasicButton& styleDangerTetiary();

		/**
		 * @brief Set the size of the BasicButton.
		 * @param[in] size The size of the BasicButton.
		 */
		BasicButton& size(const Bind<Size>& size);
		BasicButton& sizeSmall();
		BasicButton& sizeMedium();
		BasicButton& sizeLarge();
		BasicButton& sizeXLarge();

		/// @todo merge lefticon and righticon into one function
		BasicButton& lefticon(const Bind<std::string>& licon);
		BasicButton& righticon(const Bind<std::string>& ricon);

		/**
		 * @brief Display an activity indicator on the button.
		 * @param[in] show Whether to display the activity indicator.
		 */
		BasicButton& activityIndicator(const Bind<bool>& show);

		/**
		 * @brief Set the callback function to be called when the button is clicked.
		 * @param[in] callback The callback function to be called when the button is clicked.
		 */
		BasicButton& onClick(std::function<void()> callback);

	};
}
