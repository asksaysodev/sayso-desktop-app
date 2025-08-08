/**
 * @file ZoomSDKInterpretationController.h
 * @brief Interfaces for Zoom SDK Language Interpretation management.
 *
 * This file defines the interfaces for managing interpreters and interpretation channels in Zoom meetings,
 * including language info queries, interpreter management, and real-time callbacks.
 */


#import <Foundation/Foundation.h>
#import "ZoomSDKErrors.h"

NS_ASSUME_NONNULL_BEGIN
/**
 * @class ZoomSDKInterpretationLanguageInfo
 * @brief Represent interpretation language information.
 */
@interface ZoomSDKInterpretationLanguageInfo : NSObject

/**
 * @brief Get the language ID.
 * @return The language ID.
 */
- (int)getLanguageID;

/**
 * @brief Get the language alias (abbreviation).
 * @return A string representing the language abbreviation (e.g., "EN", "CN").
 */
- (NSString*_Nullable)getLanguageAbbreviations;

/**
 * @brief Get the language name.
 * @return A string representing the full language name (e.g., "English", "Chinese").
 */
- (NSString*_Nullable)getLanguageName;
@end

/**
 * @class ZoomSDKInterpreter
 * @brief Represent an interpreter user and their supported languages.
 */
@interface ZoomSDKInterpreter : NSObject

/**
 * @brief Get the interpreter's user ID.
 * @return User ID of the interpreter.
 */
- (unsigned int)getUserID;

/**
 * @brief Get the interpreter's first supported language ID.
 * @return Language ID.
 */
- (int)getLanguageID1;

/**
 * @brief Get the interpreter's second supported language ID.
 * @return Language ID.
 */
- (int)getLanguageID2;

/**
 * @brief Determine is currently available in the meeting.
 * @return YES means the interpreter is available and had join meeting, otherwise not.
 */
- (BOOL)isAvailable;
@end

/**
 * @protocol ZoomSDKInterpretationControllerDelegate
 * @brief Callback protocol for receiving interpretation-related events.
 */
@protocol ZoomSDKInterpretationControllerDelegate <NSObject>

/**
 * @brief Notify the interpretation is started.
 */
- (void)onInterpretationStart;

/**
 * @brief Notify the interpretation is stoped.
 */
- (void)onInterpretationStop;

/**
 * @brief Notify the interpreter role is changed.
 * @param userID The user id of the interpreter role change.
 * @param interpreter YES means is interpreter,otherwise not.
 */
- (void)onInterpreterRoleChanged:(unsigned int)userID isInterpreter:(BOOL)interpreter;

/**
 * @brief Notify the interpreter role is changed.
 * @param userID The user id of the interpreter.
 * @param languageID The current active language id.
 */
- (void)onInterpreterActiveLanguageChanged:(unsigned int)userID activeLanguageID:(int)languageID;

/**
 * @brief Notify the interpreter language changed.
 * @param lanID1 The language id of the first language id.
 * @param lanID2 The language id of the second language id.
 */
- (void)onInterpreterLanguageChanged:(int)lanID1  theLanguageID2:(int)lanID2;

/**
 * @brief Notify the available language.
 * @param availableLanguageArr The array contain available language object (The language object is ZoomSDKInterpretationLanguageInfo).
 */
- (void)onAvailableLanguageListUpdated:(NSArray*_Nullable)availableLanguageArr;

/**
 * @brief Notify the interpreter list changed.
 */
- (void)onInterpreterListChanged;

/**
 * @brief Notify the list of available languages that interpreters can hear has changed. When the list of available languages that interpreters can hear in a meeting is changed. All interpreters in the meeting can get this event.
 * @param availableLanguages The list of available languages that interpreters can hear.
 */
- (void)onInterpreterLanguagesUpdated:(NSArray<ZoomSDKInterpretationLanguageInfo *>*_Nullable)availableLanguages;
@end


/**
 * @class ZoomSDKInterpretationController
 * @brief Interface for managing interpretation in a Zoom meeting.
 */
@interface ZoomSDKInterpretationController : NSObject
/**
 * @brief Delegate for receiving interpretation-related events.
 */
