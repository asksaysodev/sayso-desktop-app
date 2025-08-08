//
//  ZMCBaseComponent.h
//  zChatComponent
//
//  Created by Huxley Yang on 11/12/24.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMCMessenger, ZMCBaseComponent;

@protocol ZMCCallbackHost <NSObject>

- (void)addCallbackTarget:(id)target;
- (void)removeCallbackTarget:(id)target;
@property (readonly, nonatomic) NSArray *allTargets;

@end

@interface ZMCBaseComponentImp : NSObject

@property (weak, readonly, nonatomic, nullable) __kindof ZMCBaseComponent *component;

- (instancetype)initWithComponent:(ZMCBaseComponent *)component;

@end

@interface ZMCBaseComponent : NSObject <ZMCCallbackHost>

- (instancetype)initWithMessenger:(ZMCMessenger *)messenger;

@property (weak, readonly, nonatomic) ZMCMessenger *messenger;

@property (readonly, nonatomic) BOOL enabled;

@property (strong, nonatomic, readonly) NSArray *imps;

- (NSArray <ZMCBaseComponentImp *>*)loadImps;//!load imps if needed, return imps which need forwardInvocation

@end

NS_ASSUME_NONNULL_END
