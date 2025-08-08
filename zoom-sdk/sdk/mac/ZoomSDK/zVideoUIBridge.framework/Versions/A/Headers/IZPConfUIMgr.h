//
//  IZPConfUIMgr.h
//  VideoUI
//
//  Created by martin.yu on 9/15/22.
//  Copyright © 2022 zoom.us. All rights reserved.
//

#ifndef IZPConfUIMgr_h
#define IZPConfUIMgr_h

typedef NS_ENUM(NSUInteger, ZMMTLeaveMeetingAction) {
    ZMMTLeaveMeetingActionCancel,
    ZMMTLeaveMeetingActionLeave,
};

@class ZMToast;

typedef void (^ZMMTLeaveMeetingCompletion)(ZMMTLeaveMeetingAction action);

@protocol IZPConfUIMgr <ZMRoutableObject>
@property (nonatomic, readonly, assign) BOOL isConfReady;

- (void)startSendingShareForType:(NSInteger)type; //ZMSendingShareType

- (void)askConfLeaveWithConfirm:(BOOL)confirm completion:(ZMMTLeaveMeetingCompletion)completion;

- (void)addToast:(ZMToast *)toast;

- (__kindof NSWindow *)confWindow;

@end

#endif /* IZPConfUIMgr_h */
