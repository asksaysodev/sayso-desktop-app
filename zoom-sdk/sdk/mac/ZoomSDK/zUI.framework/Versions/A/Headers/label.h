#ifndef _ZUI_LABEL_H_
#define _ZUI_LABEL_H_

#include "view.h" 

#include "text.h"
#include "image.h"
#include "zui_data_define.h"

namespace zui
{
	class ZUI_API Label : public View
	{
	public:
		Label();
		virtual ~Label() = default;

		std::shared_ptr<ViewNode> Build() override;

		Label& text(const Text& text);
		Label& image(const Image& image);
	};

}

#endif // LABEL_H
