//
//  ZMQAInputTextView.h
//  ZCommonUI
//
//  Created by ryan on 14-9-28.
//  Copyright (c) 2014 zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ZMPlaceholderTextView : NSTextView
{
    NSAttributedString* _placeHolderString;
}

@property(nonatomic, assign) NSPoint placeHolderStartPoint;
@property(nonatomic, retain) NSAttributedString* placeHolderString;

@end

@interface ZMDomainsPlaceholderTextView : NSTextView
{
    NSAttributedString* _placeHolderString;
}

@property(nonatomic, retain) NSAttributedString* placeHolderString;

@end
