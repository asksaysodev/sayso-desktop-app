#pragma once

#include "view.h"
#include "link.h"
#include "zui_data_define.h"

namespace zui
{
	/**
	 * @class InputField
	 * @brief A view representing an input field.
	 *
	 * Input fields provide a means for adding various types of user-generated entries.
	 * Text formats include numbers and tags for short and long entries.
	 *
	 */
	class ZUI_API InputField : public View
	{
	public:
		using OnEditChangedFunc = std::function<void(const std::string&)>;

		enum class Style
		{
			kDefault = 1,
			kPassword = 2,
			kSearch = 3,
			kSelect = 4, // TODO: dropdown ?
			kNumber = 5,
			kCombo = 6,
      kTextArea = 7,
		};

		/// @brief Construct a new InputField object
		InputField();

		/// @brief Virtual destructor of the InputField class.
		virtual ~InputField() = default;

		std::shared_ptr<ViewNode> Build() override;

		InputField& style(const Bind<InputField::Style>& inputStyle);
		InputField& styleDefault();
		InputField& stylePassword();
		InputField& styleSearch();
		InputField& styleSelect();
		InputField& styleNumber();
		InputField& styleCombo();

		InputField& title(const Bind<std::string>& title);

		InputField& questionMarkText(const Bind<std::string>& questionMarkText);

		InputField& link(Link&& linkText);

		InputField& required(const Bind<bool>& isRequired);
		InputField& optional(const Bind<std::string>& optional);

		/**
		 * @brief Set the placeholder text for the input field.
		 * @param[in] placeHolder The placeholder text for the input field.
		 */
		InputField& placeholder(const Bind<std::string>& placeHolder);

		/**
		 * @brief Set the text of the input field.
		 * @param[in] text The text of the input field.
		 */
		InputField& text(const Bind<std::string>& text);

		/**
		 * @brief Set the text of the comboHead.
		 * @param[in] comboHead The text of the comboHead.
		 */
		InputField& comboHead(const Bind<std::string>& comboHead);

		InputField& error(const Bind<bool>& showError);

		InputField& allowClear(const Bind<bool>& allowClear);

		/// @todo What's upArrow for?
		InputField& upArrow(const Bind<bool>& showUpArrow);

		InputField& readOnly(const Bind<bool>& readOnly);

		/**
		 * @brief Set the allowed character count and display the limit under the text input
		 * @param maxCharacterCount[in] The maximum allowed character length
		 */
		InputField& limitCharacterCount(const Bind<int>& maxCharacterCount);

		// helper text
		/// @todo If the error message is not empty, should we display the error automatically?
		InputField& errorText(const Bind<std::string>& errorText);

		InputField& helpText(const Bind<std::string>& helpText);

		/// @todo When a text limit is non zero, should we display the character count automatically?
		InputField& showCount(const Bind<bool>& show);

		InputField& onEditChanged(const OnEditChangedFunc& func);
	};
}
