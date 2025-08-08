//
//  ZMCustomEmojiManager.h
//  zChatComponent
//
//  Created by Mario Rao on 2024/11/12.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMEmojiModel.h>

typedef void (^ZMCustomEmojiSearchCallback)(NSArray<ZMEmojiModel *> *searchResult, NSString *searchAfterKey);
typedef void (^ZMCustomEmojiFetchCallback)(NSArray<ZMEmojiModel *> *allEmojis, BOOL hasError);

@interface ZMCustomEmojiManager : NSObject

+ (instancetype)shared;

//MARK: - Register
- (void)registerUISinkForSourceIfNeeded:(ZMCustomEmojiSource)source;
- (void)unregisterUISinkForSource:(ZMCustomEmojiSource)source;

//MARK: - OP
- (BOOL)isCustomEmojiEnabledForSource:(ZMCustomEmojiSource)source;
- (BOOL)isChatCustomEmojiEnabledForSource:(ZMCustomEmojiSource)source;
- (BOOL)isEditCustomEmojiEnabledForSource:(ZMCustomEmojiSource)source;

- (BOOL)isCustomEmojiEnabledForSession:(NSString *)sessionID messageID:(NSString *)messageID;
- (BOOL)isEditCustomEmojiEnabledForSession:(NSString *)sessionID;

//MARK: - Util
- (BOOL)isValidCustomEmojiNameInput:(NSString *)name;
- (NSString *)emojiOwnerIDWithFileID:(NSString *)fileID source:(ZMCustomEmojiSource)source;
- (BOOL)canLoadMoreWithSource:(ZMCustomEmojiSource)source;
- (BOOL)isLoadingWithSource:(ZMCustomEmojiSource)source;
- (NSString *)myCustomEmojiJidForSource:(ZMCustomEmojiSource)source;
- (NSDictionary *)customEmojiName2ModelMapWithSource:(ZMCustomEmojiSource)source;
- (NSString *)fixedEmojiFilePathWithPath:(NSString *)path;
- (NSString *)generateCustomEmojiPathWithExtension:(NSString *)extension fileID:(NSString *)fileID source:(ZMCustomEmojiSource)source;
- (NSArray<ZMEmojiModel *> *)customEmojisWithSource:(ZMCustomEmojiSource)source;
- (BOOL)isEmojiDeletedWithFileID:(NSString *)fileID;

- (NSString *)emojiOwnerIDWithFileID:(NSString *)fileID session:(NSString *)sessionID;
- (NSString *)customEmojiNameWithFileID:(NSString *)fileID source:(ZMCustomEmojiSource)source;
- (BOOL)canLoadMoreWithSession:(NSString *)sessionID;
- (BOOL)isLoadingWithSession:(NSString *)sessionID;
- (NSString *)myCustomEmojiJidForSession:(NSString *)sessionID;
- (NSDictionary *)customEmojiName2ModelMapWithSession:(NSString *)sessionID;
- (NSArray<ZMEmojiModel *> *)customEmojisWithSession:(NSString *)sessionID;

//MARK: - Transfer
- (NSString *)fetchFirstPageForSourceIfNeeded:(ZMCustomEmojiSource)source callback:(ZMCustomEmojiFetchCallback)callback;
- (NSString *)fetchCustomEmojiForSource:(ZMCustomEmojiSource)source callback:(ZMCustomEmojiFetchCallback)callback;
- (NSString *)uploadCustomEmojiForSource:(ZMCustomEmojiSource)source path:(NSString *)path emojiName:(NSString *)emojiName;
- (NSString *)downloadCustomEmoji:(ZMCustomEmojiModel *)emoji forSource:(ZMCustomEmojiSource)source error:(NSError **)error;
- (NSString *)deleteCustomEmoji:(ZMCustomEmojiModel *)emoji forSource:(ZMCustomEmojiSource)source;

- (NSString *)fetchFirstPageIfNeededForSession:(NSString *)session callback:(ZMCustomEmojiFetchCallback)callback;
- (NSString *)fetchCustomEmojiForSession:(NSString *)sessionID callback:(ZMCustomEmojiFetchCallback)callback;
- (NSString *)uploadCustomEmojiForSession:(NSString *)sessionID path:(NSString *)path emojiName:(NSString *)emojiName;
- (NSString *)downloadCustomEmoji:(ZMCustomEmojiModel *)emoji forSession:(NSString *)sessionID error:(NSError **)error;
- (NSString *)deleteCustomEmoji:(ZMCustomEmojiModel *)emoji forSession:(NSString *)sessionID;

//MARK: - Web Search
- (NSString *)searchCustomEmojiForSource:(ZMCustomEmojiSource)source keyword:(NSString *)keyword searchAfter:(NSString *)searchAfter remoteSearchCompletion:(ZMCustomEmojiSearchCallback)remoteCompletion;
- (NSString *)searchCurrentUserCustomEmojiForSource:(ZMCustomEmojiSource)source searchAfter:(NSString *)searchAfter remoteCompletion:(ZMCustomEmojiSearchCallback)completion;

- (NSString *)searchCustomEmojiForSession:(NSString *)sessionID keyword:(NSString *)keyword searchAfter:(NSString *)searchAfter remoteSearchCompletion:(ZMCustomEmojiSearchCallback)remoteCompletion;
- (NSString *)searchCurrentUserCustomEmojiForSession:(NSString *)sessionID searchAfter:(NSString *)searchAfter remoteCompletion:(ZMCustomEmojiSearchCallback)completion;

//MARK: - Hover
- (void)showPopoverForUnicodeEmoji:(NSString *)emoji onView:(NSView *)view frame:(CGRect)rect;
- (void)showPopoverForCustomEmoji:(ZMCustomEmojiModel *)emoji onView:(NSView *)view frame:(CGRect)rect;

//MARK: - Panel
- (void)showEditCustomEmojiWindowForSource:(ZMCustomEmojiSource)source;
- (void)showAddCustomEmojiWindowForSource:(ZMCustomEmojiSource)source isFromEdit:(BOOL)isFromEdit;

- (void)showEditCustomEmojiWindowForSession:(NSString *)sessionID;
- (void)showAddCustomEmojiWindowForSession:(NSString *)sessionID isFromEdit:(BOOL)isFromEdit;
- (void)closeAddAndEditCustomEmojiWindow;
- (void)closeAddAndEditCustomEmojiWindowOnlyForSource:(ZMCustomEmojiSource)source;

@end
