//
//  IZMTerminateMgr.h
//  PTUIBridge
//
//  Created by davies zhao on 2024/10/30.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, ZMPTTerminateCheckType) {
    ZMPTTerminateCheckTypeNone = 0,
    ZMPTTerminateCheckTypeMeeting = 1,
    ZMPTTerminateCheckTypeClips = 1 << 1,
    ZMPTTerminateCheckTypeContactCenterVideo = 1 << 2,
    ZMPTTerminateCheckTypeAll = ZMPTTerminateCheckTypeMeeting|ZMPTTerminateCheckTypeClips|ZMPTTerminateCheckTypeContactCenterVideo,
};

@protocol IZMTerminateMgr <NSObject>
- (BOOL)canTerminateDirectly;
- (void)startTerminateCheck;
- (void)continueTerminateCheckWithCheckedType:(ZMPTTerminateCheckType)type;
@end

NS_ASSUME_NONNULL_END
// ZMSharedFor(IZMTerminateMgr)
