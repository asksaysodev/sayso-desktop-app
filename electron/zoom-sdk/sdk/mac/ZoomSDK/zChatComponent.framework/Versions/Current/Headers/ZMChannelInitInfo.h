//
//  ZMChannelInitInfo.h
//  ChatUI
//
//  Created by Zoro.Fu on 2023/3/16.
//  Copyright © 2023 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, ZMWhoCanAddAppInChat)
{
    ZMWhoCanAddAppInChat_EveryOne,
    ZMWhoCanAddAppInChat_OwnerAndAdmin,
    ZMWhoCanAddAppInChat_InternalUser,
    ZMWhoCanAddAppInChat_Invalid
};

typedef NS_ENUM(NSUInteger, ZMWhoCanChannelInstantMeeting)
{
    ZMWhoCanChannelInstantMeeting_EveryOne,
    ZMWhoCanChannelInstantMeeting_Disable,
    ZMWhoCanChannelInstantMeeting_OwnerAdmin
};

typedef NS_ENUM(NSInteger, ZMChannelWhoCanEditTabs) {
    ZMChannelWhoCanEditTabs_Invalid = 0,
    ZMChannelWhoCanEditTabs_Everyone,
    ZMChannelWhoCanEditTabs_OwnerAdmin,
};

typedef NS_ENUM(NSInteger, ZMChannelIconType) {
    ZMChannelIconType_None = -1,
    ZMChannelIconType_EmojiId = 0,
    ZMChannelIconType_CustomEmojiId = 1,
    ZMChannelIconType_CustomPictureFileId = 2,
};

NS_ASSUME_NONNULL_BEGIN

@interface ZMChannelInitInfo : NSObject

@property (nonatomic, copy) NSString *channelName;
@property (nonatomic, copy) NSString *channelDes;
@property (nonatomic, copy) NSArray<NSString *> *memberJids;
@property (nonatomic, copy) NSArray<NSString *> *memberEmails;
@property (nonatomic, copy) NSArray<NSString *> *groupIds;
@property (nonatomic, copy) NSArray<NSString *> *announcers;
@property (nonatomic, assign) NSInteger channelType;
@property (nonatomic, copy) NSString *classificationId;
@property (nonatomic, assign) ZMWhoCanAddAppInChat whoCanAddAppInChat;
@property (nonatomic, assign) ZMWhoCanChannelInstantMeeting whoCanChannelInstantMeeting;
@property (nullable, nonatomic, strong) NSString* sharedSpaceID;
@property (nonatomic, assign) BOOL isDiableWorkflow;
@property (nonatomic, assign) BOOL isWorkflowAdminOnly;
@property (nonatomic, assign) ZMChannelWhoCanEditTabs whoCanEditTabs;
@property (nonatomic, copy) NSString *channelIconID;
@property (nonatomic, assign) ZMChannelIconType channelIconType;

- (BOOL)isAvailableName;

- (BOOL)isAvailableMembers;

@end

NS_ASSUME_NONNULL_END
