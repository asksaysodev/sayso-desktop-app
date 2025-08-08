//
//  ZMChatMessageCheckerUtil.h
//  ZoomMsgUI
//
//  Created by Yong Zhou on 5/17/22.
//

#import <Foundation/Foundation.h>
#import <ZoomUnit/ZMFileConstants.h>

@class ZMFileEntity;
@class ZMMessageAdapter;
@class ZMChatAppMessagePreviewInfo;
@class ZMMessageAdapter, ZMToast;

NS_ASSUME_NONNULL_BEGIN

#ifdef __cplusplus
namespace ns_zoom_messager {
    class IZoomFileInterface;
}
#endif

@protocol ZMSecurityAlertProxy <NSObject>

- (BOOL)needAlertForFileEntity:(ZMFileEntity *)fileEntity;
- (BOOL)needShowAlwaysTrustCheckButton:(ZMFileEntity *)fileEntity;

@optional
- (void)didSelectedConfirmButton:(id<ZMAlertProtocol>)alert forFileEntity:(ZMFileEntity *)fileEntity;
- (void)didSelectedCancelButton:(id<ZMAlertProtocol>)alert forFileEntity:(ZMFileEntity *)fileEntity;
- (void)didClosedAlertWithESC:(id<ZMAlertProtocol>)alert forFileEntity:(ZMFileEntity *)fileEntity;

- (NSPoint)alertCenterPosition;

@end

@interface ZMChatMessageCheckerUtil : NSObject

+ (ZMChatMessageCheckerUtil*)sharedInstance;
+ (void)releaseInstance;

+ (BOOL)checkIfBeyondMaximunMessageLength:(NSUInteger)msgLength;

///  Check if the text length exceeds the maximum limitation
///   - msgLength: msgLength description
///   - isPersistentMeetingOnPregress: the persistent meeting is on or not
///   - whenSendingMessage: If the value is YES and isPersistentMeetingOnPregress is YES, the  the max limitation will change to ' maxChatMessageLength',  cause the text length may exceed kMaxPMCChatMessageLength1K before the meeting starts.  If whenSendingMessage is NO and isPersistentMeetingOnPregress is YES, the max limitration will  set to  kMaxPMCChatMessageLength1K,  which will only check when trying to typing more...(inputing,quoting...)
///   - sessionId: for distinguish meeting chat or team chat. In meeting chat, the maximum of the message length is kMaxPMCChatMessageLength1K.
+ (BOOL)checkIfBeyondMaximunMessageLength:(NSUInteger)msgLength
            isPersistentMeetingOnPregress:(BOOL)isPersistentMeetingOnPregress
                              whenSending:(BOOL)whenSendingMessage
                                sessionId:(NSString *)sessionId;

/// check if shortAppCard reach kMaxTotalPreviewCardsSize limite
/// - Parameters:
///   - previewInfo: preview card
///   - appCardListInfoArr: should not contain unfurl link
+ (BOOL)checkIfBeyondMaximunShortAppCardPayloadLength:(ZMChatAppMessagePreviewInfo *)previewInfo
                                        existAppCards:(NSArray<ZMChatAppMessagePreviewInfo *> *)appCardListInfoArr;

+ (BOOL)checkWithToastWhenAddingToEmojiWithFileItem:(ZMFileEntity *)fileItem sessionID:(NSString *)sessionID toastInView:(NSView *)view;

+ (BOOL)checkWithToastWhenSharingFileItem:(ZMFileEntity *)fileItem sessionID:(NSString *)sessionID toastInView:(NSView *_Nullable)view;

+ (NSArray <NSString *> *)validFilesFilterSendingFiles:(NSArray <NSString *> *)filePaths sessionId:(NSString *)sessionId toastInView:(NSView *_Nullable)view;


/// Check if the filepath is valid with error toast, and copy valid image to zoom's Data dir is necessary.
/// - Attention: Sending image should regard as the Highest Priority, and should NOT be blocked by copyValidImageToDataDir, that's to say, if we failed to copy image to zoom's Data Dir, we use the local image path to send the iamge as well.
/// - Parameters:
///   - filePaths: raw filePath
///   - sessionID: can be nil in new-chat
///   - copyValidImageToDataDir: If Yes, image will copy to Data Directory, or the local path will be used.
+ (NSArray <NSString *> *)expectedFilePathsWithValidCheckFromOriginalPaths:(NSArray <NSString *> *)filePaths
                                                                 sessionID:(NSString *_Nullable)sessionID
                                                   copyValidImageToDataDir:(BOOL)copyValidImageToDataDir
                                                            errorToastView:(NSView *_Nullable)toastView;

