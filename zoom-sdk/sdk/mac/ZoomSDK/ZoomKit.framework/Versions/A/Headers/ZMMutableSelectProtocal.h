//
//  ZMMutableSelectProtocal.h
//  ZCommonUI
//
//  Created by groot.ding on 29/04/2020.
//  Copyright © 2020 zoom. All rights reserved.
//

#ifndef ZMMutableSelectProtocal_h
#define ZMMutableSelectProtocal_h

@protocol ZMMutableSelectProtocal <NSObject>

@optional

- (void)selectFromPoint:(NSPoint)fromPoint toPoint:(NSPoint)toPoint;

- (void)onClickAtPoint:(NSPoint)point;

- (void)onMouseUpAtPoint:(NSPoint)point;

- (void)selectContent;

- (BOOL)selectHybridContent:(NSPoint)point needsSelectText:(BOOL *)needsSelectText;

- (void)unselect;

- (BOOL)shouldSelectedWhenShowMenuMouseDown:(NSPoint)point;

- (void)copyContent;

@end

@protocol ZMCustomReuseViewProtocal <NSObject>

@optional

- (void)viewWillAddToReuseCache;

@end

#endif /* ZMMutableSelectProtocal_h */
