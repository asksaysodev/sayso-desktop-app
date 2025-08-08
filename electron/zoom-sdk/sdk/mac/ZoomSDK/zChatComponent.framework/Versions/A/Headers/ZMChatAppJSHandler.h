//
//  ZMChatAppJSHandler.h
//  ChatUI
//
//  Created by Zoro.Fu on 2022/8/24.
//  Copyright © 2022 Zoom. All rights reserved.
//

#include "zApp/IZappServiceInterface.h"

class ZMChatAppJSHandler : public IZappJSHandler {
public:
    virtual void OnCallJsSdkApi(const CString& jsReqId, const NS_ZOOM_APPS::Js2CppApiParams& apiParams);
};

