//
//  ZMAICChatSetting.h
//  ZoomAIC
//
//  Created by Karen Cao on 9/29/23.
//

#ifndef ZMAICChatSetting_h
#define ZMAICChatSetting_h

@protocol ZMAICChatSetting <NSObject>

@property (nonatomic) NSInteger uid;
@property (nonatomic, strong) NSString* title;
@property (nonatomic, strong) NSString* tip;
@property (nonatomic, strong) NSString* desc;
@property (nonatomic, strong) NSString* icon;
@property (nonatomic, strong) NSString* iconSelected;
@property (nonatomic, strong) NSString* iconDisabled;
@property (nonatomic, strong) NSString* queryMgrInfo;
@property (nonatomic, strong) NSString* queryParticipantsInfo;

@end

#endif /* ZMAICChatSetting_h */
