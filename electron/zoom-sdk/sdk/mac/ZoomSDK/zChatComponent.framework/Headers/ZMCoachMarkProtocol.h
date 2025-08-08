//
//  ZMCoachMarkProtocol.h
//  zChatComponent
//
//  Created by Jed Zheng on 26/9/2023.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, ZMCoachMarkButtonClick) {
    ZMCoachMarkButtonClickNone,
    ZMCoachMarkButtonClickClose,
    ZMCoachMarkButtonClickPrimary,
    ZMCoachMarkButtonClickSecondary
};

@protocol ZMCoachMarkProtocol <ZMRoutableObject>
- (BOOL)isCoachMarkAvailableWithIdentifier:(NSString *)identifier;

- (void)markCoachMarkShowingWithIdentifier:(NSString *)identifier;

- (void)markCoachMarkShownWithIdentifier:(NSString *)identifier;

- (void)trackCoachMarkCloseWithIdentifier:(NSString *)identifier clickButtonType:(ZMCoachMarkButtonClick)clickButtonType;
@end

NS_ASSUME_NONNULL_END
