//
//  ZMGridViewDelegate.h
//  mac-app
//
//  Created by likevin on 9/29/16.
//  Copyright © 2016 zoom.us. All rights reserved.
//

#ifndef ZMGridViewDelegate_h
#define ZMGridViewDelegate_h

#import <Foundation/Foundation.h>

@class ZMGridViewItem;
@class ZMEmojiItemData;

@protocol ZMGridViewDelegate <NSObject>

@optional
- (void)gridView:(id)gridView itemCountChanged:(NSUInteger)itemCount inSection:(NSUInteger)section;
- (void)gridView:(id)gridView selectedChanged:(NSUInteger)itemIdx inSection:(NSUInteger)section;
- (void)gridView:(id)gridView didClickItemAtIndex:(NSUInteger)index itemString:(NSString*)inString itemData:(ZMEmojiItemData*)inItemData inSection:(NSUInteger)section;
- (void)gridView:(id)gridView didHoverIndex:(NSUInteger)index itemView:(ZMGridViewItem *)itemView;
@end

#endif /* ZMGridViewDelegate_h */
