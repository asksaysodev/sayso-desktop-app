//
//  ZMIZMessageHelper.h
//  ChatUI
//
//  Created by Groot Ding on 2022/5/18.
//  Copyright © 2022 Zoom. All rights reserved.
//

#ifndef ZMIZMessageHelper_h
#define ZMIZMessageHelper_h

#import <Foundation/Foundation.h>
#include <zPTApp/SaasBeePTAppInterface.h>
#include <zPTApp/PTService/IPTUrlActionService.h>

extern BOOL __attribute__((noinline)) IsZoomMeetChannelID (NSString * _Nonnull sessionId);
extern BOOL __attribute__((noinline)) IsZoomMeetSubChannelID (NSString * _Nonnull sessionId);

extern ISBPTAppAPI * _Nullable __attribute__((noinline)) GetMeetingPTAppAPI(void);


extern NS_SIPCALL_APP::ISIPVMChatIntegration * _Nullable __attribute__((noinline)) GetSIPVoiceMailIntegration(void);

extern ISBPTAppAPI * _Nullable __attribute__((noinline)) GetPTAppAPIBySessionId (NSString * _Nullable sessionId);

extern BOOL __attribute__((noinline)) IsPTZappServiceReady(void);

#ifdef ENABLE_ZAPP
extern IZappService * _Nullable __attribute__((noinline)) GetConfAppAPI (void);
#endif

extern NS_ZOOM_MESSAGER::IZoomMessenger * _Nullable __attribute__((noinline)) GetIMMessenger(void);

extern NS_ZOOM_MESSAGER::IZoomMessenger * _Nullable __attribute__((noinline)) GetIZMessengerBySessionId (NSString * _Nullable sessionId);

extern NS_ZOOM_MESSAGER::ThreadDataProvider * _Nullable __attribute__((noinline)) GetThreadDataProviderBySessionId (NSString * _Nullable sessionId);

extern NS_ZOOM_MESSAGER::IZoomChatSession * _Nullable __attribute__((noinline)) GetIZSessionById (NSString * _Nonnull sessionId);

extern NS_ZOOM_MESSAGER::IZoomGroup * _Nullable __attribute__((noinline)) GetIZGroupById (NSString * _Nonnull groupId);

extern NS_ZOOM_MESSAGER::IZoomBuddy * _Nullable __attribute__((noinline)) GetIZoomBuddy (NSString * _Nonnull jid,NSString * _Nullable sessionId);

extern NS_ZOOM_MESSAGER::IZoomMessage * _Nullable __attribute__((noinline)) GetIZoomMessage (NSString * _Nonnull messageId,NSString * _Nonnull sessionId);

extern NS_ZOOM_MESSAGER::IZoomMessage * _Nullable __attribute__((noinline)) GetIZoomMessage (unsigned long long svrSideTime,NSString * _Nonnull sessionId);

extern NS_ZOOM_MESSAGER::IZoomMessage * _Nullable __attribute__((noinline)) GetIZoomMessage (unsigned long long svrSideTime,NSString * _Nonnull sessionId,BOOL useDB);

extern NS_ZOOM_MESSAGER::FileContentMgr * _Nullable __attribute__((noinline)) GetFileContentMgr(void);

extern NS_ZOOM_MESSAGER::FileContentMgr * _Nullable __attribute__((noinline)) GetFileContentMgrBySessoinId(NSString * _Nullable sessionId);

extern NS_ZOOM_MESSAGER::IZoomMessageTemplate * _Nullable __attribute__((noinline)) GetZoomMessageTemplate(NSString * _Nullable sessionId);

extern NS_ZOOM_MESSAGER::IMessageTemplate * _Nullable __attribute__((noinline)) GetMessageTemplateWithIndex(NSString * _Nullable messageId, NSString * _Nullable sessionId, int index);

extern NS_ZOOM_MESSAGER::IMessageTemplate * _Nullable __attribute__((noinline)) GetMessageTemplateWithIndexEx(NSString * _Nullable messageId, NSString * _Nullable sessionId, int index, BOOL async);

extern NS_ZOOM_MESSAGER::ISearchMgr * _Nullable __attribute__((noinline)) GetISearchMgr(void);
#ifdef ENABLE_ZAPP
extern UnifyWebView::IAppBridge * _Nullable __attribute__((noinline)) GetAppBridgeAPI(void);
#endif

extern ISBPTMeetingHelper * _Nullable __attribute__((noinline)) GetMeetingHelper(void);

extern NS_IM_NOTIFICATION::NotificationSettingMgr *_Nullable __attribute__((noinline)) GetNotificationSettingMgr(void);

extern NS_EMBEDDED_FILE_INTEGRATION::EmbeddedFileIntegrationMgr* _Nullable __attribute__((noinline))  GetEmbeddedFileIntegrationMgr(NSString * _Nullable sessionId);

extern NS_ZOOM_MESSAGER::PrivateStickerMgr * _Nullable __attribute__((noinline)) GetStickerMgr(NSString * _Nullable sessionId);

extern NS_IM_CRAWLER::LinkCrawler * _Nullable __attribute__((noinline)) GetLinkCrawler(NSString * _Nullable sessionId);

extern ISSBPTUserProfile * _Nullable __attribute__((noinline)) GetCurrentUserProfile(NSString * _Nullable sessionId);

extern NS_ZOOM_MESSAGER::IZoomPublicRoomSearchData * _Nullable __attribute__((noinline)) GetPublicRoomSearchData(NSString * _Nullable sessionId);

extern NS_ZOOM_MESSAGER::IZoomPersonalFolderMgr * _Nullable __attribute__((noinline)) GetPersonalFolderMgr (void);

extern CPTServicePtr <IPTUrlActionService> __attribute__((noinline)) GetPtUrlActionService(void);

extern NS_ZOOM_MESSAGER::ISidebarUnreadCountMgr * _Nullable __attribute__((noinline)) getSidebarUnreadMgr(void);

extern IAICPlusChatMgr * _Nullable __attribute__((noinline)) GetZoomAICPlusChatMgr(NSString *sessionId);

#endif /* ZMIZMessageHelper_h */
