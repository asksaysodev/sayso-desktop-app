//
//  NSTableCellView+ZoomKit.h
//  ZCommonUI
//
//  Created by Huxley on 2018/6/20.
//  Copyright © 2018 zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface NSTableCellView (ZoomKit)

- (void)rowSelectionDidChangeTo:(BOOL)bSelected;//!< do nothing, to override

- (BOOL)isTableViewFirstResponse;

- (NSTableView *)currentTableView;

- (NSScrollView *)currentEnclosingScrollView;

- (NSTableRowView* _Nullable)enclosingRowView;
- (NSTableView* _Nullable)enclosingTableView;
- (NSOutlineView* _Nullable)enclosingOutlineView;
@end
