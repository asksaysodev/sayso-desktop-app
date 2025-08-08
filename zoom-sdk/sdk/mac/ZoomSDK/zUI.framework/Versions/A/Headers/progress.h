#pragma once

#include "view.h"

namespace zui
{
	/**
	 * @class Progress
	 * @brief A view representing a Progress.
	 *
	 * Visual indicator to inform the user of the system status.
	 *
	 */
	class ZUI_API Progress : public View
	{
	public:
		enum class Scale
		{
			kSmall,  // height = 4.0
			kUsual,//Default  heigth = 6.0
		};

		/// @todo Adding progressValue as a parameter in constructor?.
		Progress();

		/// @brief Virtual destructor of the Progress class.
		virtual ~Progress() = default;

		std::shared_ptr<ViewNode> Build() override;

		/// @todo move to constructor?
		Progress& progressValue(const Bind<float>& progress); // max value = 1.0, progress = 0.5, 50%

		Progress& scale(const Bind<Progress::Scale>& scale);
		Progress& scaleSmall();
		Progress& scaleUsual();

		Progress& normalColor(const std::string& color);

		Progress& rangeColor(float start, float end, const std::string& color);
	};

}
