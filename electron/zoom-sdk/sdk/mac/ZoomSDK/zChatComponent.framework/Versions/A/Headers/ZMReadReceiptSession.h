//
//  ZMReadReceiptSession.h
//  zChatComponent
//
//  Created by Mario Rao on 2023/11/15.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef void (^ReadReceiptsUpdateBlock)(BOOL result, NSString *sessionID, NSString *msgID, NSInteger totalCount, NSInteger readCount);

@interface ZMReadReceiptSession : NSObject

@property (nonatomic, strong) NSString *sessionID;
@property (nonatomic, strong) NSString *messageID;
@property (nonatomic, strong) NSString *reqID;
@property (nonatomic, copy) ReadReceiptsUpdateBlock callback;

@end

@interface ZMReadReceiptMessageModel : NSObject

@property (nonatomic, strong) NSString *sessionID;
@property (nonatomic, strong) NSString *messageID;
@property (nonatomic, assign) NSInteger totalCount;
@property (nonatomic, assign) NSInteger readCount;

+ (instancetype)modelWithSessionID:(NSString *)sessionID messageID:(NSString *)messageID totalCount:(NSInteger)totalCount readCount:(NSInteger)readCount;

@end

NS_ASSUME_NONNULL_END
