//
//  ZMMessageAccessoryView.h
//  zChatUI
//
//  Created by Yong Zhou on 7/29/20.
//  Copyright © 2020 Zoom. All rights reserved.
//


#import <zChatComponent/ZMFileView.h>
#import <ZoomUnit/ZMFileConstants.h>
#import <zChatComponent/ZMHybridImageView.h>
#import <zChatComponent/ZMWhiteboardView.h>
#import <ZoomKit/ZMGridAccessibilityView.h>
#import <zChatComponent/ZMDeepLinkPreviewView.h>
#import <zChatComponent/ZMDeepLinkEntity.h>
#import <zChatComponent/ZMDeeplinkItemInfo.h>
#import <zChatComponent/ZMVoiceItemInfo.h>
#import <zChatComponent/ZMVideoPlayerView.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMFileEntity;

@interface ZMMessageAccessoryView : ZMGridAccessibilityView <ZMMutableSelectProtocal>

@property (nonatomic, weak) id<ZMFileViewDelegate> fileDelegate;

@property (nonatomic, weak) id<ZMHybridImageViewDelegate> imageDelegate;

@property (nonatomic, weak) id<ZMVideoPlayerViewDelegate> videoDelegate;

@property (nonatomic, weak) id<ZMDeepLinkPreviewViewDelegate> deppLinkDelegate;

@property (nonatomic, retain, nullable) NSArray<ZMDeeplinkItemInfo *> *deepLinkList;

@property (nonatomic, retain, nullable) NSArray<ZMFileEntity *> *whiteboardList;

@property (nonatomic, retain, nullable) NSArray<ZMVoiceItemInfo *> *voiceInfoList;

@property (nonatomic, assign) BOOL isSelected;

@property (nonatomic, assign) BOOL shouldFoldImage;

@property (nonatomic, assign) BOOL needHoverButtons;
@property (nonatomic, assign) BOOL shouldShowQuickDownloadButton;
@property (nonatomic, assign) ZMChatFileSummaryStatus fileSummaryStatus; // ZOOM-912558
@property (nonatomic, assign) BOOL isAttachedToReplyMessage;

@property (nonatomic, assign) BOOL isSingleFileMessage;

/*!
 The width of the first row of accessories.
 @discussion This property is set when the view is laid out in @c layoutSubviewByMaxWidth:messagePaneWidth:contentAbove:
 @note This property is only valid if the view was laid out with @b no content above.
 If there is content above, you should use the width of that content instead.
 */
@property (nonatomic, readonly, assign) CGFloat topRowWidth;
/*! The spacing inbetween different accessories
 @note This is both the spacing inbetween accessories of the same type (e.g. image <-> image) and accessories of different types (e.g. image <-> file)
 */
@property (nonatomic, assign) CGFloat accessorySpacing;

- (void)cleanContent;

- (void)setSMSImageList:(NSArray<ZMFileEntity *> * _Nullable)images
              videoList:(NSArray<ZMFileEntity *> * _Nullable)videos
               fileList:(NSArray<ZMFileEntity *> * _Nullable)files;

- (void)setImageList:(NSArray<ZMFileEntity *> * _Nullable)images
           videoList:(NSArray<ZMFileEntity *> * _Nullable)videos
            fileList:(NSArray<ZMFileEntity *> * _Nullable)files;

- (void)layoutSubviewByMaxWidth:(CGFloat)accessoryViewMaxWidth;
- (NSSize)calculateViewByMaxWidth:(CGFloat)accessoryViewMaxWidth maxTextWidth:(CGFloat)maxTextWidth;
// ZOOM-354983: functions for new UI
- (void)layoutSubviewByMaxWidth:(CGFloat)accessoryViewMaxWidth messagePaneWidth:(CGFloat)messagePaneWidth
                   contentAbove:(BOOL)hasContentAbove;
- (NSSize)calculateViewByMaxWidth:(CGFloat)accessoryViewMaxWidth messagePaneWidth:(CGFloat)messagePaneWidth;

@end

NS_ASSUME_NONNULL_END
