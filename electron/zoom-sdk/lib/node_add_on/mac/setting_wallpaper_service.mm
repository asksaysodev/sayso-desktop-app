#include "setting_wallpaper_wrap_core.h"
#include "meeting_service_wrap_core.h"
#import "Header_include.h"
#include "sdk_native_error.h"
#import "settingServiceDelegate.h"

ZSettingWallpaperWrap::ZSettingWallpaperWrap()
{
    m_pSink = 0;
}

ZSettingWallpaperWrap::~ZSettingWallpaperWrap()
{
    [[[[[ZoomSDK sharedSDK] getSettingService] getGeneralSetting] getWallpaperController] setDelegate:nil];
    m_pSink = 0;
}

void ZSettingWallpaperWrap::SetSink(ZNativeSDKWallpaperSettingContextWrapSink* pSink)
{
    [[[[[ZoomSDK sharedSDK] getSettingService] getGeneralSetting] getWallpaperController] setDelegate:[settingServiceDelegate share]];
    m_pSink = pSink;
}
void ZSettingWallpaperWrap::Init()
{

}

void ZSettingWallpaperWrap::Uninit()
{

}

bool ZSettingWallpaperWrap::IsMeetingWallpaperEnabled()
{
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return false;
    }
    ZoomSDKGeneralSetting *generalSetting = [service getGeneralSetting];
    if (!generalSetting) {
        return false;
    }
    ZoomSDKWallpaperController *wallpaperController = [generalSetting getWallpaperController];
    if (!wallpaperController) {
        return false;
    }
    return [wallpaperController isMeetingWallpaperEnabled];
}

bool ZSettingWallpaperWrap::IsMeetingWallpaperThumbsReady()
{
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return false;
    }
    ZoomSDKGeneralSetting *generalSetting = [service getGeneralSetting];
    if (!generalSetting) {
        return false;
    }
    ZoomSDKWallpaperController *wallpaperController = [generalSetting getWallpaperController];
    if (!wallpaperController) {
        return false;
    }
    return [wallpaperController isMeetingWallpaperThumbsReady];
}

ZNWallpaperItem ZSettingWallpaperWrap::GetCurrentMeetingWallpaperItem()
{
    ZNWallpaperItem zn_wallpaperItem;
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return zn_wallpaperItem;
    }
    ZoomSDKGeneralSetting *generalSetting = [service getGeneralSetting];
    if (!generalSetting) {
        return zn_wallpaperItem;
    }
    ZoomSDKWallpaperController *wallpaperController = [generalSetting getWallpaperController];
    if (!wallpaperController) {
        return zn_wallpaperItem;
    }
    ZoomSDKWallpaperItem *wallpaperItem = [wallpaperController getCurrentMeetingWallpaperItem];
    nativeErrorTypeHelp help;
    return help.ConvertSDKIWallpaperItemToZNWallpaperItem(wallpaperItem);
}

ZNList<ZNWallpaperItem> ZSettingWallpaperWrap::GetMeetingWallpaperList()
{
    ZNList<ZNWallpaperItem> zn_list;
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return zn_list;
    }
    ZoomSDKGeneralSetting *generalSetting = [service getGeneralSetting];
    if (!generalSetting) {
        return zn_list;
    }
    ZoomSDKWallpaperController *wallpaperController = [generalSetting getWallpaperController];
    if (!wallpaperController) {
        return zn_list;
    }
    NSArray <ZoomSDKWallpaperItem *> *listArr = [wallpaperController getMeetingWallpaperList];
    nativeErrorTypeHelp help;
    for (ZoomSDKWallpaperItem *wallpaperItem in listArr) {
        if (wallpaperItem) {
            ZNWallpaperItem zn_wallpaperItem = help.ConvertSDKIWallpaperItemToZNWallpaperItem(wallpaperItem);
            zn_list.push_back(zn_wallpaperItem);
        }
    }
    return zn_list;
}

