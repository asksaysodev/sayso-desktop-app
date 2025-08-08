//
//  ZMPSAnnotationStatusProtocol.h
//  ZoomSetting
//
//  Created by Michael Lin on 2024/2/21.
//

#ifndef ZMPSAnnotationStatusProtocol_h
#define ZMPSAnnotationStatusProtocol_h

@class ZMPSAvatarItem;
@protocol ZMPSAnnotationStatusProtocol <NSObject>

- (void)onShareAnnotationStatusChanged:(SSB_MC_ANNOTATION_NOTIFICATION)msg;

@end

#endif /* ZMPSAnnotationStatusProtocol_h */
