#pragma once

#include "view.h"

namespace zui
{
	/**
	 * @class Toggle
	 * @brief A view representing a Toggle.
	 *
	 * Toggles allow users to turn on and off.
	 *
	 */
	class ZUI_API Toggle
		: public View
	{
	public:
		/**
		 * @enum Size
		 * @brief Defines the size of a Toggle.
		 */
		enum class Size
		{
			kSmall,
			kMedium
		};

		/// @brief Construct a new Toggle object
		Toggle();

		/// @brief Virtual destructor of the Toggle class.
		virtual ~Toggle() = default;

		std::shared_ptr<ViewNode> Build() override;

		Toggle& on(const Bind<bool>& on);

		/**
		 * @brief Set the size of the Toggle.
		 * @param[in] size The size of the Toggle.
		 */
		Toggle& toggleSize(const Bind<Size>& size);
		Toggle& sizeSmall();
		Toggle& sizeMedium();
	};
}
