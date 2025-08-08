/*****************************************************************************
*
* Copyright (C) 2024, Zoom Video Communications, Inc
*
*****************************************************************************/
#ifndef INDEPENDENT_VIEW_MAC_H
#define INDEPENDENT_VIEW_MAC_H

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#import <zUI/view.h>
#import <zUI/independent_wrapper.h>


NS_ASSUME_NONNULL_BEGIN

ZUI_API
@interface IndependentViewHelperMac : NSObject
+ (nullable NSView*)getNSViewFromIndependentViewNode:(std::shared_ptr<zui::ViewNode>)viewNode;
+ (nullable NSView*)getNSViewFromIndependentWrapper:(std::shared_ptr<zui::IndependentWrapper>)wrapper;
+ (void)initApp;
+ (void)initIndependentView:(std::shared_ptr<zui::View>)view;
+ (void)initIndependentViewWrapper:(std::shared_ptr<zui::IndependentWrapper>)indepWrapper;
+ (std::shared_ptr<zui::ViewNode>)getIndepViewNodeFromID:(std::string)id;
@end

NS_ASSUME_NONNULL_END


#endif //INDEPENDENT_VIEW_MAC_H
