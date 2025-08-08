//
//  ZMTranslationManagerProtocol.h
//  zChatComponent
//
//

#import <Foundation/Foundation.h>

#ifndef ZMTranslationManagerProtocol_h
#define ZMTranslationManagerProtocol_h

NS_ASSUME_NONNULL_BEGIN

/// ISO 639-1 language codes for translation supported languages
typedef NSString* ZMLanguageCode NS_TYPED_ENUM;
extern ZMLanguageCode const ZMLanguageCodeEnglish;
extern ZMLanguageCode const ZMLanguageCodeFrench;
extern ZMLanguageCode const ZMLanguageCodeGerman;
extern ZMLanguageCode const ZMLanguageCodeSpanish;
extern ZMLanguageCode const ZMLanguageCodeItalian;
extern ZMLanguageCode const ZMLanguageCodeChineseSimplified;
extern ZMLanguageCode const ZMLanguageCodeChineseTraditional;
extern ZMLanguageCode const ZMLanguageCodeRussian;
extern ZMLanguageCode const ZMLanguageCodeJapanese;
extern ZMLanguageCode const ZMLanguageCodeKorean;
extern ZMLanguageCode const ZMLanguageCodeVietnamese;
extern ZMLanguageCode const ZMLanguageCodeDutch;
extern ZMLanguageCode const ZMLanguageCodePortugese;
extern ZMLanguageCode const ZMLanguageCodeUkrainian;
extern ZMLanguageCode const ZMLanguageCodeCzech;
extern ZMLanguageCode const ZMLanguageCodeEstonian;
extern ZMLanguageCode const ZMLanguageCodeFinnish;
extern ZMLanguageCode const ZMLanguageCodeHungarian;
extern ZMLanguageCode const ZMLanguageCodePolish;
extern ZMLanguageCode const ZMLanguageCodeTurkish;
extern ZMLanguageCode const ZMLanguageCodeHindi;
extern ZMLanguageCode const ZMLanguageCodeRomanian;

typedef void (^ZMDidRequestMessageTranslationBlock)(NSString* requestID, NSString* messageID, NSString* sessionID);
typedef void (^ZMDidTranslateMessageBlock)(BOOL success, NSString* requestID,
                                           NSString* messageID,NSString* sessionID,
                                           ZMLanguageCode sourceLanguage, NSString* translation,
                                           BOOL isRecoverableFailure,
                                           NSString* _Nullable failureDescription);
typedef void (^ZMDidRemoveMessageTranslationBlock)(NSString* messageID, NSString* sessionID);

@protocol ZMTranslationManagerProtocol <NSObject>
@property (nonatomic, readonly, assign) BOOL isTranslationEnabled;
/// Returns if the SDK can attempt translation for a message.
/// @note Translation can still fail even if this returns true. For example, if the message's text is in an unsupported language.
- (BOOL)canTranslateMessage:(NSString*)msgID inSession:(NSString*)sessionID;
- (NSString* _Nullable)requestTranslationForMessage:(NSString*)msgID inSession:(NSString*)sessionID toLanguage:(ZMLanguageCode _Nullable)targetLanguage;
- (NSString* _Nullable)requestTranslationForMessage:(NSString*)msgID inSession:(NSString*)sessionID;
- (void)removeTranslationForMessage:(NSString*)msgID inSession:(NSString*)session;
/// The languages translation supports
@property (nonatomic, readonly, strong) NSArray<ZMLanguageCode>* supportedLanguages;
/// The destination language for translation. nil on error.
- (nullable ZMLanguageCode)targetLanguage:(nullable BOOL *)isMandatory;
- (BOOL)setTargetLanguage:(ZMLanguageCode)language;

- (NSString* _Nullable)localizedNameForLanguage:(ZMLanguageCode)language;
/// Returns the display string for a language in that language
/// e.g. fr -> Français, es -> Español
- (NSString* _Nullable)nativeNameForLanguage:(ZMLanguageCode)language;
- (void)addDidRequestMessageTranslationListener:(id)listener block:(ZMDidRequestMessageTranslationBlock)block;
- (void)addDidTranslateMessageListener:(id)listener block:(ZMDidTranslateMessageBlock)block;
- (void)addDidRemoveMessageTranslationListener:(id)listener block:(ZMDidRemoveMessageTranslationBlock)block;
/// Removes all registered callback-blocks for the given listener
- (void)unregisterListener:(id)listener;
@end

#ifndef ZMShareTranslationManagerProtocol
#define ZMShareTranslationManagerProtocol ((id <ZMTranslationManagerProtocol>)ZMSharedFor(ZMTranslationManagerProtocol))
#endif

NS_ASSUME_NONNULL_END

#endif /* ZMTranslationManagerProtocol_h */
