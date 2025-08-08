//
//  ZMPSAvatarStatusProtocol.h
//  ZoomSetting
//
//  Created by Michael Lin on 2024/1/15.
//

#ifndef ZMPSAvatarStatusProtocol_h
#define ZMPSAvatarStatusProtocol_h

#import <ZoomSetting/ZMPSAvatarItem.h>

@protocol ZMPSAvatarStatusProtocol <NSObject>

@optional
- (void)psOnAvatarsConfigChanged;
- (void)psOnAvatarsItemsChanged;
- (void)psOnAvatarDownloaded:(ZMPSAvatarItem *)avatarItem success:(BOOL)success;
- (void)psOnAvatarUpdated:(ZMPSAvatarItem *)avatarItem;
- (void)psNotify3DAvatarEnableChanged;

@end

#endif /* ZMPSAvatarStatusProtocol_h */
