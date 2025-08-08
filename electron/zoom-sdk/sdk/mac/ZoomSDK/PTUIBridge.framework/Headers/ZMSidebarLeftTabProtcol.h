//
//  ZMSidebarLeftTabProtcol.h
//  PTUIBridge
//
//  Created by Jed.Zheng on 2024/12/12.
//

#ifndef ZMSidebarLeftTabProtcol_h
#define ZMSidebarLeftTabProtcol_h
#import <Cocoa/Cocoa.h>
#import "ZMPTUIBridgeDefinition.h"

typedef NS_ENUM(NSUInteger, ZMSidebarTabCountType) {
    ZMSidebarTabCountTypeDefault = 0,

    ZMSidebarTabCountTypeAll,
    ZMSidebarTabCountTypeMentions,
    ZMSidebarTabCountTypeOff,
};

typedef NS_ENUM(NSUInteger, ZMSidebarTabUpdateType) {
    ZMSidebarTabUpdateTypeAll = 0,
    ZMSidebarTabUpdateTypeReOrder,
    ZMSidebarTabUpdateTypeBadgeChanged,
    ZMSidebarTabUpdateTypeDraftPopStatuesChanged,
    ZMSidebarTabUpdateTypeSectionModeChanged,
};

@protocol ZMSidebarLeftTabProtcol <ZMRoutableObject>

- (BOOL)setTabCountTypeWithTabIdentifier:(NSString *)identifier countType:(ZMSidebarTabCountType)countType;
- (ZMSidebarTabCountType)getTabCountTypeWithTabIdentifier:(NSString *)identifier;

- (NSUInteger)getAllTabListMode;
- (BOOL)setAllTabListMode:(NSUInteger)listMode;

- (BOOL)getSidebarLeftItemSettingFlagWithIdentifier:(NSString *)identifier;
- (BOOL)setSidebarLeftItemSettingFlagWithIdentifiers:(NSArray *)identifiers;

- (void)onSidebarTabUpdateWithType:(ZMSidebarTabUpdateType)type tabIdentifier:(NSString *)identifier;
- (void)didSidebarLeftTabTabBadgeLocalDateChangedWithTabIdentifier:(NSString *)identifier isFromSetting:(BOOL)isFromSetting;

- (void)resetAllTabData;

- (void)addListenAllSidebarLeftTabInfoUpdatedByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenSidebarLeftTabInfoReOrderedByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenSidebarLeftTabTabBadgeUpdatedByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenSidebarLeftTabDraftPopStatueUpdatedByTarget:(id)target completion:(ZMCallbackBlock)completion;
- (void)addListenSidebarLeftTabAllTabSectionModeUpdatedByTarget:(id)target completion:(ZMCallbackBlock)completion;

- (void)removeAllListenByTarget:(id)target;
@end

#endif /* ZMSidebarLeftTabProtcol_h */
