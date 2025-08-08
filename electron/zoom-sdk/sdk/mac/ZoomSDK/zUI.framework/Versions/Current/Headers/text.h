#pragma once

#include "view.h"

namespace zui
{
	/**
	 * @class Text
	 * @brief A view representing a Text.
	 *
	 * Text is used to display text on the screen.
	 *
	 */
	class ZUI_API Text : public View
	{
	public:
		/**
		 * @enum Typography
		 * @brief Defines the typography of a Text.
		 * @todo Move the enum out of the class.
		 */
		enum class Typography
		{
			kDisplay1,
			kDisplay2,
			kDisplay3,
			kBodyLargeBold,
			kBodyLarge,
			kBodyBold,
			kBody,
			kSubtitleBold,
			kSubtitle,
			kCaption,
			kLabel
		};

		enum class WrapMode
		{
			kNoWrap,
			kWordWrap,
			kWrapAnyWhere
		};

		/**
		 * @brief Construct a Text object with the given text.
		 * @param[in] text The text to display.
		 * @todo remove noNeedAddParent
		 */
		explicit Text(const Bind<std::string>& text);

		/// @brief Virtual destructor of the Text class.
		virtual ~Text() = default;

		std::shared_ptr<ViewNode> Build() override;

		/**
		 * @brief Set the typography of the text.
		 * @param[in] typography The typography of the text.
		 */
		Text& typography(const Bind<Text::Typography>& typography);

		Text& display1();
		Text& display2();
		Text& display3();
		Text& bodyLargeBold();
		Text& bodyLarge();
		Text& bodyBold();
		Text& body();
		Text& subtitleBold();
		Text& subtitle();
		Text& caption();
		Text& label();

		Text& wrap();
		Text& noWrap();
		/**
		 * @brief Set the text color.
		 * @param[in] index The text color index.
		 */

		Text& color(const Bind<std::string>& color);

		Text& color(const char* color);

		/**
		 * @brief Set the text wrap mode.
		 * @param[in] mode The text wrap mode.
		 */
		Text& wrapMode(const Bind<WrapMode>& mode);

		Text& onClick(std::function<void()> callback);

	protected:
		Text(const Bind<std::string>& text, View::Type viewType);
	};
}
