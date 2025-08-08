//
//  ZMMeetingAssetsSharingResult.h
//  zChatComponent
//
//  Created by Cyan.Yang on 2025/1/17.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMMeetingAssetsSharingResult : NSObject

@property (nonatomic, copy, nullable) NSString *meetingID;
@property (nonatomic, copy, nullable) NSString *sessionID;
@property (nonatomic, copy, nullable) NSString *messageID;

@property (nonatomic, assign) BOOL success;

@end

NS_ASSUME_NONNULL_END
