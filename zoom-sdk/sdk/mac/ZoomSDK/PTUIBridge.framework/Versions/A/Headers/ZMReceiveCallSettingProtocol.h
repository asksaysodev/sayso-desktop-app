//
//  ZMReceiveCallSettingProtocol.h
//  ChatUI
//
//  Created by Evelyn Wu on 4/13/23.
//  Copyright © 2023 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef void (^ZMOptOutAction)(void);

@protocol ZMReceiveCallSettingProtocol <NSObject>
@optional
- (int)changeGlobalToggleStatus:(BOOL)turnOn configType:(int)type;
- (BOOL)updateSingleToggleStatus:(NSMutableDictionary*)dic configType:(int)type;
- (void)expandConfigList:(BOOL)bExpand configType:(int)type;
- (void)changeExpandStatus:(BOOL)bExpand configType:(int)type;

@end

@protocol ZMReceiveCallSettingAPI <ZMRoutableObject>

- (BOOL)enableSettingReceiveCall;
- (NSString*)getReceiveCallStatus;
- (NSMutableArray <ZMHMenuItem *> *)getSubitems;
- (BOOL)checkPromptBeforeOptOutCallQueueWhenLogout:(ZMOptOutAction)blockAction;

@end

NS_ASSUME_NONNULL_END
