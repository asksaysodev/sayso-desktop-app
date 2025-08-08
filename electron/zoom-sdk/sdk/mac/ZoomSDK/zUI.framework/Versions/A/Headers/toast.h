#ifndef _ZUI_TOAST_H_
#define _ZUI_TOAST_H_

#include "view.h" 

namespace zui
{
	class Link;
	class BasicButton;
	class ZUI_API Toast : public View
	{
	public:
		enum class Sentiments
		{
			kInformative,
			kSuccess,
			kWarning,
			kDanger
		};

		Toast();
		virtual ~Toast() = default;

		std::shared_ptr<ViewNode> Build() override;

		Toast& sentiments(const Bind<Sentiments>& sentiments);
		Toast& sentimentsInformative();
		Toast& sentimentsSuccess();
		Toast& sentimentsWarning();
		Toast& sentimentsDanger();

		Toast& close(const Bind<bool>& visible);
		Toast& title(const Bind<std::string>& title);
		Toast& body(const Bind<std::string>& body);
		Toast& link(Link& link);
		Toast& action(const Bind<std::string>& left, const Bind<std::string>& right);
	};
}

#endif //_ZUI_TOAST_H_
