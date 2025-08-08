//
//  ZMHybridImageView.h
//  ZCommonUI
//
//  Created by Yong Zhou on 9/23/20.
//  Copyright © 2020 zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <ZoomKit/ZMTrackingView.h>

@class ZMFileEntity;

typedef NS_OPTIONS (NSUInteger, ZMImageCornerMask) {
    ZMImageCornerMaskNone = 0,
    ZMImageCornerMaskTopLeft = 1 << 0,
    ZMImageCornerMaskTopRight = 1 << 1,
    ZMImageCornerMaskBottomRight = 1 << 2,
    ZMImageCornerMaskBottomLeft = 1 << 3,
    
    ZMImageCornerMaskTop = ZMImageCornerMaskTopLeft | ZMImageCornerMaskTopRight,
    ZMImageCornerMaskBottom = ZMImageCornerMaskBottomRight | ZMImageCornerMaskBottomLeft,
    ZMImageCornerMaskLeft = ZMImageCornerMaskTopLeft | ZMImageCornerMaskBottomLeft,
    ZMImageCornerMaskRight = ZMImageCornerMaskTopRight | ZMImageCornerMaskBottomRight,
    ZMImageCornerMaskAll = ZMImageCornerMaskTopLeft | ZMImageCornerMaskTopRight | ZMImageCornerMaskBottomRight | ZMImageCornerMaskBottomLeft
};

NS_ASSUME_NONNULL_BEGIN

@protocol ZMHybridImageViewDelegate <NSObject>

@optional

- (void)fileViewRightMouseClickedWithInfo:(ZMFileEntity *)fileModel atPoint:(NSPoint)aPoint;

- (void)fileViewOpenFileWithInfo:(ZMFileEntity *)fileModel;

// for ZMMessageAccessoryView
- (void)onImagesFoldStatusChanged:(BOOL)isFold;

- (void)downloadButtonClickWithFile:(ZMFileEntity *)file;
- (void)moreButtonClick:(NSButton *)button file:(ZMFileEntity *)file;
- (BOOL)needSaveAsButtonWithFile:(ZMFileEntity *)fileModel;
- (BOOL)needMoreButtonWithFile:(ZMFileEntity *)fileModel;
- (BOOL)needPauseIconWithFile:(ZMFileEntity *)fileModel;
@end

@interface ZMHybridImageView : ZMTrackingView

@property (nonatomic, assign) ZMImageCornerMask maskedCorners;

@property (nonatomic, assign) BOOL isSelected;

@property (nonatomic, weak) id<ZMHybridImageViewDelegate> delegate;

@property (nonatomic, retain) ZMFileEntity *fileModel;

@property (nonatomic, assign) NSSize imageMaxSize;

@property (nullable, nonatomic, copy) CALayerContentsGravity contentsGravity;

@property (nonatomic, assign) BOOL needHoverButtons;
/*! Whether the view uses the new UI from ZOOM-354983 */
@property (nonatomic, assign) BOOL imageFileSharingImprovement;

- (void)dispatchRightMouseEventWithPoint:(NSPoint)point;

@end

NS_ASSUME_NONNULL_END
