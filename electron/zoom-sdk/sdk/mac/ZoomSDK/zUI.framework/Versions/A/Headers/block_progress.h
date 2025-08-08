#pragma once

#include "view.h"

namespace zui
{
	class ZUI_API BlockProgress : public View
	{
	public:
		BlockProgress();

		virtual ~BlockProgress() = default;

		std::shared_ptr<ViewNode> Build() override;

		BlockProgress& blockNum(int num);

		BlockProgress& blockWidth(float width);

		BlockProgress& blockHeight(float height);

		BlockProgress& blockStroke(float stroke);

		BlockProgress& blockRadius(float radius);

		// value 0 ~ 1
		BlockProgress& value(const Bind<float>& value);

		BlockProgress& normalColor(const std::string& blockColor, const std::string& strokeColor);

		BlockProgress& rangeColor(float start, float end, const std::string& blockColor, const std::string& strokeColor);
	};

}
