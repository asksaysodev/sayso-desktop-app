//
//  ZMPersonNoteActiveTimeBanner.h
//  ChatUI
//
//  Created by simon shang on 2021/8/17.
//  Copyright © 2021 Zoom. All rights reserved.
//

#import <ZoomKit/ZoomKit.h>
#import <ZoomKit/ZMHMenu.h>

@class ZMBuddyAdapter;

NS_ASSUME_NONNULL_BEGIN

typedef void(^ChangeButtonClicked)(void);
typedef void(^EndTimeUpCallBack)(void);

@interface ZMPersonNoteActiveTimeBanner : ZMBaseView
- (instancetype)initWithBuddy:(ZMBuddyAdapter* _Nonnull)buddy;
@property (nonatomic, copy) ChangeButtonClicked changeButtonClicked;
@property (nonatomic, copy) EndTimeUpCallBack endTimeUpCallBack;

@end

NS_ASSUME_NONNULL_END
