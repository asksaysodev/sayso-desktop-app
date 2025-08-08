//
//  ZMTitleBarViewController.h
//  zChatComponent
//
//  Created by groot.ding on 11/15/23.
//

#import <ZoomKit/ZoomKit.h>
#import <zChatComponent/ZMButtonItem.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMTitleBarItem : ZMButtonItem

@property (nonatomic, strong, nullable) NSColor *backgroundColor;

@property (nonatomic, assign) int buttonStyle;

@property (nonatomic, assign) int radius;

@end

@interface ZMTitleBarViewController : ZMBaseViewController

@property (nonatomic,copy,nullable) NSAttributedString *attributedTitle;

@property (nonatomic,copy,nullable) NSArray <ZMTitleBarItem *> *leftItems;

@property (nonatomic,copy,nullable) NSArray <ZMTitleBarItem *> *rightItems;

@property (nonatomic,strong,nullable) ZMButton *btnDetail;

@property (nonatomic,strong,nullable) ZMButton *backButton;

@property (nonatomic, assign) BOOL showExternalLabel;

@property (nonatomic, strong) NSView *classificationLabel;

@property (nonatomic, strong) ZMImageView *archivedImage;

@end

NS_ASSUME_NONNULL_END
