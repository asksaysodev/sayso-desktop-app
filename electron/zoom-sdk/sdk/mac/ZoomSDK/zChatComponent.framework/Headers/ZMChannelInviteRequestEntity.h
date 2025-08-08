//
//  ZMChannelInviteRequestEntity.h
//  zChatComponent
//
//  Created by Felipe Bastos on 6/24/22.
//

#import <Foundation/Foundation.h>

#import <dataModule/zDataConstants.h>

typedef NS_ENUM(NSUInteger, ZMChannelInviteRequestEntityStatus) {
    ZMChannelInviteRequestEntityStatus_Pending = 0,
    ZMChannelInviteRequestEntityStatus_Accepted,
    ZMChannelInviteRequestEntityStatus_Declined,
};

NS_ASSUME_NONNULL_BEGIN

@interface ZMChannelInviteRequestEntity : NSObject

@property (nonatomic, assign) ZMChannelInviteRequestEntityStatus status;
@property (nonatomic, assign) BOOL isMyRequest;
@property (nonatomic, retain) NSString *approverJID;

- (void)didAcceptRequest;
- (void)didDeclineRequest;

@end

@interface ZMChannelInviteRequestManager : NSObject

@property (nonatomic, retain) NSMutableDictionary<NSString *, ZMChannelInviteRequestEntity *> *entityCache;

+ (instancetype)sharedInstance;

+ (BOOL)isChannelInviteType:(MessageType)messageType;

- (ZMChannelInviteRequestEntity *_Nonnull)getEntityForMessage:(NSString * _Nonnull)messageID sessionID:(NSString * _Nonnull)sessionID messageType:(MessageType)type;
- (void)removeCachedEntityForMessage:(NSString * _Nonnull)messageID sessionID:(NSString * _Nonnull)sessionID;

- (void)cacheEntity:(ZMChannelInviteRequestEntity *_Nonnull)entity
       forMessageID:(NSString *_Nonnull)messageID
          sessionID:(NSString *_Nonnull)sessionID;

@end


NS_ASSUME_NONNULL_END
