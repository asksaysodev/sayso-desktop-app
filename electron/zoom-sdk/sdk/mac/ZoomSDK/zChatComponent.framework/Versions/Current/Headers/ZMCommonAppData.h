//
//  ZMCommonAppData.h
//  zChatUI
//
//  Created by ryan on 2018/5/7.
//  Copyright © 2018 Zipow. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMCAppMessageDefines.h>
#import <zChatComponent/ZMChatAppsEntity.h>
//#import "SBPTUI.h"

typedef NS_CLOSED_ENUM(NSUInteger, ZMCommonAppSelectSourceType)
{
    ZMCommonAppSelectSourceType_members,
    ZMCommonAppSelectSourceType_channels,
    ZMCommonAppSelectSourceType_others,
};

typedef NS_CLOSED_ENUM(NSUInteger, ZMCommonAppCheckboxStyle) {
    ZMCommonAppCheckboxStyleRegular,
    ZMCommonAppCheckboxStyleSwitch,
};

typedef NS_CLOSED_ENUM(NSUInteger, ZMCommonAppAlertLevel) {
    ZMCommonAppAlertLevelError = 0,
    ZMCommonAppAlertLevelInfo = 1,
};

typedef NS_OPTIONS(NSUInteger, ZMCommonAppPostMeetingAssetMask) {
    ZMCommonAppPostMeetingAssetMaskAttachments = 1 << 0,
    ZMCommonAppPostMeetingAssetMaskTranscript = 1 << 1,
    ZMCommonAppPostMeetingAssetMaskRecording = 1 << 2,
    ZMCommonAppPostMeetingAssetMaskSummary = 1 << 3,
};

typedef NS_CLOSED_ENUM(NSUInteger, ZMCommonAppPostMeetingShareBannerType) {
    ZMCommonAppPostMeetingShareBannerTypeNone = 0,
    ZMCommonAppPostMeetingShareBannerTypeShareSummary = 1,
    ZMCommonAppPostMeetingShareBannerTypeShareRecording = 2,
    ZMCommonAppPostMeetingShareBannerTypeShareBoth = 3,
    ZMCommonAppPostMeetingShareBannerTypeShareSummaryPending = 4,
    ZMCommonAppPostMeetingShareBannerTypeShareRecordingPending = 5,
    ZMCommonAppPostMeetingShareBannerTypeShareBothPending = 6,
};

typedef NS_OPTIONS(NSUInteger, ZMCommonAppPostMeetingCardHint) {
    ZMCommonAppPostMeetingCardHintSummaryUpdated = 1 << 0,
};

NS_ASSUME_NONNULL_BEGIN

FOUNDATION_EXPORT NSString *ZMCommonAppExpandButtonAttribute;
FOUNDATION_EXPORT NSString *ZMCommonAppCollapseButtonAttribute;

FOUNDATION_EXPORT NSFont *ZMCommonAppDefaultFontWithSize(NSInteger fontSize);

@class ZMChatFileItem;
@class ZMChatMsgItemInfo;

@interface ZMAppMarkdownFile : NSObject

// png | jpg | gif | jpeg
@property (nonatomic,copy) NSString *type;
@property (nonatomic,copy) NSString *fileId;
@property (nonatomic,copy) NSString *alt;

@property (assign) long index;
@property (assign) long fileSize;
@property (assign) long width;
@property (assign) long height;
@end

@interface ZMAppMarkdownText : NSObject

@property (copy) NSString* text;
@property (copy) NSString* prefix;
@property (copy) NSString* hyperlink;
@property (copy) NSString* sip;
@property (copy) NSString* mailto;
@property (copy) NSString* mentionJID;
@property (copy) NSString* mentionGroupID;
@property (copy) NSString* profile;
@property (copy) NSString* linkToJid;
@property (copy) NSString* imageLink;
@property (copy) NSString* imageAlter;
@property (copy) NSColor * color;

@property (assign) BOOL isItalic;
@property (assign) BOOL isBold;
@property (assign) BOOL isStrikethrough;
@property (assign) BOOL isMonospace;
@property (assign) BOOL isMentionAll;
@property (assign) BOOL isConcat;

@property (assign) NSUInteger quotes;

@property (nonatomic,retain) ZMAppMarkdownFile *file;

@property (nonatomic, copy) NSString *action;

@end

