#ifndef _ZUI_MODULE_PCH_H_
#define _ZUI_MODULE_PCH_H_

#if defined _WIN32
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif

#if (defined ANDROID || defined _WIN32 ||  defined __MACOS__ || defined __LINUX_CLIENT__)
#include "cmmlib/CmmLogging.h"
#include "cmmlib/CmmThread.h"
#include "cmmlib/CmmAppContext.h"
#endif

#if defined _WIN32
#if defined zUIClient_EXPORTS
#define ZUIClient_API __declspec(dllexport)
#elif defined zUIClient_IMPORTS
#define ZUIClient_API __declspec(dllimport)
#else
#define ZUIClient_API
#endif
#else
#define ZUIClient_API __attribute__((visibility("default")))
#endif

#endif //_ZUI_MODULE_PCH_H_
