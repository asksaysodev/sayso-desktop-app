#ifndef __ZUI_BASE__
#define __ZUI_BASE__

#include "zui_data_define.h"
#include "horizontal_stack.h"
#include "vertical_stack.h"
#include "scrollview.h"
#include "basic_button.h"
#include "checkbox.h"
#include "radio_button.h"
#include "text.h"
#include "scene.h"
#include "app.h"
#include "environment.h"
#include "spacer.h"
#include "app.h"
#include "dropdown.h"
#include "button.h"
#include "option.h"
#include "link.h"
#include "accordion.h"
#include "avatar.h"
#include "toggle.h"
#include "slider.h"
#include "tab.h"
#include "popover.h"
#include "input_field.h"
#include "badge.h"
#include "loading.h"

#include "vertical_stack.h"
#include "environment.h"
#include "view.h"
#include "divider.h"
#include "progress.h"
#include "block_progress.h"
#include "tooltip.h"
#include "icon_button.h"
#include "conditional.h"
#include "types.h"
#include "toast.h"
#include "loop.h"
#include "image.h"
#include "toast.h"
#include "list.h"
#include "notifier.h"

#include "banner.h"
#include "caption.h"

#include "window.h"

#ifdef __MACOS__
#import <zUI/independent_view.h>
#import <zUI/external_view.h>
#elif defined __LINUX_CLIENT__
#include "independent_view.h"
#include "external_view.h"
#elif defined WIN32
#include "interoperability/independent_view.h"
#include "interoperability/external_view.h"
#include "windows/resource_zui.h"
#endif

#endif
