//
//  ZMAICAppViewController.h
//  ZoomAIC
//
//  Created by Howie Xu on 1/14/24.
//

#import <ZoomKit/ZoomKit.h>

NS_ASSUME_NONNULL_BEGIN

typedef void (^ZMAICAppIdBlock)(void (^callback)(NSString *appId));

@interface ZMAICAppViewController : ZMBaseViewController

@property (nonatomic, copy) ZMAICAppIdBlock requestAppIdBlock;

@property (nonatomic, copy) NSString *zappId;
@property (nonatomic, copy) NSString *webViewInstanceID;
@property (nonatomic, assign) BOOL isLoaded;


- (void)loadApp:(NSString *)appId appType:(NSInteger)appType;
- (void)makeFirstResponderIfPossible;
#ifdef ENABLE_ZAPP
- (BOOL)checkIszAppWebViewReady;
- (void)cleanUp;
#endif

@end

NS_ASSUME_NONNULL_END
