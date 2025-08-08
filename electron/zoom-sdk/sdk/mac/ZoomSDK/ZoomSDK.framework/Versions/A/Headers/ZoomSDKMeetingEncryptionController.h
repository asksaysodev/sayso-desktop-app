/**
 * @file ZoomSDKMeetingEncryptionController.h
 * @brief Zoom SDK meeting encryption controller interface.
 *
 * This file defines the @c ZoomSDKMeetingEncryptionController interface and related protocols
 * for managing meeting encryption, including E2EE (end-to-end encryption), enhanced encryption,
 * and related security codes.
 */

#import <Foundation/Foundation.h>
#import <ZoomSDK/ZoomSDKErrors.h>

NS_ASSUME_NONNULL_BEGIN
/**
 * @brief Enumeration of meeting encryption type.
 */
typedef enum
{
    /** No encryption. */
    ZoomSDKEncryptionType_None,
    /** Enhanced encryption. */
    ZoomSDKEncryptionType_Enhanced,
    /** End-to-end encryption (E2EE). */
    ZoomSDKEncryptionType_E2EE
}ZoomSDKEncryptionType;

/**
 * @protocol ZoomSDKMeetingEncryptionDelegate
 * @brief Callback events for meeting encryption status.
 */
@protocol ZoomSDKMeetingEncryptionDelegate <NSObject>
/**
 * @brief This callback will be called when the security code changes.
 */
- (void)onE2EEMeetingSecurityCodeChanged;
@end

/**
 * @class ZoomSDKMeetingEncryptionController
 * @brief Zoom SDK meeting encryption controller.
 */
@interface ZoomSDKMeetingEncryptionController : NSObject

/**
 * @brief The delegate object to receive encryption events.
 */
@property(nonatomic,assign, nullable) id<ZoomSDKMeetingEncryptionDelegate> delegate;

/**
 * @brief Get meeting encryption type.
 * @return The encryption type.
 */
- (ZoomSDKEncryptionType)getEncryptionType;

/**
 * @brief Get E2EE meeting security code.
 * @return If the function succeeds, it will return 40-digit security code. Otherwise failed.
 */
- (NSString*_Nullable)getE2EEMeetingSecurityCode;

/**
 * @brief Get security code passed seconds.
 * @return If the function succeeds, it will return the time the security code exists, in seconds. Otherwise failed.
 */
- (unsigned int)getE2EEMeetingSecurityCodePassedSeconds;

/**
 * @brief Determine whether unencrypted exception data is valid,
 * @return YES means unencrypted exception data is valid.
 * @note This method can only be called when the encryption type is ZoomSDKEncryptionType_Enhanced.
 */
- (BOOL)isUnencryptedExceptionDataValid;

/**
 * @brief Get unencrypted exception count.
 * @return If the function succeeds, it will return the unencrypted exception count. Otherwise failed.
 */
- (unsigned int)getUnencryptedExceptionCount;

/**
 * @brief Get unencrypted exception info.
 * @return If the function succeeds, it will return unencrypted exception details. Otherwise failed.
 */
- (NSString*_Nullable)getUnencryptedExceptionInfo;
@end
NS_ASSUME_NONNULL_END
