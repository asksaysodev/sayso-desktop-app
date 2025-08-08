//
//  ZMRecordViewController.h
//  zChatComponent
//
//  Created by Kevin Li on 2022/7/20.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

#define RECORD_MAX_SAMPLE 600
#define RECORD_SAMPLE_INTERVAL (0.1)
typedef NS_ENUM(NSUInteger, ZMRecordViewType)
{
    ZMRecordViewType_Normal = 0,
    ZMRecordViewType_Warning,
    ZMRecordViewType_Stop,
};

@interface ZMRecordViewController : ZMBaseViewController
@property (nonatomic, copy) NSString* sessionId;
@property (nonatomic, copy) NSString* threadId;
@property (nonatomic, copy, nullable) void (^hideRecordView)(BOOL isUserOp);
@property(nonatomic, assign) BOOL isSmallIconMode;

- (BOOL)checkRecord;
- (void)startRecord;

- (void)notifyRecordUiWithEvt:(ZMRecordViewType)eveType;

+ (ZMRecordViewController*)replyRecordViewController;
+ (ZMRecordViewController*)replyRecordViewControllerInstance;
+ (void)releaseReplyRecordViewControllerInstance;
@end


@interface ZMWaveFormView: NSView
@property (nonatomic, retain) NSColor*    waveColor;
@property (nonatomic, assign) NSInteger   maxSampleNum;
- (void)prepareWaveData;
- (void)appendSample:(unichar)sampleValue sampleIndex:(NSInteger)index; //[0-100]
@end

NS_ASSUME_NONNULL_END
