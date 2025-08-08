//
//  ZMChatsAtListWindowCtrl.h
//  zChatUI
//
//  Created by ryan on 20/06/2018.
//  Copyright © 2018 Zipow. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <ZoomUnit/ZMTableViewUtilCellView.h>
#import <zChatComponent/ZMChatsAtListEntity.h>

@class ZMBuddyAdapter;
@class ZMSessionGroupAdapter;

extern NSString* kIMChatsAllJID;

typedef NS_ENUM(NSUInteger, ZMChatsSessionViewStyle) {
    ZMChatsSessionViewSimpleStyle,
    ZMChatsSessionViewInfoStyle,
};

@interface ZMChatsAtListBaseView : NSTableCellView

@property (nonatomic, assign) BOOL showExternalIcon;

- (void)updateUI;
- (void)setSelected:(BOOL)isSelected;

@end

@interface ZMChatsAtListChannelView : ZMChatsAtListBaseView

@end

@interface ZMChatsAtListContactView : ZMChatsAtListBaseView

@end

@interface ZMChatsAtListMucView : ZMChatsAtListBaseView

@property(nonatomic, assign) ZMChatsSessionViewStyle viewStyle;

@end

@interface ZMSeparatorCellView : ZMChatsAtListBaseView
@end

@interface ZMSkeletonCellView : ZMChatsAtListBaseView
/// Tween variable from [0, 1] that slides the gradient colors.
@property (nonatomic, assign) CGFloat gradientPosition;
@end

@interface ZMChatsAtListWindow : NSWindow

- (void)invisibleInSharing;

@end

@interface ZMChatsAtListView : NSView

@end

@class ZMSearchResultItem;
@class ZMChatsAtListContactData;
@class ZMChatsAtListWindowCtrl;

@protocol ZMChatsAtListWindowDelegate <NSObject>
@optional
- (void)handleChannelUpdates:(ZMChatsAtListWindowCtrl*)sender channel:(NSString*)channelId;
- (void)selectUserInAtList:(NSDictionary*)dict;
- (void)atListWindowWillBeClose:(ZMChatsAtListWindowCtrl *)sender;
- (void)atListWindowShouldUpdateShowClose:(ZMChatsAtListWindowCtrl *)atWC;
@end

typedef NS_ENUM(NSUInteger, ZMChatAtListShowFrom) {
    ZMChatAtListShowFrom_ChatInviteSearch,
    ZMChatAtListShowFrom_ChatInviteInput
};

@interface ZMChatsAtListWindowCtrl : NSWindowController

@property (nonatomic, weak) id<ZMChatsAtListWindowDelegate> delegate;
@property (nonatomic, assign) BOOL isInEditView;
@property (nonatomic, assign) ZMChatsAtListType listType;
@property (nonatomic, copy) NSString* sessionID;
@property (nonatomic, copy) NSString* threadID;
@property (nonatomic, assign) BOOL showExternalIcon;
@property (nonatomic, assign) BOOL disableBlockedUser;
@property (nonatomic, weak) NSView *positionView;

+ (instancetype)defaultWindowController;

- (void)updateDataWithKey:(NSString*)key forType:(ZMChatsAtListType)type;
- (void)resetWithSessionID:(NSString*)sessionID;
- (void)removeFromParentWindow;

- (BOOL)setContactData:(NSArray*)data forFromType:(ZMChatAtListShowFrom)fromType memberCount:(NSInteger)memberCount;
- (BOOL)setContactData:(NSArray*)data forFromType:(ZMChatAtListShowFrom)fromType andKeepSelectedRow:(BOOL)isRemain memberCount:(NSInteger)memberCount;
- (BOOL)setContactData:(NSArray*)data forFromType:(ZMChatAtListShowFrom)fromType andKeepSelectedRow:(BOOL)isRemain memberCount:(NSInteger)memberCount sessionId:(NSString* _Nullable)sessionId searchKey:(NSString* _Nullable)searchKey;
- (NSArray<ZMChatsAtListBaseData*>*)getSessionsData;
- (void)setMucData:(NSArray *)mucs andKeepSelectedRow:(BOOL)isRemain withKey:(NSString *)key withTopic:(BOOL)withTopic;
- (BOOL)setRecentData:(NSArray*)data forFromType:(ZMChatAtListShowFrom)fromType andKeepSelectedRow:(BOOL)isRemain memberCount:(NSInteger)memberCount;
- (BOOL)setResultData:(NSArray*)data forFromType:(ZMChatAtListShowFrom)fromType andKeepSelectedRow:(BOOL)isRemain memberCount:(NSInteger)memberCount;

- (void)setMTContactData:(NSArray<ZMChatsAtListContactData*>*)data memberCount:(NSInteger)memberCount; //For Meeting NewChat

- (void)updateBuddys:(NSArray*)jids;
- (void)updateAvatars;
- (void)subscribeVisibleUsersPresence;
- (void)addSearchResult:(NSArray<ZMSearchResultItem*>*)items;
- (void)addChannelWithKey:(NSArray<ZMSessionGroupAdapter*>*)channels key:(NSString *)key;

- (BOOL)canShowWindow;

- (void)updateCurrentSelectionView:(BOOL)bSelected;

- (void)selectNextItem;
- (void)selectPreviousItem;
- (NSDictionary*)getSelectedItemInfo;

- (NSArray *)getSubscribeVisibleUsersPresence;

- (void)stopCustomEmojiWebSearch;

- (void)voiceStringWithDict:(NSDictionary*)dict;
@end
