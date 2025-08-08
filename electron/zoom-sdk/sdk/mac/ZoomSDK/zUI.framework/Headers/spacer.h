#pragma once

#include "view.h" 


namespace zui
{
	/**
	 * @class Spacer
	 * @brief A view representing a Spacer.
	 *
	 * Spacers are used to add space between views.
	 *
	 */
	class ZUI_API Spacer : public View
	{
	public:
		/// @brief Construct a new Spacer object
		Spacer();

		/// @brief Virtual destructor of the Spacer class.
		virtual ~Spacer() = default;

		std::shared_ptr<ViewNode> Build() override;
	};
}
