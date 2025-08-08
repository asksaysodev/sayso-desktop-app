//
//  ZMSipCallOutInfo.h
//  ChatUI
//
//  Created by Ben Xiao on 2023/11/1.
//  Copyright © 2023 Zoom. All rights reserved.
//

#include "zSipCallCommon/services/pbx_data_service_api.h"

using namespace zoom_sipcall_app;

@interface ZMSipCallOutInfo : NSObject

@property (nonatomic, copy) NSString *callOutId;
@property (nonatomic, copy) NSString *number;
@property (nonatomic, copy) NSString *isoCode;
@property (nonatomic, assign) CmmPBXCallOutLabelType labelType;
@property (nonatomic, copy) NSString *label;
@property (nonatomic, assign) long long option;

@property (nonatomic, copy) NSString *countryCode;
@property (nonatomic, copy) NSString *countryString;
@property (nonatomic, assign) BOOL supportCallMe;

+ (ZMSipCallOutInfo *)callOutInfoBy:(PBXCallOutInfo)callout_info;

- (PBXCallOutInfo)getPBXCallOutInfo;

- (NSString *)getLabelValue;
- (BOOL)isNotifyPressOneOptionEnabled;
- (void)setNotifyPressOneOption:(BOOL)isEnabled;

@end


@interface ZMSipCallOutInputParam : NSObject

@property (nonatomic, strong) ZMSipCallOutInfo *callOutInfo;
@property (nonatomic, copy) NSString *verificationCode;
@property (nonatomic, assign) CmmPBXCallOutVerificationType verificationType;

- (UpdateCallOutInputParam)getCallOutInputParam;

@end


@interface ZMSipVerificationCodeParam : NSObject

@property (nonatomic, copy) NSString *isoCode;
@property (nonatomic, copy) NSString *phoneNumber;
@property (nonatomic, assign) CmmPBXCallOutVerificationType verificationType;

- (SendVerificationCodeInputParam)getVerificationCodeParam;
- (BOOL)isValid;

@end
