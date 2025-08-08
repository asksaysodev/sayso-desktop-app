#pragma once

#include "zui_data_define.h"

#include "text.h"

namespace zui
{
	/**
	 * @class Link
	 * @brief A view representing a Link.
	 *
	 * Allows users to navigate to another location or resource when clicked.
	 *
	 * @todo Where is size specified?
	 */
	class ZUI_API Link : public Text
	{
	public:

		/**
		 * @enum DestType
		 * @brief Defines the destination type of a Link.
		 * @todo move this enum out of Link class
		 */
		enum class DestType
		{
			kCustom,
			kUrl,
			kWindow,
		};

		enum class LinkType
		{
			kInline,
			kStandalone,
		};

		/**
		 * @brief Construct a new Link object
		 *
		 * @param[in] text The text that describes the link, which is full text or fragment in {}
		 */
		Link(const Bind<std::string>& text);


		/**
		 * @brief Construct a new Link object with defined url
		 *
		 * @param[in] text The text that describes the link, which is full text or fragment in {}
		 * @param[in] url The text used for href param
		 */
		Link(const std::string& text, const std::string& url);
		Link(const Bind<std::string>& text, const Bind<std::string>& url);

		/// @brief Virtual destructor of the Link class.
		virtual ~Link() = default;

		std::shared_ptr<ViewNode> Build() override;

		/**
		 * @brief Set the destination of the link.
		 * @param[in] type The type of the destination.
		 * @param[in] dest The destination of the link.
		 */
		Link& destination(DestType type, const std::string& dest);

		/**
		 * @brief Set the handler when the link is clicked.
		 *
		 * @param[in] callback The callback function that will be called when the link is clicked.
		 */
		Link& onClick(std::function<void(Link::DestType, const std::string&)> callback);

		/**
		 * @brief Set link type to inline (no arrow icon)
		 */
		Link& inlineLink();

		/**
		 * @brief Set link type to standalone (arrow icon after link))
		 */
		Link& standaloneLink();
	};
}
