//
//  ZMCommonUtil.h
//  ZoomMsgUI
//
//  Created by Kevin Li on 2022/4/19.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMUserInfo;
@class ZMScheduleMeetingInfo;

typedef NS_ENUM(NSUInteger, ZMMUCDisplayNameTruncationStyle) {
    ZMMUCDisplayNameTruncationStyleNone,
    ZMMUCDisplayNameTruncationStylePlus,
    ZMMUCDisplayNameTruncationStyleParentheses,
};

@interface ZMCommonUtil : NSObject

+ (nullable NSString*)getImagePathForURL:(nonnull NSString*)url;

+ (BOOL)isMarkdownFontStyle:(unsigned long long)style;
+ (BOOL)isAccessoryFileStyle:(unsigned long long)style;
+ (BOOL)isAccessoryImageStyle:(unsigned long long)style;
+ (BOOL)isAccessoryGiphyStyle:(unsigned long long)style;
+ (BOOL)isAccessoryScreenShotStyle:(unsigned long long)style;
+ (BOOL)isFileStyle:(unsigned long long)style;
+ (BOOL)isSameAsZMLink:(NSString *)zmLink1 link2:(NSString *)zmLink2;

+ (nonnull NSString*)buddyPresenceString:(NSUInteger)presenceType;
+ (nonnull NSString*)buddyPresenceToolTip:(NSUInteger)presenceType;

+ (NSString *)getCurrentZoomDataDirectory;

+ (nullable NSAttributedString *)getMUCDisplayName:(NSArray <ZMUserInfo *> *)userInfos
                                    withAttributes:(NSDictionary*)stringAttributes
                                        pixelWidth:(CGFloat)pixelWidth
                                      maxNameIndex:(NSInteger)maxNameIndex
                                   truncationStyle:(ZMMUCDisplayNameTruncationStyle)truncationStyle
                                         addMyself:(BOOL)addMyself;
+ (nullable NSAttributedString *)getMUCDisplayName:(NSArray <ZMUserInfo *> *)userInfos
                                    withAttributes:(NSDictionary*)stringAttributes
                                        pixelWidth:(CGFloat)pixelWidth
                                      maxNameIndex:(NSInteger)maxNameIndex
                                   truncationStyle:(ZMMUCDisplayNameTruncationStyle)truncationStyle;
+ (nullable NSAttributedString *)getMUCDisplayName:(nonnull NSArray <ZMUserInfo *> *)userInfos
                                    withAttributes:(nullable NSDictionary*)stringAttributes
                                        pixelWidth:(CGFloat)pixelWidth
                                      maxNameIndex:(NSInteger)maxNameIndex;

+ (NSAttributedString *)getSessionAttributedTitle:(NSString *)sessionId maxWidth:(CGFloat)maxWidth;

+ (NSString*)getSha256HashString:(NSString*)inString;

+ (NSString *)getCurrentLanguage;

+ (void)postMessage:(nonnull NSString*)msgName dict:(nullable NSDictionary*)dict;

+ (NSString *)getMessageContentByMeetingInfo:(ZMScheduleMeetingInfo *)meetingInfo;

+ (BOOL)openChatWithPeerJid:(nullable NSString*)peerJid andCurrentSession:(nullable NSString*)inSession;

+ (void)updateUnreadCountWithUserInfo:(nonnull NSDictionary *)userInfo;

+ (NSString *)IBSupportLink;

+ (NSDictionary*)commonAppCommandInfoFromInfo:(NSDictionary*)dict;

+ (void)saveAsFileToLocal:(NSString *)filePath fromView:(NSView *)fromView;

+ (BOOL)isDogFoodLoginUser;

+ (NSString *)thousandFormat:(NSInteger)number;

@end

NS_ASSUME_NONNULL_END
