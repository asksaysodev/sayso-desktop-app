//
//  ZMStickerMgr.h
//  SaasBeePTUIModule
//
//  Created by likevin on 10/25/16.
//  Copyright © 2016 Zipow. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <cmmlib/CmmDef.h>
#include <zPTApp/SaasBeePTAppInterface.h>
#include <zChatApp/StickerInterface.h>
#import <zChatComponent/ZMGridViewDelegate.h>
#include "cmmlib/CmmDef.h"
#import "zPTApp/SaasBeePTAppInterface.h"
#include "zChatApp/StickerInterface.h"
#import "ZMGridViewDelegate.h"
#import <zChatComponent/ZMEmojiModel.h>

@class ZMChatFileItem;
@class ZMChatEmojiWindowController;
@class ZMEmojiItemData;
@class ZMStickerItemData;
@class ZMGiphyEmojiItemData;
@class ZMStickerInfo;
@class ZMFileEntity;

@protocol ZMChatEmojiOperationDelegate <NSObject>

@optional;

- (BOOL)canShowCustomEmoji;

- (void)didSelectedEmoji:(ZMEmojiModel *)emoji toneIndex:(NSInteger)index;

- (void)didSelectedGiyph:(ZMGiphyEmojiItemData *)customGiphyData;

- (void)didSelectedSticker:(ZMStickerItemData *)stickerData;

- (void)didSelectedEmojiRemoved;


- (NSString *)currentLoginUser;

- (NSString *)emojiOperation_currentSessionId;

- (BOOL)isEnableSelectedEmoji;
- (BOOL)isEnabledChatEmoji;

@end

@interface ZMStickerDataSrcMgr : NSObject

@property(nonatomic, readwrite, assign) BOOL isNeedScrollToEnd;
- (void)beginEdit;
- (void)endEdit;
- (BOOL)isHasExisted:(ZMStickerInfo*)inStickerInfo;
- (void)attach2GridView:(id)inGridView;
- (NSUInteger)itemDataCount;
- (ZMStickerItemData*)gridItemByStickerFileId:(NSString*)fileid;
- (void)removeAllItems;
- (void)cleanUp;
- (void)addSticker:(NSString*)strFilePath stickerInfo:(ZMStickerInfo*)inStickerInfo date:(NSDate*)inItemDate;
- (void)gridViewReloadAllItems;
- (void)removeMultiStickers:(NSArray*)inItemArray;
@end


@interface ZMStickerMgr : NSObject

@property(nonatomic, readonly)          BOOL isUISinkEnabled;
@property(nonatomic, readwrite, retain) ZMChatEmojiWindowController* emojiWindowController;
@property(nonatomic, readwrite, retain) ZMStickerDataSrcMgr* stickerDataSrcMgr;

@property (nonatomic, assign) BOOL isNeedHideBottomToolBar;
@property (nonatomic, assign) BOOL isNeedShowHeaderView;
@property (nonatomic, retain) NSString *currentSelectedEmoji;

+ (ZMStickerMgr*)sharedInstance;

- (void)showEmojiWithPositionView:(NSView *)positionView delayShow:(BOOL)delayShow delegate:(id<ZMChatEmojiOperationDelegate>)delegate;

- (void)showEmojiWithPositionView:(NSView *)positionView delayShow:(BOOL)delayShow emojiType:(NSInteger)emojiType delegate:(id<ZMChatEmojiOperationDelegate>)delegate;

- (void)closeEmojiWindow;

- (void)cleanUp;

- (NSUInteger)expectStickerItemCount;
- (NSUInteger)requstDownloadItemCount;
- (void)checkNotDownloadedItems;
- (void)reloadAllStickers;
- (void)makePrivateStickerByMsgWebFileID:(NSString*)inMsgWebFileId withSessionId:(NSString*)inSessionId parentWindow:(NSWindow*)inParentWindow;
- (void)makePrivateStickerByFileItem:(ZMFileEntity *)fileItem  parentWindow:(NSWindow*)inParentWindow;
- (void)uploadAndMakeSticker:(NSString*)strFilePath sessionId:(NSString*)inCurrentSessionId;
- (BOOL)discardSticker:(ZMStickerInfo*)inStickerInfo;
- (BOOL)discardMultiSticker:(NSArray*)inGridItemArray;

//callback
- (void)onPrepareRegister;
- (void)onUnregistered;
- (void)onPrivateStickersUpdated;
- (void)onMakePrivateSticker:(NSUInteger)inResult stickerInfo:(ZMStickerInfo*)inStickerInfo;
- (void)onDiscardPrivateSticker:(NSUInteger)inResult stickerInfo:(ZMStickerInfo*)inStickerInfo;
- (void)onNewStickerUploaded:(NSString*)inReqId result:(NSUInteger)inResult stickerInfo:(ZMStickerInfo*)inStickerInfo;
- (void)onStickerDownloaded:(NSString*)inReqId result:(NSUInteger)inResult;

- (void)showStickerFileSizeExceed4MAlert:(NSWindow*)parentWindow;

+ (ZMChatFileItem*)fileItemByStickerInfo:(ZMStickerInfo*)inStikerInfo;
//mail
- (void)showEmojiWithPositionView:(NSView *)posView delegate:(id)delegate show:(BOOL)show emojiType:(NSInteger)emojiType rect:(NSRect)rect;

@end
