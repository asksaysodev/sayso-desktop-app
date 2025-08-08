//
//  ZMChatFileItemSwiftAdaptor.h
//  zChatComponent
//
//  Created by Cyan.Yang on 2024/7/4.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMFileEntity;
@class ZMChatFileItem;

NS_SWIFT_NAME(ChatFileItem)
@interface ZMChatFileItemSwiftAdaptor : NSObject

@property (nonatomic, copy, nullable) NSString *filePath;
@property (nonatomic, copy, nullable) NSString *downloadURL;
@property (nonatomic, copy, nullable) NSString *sessionID;
@property (nonatomic, copy, nullable) NSString *owner;
@property (nonatomic, copy, nullable) NSDate *date;

@property (nonatomic, readonly, getter=isFileDownloading) BOOL fileDownloading;
@property (nonatomic, readonly, getter=isFileExisted) BOOL fileExisted;

- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithChatFileItem:(ZMChatFileItem *)item NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithFileEntity:(__kindof ZMFileEntity *)fileEntity;

- (BOOL)isFileIntegrationType;

- (ZMFileEntity *)toFileEntity;

@end

NS_ASSUME_NONNULL_END
