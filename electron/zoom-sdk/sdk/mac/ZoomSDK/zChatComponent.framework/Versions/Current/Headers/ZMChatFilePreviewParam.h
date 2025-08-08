//
//  ZMChatFilePreviewParam.h
//  zChatComponent
//
//  Created by Dikey.Jin on 2025/2/26.
//

#import <Foundation/Foundation.h>

@interface ZMChatFilePreviewParam : NSObject

@property (nonatomic, assign) BOOL dockout;
@property (nonatomic, copy) NSString *fileID;
@property (nonatomic, assign) BOOL useFileIndex;
@property (nonatomic, assign) NSUInteger fileIndex;
@property (nonatomic, copy) NSString *sessionId;
@property (nonatomic, copy) NSString *messageId;

- (instancetype)initWithDictionary:(NSDictionary *)dictionary;

@end
