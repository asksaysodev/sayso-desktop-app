//
//  ZMScheduleDraftViewController.h
//  zChatComponent
//
//  Created by Felipe Bastos on 4/26/23.
//  Copyright © 2023 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <zChatComponent/ZMMessageDraftHelper.h>

@protocol ZMScheduleDraftViewControllerDelegate <NSObject>

@optional
- (void)didCancelScheduleDialog:(nonnull id)sender;
- (void)didConfirmScheduleDialog:(nonnull id)sender;

- (void)didChangeTimezone:(nonnull id)sender;

@end

@interface ZMScheduleDraftViewController : ZMBaseViewController

@property (nullable, nonatomic, weak) id<ZMScheduleDraftViewControllerDelegate> delegate;
@property (nonnull, nonatomic, retain) ZMIMMessageDraft *draft;

- (instancetype)initWithDraft:(ZMIMMessageDraft *)draft;

- (void)setDraft:(nonnull ZMIMMessageDraft *)draft;

@end
