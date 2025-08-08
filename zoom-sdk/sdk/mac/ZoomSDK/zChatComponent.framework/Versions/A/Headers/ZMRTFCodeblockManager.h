//
//  ZMRTFCodeblockManager.h
//  zChatComponent
//
//  Created by Yong Zhou on 2/27/25.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMCodeblockConstants.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, ZMRTFCodeblockError) {
    ZMRTFCodeblockErrorJSContextNotInitialized = -1,
    ZMRTFCodeblockErrorInvalidPayload = -2,
    ZMRTFCodeblockErrorHighlightFunctionNotFound = -3,
    ZMRTFCodeblockErrorHighlightingFailed = -4
};

typedef void(^ZMRTFCodeblockCallback)(NSAttributedString * _Nullable outputAttrStr, NSError * _Nullable error);

@interface ZMRTFCodeblockManager : NSObject

+ (instancetype)sharedInstance;

- (void)sendCodeblockToJS:(NSDictionary*)payload callback:(ZMRTFCodeblockCallback)callback;

- (void)sendAutoHighlightCodeblockToJS:(NSDictionary*)payload callback:(ZMRTFCodeblockCallback)callback;

@end

NS_ASSUME_NONNULL_END
