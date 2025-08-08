//
//  ZMChatMessageItemInfo.h
//  SaasBeePTUIModule
//
//  Created by ryan on 9/22/16.
//  Copyright © 2016 Zipow. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class ZMChatSearchMessageInfo;

@interface ZMChatMessageItemInfo : NSObject<NSCopying>
{
    ZMChatSearchMessageInfo*        _messageInfo;
    NSMutableArray*                 _contextInfo;
    BOOL                            _isShowContext;
    BOOL                            _isWaiting;
    NSString*                       _requestID;
    float                           _height;
}

@property (retain) ZMChatSearchMessageInfo* messageInfo;
@property (copy) NSString* requestID;
@property (retain) NSMutableArray* contextInfo;
@property (assign) BOOL isShowContext;
@property (assign) BOOL isWaiting;
@property (assign) float height;
@property (assign) float showContextHeight;

@end
