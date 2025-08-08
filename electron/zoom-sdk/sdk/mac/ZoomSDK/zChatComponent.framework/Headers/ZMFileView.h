//
//  ZMFileView.h
//  zChatUI
//
//  Created by Yong Zhou on 7/30/20.
//  Copyright © 2020 Zoom. All rights reserved.
//

#import <ZoomKit/ZoomKit.h>
#import <zChatComponent/ZMChatMsgItemInfo.h>
@class ZMFileEntity;

typedef NS_ENUM(NSUInteger, ZMFileUseType) {
    ZMFileUseType_SingleFileMessage = 0,
    ZMFileUseType_HybridMessage = 1,  /* pause、resume and cancel operations are not supported when uploading. */
};

@protocol ZMFileViewDelegate <NSObject>

@optional

- (void)fileViewRightMouseClickedWithInfo:(ZMFileEntity *)fileModel atPoint:(NSPoint)aPoint;
- (void)fileViewOpenFileWithInfo:(ZMFileEntity *)fileModel;
/* Used for send-failed hybrid message. When clicking on the send failed file content area, should auto resend the whole message */
- (void)mouseClickedOnFailedFileView;
- (void)summarizeButtonClicked:(ZMFileEntity*)file button:(NSButton *)button;
- (void)moreButtonClick:(NSButton *)button file:(ZMFileEntity *)file;
- (void)quickDownloadButtonClickedWithFile:(ZMFileEntity*)file;
@end


@interface ZMFileView : ZMBaseView

@property (nonatomic, weak) id<ZMFileViewDelegate> delegate;
@property (nonatomic, assign) ZMFileUseType  useType;
@property (nonatomic, strong) ZMFileEntity   *fileModel;
@property (nonatomic, strong) ZMChatMsgItemInfo *msgItem;
/// will center the title,  and hide the detail info
@property (nonatomic, assign) BOOL showTitleOnly;
@property (nonatomic, assign) BOOL isSelected;
@property (nonatomic, assign) BOOL needMoreButton;
@property (nonatomic, assign) BOOL needDownloadButton;
@property (nonatomic, assign) ZMChatFileSummaryStatus fileSummaryStatus;
@property (nonatomic, assign) BOOL enableDownloadButton;
/*! Whether the view uses the new UI from ZOOM-354983 */
@property (nonatomic, assign) BOOL imageFileSharingImprovement;
- (void)dispatchRightMouseEventWithPoint:(NSPoint)point;

- (void)resumeUploadFile;

@end

