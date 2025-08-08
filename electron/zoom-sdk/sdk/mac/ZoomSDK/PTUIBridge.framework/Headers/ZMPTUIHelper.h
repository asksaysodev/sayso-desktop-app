//
//  ZMPTUIHelper.h
//  PTUIBridge
//
//  Created by fistice on 2024/10/25.
//

#ifndef ZMPTUIHelper_h
#define ZMPTUIHelper_h

@protocol IZMPTUIHelper <NSObject>

- (void)initCurrentLocale;
- (void)setLanguageID:(NSString*)lanID;
- (NSString*)getZoomDisplayingVersionNumber;
- (NSString*)getZoomDisplayingVersionNumberWithTime;
- (NSString*)getOpenSourceURLByLangId;
- (void)showAboutView;

- (BOOL)isLoginFacebook;
- (BOOL)canJoinByIDNow;
- (BOOL)joinFromIconTray:(int)reason;
- (void)setJoinMeetingScenarioLocation;
- (void)setStartMeetingScenarioLocation;
- (BOOL)isInMeeting;
- (BOOL)canAccessZoomWebservice;
- (BOOL)isFeedbackOff;
- (BOOL)isGovUser;
- (void)onZoomCommunity;
- (BOOL)isNoMeetingLicenseUser;
- (BOOL)canAccessZoomWebService;
- (NSInteger)getAICFTStatus;
- (BOOL)getAICFTIsProUser;

@end

#define ZMPTUIHelper ZMSharedFor(IZMPTUIHelper)

#endif /* ZMPTUIHelper_h */
