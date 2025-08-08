//
//  ZMVEVideoPreview.h
//  ZoomSetting
//
//  Created by Vinson Wang on 2024/4/15.
//

#import <ZoomKit/ZoomKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMVEVideoPreview : ZMBaseView

@property (nonatomic, assign) BOOL previewing;

- (void)viewDidAppear;

- (void)viewWillDisappear;

- (void)updateView;
@end

@interface ZMVEVideoPreview (MeetingSDK)
@property (nonatomic, strong, readonly) ZMBaseView *preview;
@property (nonatomic, strong, readonly) ZMBaseView *pickColorBgView;

- (void)setVBColorSelectMode:(BOOL)VBColorSelectMode;
- (void)updatePickColorView;
- (void)endSelectReplaceVBColorWithPoint:(NSPoint)point;
@end

NS_ASSUME_NONNULL_END
