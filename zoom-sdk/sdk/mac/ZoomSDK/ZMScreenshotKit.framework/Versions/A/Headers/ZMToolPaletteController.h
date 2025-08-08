//
//  ZMToolPaletteController.h
//  ZMScreenCapture
//
//  Created by javenlee on 2018/2/13.
//  Copyright © 2018 ZOOM. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ZMToolPaletteView : NSView

@end

typedef NS_ENUM(NSInteger, ZMToolSelect){
    ZMPencilTool = 1001,
    ZMArrowTool,
    ZMRectangleTool,
    ZMCircleTool,
    ZMTextTool,
    ZMMosaicTool,
    ZMDownloadTool,
    ZMUndoTool,
    ZMCancelTool,
    ZMCutTool,
};

@protocol ZMToolPaletteDelegate <NSObject>
@optional
- (void)toolPaletteClicked:(ZMToolSelect)senderTag;
@end

@class ZMCaptureButton, ZMBackgroundButton, ZMCancelButton;
@interface ZMToolPaletteController : NSViewController
@property (strong) ZMCaptureButton *pencilButton;
@property (strong) ZMCaptureButton *arrowButton;
@property (strong) ZMCaptureButton *recButton;
@property (strong) ZMCaptureButton *circleButton;
@property (strong) ZMCaptureButton *textButton;
@property (strong) ZMCaptureButton *mosaicButton;
@property (strong) ZMCaptureButton *downloadButton;
@property (strong) ZMCaptureButton *undoButton;
@property (strong) NSBox *line;
@property (strong) ZMBackgroundButton *captureButton;
@property (strong) ZMCancelButton *cancelButton;

@property (nonatomic, strong) NSString *captureTitle;
@property (nonatomic,assign) id <ZMToolPaletteDelegate> delegate;
@property (nonatomic,assign) BOOL canUndo;
@property (nonatomic,assign) NSRectEdge popUpEdge;

+ (id)sharedToolPaletteController;
- (void)updateCurrentGraphicClass:(ZMToolSelect)tool;
- (Class)currentGraphicClass;
- (void)popoverToTagButton:(NSInteger)tag;
- (void)closePopup;
- (void)reset;


@end
