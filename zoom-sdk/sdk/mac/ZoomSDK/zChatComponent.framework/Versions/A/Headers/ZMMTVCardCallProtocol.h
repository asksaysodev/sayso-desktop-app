//
//  ZMMTVCardCallProtocol.h
//  zChatComponent
//
//  Created by Devl on 1/24/24.
//

#ifndef ZMMTVCardCallProtocol_h
#define ZMMTVCardCallProtocol_h

NS_ASSUME_NONNULL_BEGIN

@protocol ZMMTVCardCallProtocol <ZMRoutableObject>

- (NSString*)mtVCardGetProfileURL;
- (NSString*)mtVCardGetMyProfileJID;

- (void)mtVCardRefreshProfileBigPictureWithJID:(NSString*)jid;
- (void)mtVCardProfileCardWillDisappear;
- (BOOL)isClientLogined;
@end

NS_ASSUME_NONNULL_END

#endif /* ZMMTVCardCallProtocol_h */
