//
//  ZMUserProfileMgr.h
//  SaasBeePTUIModule
//
//  Created by likevin on 11/22/16.
//  Copyright © 2016 Zipow. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ZMUserProfileInfo : NSObject
{
    NSString*   _peerJid;
    NSString*   _name;
    NSString*   _phone;
    NSString*   _countryCode;
    NSString*   _email;
    NSString*   _department;
    NSString*   _jobTitle;
    NSString*   _location;
    NSString*   _mobile;
    NSString*   _pmn;		// personal meeting number
    NSString*   _purl;		// personal url;
    NSString*   _managerName;
    NSString*   _managerjID;
    NSString *_pronoun;
    BOOL _hasAdditionalPhoneNumbers;
    NSArray *_additionalPhoneNumbers;
}
@property(nonatomic, readwrite, copy) NSString* peerJid;
@property(nonatomic, readwrite, copy) NSString* name;
@property(nonatomic, readwrite, copy) NSString* phone;
@property(nonatomic, readwrite, copy) NSString* countryCode;
@property(nonatomic, readwrite, copy) NSString* email;
@property(nonatomic, readwrite, copy) NSString* department;
@property(nonatomic, readwrite, copy) NSString* jobTitle;
@property(nonatomic, readwrite, copy) NSString* location;
@property(nonatomic, readwrite, copy) NSString* mobile;
@property(nonatomic, readwrite, copy) NSString* pmn;
@property(nonatomic, readwrite, copy) NSString* purl;
@property(nonatomic, readwrite, copy) NSString* managerName;
@property(nonatomic, readwrite, copy) NSString* managerjID;
@property(nonatomic, readwrite, copy) NSString *pronoun;
@property(nonatomic, readwrite) BOOL hasAdditionalPhoneNumbers;
@property(nonatomic, readwrite, copy) NSArray *additionalPhoneNumbers;
- (void)cleanUp;
- (BOOL)isCompareTo:(ZMUserProfileInfo*)object;
@end

