//
//  ZMChatFeatureOptionsUtil.h
//  ZoomMsgUI
//
//  Created by Yong Zhou on 6/17/22.
//

#import <Foundation/Foundation.h>

@interface ZMChatFeatureOptionsUtil : NSObject

+ (BOOL)needsShowVoiceButtonInSession:(NSString *)sessionID;

+ (BOOL)needsShowVideoButtonInSession:(NSString *)sessionID;

+ (BOOL)needsShowCodeSnippetButtonInSession:(NSString *)sessionID;

+ (BOOL)needsShowRichFormatButtonInSession:(NSString *)sessionID;

+ (BOOL)needsShowFileButtonInSession:(NSString *)sessionID;

+ (BOOL)needsShowScreenShotButtonInSession:(NSString *)sessionID;

/// Zoom Chat shows capture right button, Meeting Chat hides
+ (BOOL)needsShowCaptureRightButtonInSession:(NSString *)sessionID;

/// Zoom Chat shows all titles, Meeting Chat hides
+ (BOOL)needsHideMenuElementTitlesInSession:(NSString *)sessionID;

/// Meeting Chat hides
+ (BOOL)needsHideCustomEmojiInSession:(NSString *)sessionID;

/// Meeting Chat hides, IM reply hides.
+ (BOOL)needsShowAPPsButtonInSession:(NSString *)sessionID;

/// Op for EditReply
+ (BOOL)needsShowAPPsButtonInEditReplySession:(NSString *)sessionID;

@end
