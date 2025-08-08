//
//  ZMSendMessageFailedView.h
//  ZCommonUI
//
//  Created by groot.ding on 21/07/2020.
//  Copyright © 2020 zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMSendMessageFailedView : NSView

@property(nonatomic,copy) void (^onDeleteBtnClicked)(void);
@property(nonatomic,copy) void (^onRetryBtnClicked)(void);

@property(nonatomic,copy) NSString *errorString;
@property (nonatomic, assign) BOOL deleteEnabled;

@end

NS_ASSUME_NONNULL_END
