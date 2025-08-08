//
//  ZMChatAppsEntity.h
//  zChatComponent
//
//  Created by Yong Zhou on 8/29/22.
//

#import <Foundation/Foundation.h>
#import <ZoomKit/ZoomKit.h>

typedef NS_ENUM(NSUInteger, ZMAPPShortcutSourceType) {
    ZMAPPShortcutSourceType_InputBox = 0,
    ZMAPPShortcutSourceType_Message,
    ZMAPPShortcutSourceType_BotMessage,
    ZMAPPShortcutSourceType_Webhook,
    ZMAPPShortcutSourceType_Launcher,
    ZMAPPShortcutSourceType_DocumentPreview,
    ZMAPPShortcutSourceType_Thread,
    ZMAPPShortcutSourceType_HoverLinkInMessage,
    ZMAPPShortcutSourceType_HoverLinkInCompose,
    ZMAPPShortcutSourceType_ChatTab,
    ZMAPPShortcutSourceType_ChatTabPicker,
    ZMAPPShortcutSourceType_Format,
    ZMAPPShortcutSourceType_WebChat,
};

typedef NS_ENUM(NSUInteger, ZMLauncherApp) {
    ZMLauncherApp_Apps,
    ZMLauncherApp_Workflows,
};

typedef NS_ENUM(NSUInteger, ZMAPPShortcutGroupType) {
    ZMAPPShortcutGroupTypeNonSystem = 0,
    ZMAPPShortcutGroupTypeSystem = 1,
};

typedef NS_ENUM(NSUInteger, ZMChatSessionType)
{
    ZMChatSessionType_None = 0,
    ZMChatSessionType_1V1,
    ZMChatSessionType_Group, /* Channel, MUC */
    ZMSMSSessionType_1V1,
    ZMSMSSessionType_Group,
};

typedef NS_ENUM(NSUInteger, ZMAppLayoutType)
{
    ZMAppLayoutType_None = 0,
    ZMAppLayoutType_DocumentView,
    ZMAppLayoutType_DocumentView_PopOut,
};

typedef NS_ENUM(NSUInteger, ZMChatAppMessageEditAction)
{
    ZMChatAppMessageEditAction_None = 0,
    ZMChatAppMessageEditAction_Replace,
    ZMChatAppMessageEditAction_Append,
    ZMChatAppMessageEditAction_Insert,
    ZMChatAppMessageEditAction_ReplaceSelected,
};

typedef NS_ENUM(NSUInteger, ZMChatAppWebViewPosition) {
    ZMChatAppWebViewPositionDefault = 0,
    ZMChatAppWebViewPositionAboveInputBox,
    ZMChatAppWebViewPositionFollowMessage,
    ZMChatAppWebViewPositionFloating,
};

typedef NS_ENUM(NSUInteger, ZMChatAppTitleIcon) {
    ZMChatAppTitleIconNone = 0,
    ZMChatAppTitleIconFreeTrial,
};

typedef NS_OPTIONS(NSUInteger, ZMChatAppViewerFeature) {
    ZMChatAppViewerFeatureCopyLink = 1 << 0,
    ZMChatAppViewerFeatureBookmark = 1 << 1,
    ZMChatAppViewerFeatureForwardMessage = 1 << 2,
    ZMChatAppViewerFeaturePopup = 1 << 3,
    ZMChatAppViewerFeatureAddToTabs = 1 << 4,
    ZMChatAppViewerFeatureSplitView = 1 << 5,
    ZMChatAppViewerFeatureReload = 1 << 6,
};

typedef NS_ENUM(NSUInteger, ZMChatFloatAppOpenSource) {
    ZMChatFloatAppOpenSource_MouseHover,
    ZMChatFloatAppOpenSource_KeyFocus,
};

NS_ASSUME_NONNULL_BEGIN

static NSString* const kAppCardInfo  = @"kAppCardInfo";
static NSString* const kDefaultAPPAXDesc  = @"App Card";

