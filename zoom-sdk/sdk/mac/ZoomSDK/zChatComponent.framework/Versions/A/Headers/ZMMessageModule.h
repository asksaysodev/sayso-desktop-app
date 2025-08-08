//
//  ZMMessageModule.h
//  zChatUI
//
//  Created by likevin on 2019/5/14.
//  Copyright © 2019 Zoom. All rights reserved.
//

#import <zChatComponent/ZMMessageProtocol.h>
#import <ZoomUnit/ZMLoginProtocol.h>

@interface ZMMessageModule : NSObject <ZMMessageProtocol,ZMLoginProtocol>

+ (ZMMessageModule*)sharedInstance;

@end
