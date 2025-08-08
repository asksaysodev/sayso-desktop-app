//
//  ZMChatsAtListEntity.h
//  ZoomMsgUI
//
//  Created by Yong Zhou on 4/29/22.
//

#import <Foundation/Foundation.h>
#import <ZoomUnit/ZMTableViewUtilCellView.h>
#import <zChatComponent/ZMEmojiModel.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMBuddyAdapter;
@class ZMSessionGroupAdapter;
@class ZMMentionGroup;
@class ZMMTBuddyModel;

typedef NS_ENUM(NSUInteger, ZMChatsAtListType)
{
    ZMChatsAtListType_at,
    ZMChatsAtListType_search,
    ZMChatsAtListType_channel,
    ZMChatsAtListType_commandCall,
    ZMChatsAtListType_commandMeet,
    ZMChatsAtListType_emoji,
    ZMChatsAtListType_muc,
    ZMChatsAtListType_mentionGroup,
    ZMChatsAtListType_recent,
};

typedef void (^IsSelectedChanged)(BOOL visible);

@interface ZMChatsAtListBaseData : NSObject <ZMTableViewUtilProtocal>
@property (nonatomic, copy) IsSelectedChanged isSelectedChanged;
@property (nonatomic, assign) BOOL isSelected;
@property (nonatomic, assign) BOOL isDisabled;
@property (nonatomic, assign) BOOL isUtil;
@property (nonatomic, strong) NSString* title;
@property (nullable, nonatomic, strong) NSImage* image;
@end

@interface ZMChatsAtListSeparatorData : ZMChatsAtListBaseData
@property (nullable, nonatomic, strong) NSString* tooltip;
@end

@interface ZMChatsAtListLoadingSkeletonData : ZMChatsAtListBaseData
@end

@interface ZMChatsAtListContactData : ZMChatsAtListBaseData
@property (retain) ZMBuddyAdapter* buddy;
@property (nonatomic, retain) ZMMentionGroup* mentionGroup;
@property (nonatomic,retain) ZMMTBuddyModel* meetBuddy;
@property (assign) BOOL isAtAll;
@property (assign) BOOL isDisableBlocker;
@property (nonatomic, assign) BOOL isInChannel;
@end

@interface ZMChatsAtListChannelData : ZMChatsAtListBaseData
@property (nonatomic, copy) NSString *searchKey;
@property (retain) ZMSessionGroupAdapter* channel;
@end

@interface ZMChatsAtListEmojiData : ZMChatsAtListBaseData
@property (nonatomic, strong) ZMEmojiModel *emojiModel;
@property (nonatomic, copy) NSString *searchKey;
- (instancetype)initWithEmojiModel:(ZMEmojiModel *)emojiModel searchKey:(NSString *)searchKey;
@end

@interface ZMChatsAtListMUCData : ZMChatsAtListBaseData
@property (nonatomic, copy) NSString *searchKey;
@property (nonatomic, copy) NSString *jid;
@property (nonatomic, copy) NSString *name;
@property (nonatomic, copy) NSString *tooltip;
@property (nonatomic, copy) NSArray<ZMBuddyAdapter*> *members;
@property (assign) BOOL isCreateMUC;
@end

NS_ASSUME_NONNULL_END

