#pragma once

#include <unordered_set>
#include "view.h"
#include "text.h"
#include "popover.h"
#include "zui_data_define.h"

namespace zui
{
	/**
	 * @class Checkbox
	 * @brief A view representing a Checkbox.
	 *
	 * Allow users to select one or more items from a set.
	 */
	class ZUI_API Checkbox
		: public View
		, public enable_popover<Checkbox>
	{
	public:
		/**
		 * @enum Status
		 * @brief Defines the status of a Checkbox.
		 */
		enum class Status
		{
			kUnselected,
			kSelected,
			kIndeterminate
		};

		/**
		 * @brief Construct a BasicButton object with the given label.
		 * @param[in] label The label that describes the checkbox.
		 */
		explicit Checkbox(const Bind<std::string>& label);

		/// @brief Virtual destructor of the Checkbox class.
		virtual ~Checkbox() = default;

		std::shared_ptr<ViewNode> Build() override;

		/**
		 * @brief Set the status of the checkbox.
		 * @param checked The status of the checkbox.
		 */
		Checkbox& checked(const Bind<Checkbox::Status>& checked); //delete

		Checkbox& status(const Bind<Checkbox::Status>& checked);
		Checkbox& statusUnselected();
		Checkbox& statusSelected();
		Checkbox& statusIndeterminate();

	};

}
