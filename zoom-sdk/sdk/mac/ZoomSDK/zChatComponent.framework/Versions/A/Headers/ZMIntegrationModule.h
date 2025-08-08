//
//  ZMIntegrationModule.h
//  SaasBeePTUIModule
//
//  Created by Jed Zheng on 13/7/2021.
//  Copyright © 2021 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMIntegrationProtocal.h>

NS_ASSUME_NONNULL_BEGIN


typedef NS_ENUM(NSInteger, SharePointResult) {
    SharePointResult_Success = 0,
    SharePointResult_MessageTimeout,
    SharePointResult_FileTimeout,
    SharePointResult_Failed,
    SharePointResult_FailedNameExisted,
    SharePointResult_OtherFailed,
};

typedef NS_ENUM(NSInteger, ZMIntegrationEventActionType) {
    ZMIntegrationEventActionType_None                = -1,
    ZMIntegrationEventActionType_RenameFile          = 0,
    ZMIntegrationEventActionType_DeleteFile          = 1,
    ZMIntegrationEventActionType_UnshareFile         = 2,
    ZMIntegrationEventActionType_AddFile             = 3,
    ZMIntegrationEventActionType_RobotMessage        = 4,
    ZMIntegrationEventActionType_LinkPreview         = 5,
    ZMIntegrationEventActionType_PBX_VoiceMail       = 6,
    ZMIntegrationEventActionType_EditFile            = 7,
    ZMIntegrationEventActionType_TemplateMessage     = 8,
    ZMIntegrationEventActionType_EmojiComment        = 9,
    ZMIntegrationEventActionType_IMWebSetting        = 10,
    ZMIntegrationEventActionType_LinkUnfurling       = 11,
    ZMIntegrationEventActionType_MeetCardAssets      = 12,
    ZMIntegrationEventActionType_Classification      = 13,
    ZMIntegrationEventActionType_MeetCardPost        = 14,
    ZMIntegrationEventActionType_MoveFile            = 15,

};

@interface ZMIntegrationEvent : NSObject

@property (nonatomic, assign) BOOL isFolder;
@property (nonatomic, assign) ZMIntegrationEventActionType actionType;
@property (nonatomic, copy) NSString* name;
@property (nonatomic, copy) NSString* sessionID;
@property (nonatomic, copy) NSString* fileID;
@property (nonatomic, copy) NSString* parentID;
@property (nonatomic, copy) NSString* originalParentID;

@end


@interface ZMIntegrationFileTypeModel : NSObject
@property (nonatomic, assign) NSInteger type;
@property (nonatomic, copy) NSString *name;

@end

@interface ZMIntegrationModule : NSObject <ZMIntegrationProtocal>

+ (ZMIntegrationModule*)sharedInstance;
+ (void)releaseInstance;

- (void)setFileOperateImpl:(id<ZMIntegrationActionProtocal>)fileOperateImpl;

@end

NS_ASSUME_NONNULL_END
