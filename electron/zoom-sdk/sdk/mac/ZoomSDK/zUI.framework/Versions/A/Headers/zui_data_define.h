#ifndef ZUI_DATA_DEFINE_H
#define ZUI_DATA_DEFINE_H
#include <string>
#include <list>
#include <map>
#include <cstdint>

#ifdef _WIN32
#ifdef zUI_EXPORTS
#define ZUI_API __declspec(dllexport)
#else
#define ZUI_API __declspec(dllimport)
#endif
#else
#define ZUI_API __attribute__((visibility("default")))
#endif


namespace zui
{
	template<class...> struct conjunction : std::true_type { };
	template<class T1> struct conjunction<T1> : T1 { };
	template<class T1, class... Tn>
	struct conjunction<T1, Tn...>
		: std::conditional<bool(T1::value), conjunction<Tn...>, T1>::type
	{
	};

	template<typename...> struct disjunction : std::false_type { };
	template<typename T1> struct disjunction<T1> : T1 { };
	template<typename T1, typename... Tn>
	struct disjunction<T1, Tn...>
		: std::conditional<bool(T1::value), T1, conjunction<Tn...>>::type
	{
	};

	enum class PrismTriggerType
	{
		kUnKnown,
		kOnClick,
		kOnDisplay,
	};

	struct CoachMarkFeaturePicture
	{
		void* image_inst = nullptr;
		std::string image;
		int width = 0;
		int height = 0;
		bool is_on_top = false;
	};

	struct CoachMarkBadge
	{
		enum class CoachMarkBadgeType
		{
			kBadgeNone,
			kBadgeNew,
			kBadgeBeta,
		};
		CoachMarkBadgeType style = CoachMarkBadgeType::kBadgeNone;
		std::string text;
	};

	enum class CoachMarButton
	{
		kClose,
		kPrimary,
		kSecondary,
		kLink
	};

	class IPrismCoachMarkSink
	{
	public:
		virtual void OnCoachMarkShow(const std::string& id) { }
		virtual void OnCoachMarkHide(const std::string& id) { }
		virtual void OnCoachMarkDestory(const std::string& id) { }
		virtual void OnBtnClicked(const std::string& id, CoachMarButton cm_button) { }
		virtual void OnDescLinkActive(const std::string& id, const std::string& url) { }
	};

	enum ArrowDir
	{
		kArrowDirNone = -1,
		kArrowDirLeft,
		kArrowDirTop,
		kArrowDirRight,
		kArrowDirBottom,
	};


	struct PrismCoachMarkParam
	{
		CoachMarkFeaturePicture feature_pic;
		CoachMarkBadge badge;
		std::string title;
		std::string desc;
		std::string steps;
		std::string link_text;
		std::string link;
		std::string primary_button_text;
		std::string secondary_button_text;
		ArrowDir arrow_dir_ = kArrowDirBottom;
		bool is_noactivate = true;
		bool close_on_inactive = false;
		bool zui_popover_mode = false;
		IPrismCoachMarkSink* sink_ = nullptr;
	};


	class CoachMarkIt
	{
	public:
		virtual bool ShowCoachMark(std::string& control_id, const std::string& id, const PrismCoachMarkParam& param) = 0;
	};

	class ZUI_API ZUILib
	{
	public:
		static CoachMarkIt* GetCoachMarkIt();
	};

	class BannerCallbackIt
	{
	public:
		virtual void OnBannerClose() = 0;
	};

  enum class AccElementMode : uint32_t {
    kContain, // current control and children will get focus one by one(default)
    kIgnore,  // current control and children will not get focus and be removed from focus tree
    kCombine  // current control and children will get focus as one control
  };

	extern ZUI_API const char* colorBlack;
	extern ZUI_API const char* colorWhite;
	extern ZUI_API const char* colorRed;
	extern ZUI_API const char* colorGreen;
	extern ZUI_API const char* colorBlue;
	extern ZUI_API const char* colorYellow;
	extern ZUI_API const char* colorGray;

	extern ZUI_API const char* iconColorRed;
	extern ZUI_API const char* iconColorYellow;
	extern ZUI_API const char* iconColorGreen;

	extern ZUI_API const char* fillDefault;
	extern ZUI_API const char* fillColorRed;
	extern ZUI_API const char* fillColorBlue;

	extern ZUI_API const char* strokeColorRed;
	extern ZUI_API const char* strokeColorYellow;
	extern ZUI_API const char* strokeColorGreen;

	extern ZUI_API const char* textNeutral;
	extern ZUI_API const char* textStrongNeutral;
	extern ZUI_API const char* textStrongerNeutral;
	extern ZUI_API const char* textPrimary;
	extern ZUI_API const char* textError;

	extern ZUI_API const char* borderNeutral;
	extern ZUI_API const char* borderSubtleNeutral;
	extern ZUI_API const char* borderStrongNeutral;

	extern ZUI_API const char* fillSubtlerNeutral;
	extern ZUI_API const char* fillSubtlerPrimary;
	extern ZUI_API const char* fillSubtlerWarning;
	extern ZUI_API const char* fillSubtlerError;

	extern ZUI_API const char* inverseGlobalDefault;
	extern ZUI_API const char* bgDefault;
	extern ZUI_API const char* bgDarkerNeutral;
	extern ZUI_API const char* inverseNeutral;
	extern ZUI_API const char* statusDataVizGold40;

	extern ZUI_API const char* stateGlobalDarkTransparentPress;

	extern ZUI_API const char* customClearColor;
}

#endif
