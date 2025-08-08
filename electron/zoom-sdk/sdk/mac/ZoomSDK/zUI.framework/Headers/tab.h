#pragma once

#include <type_traits>

#include "view.h"
#include "environment.h"
#include "badge.h"

namespace zui
{

	// ====================================================
	// Tab Interface
	// ====================================================

	class ZUI_API Tab : public View
	{
	public:
		enum class IconPosition
		{
			kLeading,
			kTrailing,
		};

		template<typename... Views>
		Tab(Views&&... views)
			: View(View::Type::Tab)
		{
			int dummy[] = { 0, (AddChildView(views.GetViewNode()), 0)... };
			static_cast<void>(dummy); // Prevent unused variable warning
		}

		Tab& label(const Bind<std::string>& label);
		Tab& icon(const Bind<std::string>& icon, IconPosition iconPosition);
		Tab& showNotifier(const Bind<bool> shouldShowNotifier);
		Tab& showBadge(const Bind<bool> shouldShowBadge, Badge& badge);

		Tab& visible(const Bind<bool> visible);

	private:
		void AddChildView(std::shared_ptr<ViewNode> viewNode);
	};

	// ====================================================
	// TabView Interface
	// ====================================================

	class ZUI_API TabView : public View
	{
	public:
		enum class Style
		{
			kPrimary,
			kSecondary,
		};

    enum class Size
    {
      kPrimaryLarge,
      kMedium,
    };

		template<typename...>
		struct are_all_tabs : std::true_type { };

		template<typename T, typename... Rest>
		struct are_all_tabs<T, Rest...>
			: std::integral_constant<
			bool,
			std::is_same<typename std::decay<T>::type, Tab>::value&&
			are_all_tabs<Rest...>::value
			>
		{
		};

		template<typename... Tabs>
		TabView(Tabs&&... tabs)
			: View(View::Type::TabView)
		{
			static_assert(are_all_tabs<Tabs...>::value, "All parameters must be of type Tab"); /// @todo allow conditional & loop
			int dummy[] = { 0, (AddChildView(tabs.GetViewNode()), 0)... };
			static_cast<void>(dummy); // Prevent unused variable warning
		}

		TabView& selection(Bind<int> index);
		TabView& style(Style style);
    TabView& size(Size size);
	private:
		void AddChildView(std::shared_ptr<ViewNode> viewNode);
	};

}