ZNSDKError ZSettingWallpaperWrap::SetMeetingWallpaper(ZoomSTRING zn_wallpaperId, int transparency, ZNZoomSDKWallpaperLayoutMode mode)
{
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    ZoomSDKGeneralSetting *generalSetting = [service getGeneralSetting];
    if (!generalSetting) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    ZoomSDKWallpaperController *wallpaperController = [generalSetting getWallpaperController];
    if (!wallpaperController)
        return ZNSDKERR_SERVICE_FAILED;
    
    NSString *wallpaperId = [NSString stringWithUTF8String:zn_wallpaperId.c_str()];
    ZoomSDKWallpaperItem *item = [wallpaperController getPersonalWallpaperItemById:wallpaperId];
    item.transparency = transparency;
    nativeErrorTypeHelp help;
    item.layoutMode = help.ZoomSDKWallpaperLayoutModeMap(mode);
    ZoomSDKError ret = [wallpaperController setMeetingWallpaper:item];
    return help.ZoomSDKErrorType(ret);
}

ZNWallpaperItem ZSettingWallpaperWrap::GetMeetingWallpaperItemByID(ZoomSTRING zn_wallpaperId)
{
    ZNWallpaperItem zn_wallpaperItem;
    if (zn_wallpaperId.empty()) {
        return zn_wallpaperItem;
    }
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return zn_wallpaperItem;
    }
    ZoomSDKGeneralSetting *generalSetting = [service getGeneralSetting];
    if (!generalSetting) {
        return zn_wallpaperItem;
    }
    ZoomSDKWallpaperController *wallpaperController = [generalSetting getWallpaperController];
    if (!wallpaperController) {
        return zn_wallpaperItem;
    }
    
    NSString *wallpaperId = [NSString stringWithUTF8String:zn_wallpaperId.c_str()];
    ZoomSDKWallpaperItem *wallpaperItem = [wallpaperController getPersonalWallpaperItemById:wallpaperId];
    
    nativeErrorTypeHelp help;
    return help.ConvertSDKIWallpaperItemToZNWallpaperItem(wallpaperItem);
}

bool ZSettingWallpaperWrap::IsPersonalWallpaperEnabled()
{
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return false;
    }
    ZoomSDKGeneralSetting *generalSetting = [service getGeneralSetting];
    if (!generalSetting) {
        return false;
    }
    ZoomSDKWallpaperController *wallpaperController = [generalSetting getWallpaperController];
    if (!wallpaperController) {
        return false;
    }

    return [wallpaperController isPersonalWallpaperEnabled];
}

ZNWallpaperItem ZSettingWallpaperWrap::GetCurrentPersonalWallpaperItem()
{
    ZNWallpaperItem zn_wallpaperItem;
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return zn_wallpaperItem;
    }
    ZoomSDKGeneralSetting *generalSetting = [service getGeneralSetting];
    if (!generalSetting) {
        return zn_wallpaperItem;
    }
    ZoomSDKWallpaperController *wallpaperController = [generalSetting getWallpaperController];
    if (!wallpaperController) {
        return zn_wallpaperItem;
    }
    ZoomSDKWallpaperItem *wallpaperItem = [wallpaperController getCurrentPersonalWallpaperItem];
    nativeErrorTypeHelp help;
    return help.ConvertSDKIWallpaperItemToZNWallpaperItem(wallpaperItem);
}

