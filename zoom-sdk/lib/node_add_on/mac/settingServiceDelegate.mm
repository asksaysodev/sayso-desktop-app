#import "settingServiceDelegate.h"
#include "zoom_node_addon.h"
#include "sdk_native_error.h"
extern ZNativeSDKWrap _g_native_wrap;
@implementation settingServiceDelegate

+(settingServiceDelegate *)share
{
    static settingServiceDelegate *delegate = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        
        delegate = [[settingServiceDelegate alloc]init];
    });
    return delegate;
}

-(void)onSelectedCameraChanged:(NSString *)deviceID
{
    ZoomSDKSettingService *setting = [[ZoomSDK sharedSDK] getSettingService];
    if (!setting) {
        return;
    }
    ZoomSDKVideoSetting *video = [setting getVideoSetting];
    if (!video) {
        return;
    }
    NSArray *arr = [video getCameraList];
    if (!arr) {
        return;
    }
    ZNList<ZNCameraInfo> list;
    for (SDKDeviceInfo *info in arr) {
        NSString *str = [info getDeviceName];
        NSString *ID = [info getDeviceID];
        bool isSelected = [info  isSelectedDevice];
        ZNCameraInfo cameraInfo;
        if (str) {
            cameraInfo.deviceName = str.UTF8String;
        }else{
            cameraInfo.deviceName = "";
        }
        if (ID) {
            cameraInfo.deviceId = ID.UTF8String;
        }else{
            cameraInfo.deviceId = "";
        }
        cameraInfo.isSelectedDevice = isSelected;
        list.push_back(cameraInfo);
    }
}

-(void)onSelectedSpeakerDeviceChanged
{
    ZoomSDKSettingService *setting = [[ZoomSDK sharedSDK] getSettingService];
    if (!setting) {
        return;
    }
    ZoomSDKAudioSetting *audio = [setting getAudioSetting];
    if (!audio) {
        return;
    }
    NSArray *arr = [audio getAudioDeviceList:NO];
    if (!arr) {
        return;
    }
    ZNList<ZNSpeakerInfo> list;
    for (SDKDeviceInfo *info in arr) {
        NSString *str = [info getDeviceName];
        NSString *ID = [info getDeviceID];
        bool isSelected = [info  isSelectedDevice];
        ZNSpeakerInfo speakinfo;
        if (str) {
            speakinfo.deviceName = str.UTF8String;
        }else{
            speakinfo.deviceName = "";
        }
        if (ID) {
            speakinfo.deviceId = ID.UTF8String;
        }else{
            speakinfo.deviceId = "";
        }
        
        speakinfo.isSelectedDevice = isSelected;
        list.push_back(speakinfo);
    }
    _g_native_wrap.GetSettingServiceWrap().GetSettingAudioCtrl().onComputerSpeakerDeviceChanged(list);
}

-(void)onSelectedMicDeviceChanged
{
    ZoomSDKSettingService *setting = [[ZoomSDK sharedSDK] getSettingService];
    if (!setting) {
        return;
    }
    ZoomSDKAudioSetting *audio = [setting getAudioSetting];
    if (!audio) {
        return;
    }
    NSArray *arr = [audio getAudioDeviceList:YES];
    if (!arr) {
        return;
    }
    ZNList<ZNMicInfo> list;
    for (SDKDeviceInfo *info in arr) {
        NSString *str = [info getDeviceName];
        NSString *ID = [info getDeviceID];
        bool isSelected = [info  isSelectedDevice];
        ZNMicInfo micinfo;
        if (str) {
            micinfo.deviceName = str.UTF8String;
        }else{
            micinfo.deviceName = "";
        }
        if (ID) {
            micinfo.deviceId = ID.UTF8String;
        }else{
            micinfo.deviceId = "";
        }
        
        micinfo.isSelectedDevice = isSelected;
        list.push_back(micinfo);
    }
    _g_native_wrap.GetSettingServiceWrap().GetSettingAudioCtrl().onComputerMicDeviceChanged(list);
}

