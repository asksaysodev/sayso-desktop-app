//
//  ZMCEventTrackerProtocol.h
//  zChatComponent
//
//  Created by Mario Rao on 3/17/23.
//

@class ZMChatMsgItemInfo;

#import <events/chat_ChatInteract_enum.h>
#import <events/client_ClientInteract_enum.h>

#ifdef __cplusplus
#include <zTelemetryBiz/zTelemetryBizInterface.h>
#include <zTelemetryBiz/SnowplowBiz/chat_v2/chat_v2_telemetry_interface.h>
#import <zEventTracker/PTEventTrackingHelper.h>
#endif

@protocol ZMCEventTrackerProtocol <ZMRoutableObject>

#ifdef __cplusplus

+ (void)trackChatEventWithNameV2:(e_Interaction_event_name)name
                     featureName:(e_Interaction_feature_name)featureName
                      subFeature:(e_Interaction_sub_feature_name)subFeatureName
                          source:(e_Interaction_event_source)source
                        location:(e_Interaction_event_location)location
                       eventType:(e_Interaction_event_type)eventType
                             jid:(NSString *)jid
                         context:(NSString *)context;

+ (void)trackChatEventWithNameV2:(e_Interaction_event_name)name
                   featureName:(e_Interaction_feature_name)featureName
                    subFeature:(e_Interaction_sub_feature_name)subFeatureName
                        source:(e_Interaction_event_source)source
                      location:(e_Interaction_event_location)location
                     eventType:(e_Interaction_event_type)eventType
                           jid:(NSString *)jid
                         msgID:(NSString *)msgID
                       context:(NSString *)context;

#endif

+ (void)trackChatEventWithName:(e_chat_ChatInteract_event_name)name
                   featureName:(e_chat_ChatInteract_feature_name)featureName
                        source:(e_chat_ChatInteract_event_source)source
                      location:(e_chat_ChatInteract_event_location)location
                     eventType:(e_chat_ChatInteract_event_type)eventType
                           jid:(NSString *)jid
                       context:(NSString *)context;

+ (void)trackChatEventWithName:(e_chat_ChatInteract_event_name)name
                   featureName:(e_chat_ChatInteract_feature_name)featureName
                        source:(e_chat_ChatInteract_event_source)source
                      location:(e_chat_ChatInteract_event_location)location
                     eventType:(e_chat_ChatInteract_event_type)eventType
                           jid:(NSString *)jid
                         msgID:(NSString *)msgID
                       context:(NSString *)context;

#ifdef __cplusplus
+ (void)trackChatEventWithName:(e_chat_ChatInteract_event_name)name
                   featureName:(e_chat_ChatInteract_feature_name)featureName
                        source:(e_chat_ChatInteract_event_source)source
                      location:(e_chat_ChatInteract_event_location)location
                     eventType:(e_chat_ChatInteract_event_type)eventType
                           jid:(NSString *)jid
                     msgStruct:(ZoomPTPAAP::ChatMessageEntityInfo)msg
                       context:(NSString *)context;

+ (void)trackChatFileEventWithNameV2:(e_Interaction_event_name)name
                         featureName:(e_Interaction_feature_name)featureName
                          subFeature:(e_Interaction_sub_feature_name)subFeatureName
                              source:(e_Interaction_event_source)source
                            location:(e_Interaction_event_location)location
                           eventType:(e_Interaction_event_type)eventType
                                 jid:(NSString *)jid
                           msgStruct:(ChatV2FileTransferEntityParams)msg
                             context:(NSString *)context;

+ (void)trackChatEventWithNameV2:(e_Interaction_event_name)name
                     featureName:(e_Interaction_feature_name)featureName
                      subFeature:(e_Interaction_sub_feature_name)subFeatureName
                          source:(e_Interaction_event_source)source
                        location:(e_Interaction_event_location)location
                       eventType:(e_Interaction_event_type)eventType
                             jid:(NSString *)jid
                       msgStruct:(ChatV2MessageEntityParams)msg
                         context:(NSString *)context;

#endif

// Settings
+ (void)trackSettingInteractionWithSource:(e_client_ClientInteract_event_source)source
                                  feature:(e_client_ClientInteract_feature_name)feature
                                 location:(e_client_ClientInteract_event_location)location
                                     name:(e_client_ClientInteract_event_name)name
                                  context:(nullable NSString *)context;

@end
