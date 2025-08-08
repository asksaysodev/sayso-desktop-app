//
//  ZMPopAlertManager.h
//  ZoomMsgUI
//
//  Created by Groot Ding on 2022/5/7.
//

#import <Foundation/Foundation.h>

@interface ZMPopAlertManager : NSObject

+ (instancetype)alertManager;

- (BOOL)isChecked;

- (void)showPopAlertWithTitle:(NSString *)title
                      content:(NSString *)content
                 confirmTitle:(NSString*)confirmTitle
                   showCancel:(BOOL)showCancel
                actionConfirm:(void(^)(void))actionConfirm
                     inWindow:(NSWindow *)window;

- (void)showNewPopAlertWithTitle:(NSString *)title
                         content:(NSString *)content
                    confirmTitle:(NSString*)confirmTitle
                      showCancel:(BOOL)showCancel
                     cancelTitle:(NSString *)cancelTitle
                   actionConfirm:(void(^)(int index))actionConfirm
                        inWindow:(NSWindow *)window;

- (void)showPopAlertWithTitle:(NSString *)title
                      content:(NSString *)content
                 confirmTitle:(NSString*)confirmTitle
                 confirmStyle:(NSUInteger)confirmStyle
                  cancelTitle:(NSString *)cancelTitle
                actionConfirm:(void(^)(void))actionConfirm
                     inWindow:(NSWindow *)window;

- (void)showPopAlertWithTitle:(NSString *)title
                      content:(NSString *)content
                    checkInfo:(NSString*)checkInfo
                 confirmTitle:(NSString*)confirmTitle
                  cancelTitle:(NSString *)cancelTitle
                actionConfirm:(void(^)(void))actionConfirm
                     inWindow:(NSWindow *)window;

- (void)showPopAlertWithTitle:(NSString *)title
                      content:(NSString *)content
      contentAttributedString:(NSAttributedString *)contentAttributedString
                 confirmTitle:(NSString*)confirmTitle
                 confirmStyle:(NSUInteger)confirmStyle
                   showCancel:(BOOL)showCancel
                  cancelTitle:(NSString *)cancelTitle
                actionConfirm:(void(^)(void))actionConfirm
                moreInfoTitle:(NSAttributedString *)moreInfoTitle
               actionMoreInfo:(void(^)(void))actionMoreInfo
                     inWindow:(NSWindow *)window;

- (void)showPopAlertWithTitle:(NSString *)title
                      content:(NSString *)content
                  selectItems:(NSArray <NSString *> *)selectItems
                 confirmTitle:(NSString*)confirmTitle
                     inWindow:(NSWindow *)window
                actionConfirm:(void(^)(NSString *))actionConfirm;

//MARK: - adapt for meeting style
@property (nonatomic, assign) BOOL needPopout;
- (void)closePopoutPanelIfNeed;

@end
