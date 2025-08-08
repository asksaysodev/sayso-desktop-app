//
//  ZMChatEmojiWindowController.h
//  SaasBeePTUIModule
//
//  Created by ryan on 9/2/15.
//  Copyright (c) 2015 Zipow. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <zChatComponent/ZMGridViewDelegate.h>
#import <zChatComponent/ZMStickerMgr.h>
#import <ZoomUnit/ZoomUnit.h>
#import <zChatComponent/ZMSysEmojiMgr.h>

@class ZMButton;
@class ZMChatEmojiToolbarView;
@class ZMStickerDataSrcMgr;
@class ZMCollectionView;
@class ZMSysEmojiMgr;
@class ZMGiphyMgr;
@class ZMChatsInputContainerViewCtrl, ZMCustomSearchField;
@class ZMChatAlwaysOverlayScrollView;
@protocol ZMEmojiDisplayViewDelegate;

typedef enum {
    ADD_STICKER_STYLE = 1,
    SYNC_STICKER_STYLE,
    NORMAL_STICKER_STYLE,
} STICKER_UI_STYLE;

typedef enum {
    GIPHY_NORMAL_STYLE = 1,
    GIPHY_LOADING_STYLE,
    GIPHY_NO_MATCHED_STYLE,
    GIPHY_NET_ERROR_STYLE,
    GIPHY_SERVER_ERROR_STYLE,
} GIPHY_UI_STYLE;

typedef NS_ENUM(NSUInteger, ZMShowEmojiType) {
    ZMShowEmoji          = 1,
    ZMShowGiyph          = 1 << 1,
    ZMShowSticker        = 1 << 2,
    
    ZMShowEmojiReaction  = (ZMShowEmoji | (1 << 3)),
};

@interface ZMChatEmojiListView : NSView
{
    NSTrackingArea*     _area;
    float           _angleLoaction;
    NSColor*        _triangleBackgroundColor;
}
@property (copy) NSColor* triangleBackgroundColor;
@property (assign) float angleLocation;


@end

@interface ZMChatEmojiListWindow : NSWindow
- (void)invisibleInSharing;
@end

@interface ZMChatEmojiWindowController : NSWindowController<ZMGridViewDelegate,NSTabViewDelegate,
        ZMSysEmojiMgrDelegate, ZMEmojiDisplayViewDelegate>

@property(nonatomic, readonly) ZMSysEmojiMgr* sysEmojiMgr;
@property(nonatomic, readonly) ZMGiphyMgr*    giphyMgr;
@property(nonatomic, readonly) NSInteger showEmojiType;
@property(nonatomic, retain) id<ZMChatEmojiOperationDelegate> emojiOperationDelegate;

@property (nonatomic, assign) BOOL isNeedHideBottomToolBar;
@property (nonatomic, assign) BOOL isNeedShowHeaderView;
@property (nonatomic, copy) NSString *currentSelectedEmoji;

- (instancetype)init;

- (void)showEmojiWithPositionView:(NSView *)positionView delayShow:(BOOL)delayShow emojiType:(NSInteger)emojiType delegate:(id<ZMChatEmojiOperationDelegate>)delegate;
//mail
- (void)showEmojiWithPositionView:(NSView *)posView emojiType:(NSInteger)emojiType delegate:(id<ZMChatEmojiOperationDelegate>)delegate rect:(NSRect)rect;

- (void)setIsStickerEnabled:(BOOL)isStickerEnabled isE2ESession:(BOOL)isE2ESession;
- (BOOL)isEditMode;
- (void)createEmojiDataWhenLogin;
- (void)stickerViewScrollBottom;
- (void)schedule2CloseEmojiWnd;
- (void)schedule2CloseEmojiWndWith:(NSTimeInterval)timeInterval;
- (IBAction)addEmojiBtnClicked:(id)sender;
- (void)cancel2CloseEmojiWnd;
- (void)clickedEmojiItem:(ZMEmojiModel *)model toneIndex:(NSInteger)index;
- (BOOL)isGiphyNormalView;
- (void)updateGiphyView:(NSUInteger)viewType;
- (NSView *)positionView;

//sdk callbacks
- (void)updateSyncServerProgress:(NSUInteger)inExpectNum completeNum:(NSUInteger)inCompletNum reqNum:(NSUInteger)inReqNum;
- (void)updateSyncServerProgressWhenDownloadFailed:(NSUInteger)inExpectNum completeNum:(NSUInteger)inCompletNum reqNum:(NSUInteger)inReqNum;

@end
