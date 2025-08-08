//
//  ZMGridView.h
//  mac-app
//
//  Created by likevin on 9/28/16.
//  Copyright © 2016 zoom.us. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "ZMGridViewDelegate.h"
#import "ZMGridViewItem.h"
#import "ZMGridViewItemProperty.h"
@class ZMEmojiItemData;

@interface ZMCollectionView : NSCollectionView
{
    id<ZMGridViewDelegate>  _gridDelegate;
    NSTrackingArea *        _area;
    NSUInteger              _lastHoveredIndex;
    BOOL                     _isAllowSelected;
}
@property(nonatomic, readwrite, retain) id<ZMGridViewDelegate> gridDelegate;
@property(nonatomic, readwrite, assign) BOOL isAllowSelected;
- (void)reloadContent:(NSArray *)content;
- (void)scrollToEnd;
- (ZMEmojiItemData*)gridItemDataByFileId:(NSString*)inFileId;
- (NSUInteger)gridItemCount;
- (void)cleanUp;
- (void)deSelectAllItems;
- (NSArray*)selectedItems;
@end