@interface ZMAppStyledText : NSObject <NSCopying>

@property (copy, nonatomic) NSString *text;
@property (copy, nonatomic) NSColor *color;
@property (assign, nonatomic) BOOL bold;
@property (assign, nonatomic) BOOL italic;

@property (nonatomic, readonly, copy) NSAttributedString *attributedString;

- (instancetype)initWithString:(NSString *)string;

@end

@interface ZMCommonAppSelectItemData : NSObject

@property (copy) NSString* text;
@property (copy) NSString* value;

- (id)initWithDataPointer:(void *)pItemData;
- (BOOL)isEqualWithData:(ZMCommonAppSelectItemData*)inData;

@end

@interface ZMCommonAppSelectGroupData : NSObject

@property (copy) NSString* group;
@property (retain) NSArray<ZMCommonAppSelectItemData*>* selectItems;

//- (id)initWithData:(NS_ZOOM_MESSAGER::SelectGroup)itemData;

@end

@interface ZMCommonAppSelectAllGroupsData : NSObject

@property (retain) NSArray<ZMCommonAppSelectGroupData*>* groups;
@property (assign) ZMCommonAppSelectSourceType type;

//- (id)initWithData:(NS_ZOOM_MESSAGER::MessageTemplateSelectItems)itemData;

@end

@interface ZMCommonAppDialogData : NSObject

@property (copy) NSString* appID;
@property (copy) NSString* link;
@property (copy) NSString* title;
@property (copy) NSString* actionID;
@property (assign) BOOL isHideTitle;
@property (assign) BOOL isHideApp;
@property (assign) BOOL isHideAppIcon;
@property (assign) int width;
@property (assign) int height;

@property (assign, nonatomic) BOOL usesAppViewer;
@property (assign, nonatomic) ZMChatAppViewerFeature appViewerFeatures;
@property (copy, nonatomic) NSString *openFrom;
@property (copy, nonatomic) NSString *extensions;
@property (copy, nonatomic) NSString *customPayload;
@property (assign, nonatomic) ZMChatAppWebViewPosition webViewPosition;

@end

@interface ZMCommonAppBaseItem : NSObject

@property (copy, nullable) NSString* text;
@property (copy, nullable) NSColor* color;
@property (copy, nullable) NSString* fallBack;
@property (assign) BOOL isBold;
@property (assign) BOOL isItalic;
@property (assign) BOOL isUnsupport;
@property (assign) ZMCommonAppBodyType type;
@property (assign) ZMCommonAppControlAlignment alignment;

@end

@interface ZMCommonAppHeadItem : ZMCommonAppBaseItem

@property (retain) NSArray<ZMAppMarkdownText*>* markedTextArray;

@end

@interface ZMCommonAppActionItem : ZMCommonAppBaseItem

@property (assign) ZMCommonAppActionButtonStyle style;
@property (copy, nullable) NSString* value;
@property (copy, nullable) NSString *action;
@property (copy, nullable) NSString *actionID;
@property (retain, nullable) ZMCommonAppDialogData *dialogData;
@property (copy, nullable) NSString *toolTip;
@property (nonatomic, assign) BOOL enabled;
@property (nonatomic, assign) BOOL submitButton;
@property (nonatomic, copy, nullable) NSString *tab;
@property (nonatomic, copy, nullable) NSString *url;
@property (nonatomic, copy, nullable) NSString *iconURL;
@property (nonatomic, copy, nullable) NSString *zmAppID;
@property (nonatomic, copy, nullable) NSString *metricsAction;

@end

@interface ZMCommonAppActionItems : ZMCommonAppBaseItem

@property (retain) NSArray<ZMCommonAppActionItem*>* actions;
@property (copy) NSString* eventID;
@property (assign) NSInteger limited;
@property (retain, nullable) ZMCommonAppActionItem *overflowAction;

@end

@interface ZMCommonAppSelectItem : ZMCommonAppBaseItem

@property (copy, nullable) NSString* eventID;
@property (copy, nullable) NSString* actionID;
@property (retain, nullable) ZMCommonAppSelectAllGroupsData* data;
@property (retain, nullable) NSArray<ZMCommonAppSelectItemData*>* selectItems;
@property (nonatomic, assign) BOOL bordered;

- (BOOL)isEqualToData:(NSArray<ZMCommonAppSelectItemData*>*)dataArray;

