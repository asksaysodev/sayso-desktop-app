//
//  ZMSipCloudPBX.h
//  SaasBeePTUIModule
//
//  Created by Evelyn on 1/11/2018.
//  Copyright © 2018 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ZMSipCloudPBX : NSObject
{
    NSString*     _rcSettingsLink;
    NSString*     _mainCompanyNumber;
    NSString*     _mainCompanyNumberFormateed;
    NSString*     _extension;
    NSString*     _countryCode;
    NSString*     _countryName;
    NSString*     _areaCode;
    NSMutableArray*   _directNumbers;
    NSMutableArray*   _directNumbersFormateed;
}
@property(copy) NSString* rcSettingsLink;
@property(copy) NSString* mainCompanyNumber;
@property(copy) NSString* mainCompanyNumberFormateed;
@property(copy) NSString* extension;
@property(copy) NSString* countryCode;
@property(copy) NSString* countryName;
@property(copy) NSString* areaCode;
@property(retain) NSMutableArray* directNumbers;
@property(retain) NSMutableArray* directNumbersFormateed;

@end
