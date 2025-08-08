//
//  ZMMessageInputProtocol.h
//  zChatComponent
//
//  Created by Groot Ding on 2022/8/4.
//

#ifndef ZMMessageInputProtocol_h
#define ZMMessageInputProtocol_h

#import <Foundation/Foundation.h>

@protocol ZMMessageInputProtocol <NSObject>

@optional

- (void)inputBoxAddMention:(NSString *)jid sessionId:(NSString *)sessionId isInChannel:(BOOL)isInChannel;

- (void)inputBoxRemoveMention:(NSString *)jid sessionId:(NSString *)sessionId;

- (void)inputBoxMoveUpWithThreadId:(NSString *)jid sessionId:(NSString *)sessionId;

- (void)inputBoxHideMention:(NSString *)sessionId;

- (void)inputBoxFormatButtonExpandedFromVC:(NSViewController *)viewController;

- (void)inputBoxPrivatePasteDoneFromVC:(NSViewController *)viewController;

@end

#endif /* ZMMessageInputProtocol_h */
