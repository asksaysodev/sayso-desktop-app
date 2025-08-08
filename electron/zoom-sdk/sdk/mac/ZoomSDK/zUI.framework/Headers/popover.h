#pragma once

#include "view.h"
#include "presentation.h"
#include <string>
#include <functional>
#include "zui_data_define.h"

namespace zui
{
	/**
	 * @class Popover
	 * @brief A view representing a Popover.
	 *
	 * Displays a popover.
	 *
	 */
	class ZUI_API Popover : public Presentation
	{
	public:
		/**
		 * @enum Direction
		 * @brief Defines the direction of a Popover.
		 */
		enum class Direction
		{
			kDown,
			kDownLeft,
			kDownRight,
			kUp,
			kUpLeft,
			kUpRight,
			kLeft,
			kLeftTop,
			kLeftBottom,
			kRight,
			kRightTop,
			kRightBottom
		};

		enum class ButtonType
		{
			kClose,
			kPrimary,
			kSecondary,
			kLink,
		};

		/// @todo What's the content parameter for?
		/// @todo Explict?
		Popover(const zui::Bind<std::string>& content);

		/// @brief Virtual destructor of the Popover class.
		virtual ~Popover() = default;

		virtual PresentationNodePtr Build() override;

		virtual Presentation::Type GetType() const override { return Presentation::Type::kPopover; }

		/**
		 * @brief Set the title of the popover.
		 * @param[in] title The title of the popover.
		 */
		Popover& title(const zui::Bind<std::string>& title);

		/**
		 * @brief Set the content of the popover.
		 * @param[in] content The content of the popover.
		 */
		Popover& content(const zui::Bind<std::string>& content);

		/**
		 * @brief Set the link of the popover.
		 * @param[in] text The text of the link.
		 * @param[in] target The target of the link.
		 * @todo Can we accept a Link component as the parameter of link?
		 */
		Popover& link(const zui::Bind<std::string>& text, const zui::Bind<std::string>& target);

		/**
		 * @brief Set the primary action of the popover.
		 * @param[in] text The text of the primary action.
		 * @todo Can we accept a BasicButton component as the parameter of primary_action?
		 */
		Popover& primary_action(const zui::Bind<std::string>& text);

		/**
		 * @brief Set the secondary action of the popover.
		 * @param[in] text The text of the secondary action.
		 * @todo Can we accept a BasicButton component as the parameter of secondary_action?
		 */
		Popover& secondary_action(const zui::Bind<std::string>& text);

		/**
		 * @brief Set the direction of the popover.
		 * @param[in] direction The direction of the popover.
		 */
		Popover& direction(Popover::Direction direction);

		/**
		 * @brief Enable the close button of the popover.
		 * @param[in] enabled Whether the close button is enabled.
		 */
		Popover& close_button(bool enabled);

		Popover& onShow(std::function<void()> callback);
		Popover& onHide(std::function<void()> callback);
		Popover& onDestroy(std::function<void()> callback);
		Popover& onClick(std::function<void(Popover::ButtonType, const std::string&)> callback);
		Popover& onDescLinkActive(std::function<void(const std::string&)> callback);
	};

	template <typename T>
	class ZUI_API enable_popover
	{
	public:
		T& popover(const Popover& popover);
	};

	class ZUI_API PopoverButton
		: public View
		, public enable_popover<PopoverButton>
	{
	public:
		enum class Type
		{
			kInfo,
			kQuestion,
			kAlert,
		};

		enum class Size
		{
			kSmall,
			kMedium,
			kLarge,
		};

	public:
		PopoverButton(const std::string& content, Type type = Type::kInfo, Size size = Size::kMedium);
		PopoverButton(const Popover& popover, Type type = Type::kInfo, Size size = Size::kMedium);

		virtual ~PopoverButton() = default;

		std::shared_ptr<ViewNode> Build() override;

		PopoverButton& type(Type type);
		PopoverButton& size(Size size);
	};
}
