#pragma once

#include "view.h"
#include "zui_data_define.h"
#include "icon_name_hints.h"

namespace zui
{
	/**
	* @class Image
	* @brief A view representing an Image.
	*
	* Displays an image.
	*
	*/
	class ZUI_API Image : public View
	{
	public:

		enum class ContentMode
		{
			Fill,
			Fit
		};

		explicit Image(const Bind<std::string>& image);
		virtual ~Image() = default;

		std::shared_ptr<ViewNode> Build() override;

		/// @todo name is not clear, we would have Image().image("path/to/image") which is confusing.
		/// @todo It's likely that star will provide a better lib for image loading. use ID for image.
		Image& image(Bind<std::string> image);
		Image& icon(Bind<std::string> iconName);

		Image& alpha(Bind<float> alpha);
		Image& rotate(Bind<float> rotate);
		Image& cornerRadius(const Bind<float>& cornerRaduis);
		Image& contentMode(ContentMode contentMode);  //unused

		Image& linkColor();

		/// @todo need a method to set the aspect ratio of the image.
		/// @todo need a method to clip the image.

		static const std::string DEFAULT_ICON_PATH;
		static const std::string ICON_PREFIX;
		static const float STANDARD_ICON_SIZE;
	};
}
