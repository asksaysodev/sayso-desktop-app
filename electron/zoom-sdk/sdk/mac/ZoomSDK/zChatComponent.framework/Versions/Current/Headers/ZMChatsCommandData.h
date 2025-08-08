//
//  ZMChatsCommandData.h
//  ZoomMsgUI
//
//  Created by Yong Zhou on 5/16/22.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMChatsCommandData : NSObject

@property (retain) NSString* command;
@property (retain) NSString* jid;
@property (retain) NSString* cmdPrefix;
@property (retain) NSString* cmdDescription;
@property (retain) NSString* completeCommand;
@property (retain) NSImage* image;
@property (assign) BOOL isSelected;
@property (assign) BOOL isLastUsed;
@property (assign) BOOL isGroup;
@property (nonatomic, retain) NSString *tab;
@property (nonatomic, retain) NSString *url;

@end

NS_ASSUME_NONNULL_END



