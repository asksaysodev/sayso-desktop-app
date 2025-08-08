//
//  ZMPTHelper.h
//  PTUIBridge
//
//  Created by fistice on 2024/10/12.
//

#ifndef IZMPTHelper_h
#define IZMPTHelper_h

#import <zPTApp/PTService/IPTAppWebService.h>
#import <zPTApp/PTService/IPTUrlActionService.h>
#import <zPTApp/PTService/IPTMarketService.h>
#import <zPTApp/PTService/IPTWhiteboardsService.h>
#import <zPTApp/PTService/IPTDocsService.h>
#import <zPTApp/PTService/IPTZDMServiceMgr.h>
#import <zPTApp/PTService/IPTTasksService.h>
#import <zPTApp/PTService/IPTZoomConfService.h>
#import <zPTApp/PTService/IPTZoomEventsService.h>
#import <zPTApp/PTService/IPTCalendarService.h>
#import <zPTApp/PTService/IPTZappService.h>
#import <zPTApp/PTService/IPTEventTrackerService.h>

#import <zVideoApp/CmmShareAPI.h>
#import <zVideoApp/CmmConfAPI.h>
#import <PTUIBridge/ZMPTRouterCenter.h>
class ZMPTSink;
class IZPPTAppSink;
@protocol IZMPTHelper <NSObject>
- (ZMPTSink *)getPTSinkInstance;
- (void)setPTSinkInstance:(ZMPTSink *)instance;
- (void)setZPPTAppSink:(IZPPTAppSink*)appSink;

- (BOOL)isInGR;
- (BOOL)isInWebinarBO;
- (BOOL)isInPBO;
- (BOOL)isInSpotsBO;
- (ICmmConfMgrAPI*)getConfApi;
- (ICmmConfInst *)getConfInst;
- (ICmmConfContext*)getConfContext;
- (ICmmConfContext*)getCurrentConfContext;
- (ICmmVideoAPI*)getVideoApi;
- (ICmmVideoAPI*)getCurrentVideoApi;
- (ICmmAudioAPI*)getAudioApi;
- (ISBWebServiceAPI*)getWebServiceApi;
- (ISBPTAppAPI*)getPTAppApi;
- (CPTServicePtr<IPTWhiteboardsService>)whiteboardsService;
- (ICmmRecordAPI*)getRecordApi;
- (ICmmLocalRecordAPI*)getLocalRecordApi;
- (ICmmShareMgr *)getShareMgr;
- (ICmmASShareMgr *)getASShareMgr;
- (ICmmShareConfig *)getShareConfig;
- (ICmmASSharePortMgr *)getASSharePortMgr;
- (ICmmASShareSourceMgr *)getASShareSourceMgr;
- (ICmmASShareRenderMgr *)getASShareRenderMgr;
- (ICmmASSharePort *)getASShareDefaultPort;
- (ICmmConfStatus*)getConfStatus;
- (ICmmVideoBkgndImageMgr*)getVideoVirtualBkMgr;
- (ICmmVideoFaceMakeupDataMgr*)getVideoFaceMakeupMgr;
- (BOOL)isAdminOrOwnerRole;
- (ICmmUser*)getMySelf;
- (ICmmVideoCustom3DAvatarDataMgr *)getCustom3DAvatarDataMgr;
- (ICmmPersonalWallpaperConfigMgr *)getPersonalWallpaperConfigMgr;
- (ISSBPTUserProfile*)getUserProfile;
- (NS_ZOOM_MESSAGER::IZoomMessenger*)getMessenger;
- (ISBPTMeetingHelper*)getMeetingHelper;
- (ISBPTSettingHelper*)getSettingHelper;
- (IFBAuthHelper*)getFBAuthHelper;

//MARK: Common Layer APIS
- (CPTServicePtr<IPTAppWebService>)PTAppWebService;

- (CPTServicePtr<IPTUrlActionService>)PTUrlActionService;

- (CPTServicePtr<IPTAccountService>)PTAccountService;

- (CPTServicePtr<IPTMarketService>)PTMarketService;

- (CPTServicePtr<IPTAuthenticationService>)PTAuthenticationService;

- (CPTServicePtr<IPTZDMServiceMgr>)PTZDMService;

- (CPTServicePtr<IPTDocsService>)PTDocsService;

- (CPTServicePtr<IPTTasksService>)PTTasksService;

- (CPTServicePtr<IPTZoomConfService>)PTAppConfService;

- (CPTServicePtr<IPTZoomEventsService>)PTZoomEventsService;

- (CPTServicePtr<IPTCalendarService>)PTCalendarService;

- (CPTServicePtr<IPTZappService>)PTZappService;

- (CPTServicePtr<IPTEventTrackerService>)PTEventTrackerService;

@end

#define ZMPTHelper ZMSharedFor(IZMPTHelper)

#endif /* IZMPTHelper_h */

