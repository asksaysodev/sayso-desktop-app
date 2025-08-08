#ifndef _WHITEBOARD_SHARE_OPTION_H_
#define _WHITEBOARD_SHARE_OPTION_H_

#ifdef _WIN32
#include "zUI/view.h"
#include "zUIClient/zUI_stdafx.h"
#elif defined __MACOS__
#import <zUI/view.h>
#import <zUIClient/zUI_stdafx.h>
#else
#include "view.h"
#include "zUI_stdafx.h"
#endif

class ZUIClient_API ShareWhiteboardSetting : public zui::View
{
public:
    ShareWhiteboardSetting();
    ShareWhiteboardSetting(const ShareWhiteboardSetting& view);
    ~ShareWhiteboardSetting() = default;

    std::shared_ptr<zui::View> Clone() const override { return std::make_shared<ShareWhiteboardSetting>(*this); }
    std::shared_ptr<zui::ViewNode> Build() override;

    void Init();

private:
    void UpdateRadioButtonEnable();
    void UpdateLockTipsVisible();
    void ApplyWhiteboardShareOption();
    void SetSharePermissions();

private:
    zui::State<bool> _isWebinar = false;
    zui::State<bool> _participantsCanShareWhiteboard = false;
    zui::State<bool> _whiteboardShareNotLocked = true;
    zui::State<bool> _showMoreOwnerShip = true;
    zui::State<bool> _lockTextHidden = false;
    zui::State<bool> _allowAllTofacilitate = false;
    zui::State<bool> _lockTipsVisible = false;
    // radiobutton
    zui::State<bool> _radioButtonEnable = true;
    zui::State<zui::any> _whoCanBreakShare = zui::any(0);
    zui::State<zui::any> _newWbRoles = zui::any(0);
};

#endif
