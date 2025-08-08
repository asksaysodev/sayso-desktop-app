//
//  ZMIMListView.h
//  ZMListViewDemo
//
//  Created by groot.ding on 2/18/23.
//

#import <Cocoa/Cocoa.h>
#import <ZoomKit/ZMListView.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMIMListView : ZMListView

@property (nonatomic,readonly) NSIndexSet *selectedRowIndexes;

@property (nonatomic,assign) BOOL mutableSelectOnlyOne;

@property (nonatomic,assign) BOOL allowMutableSelected;

@property (nonatomic,copy) void (^becomeFirstResponsederBlock)(BOOL);

- (void)deselectAll:(nullable id)sender;

- (void)selectRow:(NSInteger)row;
- (void)deselectRow:(NSInteger)row;

@end

NS_ASSUME_NONNULL_END