static NSString* const ZMChatAppShortcutAction_Dialog = @"dialog";
static NSString* const ZMChatAppShortcutAction_Command = @"command";
static NSString* const ZMChatAppShortcutAction_Tab = @"tab";
static NSString* const ZMChatAppShortcutAction_URL = @"url";

static NSString * const ZMChatAppWorkflowMockAppId = @"workflow_in_channel";

@class NSImage;

@interface ZMChatAppShortcutInfo : NSObject
/* The action from InputBox or Message menu */
@property (nonatomic, assign) ZMAPPShortcutSourceType sourceType;
@property (nonatomic, assign) ZMAPPShortcutGroupType groupType;
/* sessionID,messageID,threadID, locate which message the shortcut action is related */

@property (nonatomic, assign) ZMAppLayoutType layoutType;

@property (nonatomic, copy) NSString *messageHash; // message_hash
@property (nonatomic, copy) NSString *asyncID;     // bot_message_id

@property (nonatomic, copy) NSString *action;
@property (nonatomic, copy) NSString *actionID;
@property (nonatomic, copy) NSString *zmAppID;
@property (nonatomic, copy) NSString *robotName;
@property (nonatomic, copy) NSString *robotJID;
/* shortcut name */
@property (nonatomic, copy) NSString *label;
@property (nonatomic, copy) NSString *icon;
@property (nonatomic, copy) NSString *darkIcon;
@property (nonatomic, copy) NSString *lightIconPath;
@property (nonatomic, copy) NSString *darkIconPath;

@property (nonatomic, copy) NSString *buttonTitle;
@property (nonatomic, copy) NSString *dialogTitle;
@property (nonatomic, copy) NSString *dialogIcon;
@property (nonatomic, copy) NSString *dialogDarkIcon;

/* the link to open a popup webview */
@property (nonatomic, copy) NSString *link;
/* the title of popup webview */
@property (nonatomic, copy) NSString *title;
/* hide the title of popup webview */
@property (nonatomic, assign) BOOL isHideTitle;
/* hide the app name and app icon of popup webview */
@property (nonatomic, assign) BOOL isHideApp;
@property (nonatomic, assign) BOOL isHideIcon;

@property (nonatomic, assign) BOOL isHeaderHidden;

@property (nonatomic, assign) BOOL closeWhenClickBackground;
@property (nonatomic, assign) BOOL isHideCloseBtn;
/* the width of popup webview */
@property (nonatomic, assign) int width;
/* the height of popup webview */
@property (nonatomic, assign) int height;
/* Just for AI Compose, if YES, and user did input some words in input box, the AI compose window will hard code to a smaller size*/
@property (nonatomic, assign) BOOL isNeedRemakeSize;

/* ZOOM-438509, dialog/command */
@property (nonatomic, copy) NSString *actionType;
/*ZOOM-680192 if actionType is equal to "appbridge", params for call HandleAppBridgeAction */
@property (nonatomic, copy) NSString *eventName;
@property (nonatomic, copy, nullable) NSString *tab;
@property (nonatomic, copy, nullable) NSString *url;

/* only for webhook */
@property (nonatomic, copy) NSString *triggerID;
@property (nonatomic, copy) NSString *actionFrom;

@property (nonatomic, assign) BOOL disabledForThreadWithoutReplies;
@property (nonatomic, assign) BOOL disabledForThreadWithReplies;
@property (nonatomic, assign) BOOL disabledForThreadReply;

@property (nonatomic, assign) BOOL isDisableInCMC;
@property (nonatomic, assign) BOOL isDisableInACE;

/* view email ZOOM-767811 */
@property (nonatomic, assign) BOOL isWebviewScalable;

//ZOOM-524370
@property (nonatomic, assign) BOOL isInternalAppWithZapLaunch;
@property (nonatomic, copy) NSString *allowedDomains;

@property (nonatomic, assign) NSInteger appFeatures;

