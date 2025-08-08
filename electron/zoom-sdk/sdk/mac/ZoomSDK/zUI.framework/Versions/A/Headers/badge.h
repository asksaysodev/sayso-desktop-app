#pragma once

#include "zui_data_define.h"
#include "view.h"

namespace zui
{
	/**
	 * @class Badge
	 * @brief A view representing a badge.
	 *
	 * A small UI component displaying text, such as "New" or "Beta" used to highlight information
	 * or status related to an item, often used for categorization or emphasis.
	 *
	 * @todo Add a modifier to add the leading icon to the badge.
	 */
	class ZUI_API Badge : public View
	{
	public:
		enum class Style
		{
			kBadgeGray,
			kBadgeRed,
			kBadgeOrange,
			kBadgeYellow,
			kBadgeGreen,
			kBadgeTeal,
			kBadgeBlue,
			kBadgePurple,
			kBadgeInverse,
			kBadgeNew,
			kBadgeBeta,
		};

	public:
		/// @brief Construct a new Avatar View
		Badge();

		/// @brief Virtual destructor of the Badge class.
		virtual ~Badge() = default;

		std::shared_ptr<ViewNode> Build() override;

		/**
		 * @brief Define the label displayed in the badge.
		 *
		 * @param[in] text The label displayed in the badge.
		 * @todo Change text to label to follow the naming convention.
		 */
		Badge& text(const Bind<std::string>& text);

		/**
		 * @brief Define the style of the badge.
		 *
		 * @param[in] type The style of the badge.
		 * @todo Change "badgetype" to "badgeStyle".
		 */
		Badge& badgeStyle(const Bind<Style>& style);
		Badge& styleGray();
		Badge& styleRed();
		Badge& styleOrange();
		Badge& styleYellow();
		Badge& styleGreen();
		Badge& styleTeal();
		Badge& styleBlue();
		Badge& stylePurple();
		Badge& styleInverse();
		Badge& styleNew();
		Badge& styleBeta();
	};
}
