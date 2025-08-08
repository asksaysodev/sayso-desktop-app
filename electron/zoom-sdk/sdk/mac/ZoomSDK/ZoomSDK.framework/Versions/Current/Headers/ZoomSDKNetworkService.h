/**
 * @file ZoomSDKNetworkService.h
 * @brief Interfaces related to proxy and SSL certificate handling for Zoom SDK network services.
 */


#import <Foundation/Foundation.h>
NS_ASSUME_NONNULL_BEGIN

/**
 * @class ZoomSDKProxySettings
 * @brief Model class for proxy configuration.
 */
@interface ZoomSDKProxySettings : NSObject

/**
 * @brief Set the network proxy.
 */
@property(nonatomic,copy,nullable)NSString   *proxy;

/**
 * @brief Set the network autoDetect.
 */
@property(nonatomic,assign)BOOL  autoDetct;

@end


/**
 * @class ZoomSDKProxySettingHelper
 * @brief Helper interface for proxy authentication and configuration.
 */
@interface ZoomSDKProxySettingHelper : NSObject
{
    NSString*   _proxyHost;
    int         _proxyPort;
    NSString*   _proxyDescription;
}
/**
 * @brief Get proxy host.
 * @return The proxy host.
 */
- (NSString*)getProxyHost;
/**
 * @brief Get proxy port.
 * @return The proxy port.						  
 */
- (int)getProxyPort;
/**
 * @brief Get description of proxy.
 * @return The proxy description.
 */
- (NSString*)getProxyDescription;
/**
 * @brief Authentication of proxy.
 * @param userName Input username for authentication.
 * @param password Input password for authentication.
 */
- (void)proxyAuth:(NSString*)userName password:(NSString*)password;
/**
 * @brief Cancel authentication of proxy.
 */
- (void)cancel;
@end


/**
 * @class ZoomSDKSSLVerificationHelper
 * @brief Helper interface for SSL certificate verification.
 */
@interface ZoomSDKSSLVerificationHelper :NSObject
/**
 * @brief The certificate is issued to whom.
 * @return The user to whom the certificate is issued.
 */
- (NSString*)getCertIssueTo;
/**
 * @brief The certificate is issued by whom.
 * @return The user by whom the certificate is issued.									  
 */
- (NSString*)getCertIssueBy;
/**
 * @brief Get serial number of certificate.
 * @return The serial number of the certificate.
 */
- (NSString*)getCertSerialNum;
/**
 * @brief Get fingerprint of certificate.
 * @return The fingerprint of the certificate.
 */
- (NSString*)getCertFingerprint;
/**
 * @brief Trust the certificate.
 */
- (void)trust;
/**
 * @brief Cancel the certificate.
 */
- (void)cancel;

@end


/**
 * @protocol ZoomSDKNetworkServiceDelegate
 * @brief Protocol for receiving network-related events such as proxy and SSL certificate prompts.
 */
@protocol ZoomSDKNetworkServiceDelegate <NSObject>
/**
 * @brief The callback will be triggered if the proxy requests to input the username and password.
 * @param proxyHelper A ZoomSDKProxySettingHelper object containing proxy information.
 */
- (void)onProxySettingNotification:(ZoomSDKProxySettingHelper*_Nullable)proxyHelper;

/**
 * @brief The callback will be triggered when the SSL needs to be verified.
 * @param sslHelper A ZoomSDKSSLVerificationHelper object contains SSL verification information.
 *
 */
- (void)onSSLCertVerifyNotification:(ZoomSDKSSLVerificationHelper*_Nullable)sslHelper;

@end


/**
 * @class ZoomSDKNetworkService
 * @brief Network service interface for configuring proxies and handling SSL verification.
 */
@interface ZoomSDKNetworkService : NSObject
{
    id<ZoomSDKNetworkServiceDelegate>  _delegate;
}
@property(nonatomic, retain, nullable) id<ZoomSDKNetworkServiceDelegate> delegate;

/**
 * @brief Configure the proxy for Zoom SDK.
 * @param settings A struct object containing proxy information.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)ConfigureProxy:(ZoomSDKProxySettings*)settings;

@end
NS_ASSUME_NONNULL_END
