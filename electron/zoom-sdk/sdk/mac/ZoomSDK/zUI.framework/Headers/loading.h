#pragma once

#include "view.h"
#include "container.h"

namespace zui
{
	/**
	 * @class Loading
	 * @brief A view representing a loading spinner.
	 *
	 * A visual indicator used to convey system is processing.
	 *
	 */
	class ZUI_API Loading : public View
	{
	public:
		/**
		 * @enum Size
		 * @brief Defines the size of a Loading.
		 */
		enum class Size
		{
			kLarge,
			kMedium,
			kSmall,
		};

		/// @brief Construct a Loading object.
		Loading();

		/// @brief Virtual destructor of the Loading class.
		virtual ~Loading() = default;

		/**
		 * @brief Set the size of the loading spinner.
		 * @param[in] size The size of the loading spinner.
		 */
		Loading& size(const Bind<Size>& size);
		Loading& sizeLarge();
		Loading& sizeMedium();
		Loading& sizeSmall();

		std::shared_ptr<ViewNode> Build() override;
	};
}
