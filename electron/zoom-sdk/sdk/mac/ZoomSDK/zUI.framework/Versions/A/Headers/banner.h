#pragma once

#include "view.h"
#include "zui_data_define.h"

namespace zui
{
	class ZUI_API Banner : public View
	{
	public:
		enum class Sentiments
		{
			kInformation,
			kSuccess,
			kWarning,
			kError,
		};

		explicit Banner(const Bind<std::string>& message);

		virtual ~Banner() = default;

		std::shared_ptr<ViewNode> Build() override;

		Banner& sentiment(Sentiments sentiment);

		Banner& sentimentInformation();
		Banner& sentimentSuccess();
		Banner& sentimentWarning();
		Banner& sentimentError();

		Banner& title(const Bind<std::string>& title);

		Banner& closeButton(std::function<void()> callback);
	};

}
