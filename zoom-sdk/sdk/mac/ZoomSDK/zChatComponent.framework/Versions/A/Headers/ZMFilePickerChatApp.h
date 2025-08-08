//
//  ZMFilePickerChatApp.h
//  zChatComponent
//
//  Created by Jax Wu on 2024/7/24.
//

#import <Foundation/Foundation.h>
#import <ZoomUnit/ZPZAppAdapter.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMFilePickerChatApp;

@protocol ZMFilePickerChatAppProtocol <NSObject>
- (void)backToLegacyDocMessage:(ZMFilePickerChatApp *)app;
@end

@interface ZMFilePickerChatApp : NSObject

@property(nonatomic, weak) id <ZMFilePickerChatAppProtocol> delegate;
@property (nonatomic, readonly) ZMFilePickerItemType itemType;
@property (nonatomic, readonly) NSString *threadID;
@property (nonatomic, readonly) NSString *sessionID;

+ (BOOL)isFilePickerChatAppEnable:(ZMFilePickerScene)scene itemType:(ZMFilePickerItemType)itemType isGroup:(BOOL)isGroup;

- (instancetype)initWith:(ZMFilePickerItemType)type scene:(ZMFilePickerScene)scene threadID:(NSString *)threadID sessionID:(NSString *)sessionID isGroup:(BOOL)isGroup;
- (void)updateConfNodeID:(long)userNodeID;
- (void)configE2EChat:(BOOL)isE2EChat;
- (void)configSupportedFileType:(ZMSupportedFileTypeOption)supportedFileType;
- (void)showApp;
- (void)closeApp;
- (void)setPosition:(NSPoint)position forCorner:(NSRectCorner)corner animate:(BOOL)animate;

@end

NS_ASSUME_NONNULL_END
