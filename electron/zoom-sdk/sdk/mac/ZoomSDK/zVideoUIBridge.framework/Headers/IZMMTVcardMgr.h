//
//  IZMMTVcardMgr.h
//  VideoUIBridge
//
//  Created by davies zhao on 10/24/23.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN
@class ZMWKWebUserProfileViewController;
@class ZMBubbleObject;

@protocol IZMMTVcardMgr <ZMRoutableObject>
- (void)showVCard:(ZMUser*)user;
- (void)closeVCard;
- (BOOL)enableShowVcard:(ZMUser*)user;
- (BOOL)isUserLeft:(ZMUser *)user;

@property (nonatomic, retain) ZMWKWebUserProfileViewController *webProfileVC;
@property (nonatomic, retain) ZMBubbleObject *previousShowedBubble;

@end

NS_ASSUME_NONNULL_END