ZNList<ZNWallpaperItem> ZSettingWallpaperWrap::GetPersonalWallpaperList()
{
    ZNList<ZNWallpaperItem> zn_list;
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return zn_list;
    }
    ZoomSDKGeneralSetting *generalSetting = [service getGeneralSetting];
    if (!generalSetting) {
        return zn_list;
    }
    ZoomSDKWallpaperController *wallpaperController = [generalSetting getWallpaperController];
    if (!wallpaperController) {
        return zn_list;
    }
    NSArray <ZoomSDKWallpaperItem *> *listArr = [wallpaperController getPersonalWallpaperList];
    nativeErrorTypeHelp help;
    for (ZoomSDKWallpaperItem *wallpaperItem in listArr) {
        if (wallpaperItem) {
            ZNWallpaperItem zn_wallpaperItem = help.ConvertSDKIWallpaperItemToZNWallpaperItem(wallpaperItem);
            zn_list.push_back(zn_wallpaperItem);
        }
    }
    return zn_list;
}

ZNSDKError ZSettingWallpaperWrap::SetPersonalWallpaper(ZoomSTRING zn_wallpaperId, int transparency, ZNZoomSDKWallpaperLayoutMode mode)
{
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    ZoomSDKGeneralSetting *generalSetting = [service getGeneralSetting];
    if (!generalSetting) {
        return ZNSDKERR_SERVICE_FAILED;
    }
    ZoomSDKWallpaperController *wallpaperController = [generalSetting getWallpaperController];
    if (!wallpaperController)
        return ZNSDKERR_SERVICE_FAILED;
    
    NSString *wallpaperId = [NSString stringWithUTF8String:zn_wallpaperId.c_str()];
    ZoomSDKWallpaperItem *item = [wallpaperController getPersonalWallpaperItemById:wallpaperId];
    item.transparency = transparency;
    nativeErrorTypeHelp help;
    item.layoutMode = help.ZoomSDKWallpaperLayoutModeMap(mode);
    ZoomSDKError ret = [wallpaperController setPersonalWallpaper:item];
    return help.ZoomSDKErrorType(ret);
}

ZNWallpaperItem ZSettingWallpaperWrap::GetPersonalWallpaperItemByID(ZoomSTRING zn_wallpaperId)
{
    ZNWallpaperItem zn_wallpaperItem;
    if (zn_wallpaperId.empty()) {
        return zn_wallpaperItem;
    }
    
    ZoomSDKSettingService *service = [[ZoomSDK sharedSDK] getSettingService];
    if (!service) {
        return zn_wallpaperItem;
    }
    ZoomSDKGeneralSetting *generalSetting = [service getGeneralSetting];
    if (!generalSetting) {
        return zn_wallpaperItem;
    }
    ZoomSDKWallpaperController *wallpaperController = [generalSetting getWallpaperController];
    if (!wallpaperController) {
        return zn_wallpaperItem;
    }
    
    NSString *wallpaperId = [NSString stringWithUTF8String:zn_wallpaperId.c_str()];
    ZoomSDKWallpaperItem *wallpaperItem = [wallpaperController getPersonalWallpaperItemById:wallpaperId];
    
    nativeErrorTypeHelp help;
    return help.ConvertSDKIWallpaperItemToZNWallpaperItem(wallpaperItem);
}


//callback
void ZSettingWallpaperWrap::onMeetingWallpaperChanged(ZNWallpaperItem item)
{
    if (m_pSink) {
        m_pSink->onMeetingWallpaperChanged(item);
    }
}
void ZSettingWallpaperWrap::onMeetingWallpaperImageDownloadStatus(ZNZoomSDKWallpaperSettingStatus status, ZoomSTRING wallpaperId)
{
    if (m_pSink) {
        m_pSink->onMeetingWallpaperImageDownloadStatus(status, wallpaperId);
    }
}
void ZSettingWallpaperWrap::onPersonalWallpaperChanged(ZNWallpaperItem item)
{
    if (m_pSink) {
        m_pSink->onPersonalWallpaperChanged(item);
    }
}
void ZSettingWallpaperWrap::onPersonalWallpaperImageDownloadStatus(ZNZoomSDKWallpaperSettingStatus status, ZoomSTRING wallpaperId)
{
    if (m_pSink) {
        m_pSink->onPersonalWallpaperImageDownloadStatus(status, wallpaperId);
    }
}