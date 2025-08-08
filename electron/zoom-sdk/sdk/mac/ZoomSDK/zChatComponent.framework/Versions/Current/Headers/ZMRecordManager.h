//
//  ZMRecordManager.h
//  zChatComponent
//
//  Created by groot.ding on 10/13/23.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@protocol ZMAudioRecordCallback <NSObject>

- (NSString *)outputRecordFilePath;

- (void)didStartRecord;

- (void)didStopRecord;

- (NSView *)dispalyUIView;

- (void)updateRecordSample:(unichar)sampleValue sampleIndex:(NSUInteger)sampleIdx audioLenght:(NSUInteger)audioLenght;

- (NSString *)identifier;

@end

@interface ZMRecordManager : NSObject
+ (ZMRecordManager*)sharedInstance;
+ (void)releaseInstance;

@property (nonatomic, copy, readonly) NSString* rcdFilePath;

- (BOOL)checkAudioRecord;

//get record file path if can record audio
- (NSString *)startRecordWithCallback:(id<ZMAudioRecordCallback>)callback;

- (void)discardRecordCallback:(id<ZMAudioRecordCallback>)callback;

- (void)stopRecord;
- (void)stopRecordWithCallback:(id<ZMAudioRecordCallback>)callback;

- (void)interruptRecordWithCallback:(id<ZMAudioRecordCallback>)callback;
- (void)interruptRecord;

- (BOOL)isRecording;

- (BOOL)checkHadRecordingByIdentifier:(NSString *)identifier;
- (NSDate *)recordDateByIdentifier:(NSString *)identifier;

- (void)onRecordMessageSettingChanged;

@end

@interface ZMRecordVideoManger : NSObject

- (void)openVirtualBackgroundSettiing;

@end

NS_ASSUME_NONNULL_END
