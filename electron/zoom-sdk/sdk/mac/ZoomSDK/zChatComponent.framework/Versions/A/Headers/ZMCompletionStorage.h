//
//  ZMCompletionStorage.h
//  zChatComponent
//
//  Created by Zoro.Fu on 2023/9/6.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMCompletionStorage : NSObject

@property (nonatomic, assign) BOOL isCompletionEnabled;
@property (nonatomic, assign) BOOL isShowing;
@property (nonatomic, assign) BOOL isHover;
@property (nonatomic, readonly) NSString *completionString;
@property (nonatomic, copy) NSDictionary *completionAttributes;
@property (nonatomic, copy) NSString *suggestionString;

@property (nonatomic, readonly) NSAttributedString *templateAttrString;

@property (nonatomic, copy) NSAttributedString *attributedString;

@property (nonatomic, copy) void(^OnCompletionHover)(NSRect completionRect);

- (NSString *)inputString;

@end

NS_ASSUME_NONNULL_END
