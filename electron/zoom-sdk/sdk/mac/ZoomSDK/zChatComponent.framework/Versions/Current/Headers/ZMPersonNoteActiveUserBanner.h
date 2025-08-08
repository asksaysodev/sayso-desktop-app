//
//  ZMPersonNoteActiveUserBanner.h
//  ChatUI
//
//  Created by simon shang on 2021/8/17.
//  Copyright © 2021 Zoom. All rights reserved.
//

#import <ZoomKit/ZoomKit.h>

@class ZMBuddyAdapter;

NS_ASSUME_NONNULL_BEGIN

typedef void(^CloseButtonClicked)(void);

@interface ZMPersonNoteActiveUserBanner : ZMBaseView

@property (nonatomic, readonly) NSArray *allBuddies;

@property (nonatomic, copy) CloseButtonClicked closeButtonClicked;

- (void)addPersonNoteInfoWithBuddy:(ZMBuddyAdapter *)buddy;

- (void)removePersonNoteInfoWithBuddy:(ZMBuddyAdapter *)buddy;

- (void)clearPersonNotesData;

@end

NS_ASSUME_NONNULL_END
