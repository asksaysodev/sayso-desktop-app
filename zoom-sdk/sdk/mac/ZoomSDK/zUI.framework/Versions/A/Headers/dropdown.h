#pragma once

#include "view.h" 

#include "text.h"
#include "zui_data_define.h"

namespace zui
{
	/**
	 * @class Dropdown
	 * @brief A dropdown view.
	 *
	 * Dropdowns help users navigate through options allowing them to narrow down the list of items to a single specified criteria
	 */
	class ZUI_API Dropdown : public View
	{
	public:
		enum class Style
		{
			kBasic,
			kExposedEditable,
			kExposedReadOnly
		};

		enum class Size
		{
			kSmall,//24.0 Height
			kMedium,//32.0 Height, default
			kLarge//40.0 Height
		};

		/**
		 * @brief Construct a Dropdown object with the given label.
		 * @todo Should we remove the label parameter since it could be optional?
		 */
		explicit Dropdown(const Bind<std::string>& label = Bind<std::string>(""));

		/// @brief Virtual destructor of the Dropdown class.
		virtual ~Dropdown() = default;

		std::shared_ptr<ViewNode> Build() override;

		Dropdown& label(const Bind<std::string>& label);
		Dropdown& unselectedTip(const Bind<std::string>& tip);

		template<typename... Args,
			typename = typename std::enable_if<conjunction<std::is_same<Args, Text>...>::value>::type>
			Dropdown& option(Args&&... args)
		{
			ProcessArgs(args...);
			return *this;
		}

		Dropdown& option(const std::list<Text>& text_list)
		{
			for (auto& text : text_list)
				DoOptionArgs(const_cast<Text&>(text));
			return *this;
		}

		Dropdown& option(const Bind<std::list<std::string>>& text_list);
		Dropdown& option(const std::vector<std::string>& options);
		Dropdown& userData(const Bind<std::list<std::string>>& data_list);

		Dropdown& style(const Dropdown::Style& style);
		Dropdown& styleBasic();
		Dropdown& styleEditable();
		Dropdown& styleReadOnly();

		Dropdown& size(const Dropdown::Size& size);
		Dropdown& sizeSmall();
		Dropdown& sizeMedium();
		Dropdown& sizeLarge();


		/// @todo using text of option is not a reliable way to identify an option since the text could change
		Dropdown& select(const Bind<std::string>& option);
		/// @todo Using index is not a reliable way to identify an option since the index could change when the options are sorted or filtered.
		Dropdown& select(const Bind<int>& index);

		Dropdown& fullDisplay();
    	Dropdown& disableFullDisplay();

		Dropdown& onOpened(const std::function<void()>& cb);

	protected:
		void DoOptionArgs(const std::string& arg);
		void DoOptionArgs(Text& arg);

		template<typename Arg, typename... Rest,
			typename = typename std::enable_if<conjunction<std::is_same<Arg, Text>, std::is_same<Rest, Text>...>::value>::type>
			void ProcessArgs(Arg& arg, Rest&... rest)
		{
			DoOptionArgs(arg);
			ProcessArgs(rest...);
		}

		void ProcessArgs() { }
	};

	/**
	 * @class CustomDropdown
	 * @brief A dropdown view with custom items.
	 *
	 * Dropdowns help users navigate through options allowing them to narrow down the list of items to a single specified criteria
	 * user-defined item views can provide more information and rich visual elements
	 */
	class ZUI_API CustomDropdown : public View
	{
	public:
		/*
		 * @enum  FilterInfoType
		 * @brief dropdown filter display info in different ways
		 *        kFullInfo - all info defined in custom view
		 *        kSimpleString - specified brief info in string
		 */
		enum class FilterInfoType
		{
			kFullInfo,
			kSimpleString,
		};

		explicit CustomDropdown(FilterInfoType infoType = FilterInfoType::kFullInfo);

		explicit CustomDropdown(Dropdown::Size size);

		virtual ~CustomDropdown() = default;

		std::shared_ptr<ViewNode> Build() override;

		CustomDropdown& select(const Bind<int>& index);

		// list update
		CustomDropdown& itemData(const Bind<std::vector<zui::any>>& data);
		CustomDropdown& itemView(const std::function<std::shared_ptr<View>(zui::any& itemData, size_t index)>& func);
		CustomDropdown& itemSelectedView(const std::function<std::shared_ptr<View>(zui::any& itemData, size_t index)>& func);
		CustomDropdown& itemSelectedString(const std::function<std::string(zui::any& itemData, size_t index)>& func);
	};

	// ====================================================
	// CustomDropDown_New Interface
	// ====================================================

	class ZUI_API CustomDropDown_New : public View
	{
	public:
		// -- Constructors and Destructor --
		template<typename... Views>
		CustomDropDown_New(Views&&... views)
			: View(View::Type::CustomDropDownNewGroup)
		{
			int dummy[] = { 0, (AddChildView(views.GetViewNode()), 0)... };
			static_cast<void>(dummy); // Prevent unused variable warning
		}

		~CustomDropDown_New() = default;

		// -- CustomDropDown_New --
		CustomDropDown_New& dropdownValue(const Bind<any>& dropdownValue);

	private:
		void AddChildView(std::shared_ptr<ViewNode> viewNode);
	};

	// ====================================================
	// CustomDropDownItem_New Interface
	// ====================================================

	class ZUI_API CustomDropDownItem_New : public View
	{
	public:
		// -- Constructors and Destructor --
		explicit CustomDropDownItem_New();
		virtual ~CustomDropDownItem_New() = default;

		// -- Accessors --
		template<typename... Views>
		CustomDropDownItem_New& selectedView(Views&&... views)
		{
			int dummy[] = { 0, (AddSelectedView(views.GetViewNode()), 0)... };
			static_cast<void>(dummy); // Prevent unused variable warning

			return *this;
		}

		template<typename... Views>
		CustomDropDownItem_New& detailedView(Views&&... views)
		{
			int dummy[] = { 0, (AddDetailedView(views.GetViewNode()), 0)... };
			static_cast<void>(dummy); // Prevent unused variable warning

			return *this;
		}

		CustomDropDownItem_New& value(const any& value);

	protected:
		void AddDetailedView(std::shared_ptr<ViewNode> viewNode);
		void AddSelectedView(std::shared_ptr<ViewNode> viewNode);
	};

}
