//
//  ZMMessageMentionInfo.h
//  zChatComponent
//
//  Created by Felipe Bastos on 7/2/24.
//

#import <Foundation/Foundation.h>

NS_SWIFT_NAME(MessageMentionInfo.MentionType)
typedef NS_ENUM(NSInteger, ZMMentionType) {
    ZMMentionTypeNone = 0,
    ZMMentionTypeBuddy,
    ZMMentionTypeAll,
    ZMMentionTypeChannel,
    ZMMentionTypeMentionGroup,
    ZMMentionTypeBuddyNotInChannel,
};

NS_SWIFT_NAME(MessageMentionInfo)
@interface ZMMessageMentionInfo : NSObject

@property (nonatomic,assign) ZMMentionType mentionType;
@property (nonatomic,copy) NSString* _Nonnull messageID;
@property (nonatomic,assign) NSRange range;
@property (nonatomic, copy) NSString *_Nullable mentionName;

/// Takes the current fontStyle/atMentions for a message and returns the jids for not-in-channel users
///  as well as a patched at-mention list containing both in-channel+not-in-channel at-mentions.
+ (NSArray<ZMMessageMentionInfo*>*)getMentionsOrderedAscending:(BOOL)isAscending
                                              notInChannelJids:(NSSet<NSString*>** _Nonnull)outJids
                                                 fromFontStyle:(NSArray<ZMMessageFontInfo*>* _Nonnull)fontInfoList
                                                        atList:(NSArray<ZMMessageMentionInfo*>* _Nonnull)atList
                                                 messageString:(NSAttributedString* _Nonnull)attributedString;

- (instancetype _Nonnull)initWithRange:(NSRange)range
                             messageID:(NSString *_Nonnull)messageID
                           mentionType:(ZMMentionType)mentionType;

+ (nullable NSAttributedString *)getMentionString:(nonnull NSString *)name
                               inSession:(nonnull NSString *)sessionID
                                     jid:(nonnull NSString *)jid
                             mentionType:(ZMMentionType)mentionType
                                  asLink:(BOOL)asLink;

@end
