//
//  ZPResponderButtons+A.h
//  ZoomSetting
//
//  Created by Cooper.Chen on 2024/4/29.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSMutableArray (ZPResponderButtons)

/// judege is first responder
- (BOOL)isFirstResponder;

/// handle click key donw
/// @param keyCode key code
/// @param target action target
- (void)clickKeyDonw:(unsigned short)keyCode withTarget:(id)target;

@end

NS_ASSUME_NONNULL_END
