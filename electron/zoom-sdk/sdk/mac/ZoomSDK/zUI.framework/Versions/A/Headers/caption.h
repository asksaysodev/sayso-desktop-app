#pragma once

#include "view.h"
#include "zui_data_define.h"

namespace zui
{

	/**
	 * @class Caption
	 * @brief Used to provide the text of a show's audio portion to those who may have trouble hearing it.
	 */
	class ZUI_API Caption
		: public View
	{
	public:
		explicit Caption();
		virtual ~Caption() = default;

		std::shared_ptr<ViewNode> Build() override;

		Caption& avatar(const Bind<std::string>& avatar);
		Caption& avatarSize(const Bind<float>& avatarSize);

		Caption& caption(const Bind<std::string>& caption);
		Caption& captionSize(const Bind<float>& captionSize);
		Caption& captionColor(const Bind<std::string>& captionColor);

		Caption& translation(const Bind<std::string>& translation);
		Caption& translationSize(const Bind<float>& translationSize);
		Caption& translationColor(const Bind<std::string>& translationColor);
		Caption& backgroundColor(const Bind<std::string>& translationColor);

		Caption& fontType(const Bind<std::string>& fontType);

	};

}
