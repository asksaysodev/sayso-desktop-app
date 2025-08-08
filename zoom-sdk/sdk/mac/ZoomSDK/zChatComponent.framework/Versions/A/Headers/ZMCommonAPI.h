//
//  ZMCommonAPI.h
//  zSIPCallUI
//
//  Created by groot.ding on 12/21/19.
//  Copyright © 2019 Evelyn. All rights reserved.
//

#ifndef ZMCommonAPI_h
#define ZMCommonAPI_h

#import <Foundation/Foundation.h>
#import <cmmlib/CmmString.h>
#include <list>
#include <map>
#include <vector>
#include <set>
#include <zPTApp/SaasBeePTAppInterface.h>

NS_ASSUME_NONNULL_BEGIN

using Cmm::CString;

@class ZMFileEntity;

extern NSString * _Nonnull __attribute__((noinline)) ZM_CString2NSString (const CString & c_String);

extern NSArray * _Nonnull __attribute__((noinline)) ZM_CStringList2NSArray (const std::list<CString> & c_strings);

extern NSArray * _Nonnull __attribute__((noinline)) ZM_CStringVector2NSArray (const std::vector<CString> & c_strings);

// F must be callable with the type value of _Container and return a type that can be contained in an NSArray.
// In C++17, the callability of F(_Container::value_type) can be enforced with:
//    template <typename _Container, typename F,
//              std::enable_if_t<std::is_invocable_v<F, typename _Container::value_type>, int>>
// _Container must be a container that defines value_type, where _Container = some_container_type<T> and _Container::value_type = T.
#if __cplusplus >= 201703L
template <typename _Container, typename F,
          std::enable_if_t<std::is_invocable_v<F, typename _Container::value_type>, int>>
#else
template <typename _Container, typename F>
#endif
extern NSArray *_Nonnull __attribute__((noinline)) ZM_CContainer2NSArray(const _Container &cont, F transform) {
    if(cont.empty()) return [NSArray array];
    NSMutableArray *arr = [NSMutableArray arrayWithCapacity:cont.size()];
    for (auto obj : cont) {
        [arr addObject:transform(obj)];
    }
    return arr;
}

extern NSArray *_Nonnull __attribute__((noinline)) ZM_CStringSet2NSArray (const std::set<CString>& c_strings);

extern NSArray *_Nonnull __attribute__((noinline)) ZM_CStringUSet2NSArray (const std::unordered_set<CString>& c_strings);

extern std::vector<CString> __attribute__((noinline)) ZM_NSArray2CStringVector(NSArray <NSString *> * _Nonnull array);

extern std::set<CString> __attribute__((noinline)) ZM_NSArray2CStringSet(NSArray <NSString *> * _Nonnull array);

extern std::unordered_set<CString> __attribute__((noinline)) ZM_NSArray2CStringUSet(NSArray <NSString *> *_Nonnull array);

extern NSString * _Nonnull __attribute__((noinline)) ZM_GetOriginNSStringIfNoChange (NSString *_NonnulloriginStr, NSString *_NonnulltmpStr);


extern std::list<CString> __attribute__((noinline)) ZM_NSArray2CStringList(NSArray <NSString *> *_Nonnull array);

extern NS_ZWEBSERVICE::StyleOffset __attribute__((noinline)) ZM_FileEntity2StyleOffset(ZMFileEntity * _Nonnull fileInfo);

NS_ASSUME_NONNULL_END

#endif /* ZMCommonAPI_h */