@end

@interface ZMCommonAppAlertItem : ZMCommonAppBaseItem

@property (copy, nullable, nonatomic) NSString *eventID;
@property (assign, nonatomic) ZMCommonAppAlertLevel level;
@property (assign, nonatomic) BOOL closable;
@property (retain, nullable, nonatomic) NSArray<ZMAppMarkdownText *> *markedTextArray;

@end

@interface ZMCommonAppDateTimePickerItem : ZMCommonAppBaseItem

@property (copy, nullable) NSString *eventID;
@property (copy, nullable) NSString *actionID;
@property (retain, nullable) NSDateComponents *dateComponents;

@end

@interface ZMCommonAppCheckboxItem : ZMCommonAppBaseItem

@property (copy, nonatomic, nullable) NSString *eventID;
@property (copy, nonatomic, nullable) NSString *actionID;
@property (assign, nonatomic) ZMCommonAppCheckboxStyle style;
@property (copy, nonatomic) NSSet<NSString *> *selectedValues;
@property (assign, nonatomic, readonly) NSUInteger numberOfItems;

- (void)addItemWithTitle:(NSString *)title value:(NSString *)value;

- (NSString *)titleOfItemAtIndex:(NSUInteger)index;
- (NSString *)valueOfItemAtIndex:(NSUInteger)index;

@end

@interface ZMCommonAppRadioButtonItem : ZMCommonAppBaseItem

@property (copy, nullable) NSString *eventID;
@property (copy, nullable) NSString *actionID;
@property (copy, nonatomic, nullable) NSString *selectedValue;
@property (assign, nonatomic) NSUInteger numberOfChoices;

- (void)addChoiceWithTitle:(NSString *)title value:(NSString *)value;

- (NSString *)titleOfChoiceAtIndex:(NSUInteger)index;
- (NSString *)valueOfChoiceAtIndex:(NSUInteger)index;

@end

@interface ZMCommonAppProgressBarItem : ZMCommonAppBaseItem

@property (assign, nonatomic) NSInteger value;

@end

@interface ZMCommonAppDividerItem : ZMCommonAppBaseItem

@property (nonatomic, copy) NSColor *lineColor;
@property (nonatomic, assign) BOOL bold;
@property (nonatomic, assign) BOOL dotted;

@end

@interface ZMCommonAppMessageItem : ZMCommonAppActionItem

@property (copy) NSString* link;
@property (copy) NSString* eventID;
@property (retain) NSArray<ZMAppMarkdownText*>* markedTextArray;
@property (assign) BOOL isEditalbe;
@property (assign) NSInteger numberOfLines;
@property (assign) BOOL expandable;

@end

// Template File is very similar with Normal file in appearance.
@interface ZMCommonAppTemplateFileItem : ZMCommonAppBaseItem
@property (copy, nullable) NSString *iconURL;
@property (copy, nullable) ZMAppStyledText *title;
@property (copy, nullable) NSString *fileURL;
@property (copy, nullable) ZMAppStyledText *desc;
@property (assign) BOOL singleLineIcon;
@end

@interface ZMCommonAppFieldItem : ZMCommonAppMessageItem

@property (copy) NSString* key;
@property (assign) BOOL isName;
@property (assign) BOOL isShort;

@end

@interface ZMCommonAppFieldItems : ZMCommonAppBaseItem

@property (retain) NSArray<ZMCommonAppFieldItem*>* fieldItems;
@property (copy) NSString* eventID;

@end

@interface ZMCommonAppAttachItem : ZMCommonAppBaseItem

@property (retain) NSString* resourceUrl;
@property (retain) NSString* imageUrl;
@property (retain) NSString* extention;
@property (assign) long long size;
@property (assign) NSInteger index;
@property (retain) ZMCommonAppBaseItem* descriptionItem;

@property (nonatomic, copy) NSString *fileId;
@property (nonatomic, assign) NSInteger fileIndex;

@end

@interface ZMCommonAppSubBuddyItem : ZMCommonAppBaseItem

@property (copy) NSColor* barColor;
@property (retain) NSString* footer;
@property (retain) NSString* iconURL;
@property (retain) NSString* footerFallBack;
@property (retain) NSMutableArray* bodyItems;
@property (retain) NSDate* time;
@property (retain) NSMutableArray<ZMAppMarkdownText*>* markedTextArray;
@property (assign) BOOL isAddDateToMarkedTextArray;
@property (assign) BOOL isFooterUnsupport;
@property (assign) BOOL prefersSingleLine;

