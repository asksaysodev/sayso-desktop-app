//
//  IZMCCIHelper.h
//  PTUIBridge
//
//  Created by fistice on 2024/10/25.
//

#ifndef IZMCCIHelper_h
#define IZMCCIHelper_h

@protocol IZMCCIHelper <NSObject>

- (BOOL)IsInCCICall;
- (BOOL)isInCCINativeVideo;
- (void)leaveNativeVideoWhenPTQuit;

@end

#define ZMCCIHelper ZMSharedFor(IZMCCIHelper)

#endif /* IZMCCIHelper_h */
