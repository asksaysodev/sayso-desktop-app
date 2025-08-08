//
//  ZMInputboxSyncInfo.h
//  zChatComponent
//
//  Created by Yong Zhou on 2/20/24.
//

#import <Foundation/Foundation.h>

@class ZMChatAppMessagePreviewInfo;

NS_ASSUME_NONNULL_BEGIN

@interface ZMInputBoxLinkInfoSyncedResult : NSObject

@property (nonatomic, copy) NSString *title;
@property (nonatomic, copy) NSString *link;
@property (nonatomic, copy) NSString *iconUrl;
@property (nonatomic, copy) NSString *sessionID;
@property (nonatomic, copy) NSString *requireID;
@property (nonatomic, copy) NSString *threadID;
/// Used to determine the position of the inserted compose card when pasting a link while editing a message. Leave empty for other cases.
@property (nonatomic, copy) NSString *msgID;
@property (nonatomic, strong) ZMChatAppMessagePreviewInfo *previewInfo;

@end

@interface ZMDocUrlEntity : NSObject

@property (nonatomic, copy) NSString *title;
@property (nonatomic, copy) NSString *iconPath;
@property (nonatomic, copy) NSString *iconUrl;
@property (nonatomic, copy) NSString *rawContent;
@property (nonatomic, readonly, copy) NSString *placeholder;

- (instancetype)initDocUrlEntityWithTitle:(NSString*)title
                                 iconPath:(NSString*)iconPath
                                  iconUrl:(NSString*)iconUrl
                               rawContent:(NSString*)rawContent;

@end

NS_ASSUME_NONNULL_END
