//
//  NSString+Chat.h
//  zChatComponent
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSString (Chat)
- (NSArray <NSString *> *)splitString;
- (NSArray<NSString*>* _Nullable)tryParsingAsGoogleCalendarEmails;
- (NSString *)trimLeadingWhitespaceAndNewlines;
- (BOOL)checkValidEmail;
@end

NS_ASSUME_NONNULL_END