// ZOOM-788277
@property (nonatomic, assign) BOOL usesAppViewer;
@property (nonatomic, assign) ZMChatAppViewerFeature appViewerFeatures;
@property (nonatomic, copy) NSString *extensions;

// ZOOM-560837
@property (nonatomic, assign) ZMChatAppWebViewPosition webViewPosition;
@property (nonatomic, assign) ZMChatAppTitleIcon titleIcon;

@property (nonatomic, assign) BOOL isPopupByDefault;
/// A Boolean value that indicates whether the app is presented in a
/// pop-up instead of a modal dialog.
@property (readonly, getter=isPopUpPresentation) BOOL popUpPresentation;

- (BOOL)isLauncherApp;

/*just for telemetry, Hard code to determine whether it is summary.*/
- (BOOL)isSummaryApp;

@end


@interface ZMChatAppInfo : NSObject

@property (nonatomic, copy) NSString *robotJID;
@property (nonatomic, copy) NSString *robotName;
@property (nonatomic, copy) NSString *robotIcon;
@property (nonatomic, copy) NSArray<ZMChatAppShortcutInfo*> *shortcuts;

@property (nonatomic, copy) NSString *lightIconPath;
@property (nonatomic, copy) NSString *darkIconPath;

@property (nonatomic, assign) BOOL isDisableAppInCMC;
@property (nonatomic, assign) BOOL isDisableAppInACE;

// ZOOM-560837
@property (nonatomic, assign) ZMChatAppWebViewPosition webViewPosition;

- (NSImage *)iconImage:(NSSize)size radius:(CGFloat)radius;

+ (void)cleanIconCache;

@end

// ZMChatAppMessagePreviewField.h
@interface ZMChatAppMessagePreviewField : NSObject
@property (nonatomic, copy) NSString *fieldName; // UI display
@property (nonatomic, copy) NSString *fieldValue;     // logic value
@property (nonatomic, copy) NSString *zmURLString;     // logic value
@end

// ZMChatAppMessagePreviewSelect.h
@interface ZMChatAppMessagePreviewSelect : NSObject
@property (nonatomic, copy) NSString *text; // e.g.: permission scope
@property (nonatomic, strong) NSArray<ZMChatAppMessagePreviewField *> *fields; // all items
@property (nonatomic, strong) ZMChatAppMessagePreviewField *selectedField; // selected item
@end

@interface ZMChatAppMessagePreviewInfo : NSObject
 
@property (nonatomic, copy) NSString *threadID;
@property (nonatomic, copy) NSString *sessionID;
@property (nonatomic, copy) NSString *msgID;
@property (nonatomic, copy) NSString *zoomAppID;
@property (nonatomic, copy) NSString *previewID;
@property (nonatomic, copy) NSImage *icon;
/// UI hover or click to show preview, if link in compose card is not nil, show card, defined by Alan Gao
@property (nonatomic, copy) NSString *link;
@property (nonatomic, copy) NSString *title;
@property (nonatomic, copy) NSString *desp;
@property (nonatomic, copy) NSString *appPayload;
@property (nonatomic, assign) BOOL isLinkUnfurlingInComposebox;
@property (nonatomic, strong) ZMChatAppMessagePreviewSelect *previewSelect;

@property (nonatomic, copy, readonly) NSString *accDescription;

- (BOOL)isUnfulLinkPreview;
- (BOOL)hasPermissionControl;
- (BOOL)hasPermissionControlAndHasNoPermissionsOptions;

@end

@interface ZMChatAppComposeMessageInfo : NSObject

@property (nonatomic, copy) NSString *zappId;
@property (nonatomic, copy) NSString *sessionId;
@property (nonatomic, copy) NSString *threadId;
@property (nonatomic, copy) NSString *msgId;
@property (nonatomic, copy) NSString *message;
@property (nonatomic, assign) ZMChatAppMessageEditAction action;
@property (nonatomic, assign) NSRange insertRange;
@property (nonatomic, assign, readonly) BOOL isReply;
@property (nonatomic, assign) BOOL isEditingMessage;

