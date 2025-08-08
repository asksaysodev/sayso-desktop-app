//
//  ZMCloudContactProtocol.h
//  
//
//  Created by lisa.si on 2025/3/13.
//

#ifndef ZMCloudContactProtocol_h
#define ZMCloudContactProtocol_h
#import "ZMSipCloudContactEditView.h"
#include <zPTApp/SaasBeePTAppInterface.h>
#include <zChatApp/IZoomMessenger.h>
#include "zChatApp/IZoomMessenger.h"

@protocol ZMCloudContactProtocol <NSObject>

- (ns_3rd_contacts_integration::IContactsIntegrationServiceHelper  *)getContactsIntegrationServiceHelper;
//create
- (NSString *)createCloudContact:(NSMutableDictionary *)dic source:(ZMCloudContactSourceType)soucrceType;
- (NSString *)addToCloudContact:(NSMutableDictionary *)dic source:(ZMCloudContactSourceType)soucrceType;
- (NSString *)createPersonalContactWith:(ns_zoom_messager::PersonalContactBuddy)contact othersInfo:(NSDictionary *)dic;

//search
- (NSMutableArray *)findContactsListWithUserName:(NSString *)key;
- (NSMutableArray*)searchPersonalContactByKey:(NSString*)key;

@end




#ifndef ZMSharedCloudContactProtocol
#define ZMSharedCloudContactProtocol (id <ZMCloudContactProtocol>)ZMSharedFor(ZMCloudContactProtocol)
#endif

#endif /* ZMCloudContactProtocol_h */
