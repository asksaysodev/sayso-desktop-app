//
//  ZMUnifyWebViewPTProtocol.h
//  zUnifyWebView
//
//  Created by Ethan Wu on 2025/1/4.
//

typedef void (^ZMCefDownloadCompletion)(BOOL);
typedef void (^ZMCefDownloadProgress)(NSNumber *);

//MARK: - chat ui api, call for unifyWebview
@protocol ZMUnifyWebViewPTProtocol <NSObject>

@optional
- (void)startCefFrameworkDownload:(BOOL)silentDownload progress:(void (^)(float))progress completion:(void (^)(BOOL, NSString *))completion;
- (void)startCefWebViewDownload:(BOOL)silentDownload progress:(void (^)(float))progress completion:(void (^)(BOOL, NSString *))completion;
- (void)cancelCefFrameworkDownload;
- (void)cancelCefWebViewDownload;
- (NSString*)getLanguageForCookie;

@end

//MARK: - unifywebview api, call for pt
@protocol ZMUnifyWebViewHelperProtocol <NSObject>

@optional
- (BOOL)isCefPluginReady;
- (NSString *)getDownloadedCefFrameworkVersion;
- (NSString *)getDownloadedCefWebViewVersion;
- (NSString *)getCefFrameworkDirWithVersion:(NSString *)version;
- (NSString *)getCefWebViewDirWithVersion:(NSString *)version;
- (BOOL)isNewCefPluginVersionValid:(NSString *)newVersion currentVersion:(NSString *)currentVersion;

@end
