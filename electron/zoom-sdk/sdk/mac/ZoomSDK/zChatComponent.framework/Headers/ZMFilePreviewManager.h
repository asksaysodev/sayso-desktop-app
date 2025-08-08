//
//  ZMFilePreviewManager.h
//  MessageListDemo
//
//  Created by groot.ding on 2/6/20.
//  Copyright © 2020 groot.ding. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMFilePreViewItem.h>
#import "ZMPreviewContentView.h"

NS_ASSUME_NONNULL_BEGIN

@protocol ZMFilePreviewDelegate <NSObject>

@optional
- (void)didSelectedWithIndex:(NSInteger)index;

- (void)dismissPreview;

- (void)previewDidDisappear;

- (void)reloadFileWithItem:(ZMFilePreViewItem *)item;

- (void)previewContentView:(NSView *)previewContentView rightMouseDown:(NSEvent *)event;

@end

@interface ZMFilePreviewManager : NSObject

@property (nonatomic,readonly) ZMFilePreViewItem * currentSelectedItem;

@property (nonatomic,readonly) NSInteger currentIndex;

@property (nonatomic,readonly) NSView *view;

@property (nonatomic,retain) NSView *headView;

@property (nonatomic,weak) id <ZMFilePreviewDelegate> delegate;

@property (nonatomic,assign) BOOL shouldMouseUpOutSizeClosePreview;

@property (nonatomic,assign) BOOL shouldEscKeyDownClosePreview;

@property (nonatomic,assign) BOOL isAnnotationMode;

@property (nonatomic,retain) ZMPreviewContentView *previewContentView;

- (void)selectedIndex:(NSInteger)index;

- (NSArray <ZMFilePreViewItem *> *)currentItems;

- (void)showItems:(NSArray <ZMFilePreViewItem *> *)items;

- (void)dismiss;

- (void)insertItems:(NSArray *)items index:(NSInteger)index;

- (void)deleteItem:(ZMFilePreViewItem *)item;

- (void)updateItem:(ZMFilePreViewItem *)item;

- (void)rotateLeft;
- (void)rotateRight;

- (void)forceRefresh;
@end

NS_ASSUME_NONNULL_END
