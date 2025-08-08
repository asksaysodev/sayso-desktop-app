#ifndef _SETTING_PANEL_TEAM_CHAT_H_
#define _SETTING_PANEL_TEAM_CHAT_H_

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

#include "zui_client_data_define.h"


class ZUIClient_API SettingPanelBlockedUsers : public zui::View
{
public:
    SettingPanelBlockedUsers();
    ~SettingPanelBlockedUsers() = default;
    
    std::shared_ptr<zui::View> Clone() const override { return std::make_shared<SettingPanelBlockedUsers>(*this); }
    std::shared_ptr<zui::ViewNode> Build() override;
    
private:
    
    zui::State<std::vector<zui::any>> _allBlockedUsers;
};

class ZUIClient_API CTeamChatSettingDataModel : public IInitializable
{
private:
    
public:
    
    static CTeamChatSettingDataModel& GetInstance();
    CTeamChatSettingDataModel();
    ~CTeamChatSettingDataModel() = default;

    virtual void initializeState() override;
    virtual void initializeStateHandler() override;

private:
  void InitIMIdleTimeSetting();

public:
  zui::State<bool> _showWhiteSidebarThemeOption = true;
    //Parameters in base view
    RadioGroupProperty _exclusiveSidebarLook = RadioGroupProperty{true,
        true,
        zui::any(0)};//0 white, 1 black
    RadioGroupProperty _radioMessageGrouping = RadioGroupProperty{true,
        true,
        zui::any(0)};//"Message grouping"
    RadioGroupProperty _radioOpenSectionsOrFolders = RadioGroupProperty{true,
        true,
        zui::any(0)};//"When opening sections or folders:"
    RadioGroupProperty _radioWhenViewingUnreadMessages = RadioGroupProperty{true,
        true,
        zui::any(0)};//"When viewing unread messages in a channel:"
    
    SwitchControl _toggleMoveMessages = SwitchControl{true,
        true,
        false};//"Move messages with new replies to the bottom of the chat"
    
    SwitchControl _toggleReadReceipts = SwitchControl{true,
        true,
        false};//"Read receipts"
    zui::State<std::string> _readReceiptsDetail = zui::State<std::string>("") ;
    
    zui::State<bool> _changeStatusVisible = true;
    zui::State<bool> _changeStatusEnable = true;
    zui::State<bool> _toggleChangeStatusOn = false;//"Change status to “Away” when inactive for:"
    zui::State<int> _dropChangeStatusIndex = 0;//"Change status to “Away” when inactive for:" minutes in int of index
    
#ifdef __MACOS__
    zui::State<bool> _bounceApplicationVisible = true;
    zui::State<bool> _bounceApplicationEnable = true;
    zui::State<bool> _toggleBounceApplicationIcon = false;//"Bounce application icon when receiving a new message" whether the toggle is on
    zui::State<int> _dropIndexBounceApplicationIcon = 0;//"Bounce application icon when receiving a new message" the index of the drop box, 0 once, 1 continuously
#endif
    
    SwitchControl _toggleShowLinkPreview = {true,
        true,
        false};//"Show link preview"
    
    zui::State<bool> _languageVisible = true;
    zui::State<bool> _languageEnable = true;
    zui::State<std::list<std::string>> _dropLanguageStrings;//"Preferred message translation language:" all languages
    zui::State<int> _dropIndexLanguage = 0;//"Preferred message translation language:"
    void SetChangeStatusMinutes(int minutes);

    //Parameters in badge viewf
    DropDownProperty _dropDMsIndex = {true,
        true,
        0};//"DMs", The index of dropview in int
    DropDownProperty _dropChannelsIndex = {true,
        true,
        0};//"Channels", The index of dropview in int
    DropDownProperty _dropMeetingChatsIndex = {true,
        true,
        0};//"Meeting chats", The index of dropview in int
    DropDownProperty _dropSharedSpacesIndex = {true,
        true,
        0};//"Shared spaces", The index of dropview in int
    DropDownProperty _dropAppsIndex = {true,
        true,
        0};//"Apps", The index of dropview in int
    DropDownProperty _dropStarredIndex = {true,
        true,
        0};//"Starred", The index of dropview in int
    DropDownProperty _dropFoldersIndex = {true,
        true,
        0};//"Folders", The index of dropview in int
    
    SwitchControl _toggleShowUnreadCountForChannels = {true,
        true,
        true};//"Show unread count for Channels", whether the toggle is on
    SwitchControl _toggleShowUnreadCountForMeetingChats = {true,
        true,
        true};//"Show unread count for meeting chats", whether the toggle is on
    
    //The page controller
    zui::State<bool> _managingBadgeSettings = false;

    //Team Chat enabled
    zui::State<bool> _teamChatEnabled = true;
};

class ZUIClient_API SettingPanelTeamChatBase : public zui::View
{
public:
    SettingPanelTeamChatBase();
    ~SettingPanelTeamChatBase() = default;
    
    std::shared_ptr<zui::View> Clone() const override { return std::make_shared<SettingPanelTeamChatBase>(*this); }
    std::shared_ptr<zui::ViewNode> Build() override;
    
private:
    zui::View BuildLeftSidebarBehaviorGroup();
    zui::View BuildUnreadMessagesGroup();
    zui::View BuildChatPreferencesGroup();
    zui::View BuildMessageTranslationGroup();
  
    zui::State<std::string> _bkColor_translation = std::string(zui::fillDefault);

};

class ZUIClient_API SettingPanelTeamChatDisabled : public zui::View
{
public:
    SettingPanelTeamChatDisabled() = default;
    ~SettingPanelTeamChatDisabled() = default;

    std::shared_ptr<zui::View> Clone() const override { return std::make_shared<SettingPanelTeamChatDisabled>(*this); }
    std::shared_ptr<zui::ViewNode> Build() override;

private:
    zui::View BuildChatPreferencesGroup();

};


class ZUIClient_API SettingPanelTeamChatBadge : public zui::View
{
public:
    SettingPanelTeamChatBadge();
    ~SettingPanelTeamChatBadge() = default;
    
    std::shared_ptr<zui::View> Clone() const override { return std::make_shared<SettingPanelTeamChatBadge>(*this); }
    std::shared_ptr<zui::ViewNode> Build() override;
    
private:
    zui::View BuildNotifierToggle(const zui::Bind<std::string>& label, const zui::Bind<unsigned int>& value, zui::State<std::string>& bkcolor, const SwitchControl& property);
    
private:
    zui::State<std::string> _bkColorChannels = std::string(zui::fillDefault);
    zui::State<std::string> _bkColorMeetingChats = std::string(zui::fillDefault);
};

class ZUIClient_API SettingPanelTeamChat : public zui::View
{
public:
    SettingPanelTeamChat();
    ~SettingPanelTeamChat() = default;
    
    std::shared_ptr<zui::View> Clone() const override { return std::make_shared<SettingPanelTeamChat>(*this); }
    std::shared_ptr<zui::ViewNode> Build() override;
    
private:
    
};

#define TeamChatPanelDataModel CTeamChatSettingDataModel::GetInstance()

#endif
