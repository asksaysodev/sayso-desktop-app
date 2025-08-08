//
//  ZMSipNumberFormat.h
//  zChatComponent
//
//  Created by lisa.si on 2023/4/11.
//

#import <Foundation/Foundation.h>


@interface ZMSipNumberFormat : NSObject

+ (BOOL)isMatchDigital:(NSString *)key;
+ (BOOL)isValitedNumber:(NSString*)number;
+ (BOOL)isE164Format:(NSString*)number;
+ (BOOL)isEmergencyNumber:(NSString*)number;
+ (BOOL)isActiveEmergencyNumber:(NSString*)number;
// number = AAAA,return  2222;
+ (NSString*)formatCalloutUriVanityNumber:(NSString *)number;

/*
 number must E164.
 eg. number = +12056078866  return :(205)607-8866
 eg. number = 12056078866  return :12056078866.    12056078866  is not E164.
 */
+ (NSString *)formatE164Number:(NSString *)number;


/*
 number needn't E164.
 If countryCode or areaCode is nil ,will use the countryCode and areaCode set from web.
 eg. countryCode = 1,areaCode = 209 set in web;

 eg. countryCode = @"",areaCode = @"" ,phoneNumber =  12056078866 ,return  (205) 607-8866
 eg. countryCode = @"",areaCode = @"" ,phoneNumber =  8617774000331 ,return  8617774000331
 eg. countryCode = @"",areaCode = @"" ,phoneNumber =  +8617774000331 ,return  +86 177 7400 0331
 eg. countryCode = @"86",areaCode = @"" ,phoneNumber =  17774000331 ,return  +86 177 7400 0331
 eg. countryCode = @"1",areaCode = @"205" ,phoneNumber =  8374421 ,return  (205) 837-4421
 eg. countryCode = @"",areaCode = @"" ,phoneNumber =  8374421 ,return  (209) 837-4421

 */
+ (NSString *)formatNumberWithCountryCode:(NSString *)countryCode areaCode:(NSString *)areaCode phoneNumber:(NSString *)number;


/*
 number needn't E164.use the countryCode and areaCode set from web.
 eg.In Web: countryCode = 1,areaCode = 205
    number = 12056078866  return:(205) 607-8866
    number = 6078866  return:(209) 607-8866
    number = 17774000331     return: 17774000331
 */
+ (NSString *)formatPhoneNumber:(NSString *)number;



/*
 If countryCode or areaCode is nil ,will use the countryCode and areaCode set from web.
 eg. number = 3542222,countryCode = 1,areaCode = 205  return +12053542222;
 eg. number = 3542222,countryCode = "",areaCode = ""  return +12093542222;
 */
+ (NSString *)formatPhoneNumberAsE164:(NSString*)number countryCode:(NSString*)countryCode areaCode:(NSString*)areaCode;



/*
 By default, the country code and area code configured on the web UI are used
 Is same result for the API formatPhoneNumberAsE164:countryCode:areaCode while countryCode and areaCode is nil;
 */
+ (NSString *)formatPhoneNumberAsE164:(NSString*)number;



/*
 eg.numberOne = +12056078866,numberTow = 2056078866 ignore = NO , return NO;
 eg.numberOne = +12056078866,numberTow = 2056078866 ignore = YES , return YES;
 eg.numberOne = +862056078866,numberTow = 2056078866 ignore = YES , return YES;
*/
+ (BOOL)isSameNumber:(NSString *)numberOne numberTwo:(NSString *)numberTwo ignoreCountyCode:(BOOL)ignore;


/*
 By default, the country code and area code configured on the web UI are used.
 eg.In Web: countryCode = 1,areaCode = 205

 eg. numberOne = +8617774000331, numberTwo = 17774000331 return NO;
 eg. numberOne = 77339221, numberTwo = +120577339221 return YES;
 eg. numberOne = 20577339221, numberTwo = 20977339221 return NO;
 */
+ (BOOL)isSameNumberByDefaultCountryCode:(NSString *)numberOne numberTwo:(NSString *)numberTwo;

+ (NSString *)removeSymbol:(NSString *)number;

+ (NSString *)getNationalNumber:(NSString *)number;

+ (NSString *)getNumberCountryCode:(NSString *)number;

@end