@end

@interface ZMChatAppWebSetting : NSObject

@property (nonatomic, assign) BOOL closeWhenClickBackground;
@property (nonatomic, assign) BOOL webViewHeaderHidden;

- (void)updateWithJsonObject:(NSDictionary *)config;

@end

@interface ZMChatAppViewModel : NSObject

@property (nonatomic, retain) ZMChatAppWebSetting *webSetting;
@property (nonatomic, assign) NSSize resize;

+ (instancetype)viewModelWithShortcutInfo:(ZMChatAppShortcutInfo *)shortcutInfo;
- (void)updateShortcutInfo:(ZMChatAppShortcutInfo *)shortcutInfo;

- (BOOL)isCloseButtonHidden;
- (BOOL)isTitleHidden;
- (BOOL)isAppNameHidden;
- (BOOL)isAppIconHidden;

- (NSInteger)titleLineHeight;

- (NSInteger)nameLineHeight;

- (NSInteger)webViewTopMargin;

- (CGSize)webViewSize;

- (CGSize)windowSize;

- (NSEdgeInsets)contentMargin;

- (NSString *)windowAccessibilityLabel;

- (NSString *)dialogSubTitle;

- (BOOL)windowMovableByWindowBackground;

@end

@interface ZMChatAppContext : NSObject

@property (nonatomic, retain, readonly) ZMChatAppShortcutInfo *shortcutInfo;

@property (nonatomic, readonly) NSString *chatAppId;
@property (nonatomic, readonly) NSString *chatAppUrl;

@property (nonatomic, assign) ZMChatSessionType sessionType;
@property (nonatomic, copy, nullable) NSString *sessionID;
@property (nonatomic, copy, nullable) NSString *messageID;
@property (nonatomic, copy, nullable) NSString *threadID;

@property (nonatomic, weak) NSWindow *hostWindow;
@property (nonatomic, assign) NSRectCorner windowResizeCorner;

@property (nonatomic, copy, nullable) NSString *webViewId;

/// Draft
@property (nonatomic, copy, nullable) NSString *inputMessage;
@property (nonatomic, copy, nullable) NSString *selectedInputText;

@property (nonatomic, copy, nullable) NSString *customPayload;

@property (nonatomic, assign, getter=isMovableWindow) BOOL movableWindow;

/// deprecated
@property (nonatomic, assign, getter=isAutoClose) BOOL autoClose;

@property (nonatomic,copy) NSString *smsInfo; //ZOOM-610954 for sms

@property (nonatomic, assign) BOOL didWindowMoved;

/// float window, when hover link
@property (nonatomic, weak) NSView *associatedView;
@property (nonatomic, assign) NSRect relativeRect;
@property (nonatomic, assign) NSPoint position;
@property (nonatomic, copy) NSString *hoveredLink;
@property (nonatomic, assign) BOOL hideWhenScrollStarted;
@property (nonatomic, assign) ZMChatFloatAppOpenSource openSource;
@property (nonatomic, assign) float closeDelayTimeinSeconds;
@property (nonatomic, assign) BOOL disableAsKeyWindow;

@property (nonatomic, copy) NSString *routingUrl;
@property (nonatomic) NSInteger appLocation;

@property (nonatomic, assign, readonly) BOOL isLocalApp;

@property (nonatomic, assign, readonly) BOOL isEditingMessage;

@property (nonatomic, assign) BOOL isFromWebChat;
@property (nonatomic, copy) NSString *webChatViewId;
@property (nonatomic, assign) BOOL didInput;

+ (instancetype)contextWithShortcutInfo:(ZMChatAppShortcutInfo *)shortcutInfo;

- (NSImage *)shortcutIcon:(NSSize)size radius:(CGFloat)radius;

- (NSImage *)dialogTitleIcon;

@end

NS_ASSUME_NONNULL_END
