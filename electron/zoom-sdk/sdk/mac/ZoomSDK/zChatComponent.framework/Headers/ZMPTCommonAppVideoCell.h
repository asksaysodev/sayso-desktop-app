//
//  ZMPTCommonAppVideoCell.h
//  zChatComponent
//
//  Created by Cyan.Yang on 2024/3/5.
//

#import <Cocoa/Cocoa.h>
#import <ZoomKit/ZoomKit.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMCBotInlineVideoPreviewAppContext;

@interface ZMPTCommonAppVideoCardView : ZMBaseView

@property (nonatomic, nullable, copy) NSString *sessionID;
@property (nonatomic, nullable, copy) NSString *messageID;
@property (nonatomic, nullable, copy) NSString *actionID;

@property (nonatomic, nullable, copy) NSURL *thumbnailURL;

@property (nonatomic, nullable, copy) NSString *appID;
@property (nonatomic, nullable, strong) ZMCBotInlineVideoPreviewAppContext *appContext;

@end

NS_ASSUME_NONNULL_END
