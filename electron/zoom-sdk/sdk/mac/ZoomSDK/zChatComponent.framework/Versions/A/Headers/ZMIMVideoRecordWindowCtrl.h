//
//  ZMIMVideoRecordWindowCtrl.h
//  ChatUI
//
//  Created by simon shang on 2022/1/12.
//  Copyright © 2022 Zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMIMVideoRecordWindowContentView : ZMNonLayerBaseView

- (void)willRemoveFromHostWindow;

@end

@interface ZMIMVideoRecordWindowCtrl : NSWindowController

@property (nonatomic, copy) NSString *sessionID;
@property (nonatomic, copy) NSString *threadID;

- (void)handleCancelAction;

@end

NS_ASSUME_NONNULL_END
