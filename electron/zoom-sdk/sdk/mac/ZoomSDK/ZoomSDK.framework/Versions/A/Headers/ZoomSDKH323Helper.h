/**
 * @file ZoomSDKH323Helper.h
 * @brief Interfaces for managing H.323 devices and integration in a Zoom meeting.
 */

#import <Foundation/Foundation.h>
#import <ZoomSDK/ZoomSDKErrors.h>

NS_ASSUME_NONNULL_BEGIN


/**
 * @class H323DeviceInfo
 * @brief Represents H.323 device information used in Zoom meetings.
 */
@interface H323DeviceInfo : NSObject
{
    NSString* _name;
    NSString* _ip;
    NSString* _e164num;
    H323DeviceType _type;
    EncryptType  _encryptType;
}
/**
 * @brief Get or set the name of the H.323 device.
 */
@property(nonatomic, retain, nullable)NSString* name;
/**
 * @brief Get or set the IP address of the H.323 device.
 */
@property(nonatomic, retain, nullable)NSString* ip;
/**
 * @brief Get or set the E.164 number of the H.323 device.
 */
@property(nonatomic, retain, nullable)NSString* e164num;
/**
 * @brief Get or set the type of the H.323 device.
 */
@property(nonatomic, assign)H323DeviceType type;
/**
 * @brief Get or set the encryption type used by the H.323 device.
 */
@property(nonatomic, assign)EncryptType encryptType;
@end


/**
 * @protocol ZoomSDKH323HelperDelegate
 * @brief Protocol for receiving H.323 related event callbacks.
 */
@protocol ZoomSDKH323HelperDelegate <NSObject>
@optional
/**
 * @brief Receive outgoing call status of H.323 device.
 * @param calloutStatus Notify user if the outgoing call status changes.
 */
- (void) onCalloutStatusReceived:(H323CalloutStatus)calloutStatus;

/**
 * @brief Callback event of H.323 device pairing meeting.
 * @param pairResult Notify user if the paring status changes.
 * @param meetingNum The meeting number of the meeting.
 */
- (void) onPairCodeResult:(H323PairingResult)pairResult MeetingNumber:(long long)meetingNum;
@end

/**
 * @class ZoomSDKH323Helper
 * @brief Provides helper APIs for H.323 device management in Zoom meetings.
 */
@interface ZoomSDKH323Helper : NSObject
{
    id<ZoomSDKH323HelperDelegate> _delegate;
}
/**
 * @brief Set or get the delegate to receive H.323 related events.
 */
@property (assign, nonatomic, nullable) id<ZoomSDKH323HelperDelegate> delegate;
/**
 * @brief Get the H.323 device address of the current meeting.
 * @return If the function succeeds, it will return the address. Otherwise failed.
 */
- (NSArray*)getH323DeviceAddress;

 /**
 * @brief Get the H.323 device password of the current meeting.
 * @return If the function succeeds, it will return the password. Otherwise failed.
 */
- (NSString*)getH323Password;

/**
 * @brief Send meeting paring code.
 * @param pairCode Pairing code of the specified meeting.
 * @param meetingNum Meeting number for pairing.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)sendMeetingPairingCode:(NSString*)pairCode meetingNum:(long long)meetingNum;

/**
 * @brief This method is used to call out a H.323 device.
 * @param deviceInfo The information of H.323 device that you want to call out.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)calloutH323Device:(H323DeviceInfo*)deviceInfo;

/**
 * @brief Get the list of H.323 devices for the current meeting.
 * @return If the function succeeds, it will return the list, otherwise not.
 */
- (NSArray*_Nullable)getRoomH323DeviceArray;

/**
 * @brief Cancel the latest operation of calling out H.323 device. 
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)cancelCallOutH323;

/**
 * @brief This method is used to invite others to meeting by default email.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)inviteToMeetingByDefaultMail;

/**
 * @brief This method is used to invite others to meeting by gmail.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)inviteToMeetingByGmail;

/**
 * @brief This method is used to invite others to meeting by Yahoo mail.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)inviteToMeetingByYahooMail;
@end
NS_ASSUME_NONNULL_END
