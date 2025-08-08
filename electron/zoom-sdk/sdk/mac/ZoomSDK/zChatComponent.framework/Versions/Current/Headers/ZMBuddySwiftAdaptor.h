//
//  ZMBuddySwiftAdaptor.h
//  zChatComponent
//
//  Created by Cyan.Yang on 2024/6/27.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMBuddyAdapter;

@interface ZMBuddySwiftAdaptor : NSObject

@property (nonatomic, readonly, strong) ZMBuddyAdapter *buddy;

@property (nonatomic, readonly) NSString *screenName;
@property (nonatomic, readonly) NSString *jID;

@property (nonatomic, readonly, getter=isMyself) BOOL myself;

+ (nullable instancetype)buddyByJId:(NSString *)jid;

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithBuddy:(ZMBuddyAdapter *)buddy NS_DESIGNATED_INITIALIZER;

@end

NS_ASSUME_NONNULL_END
