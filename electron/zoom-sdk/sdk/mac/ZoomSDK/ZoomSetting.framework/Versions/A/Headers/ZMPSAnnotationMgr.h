//
//  ZMPSAnnotationMgr.h
//  ZoomClips
//
//  Created by Michael Lin on 2023/1/11.
//

#import <Foundation/Foundation.h>
#import <ZoomSetting/ZMPSObject.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMPSAnnotationMgr : ZMPSObject

- (void)unregisterSink;
- (void)registerSink;
- (BOOL)isAnnoToolbarShown;
- (void)showOrHideAnnoToolbar:(BOOL)toShow;
- (void)clearAllAnnotation;

@end

NS_ASSUME_NONNULL_END
