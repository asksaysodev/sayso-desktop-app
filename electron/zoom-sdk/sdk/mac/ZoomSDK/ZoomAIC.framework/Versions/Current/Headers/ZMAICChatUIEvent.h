//
//  ZMAICChatUIEvent.h
//  ZoomAIC
//
//  Created by Karen Cao on 9/13/23.
//

#ifndef ZMAICChatUIEvent_h
#define ZMAICChatUIEvent_h

@protocol ZMAICQuestion;
@protocol ZMAICChatUIEvent <NSObject>

@optional
-(void)onStartViewExit:(NSInteger)selectedSetting;
-(void)onSentQuestion:(id<ZMAICQuestion>)question;
-(void)onSentFeedback:(ZMAICAIFeedbackType)feedback withId:(NSString*)itemId;

-(void)onAICChatWillRelayout;
-(void)onAICChatDidRelayout;
@end
#endif /* ZMAICChatUIEvent_h */
