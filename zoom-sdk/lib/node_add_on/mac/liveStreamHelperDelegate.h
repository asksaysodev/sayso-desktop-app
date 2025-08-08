//

#import <Foundation/Foundation.h>
#import "Header_include.h"

NS_ASSUME_NONNULL_BEGIN

@interface liveStreamHelperDelegate : NSObject <ZoomSDKLiveStreamHelperDelegate>

@property(nonatomic,retain,nullable)ZoomSDKRequestRawLiveStreamPrivilegeHandler*  rawLiveStreamPrivilegeHandler;

+(liveStreamHelperDelegate *)shared;
@end

NS_ASSUME_NONNULL_END