+ (ZMFileCheckResult)checkValidFileAtPath:(NSString *)path sessionID:(nullable NSString*)sessionID isSkipExtensionCheck:(BOOL)isSkipExtensionCheck;
+ (ZMFileCheckResult)checkValidFileSizeInSession:(NSString *)sessionID fileSize:(unsigned long long)fileSize;
+ (ZMFileCheckResult)checkFileTypeAndSizeWithFileItem:(ZMFileEntity *)fileItem sessionID:(NSString *_Nullable)sessionID;
+ (ZMFileCheckResult)checkFileTypeAndViewSizeWithFileItem:(ZMFileEntity *)fileItem sessionID:(NSString *_Nullable)sessionID;
#ifdef __cplusplus
+ (BOOL)checkValidFileExtInSession:(NSString *)sessionID
                              file:(ns_zoom_messager::IZoomFileInterface *)zoomFile;
+ (BOOL)checkValidFileSizeInSession:(NSString *)sessionID
                               file:(ns_zoom_messager::IZoomFileInterface *)zoomFile;
#endif

+ (ZMToast *)toastForInvalidFileWithErrorType:(ZMFileCheckResult)errorType filePath:(NSString*)filePath inSession:(NSString*_Nullable)sessionID;

+ (BOOL)checkWithToastWhenSaveAsWithFileItem:(nonnull ZMFileEntity *)fileItem fromView:(nullable NSView *)fromView;

+ (BOOL)checkWithToastIfValidFileAtPath:(NSString *)path sessionID:(NSString *)sessionID fromView:(nullable NSView *)fromView isSkipExtensionCheck:(BOOL)isSkipExtensionCheck;

+ (BOOL)checkIfNeedsShowingTipForSuspiciousLink:(NSString*)suspiciousLink
                                    displayLink:(NSString *_Nullable *_Nullable)displayLink
                                    textStorage:(NSTextStorage*)textStorage
                                      charIndex:(NSInteger)charIndex
                                      sessionId:(NSString *)sessionId;

- (void)showTipForSuspiciousLink:(NSString *)suspiciousLink
                     displayLink:(NSString *)displayLink
                           allow:(void(^)(BOOL result))allowed
                       sessionId:(NSString *)sessionId;

- (void)closeSuspiciousLinkAlert:(NSString *)sessionId;

+ (BOOL)needToShowAtAllWarningForMessageBody:(NSAttributedString*)message inSession:(NSString*)session;
- (void)showAtAllWarningInWindow:(NSWindow*)window forSession:(NSString*)session onConfirm:(void (^)(void))userConfirmedSend;

+ (BOOL)needShowPotentialRiskDetectedAlertForFileEntity:(ZMFileEntity *)fileEntity;
- (void)showPotentialRiskDetectedAlertForFileEntity:(ZMFileEntity *)fileEntity
                                           allowed:(void(^)(BOOL result))allowed;
- (void)closePotentialRiskDetectedAlertForSession:(NSString *)sessionId;

+ (BOOL)canSendMessage:(nullable ZMMessageAdapter*)messageAdapter withSessionId:(NSString*)sessionId fromView:(NSView*)fromView;

/* NSString Util */
+ (NSString *)getPathExtensionWithDotFromFile:(NSString *)fileName;

+ (NSString *)convertLongStringByTruncatingTai:(NSString *)aString toMaxLength:(NSUInteger)maxLength;

+ (NSUInteger)getTotalLengthFromInsertLink:(NSArray<ZMMessageFontInfo*> *)fontInfoArray;

/* SuspiciousLink */
+ (BOOL)hasAuthorizedVisitForSuspiciousLink;

+ (void)setAuthorizedVisitForSuspiciousLink;

+ (void)registerSecurityAlertProxy:(id<ZMSecurityAlertProxy>)proxy forSession:(NSString *)sessionId;
+ (void)removeSecurityAlertProxyForSession:(NSString *)sessionId;

+ (void)precheckSendMessage:(NSAttributedString *)attributedString fileList:(NSArray <ZMFileEntity *> *)fileList sessionId:(NSString *)sessionId fromView:(NSView *)view completion:(void (^)(BOOL))completion;

+ (NSUInteger)maxChatComposeTextLength;
+ (NSUInteger)maxChatMessageLength;

@end

NS_ASSUME_NONNULL_END
