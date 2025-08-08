//
//  ZMIMRecordVideoView.h
//  ChatUI
//
//  Created by simon shang on 2022/1/12.
//  Copyright © 2022 Zoom. All rights reserved.
//

#import <ZoomKit/ZoomKit.h>
#import "ZMIMRecordVideoMgr.h"
#import "zPTApp/SaasBeePTAppInterface.h"

NS_ASSUME_NONNULL_BEGIN

@protocol ZMIMRecordVideoViewDelegate <NSObject>

- (void)onRecordReachTimeLimit;
- (void)recordViewRested;

@end

@interface ZMIMRecordVideoView : ZMBaseView

@property std::shared_ptr<media_client::IVideoRender> render;

@property (nonatomic, weak) id<ZMIMRecordVideoViewDelegate> recordDelegate;

@property (nonatomic, copy) NSString *sessionID;
@property (nonatomic, copy) NSString *threadID;
@property (nonatomic, readonly) NSString *localRecordVideoPath;

- (void)updateRecordView:(BOOL)showLivePreview;

- (BOOL)startRecord;
- (void)stopRecord;
- (void)resetPlayer;
- (void)resetRecordView;

@end

NS_ASSUME_NONNULL_END
