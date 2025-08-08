//
//  IZMPTAlertMgr.h
//  PTUIBridge
//
//  Created by fistice on 2024/10/25.
//

#ifndef IZMPTAlertMgr_h
#define IZMPTAlertMgr_h

@protocol IZMPTAlertMgr <NSObject>


- (void)closeAlert;
- (void)closeAlertWithType:(int)inType;
- (BOOL)isChecked;

- (void)showAlertWithInfo:(NSString*)inInfo;
- (void)showAlertWithInfo:(NSString*)inInfo alertType:(int)inType;
- (void)showAlertWithInfo:(NSString*)inInfo autoCloseTime:(int)inTime;
- (void)showAlertWithInfo:(NSString*)inInfo autoCloseTime:(int)inTime alertType:(int)inType;
- (void)showAlertWithInfo:(NSString*)inInfo autoCloseTime:(int)inTime windowTitle:(NSString *)windowTitle;

- (void)showSecurityAlertWithAttributeInfo:(NSAttributedString*)inInfo alertType:(int)inType customView:(NSView *)view;

- (void)showSheetWithInfo:(NSString*)inInfo window:(NSWindow *)window alertType:(int)inType completionHandler:(void (^)(NSInteger index))handler;
- (void)showDeviceAuthorizedAlert:(NSString*)type;//ZOOM-57569
- (void)closeDeviceAuthorizedAlert;

// ZMPopupAlertViewController
- (void)showPopAlertWithTitle:(NSString *)title content:(NSString *)content confirmTitle:(NSString*)confirmTitle showCancel:(BOOL)showCancel actionConfirm:(void(^)())actionConfirm;
- (void)showPopAlertWithTitle:(NSString *)title content:(NSString *)content confirmTitle:(NSString*)confirmTitle showCancel:(BOOL)showCancel actionConfirm:(void(^)())actionConfirm inWindow:(NSWindow *)window;

- (void)showPopAlertWithTitle:(NSString *)title content:(NSString *)content confirmTitle:(NSString*)confirmTitle cancelTitle:(NSString *)cancelTitle actionConfirm:(void(^)())actionConfirm inWindow:(NSWindow *)window;

- (void)showPopAlertWithTitle:(NSString *)title content:(NSString *)content checkInfo:(NSString*)checkInfo confirmTitle:(NSString*)confirmTitle cancelTitle:(NSString *)cancelTitle actionConfirm:(void(^)())actionConfirm inWindow:(NSWindow *)window;

- (void)showPopAlertWithTitle:(NSString *)title content:(NSString *)content confirmTitle:(NSString*)confirmTitle cancelTitle:(NSString *)cancelTitle elapsedTime:(NSInteger)seconds actionConfirm:(void(^)())actionConfirm inWindow:(NSWindow *)window;

- (void)showPopAlertWithTitle:(NSString *)title content:(NSString *)content confirmTitle:(NSString*)confirmTitle confirmStyle:(NSUInteger)confirmStyle cancelTitle:(NSString *)cancelTitle actionConfirm:(void(^)())actionConfirm inWindow:(NSWindow *)window;

- (void)showPopAlertWithTitle:(NSString *)title content:(NSString *)content selectItems:(NSArray <NSString *> *)selectItems confirmTitle:(NSString*)confirmTitle inWindow:(NSWindow *)window actionConfirm:(void(^)(NSString *))actionConfirm;
- (void)showPopAlertWithTitle:(NSString *)title contentAttributedString:(NSAttributedString *)contentAttributedString confirmTitle:(NSString*)confirmTitle showCancel:(BOOL)showCancel actionConfirm:(void(^)())actionConfirm inWindow:(NSWindow *)window;
- (void)showPopAlertWithTitle:(NSString *)title content:(NSString *)content contentAttributedString:(NSAttributedString *)contentAttributedString confirmTitle:(NSString*)confirmTitle confirmStyle:(NSUInteger)confirmStyle showCancel:(BOOL)showCancel cancelTitle:(NSString *)cancelTitle actionConfirm:(void(^)())actionConfirm moreInfoTitle:(NSAttributedString *)moreInfoTitle actionMoreInfo:(void(^)())actionMoreInfo inWindow:(NSWindow *)window;

- (void)showNewPopAlertWithTitle:(NSString *)title content:(NSString *)content confirmTitle:(NSString*)confirmTitle showCancel:(BOOL)showCancel cancelTitle:(NSString *)cancelTitle actionConfirm:(void(^)(int index))actionConfirm inWindow:(NSWindow *)window;

- (void)showTranscodeFileNotExistAlert:(NSArray *)fileArray;
- (void)showTranscodeIsLaunchingAlert:(NSArray *)fileArray;
- (void)showUnconvertRecordAlert:(NSArray *)fileArray;

- (void)showDisableDirectShareAlert;
- (void)showDirectShareMonitorDisconnectedAlert;
- (void)showDirectShareWindowClosedAlert;

- (void)showOpenDirFailedAlert:(BOOL)inThisComputer;

- (void)showCrashReportPanel;
+ (NSInteger)checkAudioDeviceAuthorized;
+ (NSInteger)checkVideoDeviceAuthorized;

- (void)showRecaptchaControllerWithImagePath:(NSString *)recaptchaImagePath audioPath:(NSString *)recaptchaAudioPath lastFailed:(BOOL)lastFailed;

- (void)showSessionExpiredAlert;

- (void)showDeepLinkError:(NSInteger)result title:(NSString *)title message:(NSString *)message;
//OnZoom
- (void)showUnableJoinLobbyAlert;
- (void)showZrcDetectFailedAlert:(NSString *)title information:(NSString *)information;
- (void)showSaveZRWBAlert:(NSString *)title
              information:(NSString *)information
             acceptAction:(void(^)())acceptAction
            declineAction:(void(^)())declineAction;
- (void)closeSaveZRWBAlert;

- (void)showReportFinishAlert;

- (void)showUnableScreenRecordingAlert;

@end
#endif /* IZMPTAlertMgr_h */
// ZMSharedFor(IZMPTAlertMgr)
