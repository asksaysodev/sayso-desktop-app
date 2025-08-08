//
//  ZMRoomCalloutDevice.h
//  SaasBeePTUIModule
//
//  Created by John on 11/27/14.
//  Copyright (c) 2014 Zipow. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ZMRoomCalloutDevice : NSObject
{
    NSString*   _name;
    NSString*   _ip;
    NSString*   _e164num;
    int         _type;
    int         _encrypt;
}
@property(nonatomic, retain) NSString* name;
@property(nonatomic, retain) NSString* ip;
@property(nonatomic, retain) NSString* e164num;
@property(nonatomic, assign) int type;
@property(nonatomic, assign) int encrypt;
@end
