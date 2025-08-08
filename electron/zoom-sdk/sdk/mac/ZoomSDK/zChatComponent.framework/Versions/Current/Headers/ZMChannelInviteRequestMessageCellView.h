//
//  ZMChannelInviteRequestMessageCellView.h
//  zChatComponent
//
//  Created by Felipe Bastos on 6/22/22.
//

#import <Foundation/Foundation.h>

#import <zChatComponent/ZMIMAvatarView.h>
#import <zChatComponent/ZMBubbleMessageCellView.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMChannelInviteRequestBubbleView : ZMBaseView

@end

@interface ZMChannelInviteRequestMessageCellView : ZMBubbleMessageCellView

@property (nonatomic, strong, nullable) ZMButton *acceptButton;
@property (nonatomic, strong, nullable) ZMButton *cancelButton;
@property (nonatomic, strong, nullable) NSProgressIndicator *progressIndicator;
@property (assign) BOOL isActionPending;

@end

NS_ASSUME_NONNULL_END
