//
//  ZMCBotInlineVideoPreviewHelper.h
//  zChatComponent
//
//  Created by Cyan.Yang on 2024/3/19.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMCBotInlineVideoPreviewAppContext : NSObject

@property (nonatomic, copy) NSString *messageID;
@property (nonatomic, copy) NSString *sessionID;
@property (nonatomic, copy) NSString *actionID;
@property (nonatomic, copy) NSString *eventID;
@property (nonatomic, assign) NSInteger index;
@property (nonatomic, strong) NSURL *titleURL;

@end

@interface ZMCBotInlineVideoPreviewHelper : NSObject

+ (void)setCurrentAppContext:(ZMCBotInlineVideoPreviewAppContext * _Nullable)appContext;
+ (ZMCBotInlineVideoPreviewAppContext *)currentAppContext;

@end

NS_ASSUME_NONNULL_END
