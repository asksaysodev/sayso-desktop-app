//
//  ZMPTRouterCenter.h
//  PTUIBridge
//
//  Created by fistice on 2024/10/22.
//

#ifndef ZMPTRouterCenter_h_h
#define ZMPTRouterCenter_h_h

#import <ZoomKit/ZoomKit.h>

@protocol IZMPTRouterCenter <ZMRouterCenter>
@end

#define ZMPTRouterCenter (id<IZMPTRouterCenter>)[ZMRouterCenter.shared singletonForProtocol:@protocol(IZMPTRouterCenter)]
#define ZMPTSharedFor(P) ((id<P>)[ZMPTRouterCenter singletonForProtocol:@protocol(P)])
#define ZMPTEventPosterFor(P) ((id<P>)[ZMPTRouterCenter posterForProtocol:@protocol(P)])
#define ZMPTEventPosterV2(P,O) ((id<P>)[ZMPTRouterCenter posterForProtocol:@protocol(P) sender:nil object:O])

#endif /* ZMPTRouterCenter_h_h */
/**
 *ZMPTRoutableObject is only available after login;
 *If your object need to be initialized by PTRouterCenter, you can make your Protocol inherit from ZMPTRoutableObject
 *if your object need to be initialized when login success, set your class`needPreInitialized` return YES;
 *if your object need to be initialized when used, set your class`needPreInitialized` return NO; Or you can just do no implement this method
 *if your object is ready to be initialized, set readyForInitialized return YES, otherwise return NO; Or you can just do no implement this method`readyForInitialized`;
 *when your object is created by PTRouterCenter, `launchModule` will be called;
 *when your object will be released by PTRouterCenter, `termModule` will be called;
 */
@protocol ZMPTRoutableObject <ZMRoutableObject>
@optional
+ (BOOL)needPreInitialized;//if your code must be initialized early, return yes;
+ (BOOL)readyForInitialized;//if it's ready, return yes;
- (void)launchModule;
- (void)termModule;
@end
/**
 Example:
 I want to create an manager class to manager my business called TestDemo, this need to be Lazy loading;
 I can create the TestDemo Manager class like following:
 
 @protocol IZMTestDemoMgr <ZMPTRoutableObject>
 - (void)doTest;
 @end
 
 @interface ZMTestDemoMgr : NSObject <IZMTestDemoMgr>
 ...
 @end
 
 @routerable(ZMTestDemoMgr, IZMTestDemoMgr)
 @implementation ZMTestDemoMgr
 + (BOOL)needPreInitialized
 {
    return NO;
 }
 + (BOOL)readyForInitialized
 {
    ...
    BOOL available = ptapp->IsTestDemoAvailable();
    return available;
 }
 - (void)launchModule
 {
    // do launch
 }
 - (void)termModule
 {
    // do release
 }
 - (void)doTest
 {
    //do test
 }
 @end
 
 ...
 
 - (void)clickDoTestButton:(id)sender
 {
    [ZMPTSharedFor(IZMTestDemoMgr) doTest];
 }
 */

@interface ZMPTRouter : NSObject

+ (id<IZMPTRouterCenter>)ptRouterCenter;
+ (id)ptSharedForProtocol:(Protocol *)protocol;

@end
