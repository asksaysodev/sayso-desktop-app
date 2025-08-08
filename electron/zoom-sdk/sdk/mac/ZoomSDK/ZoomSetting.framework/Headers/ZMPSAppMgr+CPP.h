//
//  ZMPSAppMgr+CPP.h
//  ZoomSetting
//
//  Created by Michael Lin on 2024/1/11.
//

#import <ZoomSetting/ZoomSetting.h>
#import <ZoomSetting/ZMPSAppMgr.h>
#import <zPSApp/app/IPSApp.h>
#import <zPSApp/media/IPSMediaAgent.h>
#import <zPSApp/media/IPSRender.h>
#import <zPSApp/product/IPSAsyncRecording.h>
#import <zPSApp/media/IPSShare.h>
#import <ZoomSetting/ZMPSObject.h>
#import <ZoomSetting/ZMPSAppHolder.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMPSAppMgr (CPP)

@property (nonatomic, strong) ZMPSAppHolder *psAppHolder;
- (nullable ps::IPSMediaAgent *)mediaAgent;
- (nullable ps::IPSAudioAgent *)ipsAudioAgent;
- (nullable ps::IPSVideoAgent *)ipsVideoAgent;
- (nullable ps::IPSShareAgent *)ipsShareAgent;
- (nullable ps::IPSAsyncRecordingMgr *)ipsAsyncRecordingMgr;
- (nullable ps::IPSRenderMgr *)ipsRenderMgr;
- (nullable ps::IPSCanvasMgr *)ipsCanvasMgr;
- (nullable ps::IPSWebAgentAPI *)ipsWebAgentAPI;
- (nullable ps::IPSMediaClient *)ipsMediaClient;
- (nullable ps::IPSContext *)ipsContext;

@end

NS_ASSUME_NONNULL_END
