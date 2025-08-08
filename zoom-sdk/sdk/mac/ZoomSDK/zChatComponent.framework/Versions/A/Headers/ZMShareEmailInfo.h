//
//  ZMShareEmailInfo.h
//  ChatUI
//
//  Created by Groot Ding on 2022/6/20.
//  Copyright © 2022 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMMessageAdapter;

@interface ZMShareEmailInfo : NSObject 

@property (nonatomic,copy) NSString *mailTitle;
@property (nonatomic,copy) NSString *fromJid;
@property (nonatomic,copy) NSString *fromName;
@property (nonatomic,copy) NSString *mailBody;
@property (nonatomic,copy) NSString *mailId;
@property (nonatomic,assign) BOOL isFromZMail;
@property (nonatomic,strong) NSString *validInfo;

@property (nonatomic,assign) unsigned long long mailTime;

@property (nonatomic,weak) ZMMessageAdapter *message;

@end

NS_ASSUME_NONNULL_END
