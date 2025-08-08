#ifndef ZUI_VIEW
#define ZUI_VIEW

#include <memory>
#include <string>
#include <initializer_list>
#include "types.h"

#ifdef __MACOS__
#import <zUI/model.h>
#else
#include "model/model.h"
#endif

#include "presentation.h"
#include "zui_data_define.h"
// forward declaration

namespace zui
{
	class ViewNode;
	struct Variant;
#define MAKE_VIEW(...) std::shared_ptr<View>(new __VA_ARGS__)

	class ZUI_API View
	{
	public:
		/// @todo better place this in class View
		enum class Type
		{
			Undefined,
			Scene,
			BasicButton,
			Button,
			IconButton,
			HorizontalStack,
			VerticalStack,
			ScrollView,
			Spacer,
			Option,
			RadioButton,
			RadioButtonGroup,
			Checkbox,
			CheckboxGroup,
			Dropdown,
			Text,
			Image,
			Label,
			Link,
			Popover,
			PopoverButton,
			Accordion,
			Avatar,
			Toggle,
			Independent,
			Notifier,
			Badge,
			Slider,
			InputField,
			LoadingIndicator,
			Divider,
			Progress,
			BlockProgress,
			Conditional,
			MultiConditional,
			Loop,
			Custom,
			External,
			Toast,
			List,
			ListCell,
			TabView,
			Tab,
			TabBar,
			Icon,
			CustomDropdown,
			CustomDropdownItem,
			Banner,
			CustomDropDownNewGroup,
			CustomDropDownNewItem,
			Caption
		};

		/**
		 * @brief Construct a new View object
		 *
		 * This constructor should only be used by user-defined views(custom views).
		 * The constructor will create a new CustomViewNode object for the view.
		 *
		 */
		View();

		/// @todo delete this constructor
		View(std::shared_ptr<ViewNode> viewNode);

		View(View::Type viewType);

		/// @brief Virtual destructor of the View class.
		virtual ~View();

		std::shared_ptr<ViewNode> GetViewNode();

		virtual std::shared_ptr<ViewNode> Build() { return nullptr; }
		virtual std::shared_ptr<View> Clone() const { return nullptr; }
		virtual bool IsListCell() { return false; }

		// Layout Modifier for all views
		View& minSize(float minWidth, float minHeight);
		View& minWidth(float minWidth);
		View& minHeight(float minHeight);

		View& maxSize(float maxWidth, float maxHeight);
		View& maxWidth(float maxWidth);
		View& maxHeight(float maxHeight);

		View& size(float width, float height);
		View& width(float width);
		View& height(float height);

    // [0, 1.f]
		View& percentWidth(float percent);
		View& percentHeight(float percent);
    View& percentMinWidth(float percent);
    View& percentMinHeight(float percent);
    View& percentMaxWidth(float percent);
    View& percentMaxHeight(float percent);

		View& borderBox();
		View& contentBox();

		View& alignment(Layout::Alignment alignment);
		View& alignmentUndefined();
		View& alignmentCenter();
		View& alignmentStart();
		View& alignmentEnd();
		View& alignmentFill();

		View& justifyContent(Layout::JustifyContent justify);
		View& justifyContentStart();
		View& justifyContentEnd();
		View& justifyContentSpaceBetween();
		View& justifyContentCenter();
		View& justifyContentSpaceAround();
		View& justifyContentSpaceEvenly();

		View& padding(const Layout::EdgeInsets& insets);
		View& padding(float left, float top, float right, float bottom);
		View& padding(const Layout::EdgeSet& edges, float length);
		View& padding(float length);
		View& paddingLeading(float length);
		View& paddingTrailing(float length);
		View& paddingTop(float length);
		View& paddingBottom(float length);

		View& wrap(Layout::Wrap wrap);

		View& background(const Bind<std::string>& color);
		View& borderColor(const Bind<std::string>& color);

		View& background(const char* color);
		View& borderColor(const char* color);

		View& borderRadius(const Bind<float>& radius);
		View& borderSize(const Bind<float>& size);
		View& borderSize(const Bind<float>& xWidth, const Bind<float>& yWidth);

		View& environment(const std::string& key, const Variant& val);

		View& hidden(const Bind<bool>& hidden);  // has space
		View& visible(const Bind<bool>& visible); // no space

		View& scaling(const Layout::ScalingFactors& scalingFactors);

		View& enabled(const Bind<bool>& enabled);

        View& forceEnabled();

		View& attribute(const std::string& attribute);

		View& coachMark(const std::string& id, const PrismTriggerType type, std::function<void(const std::string& id, PrismCoachMarkParam& param)> callback);
    
    View& fixedSize();
    View& fixedSize(bool width, bool height);
    View& layoutPriority(float priority);

		// Acc text
		View& accPrefix(const Bind<std::string>& str);
		View& accDisplay(const Bind<std::string>& str);
		View& accSuffix(const Bind<std::string>& str);

    // Accessibility
    View& accessibilityName(const Bind<std::string>& accName);
    View& accessibilityDescription(const Bind<std::string>& accDescription);
    View& accessibilityElement(AccElementMode mode);

    View& telemetrySubFeatureName(int type);
    View& telemetryEventSource(int source);
    View& telemetryFeatureName(int name);
    View& telemetryEventLocation(int location);
    View& telemetryEventName(int name);
    View& telemetryEnabled(bool enabled);
    View& telemetryData(int eventSource, int featureName, int subFeatureName, int eventLocation, int eventName, bool enabled);
		View& onChange(std::function<void()> callback);

		operator std::shared_ptr<ViewNode>() { return GetViewNode(); }

		virtual void SetHidden(bool hidden);

		// life cycle
		View& onAppear(const std::function<void()>& action);
		View& onDisappear(const std::function<void()>& action);

		View& id(const std::string& viewId);

		View& focused(const Bind<bool>& focused);

	private:
		std::shared_ptr<ViewNode> _viewNode = nullptr;
	};

}

#endif //ZUI_VIEW
