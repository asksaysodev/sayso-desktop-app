//
//  ZMAICChatViewController.h
//  ZoomAIC
//
//  Created by Jun Pang on 9/9/23.
//

NS_ASSUME_NONNULL_BEGIN

@class ZMAICChatState;
@class ZMAICChatStateEvent;
@protocol ZMAICChatDelegate;
@interface ZMAICChatViewController : ZMBaseViewController
@property(nonatomic, weak) id<ZMAICChatDelegate> delegate;

-(instancetype)initWithScenario:(ZMAICScenario)scenario;
-(ZMAICChatState*)curState;
-(void)postChatStateEvent:(ZMAICChatStateEvent*)event;
-(void)relayoutAIC;
-(void)cleanUp;
@end

NS_ASSUME_NONNULL_END
