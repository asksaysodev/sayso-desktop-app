//
//  SwiftViewInclude.h
//  zUI
//
//  Created by Rob Christenson on 9/13/24.
//

#ifndef SwiftViewInclude_h
#define SwiftViewInclude_h

#ifdef __cplusplus
// Headers for custom views for swift need to be added to 
// common/include/zUI/interoperability/macos/swift_view_headers/
// and included here for c++ common layer
#include "IndependentViewExample.h"
#endif

// Create mapping between enum and view ctor for that enum
// see SwiftViewBridge.h for usage on getting NSView from View in swift
#define CUSTOM_VIEW_LIST(DECLARE) \
DECLARE(kIndependentViewExample, IndependentViewExample()) \

enum CustomViews {
#define ENUM_ENTRY(NAME, TEXT) NAME,
    CUSTOM_VIEW_LIST(ENUM_ENTRY)
#undef ENUM_ENTRY
};


#endif /* SwiftViewInclude_h */
