//
//  ZMChatViewController.h
//  zChatComponent
//
//  Created by groot.ding on 11/14/23.
//

#import <ZoomKit/ZoomKit.h>
#import <zChatComponent/ZMMessageContentViewController.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMChatsInputContainerViewCtrl;
@class ZMTitleBarViewController;

@interface ZMSubChatMessageContentViewController : ZMMessageContentViewController

@end

@interface ZMChatViewController : ZMBaseViewController

@property (nonatomic,copy) NSString* sessionID;

@property (nonatomic,strong) ZMSubChatMessageContentViewController *messageViewController;

@property (nonatomic,strong) ZMChatsInputContainerViewCtrl *inputViewController;

@property (nonatomic,strong) ZMTitleBarViewController *titlebarViewController;

@property (nonatomic, assign) BOOL showExternalLabel;

- (instancetype)initWithSessionID:(NSString*)sessionID;

- (void)updateTitleBarWithTitle:(NSString *)title
              showExternalLabel:(BOOL)showExternalLabel;

- (void)updateTitleBarWithTitle:(NSString *)title;

@end

NS_ASSUME_NONNULL_END
