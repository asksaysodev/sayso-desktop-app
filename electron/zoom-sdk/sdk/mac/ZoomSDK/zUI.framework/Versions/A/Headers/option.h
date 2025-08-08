#ifndef _ZUI_OPTION_H_
#define _ZUI_OPTION_H_

#include "view.h"

namespace zui
{

	class ZUI_API Option : public View
	{
	public:
		// temporary interface for selective block

		Option();
		virtual ~Option() = default;

		std::shared_ptr<ViewNode> Build() override;

		Option& normalimage(const Bind<std::string>& image);
		Option& selectedimage(const Bind<std::string>& image);
		Option& foreimage(const Bind<std::string>& image);

		Option& imageSize(float width, float height);

		Option& showSelectedFrame(bool enabled);
		Option& selectedFrameWidth(float value);
		Option& selectedFrameRadius(float value);

		Option& value(const any& value);
		Option& groupValue(const Bind<any>& groupValue);

		Option& fillImage(const Bind<std::string>& fillImage);
		Option& fillColor(const Bind<std::string>& fillColor);
		Option& onClick(std::function<bool()> callback);

		Option& groupName(const std::string& groupName);
	};
}

#endif // OPTIONAL_H