@property(nonatomic,assign,nullable)id<ZoomSDKInterpretationControllerDelegate>  delegate;

/**
 * @brief Determine if the interpretation function is enabled.
 * @return YES means interpretation function is enable,otherwise not.
 */
- (BOOL)isInterpretationEnabled;

/**
 * @brief Determine if the interpretation function is started.
 * @return YES means interpretation is started,otherwise not.
 */
- (BOOL)isInterpretationStarted;

/**
 * @brief Determine if self is interpreter.
 * @return YES means self is interpreter,otherwise not.
 */
- (BOOL)isInterpreter;

/**
 * @brief Get interpretation language info by language ID.
 * @param languageID The id of language.
 * @return If the function succeeds, the return value is ZoomSDKInterpretationLanguageInfo object.
 */
- (ZoomSDKInterpretationLanguageInfo*_Nullable)getInterpretationLanguageByID:(int)languageID;

/**
 * @brief Get the full list of supported interpretation languages.
 * @return If the function succeeds, the return array contain language object.(The language object is ZoomSDKInterpretationLanguageInfo)
 */
- (NSArray*_Nullable)getAllLanguageList;

/**
 * @brief Get the list of all interpreters in the meeting.
 * @return If the function succeeds, the return array contain interpreter object.(The language object is ZoomSDKInterpreter)
 */
- (NSArray*_Nullable)getAllInterpreterList;

/**
 * @brief Add an interpreter.
 * @param userID User ID.
 * @param lanID1 First language ID.
 * @param lanID2 Second language ID.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)addInterpreter:(unsigned int)userID languageID1:(int)lanID1 languageID2:(int)lanID2;

/**
 * @brief Remove an interpreter.
 * @param userID User ID.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)removeInterpreter:(unsigned int)userID;

/**
 * @brief Modify the supported languages of an interpreter.
 * @param userID User ID.
 * @param lanID1 First language ID.
 * @param lanID2 Second language ID.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)modifyInterpreter:(unsigned int)userID languageID1:(int)lanID1 languageID2:(int)lanID2;

/**
 * @brief Start interppretation.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)startInterpretation;

/**
 * @brief Stop interppretation.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)StopInterpretation;

/**
 * @brief Get the list of available interpretation languages.
 * @return Array of ZoomSDKInterpretationLanguageInfo.
 */
- (NSArray*_Nullable)getAvailableLanguageList;

/**
 * @brief Join an interpretation language channel.
 * @param languageID Language ID. -1 means join the major audio.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)joinLanguageChannel:(int)languageID;

/**
 * @brief Get the language ID currently joined.
 * @return Language ID.
 */
- (int)getJoinedLanguageID;

/**
 * @brief Turn off the major meeting audio.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)turnOffMajorAudio;

/**
 * @brief Turn on the major meeting audio.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)turnOnMajorAudio;

/**
 * @brief Determine if major audio is turn off.
 * @return YES means major audio is turn off,otherwise not.
 */
- (BOOL)isMajorAudioTurnOff;

/**
 * @brief Get interpreter support language.
 * @param lanID1 The id is interpreter first language.
 * @param lanID2 The id is interpreter second language.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)getInterpreterLans:(int*)lanID1 languageID2:(int*)lanID2;

/**
 * @brief Set interpreter active language.
 * @param activeLanID Active language ID.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)setInterpreterActiveLan:(int)activeLanID;

/**
 * @brief Get interpreter current active language id.
 * @return Active language ID.
 */
- (int)getInterpreterActiveLanID;

/**
 * @brief Get the list of available languages that interpreters can hear.
 * @return If the function succeeds, the retuan value is an array of ZoomSDKInterpretationLanguageInfo. Otherwise nil.
 */
- (NSArray <ZoomSDKInterpretationLanguageInfo *>*_Nullable)getInterpreterAvailableLanguages;

/**
 * @brief Set a language that I can hear as an interpreter.
 * @param languageID Specify the selected language that I can hear as an interpreter.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)setInterpreterListenLan:(int)languageID;

/**
 * @brief Get a language that I can hear as an interpreter.
 * @return Language ID.
 */
- (int)getInterpreterListenLanID;
@end

NS_ASSUME_NONNULL_END
