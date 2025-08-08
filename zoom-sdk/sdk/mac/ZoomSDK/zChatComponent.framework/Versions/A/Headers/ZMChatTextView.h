//
//  ZMChatTextView.h
//  IMChat
//
//  Created by John on 5/8/14.
//  Copyright (c) 2014 John. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ZMChatTextView : NSTextView
{
    NSMutableParagraphStyle*    _contentParagraphStyle;
    BOOL                        _isHistory;
    BOOL                        _isMouseDown;
    BOOL                        _isFromMe;
    BOOL                        _shouldDoMouseDown;
    NSString*                   _selectedFileMessageID;
    NSString*                   _selectedFilePath;
    NSDictionary*               _textAttribute;
}
@property (retain) NSMutableParagraphStyle* contentParagraphStyle;
@property (nonatomic, readwrite, assign) BOOL isHistory;
@property (copy) NSString* selectedFileMessageID;
@property (copy) NSString* selectedFilePath;
@property (assign) BOOL isMouseDown;
@property (assign) BOOL shouldDoMouseDown;
@property(nonatomic, retain) NSDictionary*  textAttribute;

- (BOOL)lastMessageVisible;
- (BOOL)shouldAutoScroll;
- (BOOL)isSelectSigleFile;
- (NSPoint)mouseLocationInTextContiner;
- (NSRect)rectForCharacterIndex:(NSInteger)charIndex;

@end

@interface ZMAccessibilityStaticTextField : NSTextField
{
    NSString*                   _accessibilityStringValue;
}
@property (retain) NSString* accessibilityStringValue;
@end