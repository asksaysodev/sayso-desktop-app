#ifndef _ZUI_TOOLTIP_H_
#define _ZUI_TOOLTIP_H_

#include "presentation.h"
#include <string>

namespace zui
{

	class ZUI_API ToolTip : public Presentation
	{
	public:
		enum class Modifier : uint32_t
		{
			kShift,
			kControl,
			kOption,
			kAlternate,
			kCommand,
		};

		ToolTip(const std::string& text);
		virtual ~ToolTip() = default;

		virtual PresentationNodePtr Build() override;

		virtual Presentation::Type GetType() const override { return Presentation::Type::kTooltip; }

		ToolTip& text(const std::string& text);
		ToolTip& keyInput(const std::string& input);

		template<typename... Args,
			typename = typename std::enable_if<conjunction<std::is_same<Args, ToolTip::Modifier>...>::value>::type>
			ToolTip& keyModifiers(Args... args);

	protected:
		template<typename Arg, typename... Rest,
			typename = typename std::enable_if<conjunction<std::is_same<Arg, ToolTip::Modifier>, std::is_same<Rest, ToolTip::Modifier>...>::value>::type>
			void DoKeyModifiers(Arg arg, Rest... rest);

		void DoKeyModifiers() { }

		void DoKeyModifier(ToolTip::Modifier modifier);
	};

	template<typename... Args, typename>
	ToolTip& ToolTip::keyModifiers(Args... args)
	{
		DoKeyModifiers(args...);
		return *this;
	}

	template<typename Arg, typename... Rest, typename>
	void ToolTip::DoKeyModifiers(Arg arg, Rest... rest)
	{
		DoKeyModifier(arg);
		DoKeyModifiers(rest...);
	}

	template<typename T>
	class ZUI_API enable_tooltip
	{
	public:
		T& tooltip(const ToolTip& tooltip);
	};
}

#endif // TOOLTIP_H
