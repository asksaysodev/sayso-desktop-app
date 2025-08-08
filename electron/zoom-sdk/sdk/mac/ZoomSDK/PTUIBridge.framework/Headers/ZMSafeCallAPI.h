//
//  ZMSafeCallAPI.h
//  PTUIBridge
//
//  Created by Slevin Zhang on 2025/4/29.
//

#ifndef ZMSafeCallAPI_h
#define ZMSafeCallAPI_h

template<typename T, typename MethodPtr, typename R>
R SafeCall(T* ptr, MethodPtr method, R defaultValue) {
    return ptr ? (ptr->*method)() : defaultValue;
}

template<typename T, typename MethodPtr, typename... Args>
void SafeCallVoid(T* ptr, MethodPtr method, Args&&... args) {
    if (ptr) {
        (ptr->*method)(std::forward<Args>(args)...);
    }
}

template<typename T, typename MethodPtr, typename Out>
Out SafeFill(T* ptr, MethodPtr method, Out defaultValue = Out{}) {
    Out result = defaultValue;
    if (ptr) {
        (ptr->*method)(result);
    }
    return result;
}

using API = ISBPTAppAPI;

template<typename MethodPtr>
auto SafeCallAPI(MethodPtr method, decltype((std::declval<API*>()->*std::declval<MethodPtr>())()) defaultValue) {
    using R = decltype((std::declval<API*>()->*method)());
    API* ptAppAPI = [ZMPTHelper getPTAppApi];
    auto derivedMethod = static_cast<R (API::*)()>(method);
    return ptAppAPI ? (ptAppAPI->*derivedMethod)() : defaultValue;
}

template<typename T, typename MethodPtr, typename R>
R SafeCall(CPTServicePtr<T> wrapper, MethodPtr method, R defaultValue) {
    T* ptr = wrapper.UnboxInterface();
    return ptr ? (ptr->*method)() : defaultValue;
}

#endif