-(void)onNotifyCloudRecordingStorageInfo:(long long)totalSize usedSize:(long long)usedSize isAllowExceedStorage:(BOOL)allowExceedStorage
{
    _g_native_wrap.GetSettingServiceWrap().GetSettingRecordingCtrl().onCloudRecordingStorageInfo(totalSize, usedSize, allowExceedStorage);
}

-(void)onMeetingWallpaperChanged:(ZoomSDKWallpaperItem * _Nullable)item
{
    nativeErrorTypeHelp Help_type;
    ZNWallpaperItem zn_item = Help_type.ConvertSDKIWallpaperItemToZNWallpaperItem(item);
    _g_native_wrap.GetSettingServiceWrap().GetSettingWallpaperCtrl().onMeetingWallpaperChanged(zn_item);
}

-(void)onMeetingWallpaperImageDownloadStatus:(ZoomSDKWallpaperSettingStatus)status wallpaper:(NSString *)wallpaperId
{
    nativeErrorTypeHelp Help_type;
    ZNZoomSDKWallpaperSettingStatus zn_status = Help_type.ZNSDKWallpaperSettingStatusMap(status);
    ZoomSTRING zn_wallpaperId = wallpaperId ? [wallpaperId UTF8String] : "";
    _g_native_wrap.GetSettingServiceWrap().GetSettingWallpaperCtrl().onMeetingWallpaperImageDownloadStatus(zn_status, zn_wallpaperId);
}

-(void)onPersonalWallpaperChanged:(ZoomSDKWallpaperItem * _Nullable)item
{
    nativeErrorTypeHelp Help_type;
    ZNWallpaperItem zn_item = Help_type.ConvertSDKIWallpaperItemToZNWallpaperItem(item);
    _g_native_wrap.GetSettingServiceWrap().GetSettingWallpaperCtrl().onPersonalWallpaperChanged(zn_item);
}

-(void)onPersonalWallpaperImageDownloadStatus:(ZoomSDKWallpaperSettingStatus)status wallpaper:(NSString *)wallpaperId
{
    nativeErrorTypeHelp Help_type;
    ZNZoomSDKWallpaperSettingStatus zn_status = Help_type.ZNSDKWallpaperSettingStatusMap(status);
    ZoomSTRING zn_wallpaperId = wallpaperId ? [wallpaperId UTF8String] : "";
    _g_native_wrap.GetSettingServiceWrap().GetSettingWallpaperCtrl().onPersonalWallpaperImageDownloadStatus(zn_status, zn_wallpaperId);
}

-(void)onVBImageDidDownloaded:(NSString*)filePath
{
    _g_native_wrap.GetSettingServiceWrap().GetSettingVirtualBGCtrl().onVBImageDidDownloaded();
}

-(void)onGreenVBDidUpdateWithReplaceColor:(NSColor*)selectedColor
{
    if (selectedColor) {
        ZNZoomSDKColor zn_color;
        CGFloat red = 0, green = 0, blue = 0, alpha = 0;
        [selectedColor getRed:&red green:&green blue:&blue alpha:&alpha];
        zn_color.red = (int)(red * 255);
        zn_color.green = (int)(green * 255);
        zn_color.blue = (int)(blue * 255);
        _g_native_wrap.GetSettingServiceWrap().GetSettingVirtualBGCtrl().onGreenVBDidUpdateWithReplaceColor(zn_color);
    }
}

-(void)onSelectedVBImageChanged
{
    _g_native_wrap.GetSettingServiceWrap().GetSettingVirtualBGCtrl().onSelectedVBImageChanged();
}

-(void)onVBVideoUploadedResult:(BOOL)success failedError:(ZoomSDKSettingVBVideoError)error
{
    nativeErrorTypeHelp Help_type;
    ZNVBVideoError zn_error = Help_type.ZNVBVideoErrorMap(error);
    _g_native_wrap.GetSettingServiceWrap().GetSettingVirtualBGCtrl().onVBVideoUploadedResult(success, zn_error);
}

@end