@end

@interface ZMCommonAppMetaData : NSObject

@property (nonatomic, copy) NSString *type;
@property (nonatomic, copy) NSString *displayProfileId;
@property (nonatomic, copy) NSString *displayName;
@property (nonatomic, copy) NSString *displayIcon;
@property (nonatomic, copy) NSString *displayLabel;

@end

@class ZMPTCommonAppExpandCell;

@interface ZMCommonAppData : NSObject

@property (retain) ZMCommonAppHeadItem* head;
@property (retain) ZMCommonAppHeadItem* subHead;
@property (retain) NSMutableArray* bodyItems;
@property (assign) BOOL isUnsupportMessage;
@property (nonatomic, assign) BOOL hasUnsupportedComponents;
@property (assign) BOOL hasSplitSideBar;
@property (assign) BOOL sideBarDisabled;
@property (assign) BOOL hasSubBody;
@property (assign) BOOL hasMonospace;
@property (assign) BOOL hasHighlight;
@property (assign) BOOL hasProfile;

// ZOOM-520700: new protocol for embedding profile links without mentions.
// "@all" cell will not be highlighted when this flag is set to `YES`.
@property (assign) BOOL hasProfileV2;

// ZOOM-685361
@property (nonatomic, copy) NSString *subCMCSessionID;

// ZOOM-786044
@property (nonatomic, assign) BOOL hasMeetingCard;
@property (nonatomic, assign) BOOL hasInFlightMeetingCard;

@property (assign) BOOL hasSip;
@property (assign) BOOL hasShortField;
@property (assign) BOOL isOnlyVisibleToMe;
@property (copy) NSColor* defaultBarColor;
@property (nonatomic, copy) NSNumber *maxCardWidth;
@property (nonatomic, copy) NSNumber *minCardWidth;
@property (nonatomic, copy) NSString *botJID;

@property (assign) NSInteger headLoaction;
@property (assign) BOOL hasContent;
@property (assign, readonly) int index;

@property (assign) BOOL couldForward;

@property (nonatomic, retain) ZMCommonAppMetaData *appMetaData;

@property (nonatomic, weak) ZMChatMsgItemInfo *messageInfo;

- (id)initWithSessionID:(NSString*)sessionID messageID:(NSString*)messageID index:(int)index;
- (void)constructData;
- (NSMutableAttributedString*)contentString;
- (nullable NSViewController *)hoverActionsViewController;
- (void)setAttachmentsHide:(BOOL)hide byAttachmentIndex:(NSInteger)attachmentIndex;
- (ZMCommonAppSelectItem*)getSelectItemByEventID:(NSString*)eventID;

- (void)expandTextByItem:(ZMCommonAppBaseItem *)item;
- (void)collapseTextByItem:(ZMCommonAppBaseItem *)item;

@end

@interface ZMPTCommonSpacerCell : NSTextAttachmentCell

@property (nonatomic, assign) CGFloat length;

@end

@interface ZMPTCommonBaseCell : NSTextAttachmentCell

@property (nonatomic, readonly) BOOL needFocusRing;
@property (nonatomic, assign) BOOL hoverred;

@end

@interface ZMPTCommonAppNameCell : ZMPTCommonBaseCell

@property (copy) NSAttributedString* name;

@end

typedef NS_OPTIONS(NSUInteger, ZMPTCommonAppImageCellType)
{
    ZMPTCommonAppImageCell_Footer,
    ZMPTCommonAppImageCell_InlineImage,
    ZMPTCommonAppImageCell_NormalImage,
    ZMPTCommonAppImageCell_DefaultInlineImage,
};

@interface ZMPTCommonAppImageCell : ZMPTCommonBaseCell

@property (nonatomic, assign) ZMPTCommonAppImageCellType cellType;
@property (nonatomic, retain) NSString* imageAlter;

@end

@interface ZMPTCommonAppActionCell : ZMPTCommonBaseCell

@property (retain) ZMCommonAppMessageItem* item;
@property (assign) float fontSize;
@property (assign) NSInteger length;
@property (nonatomic, assign) int messageTemplateIndex;
@end

NS_ASSUME_NONNULL_END
