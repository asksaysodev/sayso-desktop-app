//
//  ZMPreviewContentView.h
//  MessageListDemo
//
//  Created by groot.ding on 2/6/20.
//  Copyright © 2020 groot.ding. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "ZMPreviewImageItemView.h"

NS_ASSUME_NONNULL_BEGIN

@class ZMFilePreViewItem;

@protocol ZMFilePreviewContentDelegate <NSObject>

- (void)didSelectedWithIndex:(NSInteger)index;
- (void)mouseUpOutsideContent;
- (void)reloadFileWithItem:(ZMFilePreViewItem *)item;
- (void)previewContentView:(NSView *)previewContentView rightMouseDown:(NSEvent *)event;
@end

@interface ZMPreviewContentView : NSView

@property (nonatomic,readonly) NSInteger currentIndex;

@property (nonatomic,readonly) ZMFilePreViewItem * currentSelectedItem;

@property (nonatomic,weak) id <ZMFilePreviewContentDelegate> delegate;

@property (nonatomic,retain) ZMPreviewBaseItemView *preContentView;

-(void)selectedIndex:(NSInteger)index;

- (NSArray <ZMFilePreViewItem *> *)currentItems;

-(void)showItems:(NSArray <ZMFilePreViewItem *> *)items;

-(void)showNext;
-(void)showPre;
- (void)rotateLeft;
- (void)rotateRight;

-(void)dismiss;

- (void)insertItems:(NSArray *)items index:(NSInteger)index;
- (void)deleteItem:(ZMFilePreViewItem *)item;
- (void)updateItem:(ZMFilePreViewItem *)item;
- (void)updateContentViewForAnnotationMode:(BOOL)isAnnotationMode;

- (void)forceRefresh;
@end

NS_ASSUME_NONNULL_END
