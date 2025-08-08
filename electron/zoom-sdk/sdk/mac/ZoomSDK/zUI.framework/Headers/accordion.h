#pragma once

/**
 * @todo what is the purpose of this file? Ross
 */
#include "zui_data_define.h"

#include "container.h"

namespace zui
{
	/**
	 * @class Accordion
	 * @brief A container representing an accordion UI component.
	 *
	 * Allows users to view and interact with a large amount of information
	 * in a compact space by hiding or showing content in sections.
	 */
	class ZUI_API Accordion : public Container
	{
	public:
		/**
		 * @enum ChevronPosition
		 * @brief Defines the position of the chevron icon in the accordion.
		 */
		enum class ChevronPosition
		{
			kLeading,  ///< The chevron icon is on the leading side of the accordion.
			kTrailing, ///< The chevron icon is on the trailing side of the accordion.
		};

		/**
		 * @brief Construct a new Accordion object
		 *
		 * @tparam View
		 * @param views
		 */
		template<typename... Views>
		Accordion(Views&&... views)
			: Container(View::Type::Accordion, views...)
		{
		}

		/// @brief Virtual destructor of the Accordion class.
		virtual ~Accordion() = default;

		/**
		 * @brief Build the view
		 *
		 * @return std::shared_ptr<ViewNode>
		 */
		std::shared_ptr<ViewNode> Build();

		/**
		 * @brief Define the chevron icon position
		 *
		 * @param[in] status The position of the chevron icon.
		 */
		Accordion& chevron(const Bind<ChevronPosition>& status);

		/**
		 * @brief Define the label of the accordion.
		 *
		 * @param[in] label The label of the accordion.
		 */
		Accordion& label(const Bind<std::string>& label);

		/**
		 * @brief Define the icon of the accordion.
		 *
		 * @param icon The path of the icon.
		 */
		Accordion& icon(const Bind<std::string>& icon, const Bind<std::string>& disableIcon);
	};
}

