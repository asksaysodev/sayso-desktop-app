#pragma once

#include "view.h"

namespace zui
{
	/**
	 * @class Slider
	 * @brief A view representing a Slider.
	 *
	 * Sliders allow users to make selections from a range of values.
	 *
	 */
	class ZUI_API Slider : public View
	{
	public:
		/// @brief Construct a new Slider object
		Slider();

		/// @brief Virtual destructor of the Slider class.
		virtual ~Slider() = default;

		std::shared_ptr<ViewNode> Build() override;

		/**
		 * @brief Set the value of the slider.
		 * @param[in] value The value of the slider.
		 */
		Slider& value(const Bind<float>& value);

		/**
		 * @brief Set the range of the slider.
		 * @param[in] from The minimum value of the slider.
		 * @param[in] to The maximum value of the slider.
		 */
		Slider& range(const Bind<float>& from, const Bind<float>& to);

		/**
		 * @brief Set the step count of the slider.
		 * @param[in] step The step value of the slider.
		 */
		Slider& step(const Bind<float>& step);

		Slider& showStep(const Bind<bool>& show);

		/**
		 * @brief Set the show tooltip of the slider.
		 * @param[in] showToolTip The show tooltip of the slider.
		 */
		Slider& showToolTip(const Bind<bool>& showToolTip);

		Slider& onValueChanged(const std::function<void(float)>& func);

		Slider& onClick(std::function<void()> callback);
	};
}
