//
//  ZMWhiteboardView.h
//  ZoomUnit
//
//  Created by Kevin Li on 2022/3/31.
//

#import <Cocoa/Cocoa.h>

@protocol ZMHybridImageViewDelegate ;

NS_ASSUME_NONNULL_BEGIN

@class ZMFileEntity;

@interface ZMWhiteboardView : ZMTrackingView

@property (nonatomic, assign) BOOL isSelected;

@property (nonatomic, weak) id<ZMHybridImageViewDelegate> delegate;

@property (nonatomic, retain) ZMFileEntity *fileModel;

@property (nonatomic, assign) BOOL needHoverButtons;
@property (nonatomic, assign) NSRectCorner maskedCorners;

- (void)dispatchRightMouseEventWithPoint:(NSPoint)point;

@end

NS_ASSUME_NONNULL_END
