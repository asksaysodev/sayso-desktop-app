//
//  ZMEmojiModel.h
//  zChatComponent
//
//  Created by Mario Rao on 4/24/23.
//

#import <Foundation/Foundation.h>
#import <ZoomUnit/ZMEmojiModelProtocal.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, CustomEmojiDownloadErrorCode) {
    CustomEmojiDownloadErrorCodeNone = 0,
    CustomEmojiDownloadErrorCodeUnknown = -1000,
    CustomEmojiDownloadErrorCodeNoFileID = -1001,
    CustomEmojiDownloadErrorCodeDuplicate = -1002,
    CustomEmojiDownloadErrorCodeNoExist = -1003,
    CustomEmojiDownloadErrorCodeDuplicateRequest = -1004,
    CustomEmojiDownloadErrorCodeImageSizeExcceed = -1005,
    CustomEmojiDownloadErrorCodeInvalid = -1006,
    CustomEmojiDownloadErrorCodeTimeOut = -1007
};

typedef NS_ENUM(NSUInteger, ZMCustomEmojiSource) {
    ZMCustomEmojiSourceNone = 0,
    ZMCustomEmojiSourceChat = 1,
    ZMCustomEmojiSourceMeeting = 2
};

@class ZMChatFileItem, ZMSelectedEmojiModel, ZMUnicodeEmojiModel;

@interface ZMCustomEmojiModel : NSObject

@property (nonatomic, strong) NSString *emojiName;
@property (nonatomic, strong) NSString *fileID;
@property (nonatomic, assign) ZMCustomEmojiSource source;

//for message custom emoji downloading
@property (nonatomic, strong) NSString *sessionID;
@property (nonatomic, strong) NSString *messageID;
@property (nonatomic, assign) NSInteger fileIndex;

///If it's been removed
@property (nonatomic, assign, readonly) BOOL isDeleted;

- (instancetype)initWithEmojiName:(NSString *)emojiName fileID:(NSString *)fileID source:(ZMCustomEmojiSource)source;
- (instancetype)initWithCustomEmojiString:(NSString *)emojiString source:(ZMCustomEmojiSource)source;
- (NSString *)downloadIfNeededWithError:(NSError **)error;
- (void)getFileInfoWithCompletion:(void (^)(BOOL isDownloading, BOOL isSDKDownloaded, NSString *filePath))completion;
- (NSString *)stringValue;
- (NSString *)emojiNameWithoutColon;
- (NSString *)SDKFilePath;
- (NSString *)filePath;
- (BOOL)isDownloading;
- (BOOL)isDownloaded;
- (BOOL)isSDKDownloaded;
- (BOOL)isValid;

@end

@interface ZMEmojiModel : NSObject<ZMEmojiModelProtocal>

@property (nonatomic, strong, readonly) ZMUnicodeEmojiModel *unicodeEmoji;
@property (nonatomic, strong, readonly) ZMCustomEmojiModel *customEmoji;

- (instancetype)initWithEmojiName:(NSString *)emojiName unicode:(NSString *)unicode skins:(nullable NSArray<NSString *> *)skins keywords:(nullable NSArray<NSString *> *)keywords;
- (instancetype)initWithEmojiString:(NSString *)emojiString source:(ZMCustomEmojiSource)source;;
- (instancetype)initWithEmojiName:(NSString *)emojiName fileID:(NSString *)fileID source:(ZMCustomEmojiSource)source;;
- (nullable id)getUnicodeEmoji;
- (nullable NSString *)unicodeEmojiAtToneIndex:(NSInteger)index;
- (BOOL)isCustomEmoji;
- (BOOL)isValid;
- (NSString *)emojiName;
- (NSString *)emojiNameWithoutColon;
- (NSString *)downloadIfNeeded;
- (NSString *)identifierWithToneIndex:(NSInteger)idx;
- (BOOL)isEqualToModel:(ZMEmojiModel *)model;

@end

NS_ASSUME_NONNULL_END
