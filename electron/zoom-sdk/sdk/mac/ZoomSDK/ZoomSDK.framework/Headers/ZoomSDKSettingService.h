/**
 * @file ZoomSDKSettingService.h
 * @brief Interface declarations for Zoom SDK setting services.
 */

#import <Foundation/Foundation.h>
#import <ZoomSDK/ZoomSDKErrors.h>
NS_ASSUME_NONNULL_BEGIN

/**
 * @brief Enumeration of the virtual background URL types.
 */
typedef enum{
    /** The learn more url type*/
    SettingComponent_VirtualBackground_Leran_More,
    /** The help url type*/
    SettingComponent_VirtualBackground_Help,
}SDKURLType;


/**
 * @class ZoomSDKAutoFramingParameter
 * @brief Parameters for auto-framing feature in Zoom SDK.
 */
@interface ZoomSDKAutoFramingParameter : NSObject
/**
 * @brief The zoom in ratio of auto-framing, valid range of values: 1~10(when mode is ZoomSDKAutoFramingMode_Center_Coordinates), 0.1~10(when mode is ZoomSDKAutoFramingMode_Face_Recognition).
 */
@property (assign,nonatomic) float ratio;
/**
 * @brief The strategy when face recognition failed.
 */
@property (assign,nonatomic) ZoomSDKFaceRecognitionFailStrategy failStrategy;
@end

/**
 * @protocol ZoomSDKSettingTestAudioDelegate
 * @brief Delegate to receive audio testing related events.
 */
@protocol ZoomSDKSettingTestAudioDelegate <NSObject>
@optional
/**
 * @brief Notification of microphone level changes when testing.
 * @param level The level of microphone.
 */
- (void)onMicLevelChanged:(unsigned int)level;
/**
 * @brief Notification of speaker level changes when testing.
 * @param level The speaker level.
 */
- (void)onSpeakerLevelChanged:(unsigned int)level;
/**
 * @brief Notify the microphone test status has changed.
 * @param status Test status of microphone.
 */
- (void)onMicTestStatusChanged:(ZoomSDKTestMicStatus)status;
/**
 * @brief Notification of speaker status changes when testing.
 * @param isTesting YES means the speaker is in process of test, otherwise not.
 */
- (void)onSpeakerTestStatusChanged:(BOOL)isTesting;
@end


/**
 * @protocol ZoomSDKSettingAudioDeviceDelegate
 * @brief Delegate to receive audio device related events.
 */
@protocol ZoomSDKSettingAudioDeviceDelegate <NSObject>
@optional
/**
 * @brief Notify the microphone device status has changed in the meeting.
 * @param status The microphone device status.
 */
- (void)onMicDeviceStatusChanged:(ZoomSDKDeviceStatus)status;
/**
 * @brief Notify the speaker device status has changed in the meeting.
 * @param status The speaker device status.
 */
- (void)onSpeakerDeviceStatusChanged:(ZoomSDKDeviceStatus)status;
/**
 * @brief Notification that the selected microphone device is changed.
 */
- (void)onSelectedMicDeviceChanged;
/**
 * @brief Notification that the selected speaker device is changed.
 */
- (void)onSelectedSpeakerDeviceChanged;
@end


/**
 * @protocol ZoomSDKSettingVideoDelegate
 * @brief Delegate to receive video device related events.
 */
@protocol ZoomSDKSettingVideoDelegate <NSObject>
@optional
/**
 * @brief Notification of camera status changes in the meeting.
 * @param status The camera device status.
 */
- (void)onCameraStatusChanged:(ZoomSDKDeviceStatus)status;
/**
 * @brief Notification that the selected camera device is changed.
 * @param deviceID The ID of camera.
 */
- (void)onSelectedCameraChanged:(NSString*)deviceID;
@end


/**
 * @class ZoomSDKSettingTestSpeakerDeviceHelper
 * @brief Helper class to test speaker devices.
 */
@interface ZoomSDKSettingTestSpeakerDeviceHelper: NSObject
{
    id<ZoomSDKSettingTestAudioDelegate>     _delegate;
    BOOL                                    _isSpeakerInTesting;
    NSString*                               _speakerID;
}
/**
 * @brief Indicates whether speaker testing is in progress.
 */
@property (nonatomic, readwrite, assign)BOOL                    isSpeakerInTesting;

/**
 * @brief Delegate to receive audio testing events.
 */
@property(nonatomic, assign, nullable)id<ZoomSDKSettingTestAudioDelegate> delegate;

/**
 * @brief This method is used to start playing when testing speaker. 
 * @param deviceID The ID of the speaker device.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)SpeakerStartPlaying:(NSString*)deviceID;
/**
 * @brief This method is used to stop playing when testing speaker.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)SpeakerStopPlaying;
@end


/**
 * @class ZoomSDKSettingTestMicrophoneDeviceHelper
 * @brief Helper class for testing microphone devices.
 */
@interface ZoomSDKSettingTestMicrophoneDeviceHelper: NSObject
{
    id<ZoomSDKSettingTestAudioDelegate>         _delegate;
    ZoomSDKTestMicStatus         _testMicStatus;
    NSString*                    _microphoneID;
}
/**
 * @brief Delegate to receive audio testing events.
 */
@property(nonatomic, assign, nullable)id<ZoomSDKSettingTestAudioDelegate> delegate;
/**
 * @brief This method is used to start recording when testing microphone.
 * @param deviceID The ID of the microphone device.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)startRecordingMic:(NSString*)deviceID;
/**
 * @brief This method is used to stop recording when testing microphone.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)stopRecrodingMic;
/**
 * @brief This method is used to play recorded sounds when testing microphone.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)playRecordedMic;
/**
 * @brief This method is used to stop playing recorded sounds when testing microphone.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)stopPlayRecordedMic;
/**
 * @brief This method is used to get the status when testing microphone.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKTestMicStatus)getTestMicStatus;
@end


/**
 * @class ZoomSDKSettingTestVideoDeviceHelper
 * @brief Helper class for testing video devices.
 */
@interface ZoomSDKSettingTestVideoDeviceHelper: NSObject
{
    id<ZoomSDKSettingVideoDelegate>       _delegate;
}
/**
 * @brief Delegate to receive video device events.
 */
@property(nonatomic, assign, nullable)id<ZoomSDKSettingVideoDelegate> delegate;
/**
 * @brief This method is used to set the frame of video preview and tell Zoom client the parent view where the video preview will be drawn. 
 * @param parentView The parent view where the video preview will be drawn.
 * @param containerRect The frame displaying video preview.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)SetVideoParentView:(NSView*)parentView VideoContainerRect:(NSRect)containerRect;
/**
 * @brief Preview user's video.
 * @param deviceID The ID of camera device.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success, otherwise failed.
 * @deprecated This method is no longer used.
 */
- (ZoomSDKError)StartPreview:(NSString*)deviceID DEPRECATED_MSG_ATTRIBUTE("No longer used");

/**
 * @brief Preview user's video with default camera.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)StartPreview;
/**
 * @brief Stop previewing user's video.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)StopPreview;
@end


/**
 * @class ZoomSDKAudioStatisticsInfo
 * @brief Class representing audio transmission statistics.
 */
@interface ZoomSDKAudioStatisticsInfo : NSObject
{
    int _frequencySend;
    int _frequencyReceive;
    int _latencySend;
    int _latencyReceive;
    int _jitterSend;
    int _jitterReceive;
    float _packageLossAvgSend;
    float _packageLossAvgReceive;
    float _packageLossMaxSend;
    float _packageLossMaxReceive;
}
/**
 * @brief Get frequency when transferring audio data.
 * @param isSend YES means sending data, NO receiving data.
 */
- (int)getFrequency:(BOOL)isSend;
/**
 * @brief Get latency time when transferring audio data.
 * @param isSend YES means sending data, NO receiving data.
 */
- (int)getLatency:(BOOL)isSend;
/**
 * @brief Get jitter when transferring audio data.
 * @param isSend YES means sending data, NO receiving data.
 */
- (int)getJitter:(BOOL)isSend;
/**
 * @brief Get the rate of losing package when transferring audio data.
 * @param isSend YES means sending data, NO receiving data.
 * @param isMax YES means the max rate of losing package, NO the average rate of losing package.
 */
- (float)getPackageLoss:(BOOL)isSend Max:(BOOL)isMax;
@end


/**
 * @class ZoomSDKVideoASStatisticsInfo
 * @brief Class representing video and sharing transmission statistics.
 */
@interface ZoomSDKVideoASStatisticsInfo : NSObject
{
    int _resolutionSend;
    int _resolutionReceive;
    int _fpsSend;
    int _fpsReceive;
    int _latencySend;
    int _latencyReceive;
    int _jitterSend;
    int _jitterReceive;
    float _packageLossAvgSend;
    float _packageLossAvgReceive;
    float _packageLossMaxSend;
    float _packageLossMaxReceive;
}
/**
 * @brief Get latency time when transferring video or sharing data.
 * @param isSend YES means sending data, NO receiving data.
 */
- (int)getLatency:(BOOL)isSend;
/**
 * @brief Get jitter when transferring video or sharing data.
 * @param isSend YES means sending data, NO receiving data.
 */
- (int)getJitter:(BOOL)isSend;
/**
 * @brief Get the rate of losing package when transferring video or sharing data.
 * @param isSend YES means sending data, NO receiving data.
 * @param isMax YES means the max rate of losing package, NO the average rate of losing package.
 */
- (float)getPackageLoss:(BOOL)isSend Max:(BOOL)isMax;
/**
 * @brief Get resolution when transferring video or sharing data.
 * @param isSend YES means sending data, NO receiving data.
 * @note height can get through (Resolution >> 16), width can get through ((Resolution << 16) >> 16).
 */
- (int)getResolution:(BOOL)isSend;
/**
 * @brief Get the fram rate when transferring video or sharing data.
 * @param isSend YES means sending data, NO receiving data.
 */
- (int)getFPS:(BOOL)isSend;
@end


/**
 * @class SDKDeviceInfo
 * @brief Class representing information about a device (microphone, speaker, camera).
 */
@interface SDKDeviceInfo : NSObject
/**
 * @brief Get the ID of device, such as microphone, speaker, camera.
 */
- (NSString*)getDeviceID;
/**
 * @brief Get the device name.
 */
- (NSString*)getDeviceName;
/**
 * @brief Query if the device is selected.
 */
- (BOOL)isSelectedDevice;
@end



/**
 * @class ZoomSDKAudioSetting
 * @brief Provides APIs to manage audio devices, audio volumes, and audio-related meeting features.
 */
@interface ZoomSDKAudioSetting: NSObject
{
    ZoomSDKSettingTestSpeakerDeviceHelper* _speakerTestHelper;
    ZoomSDKSettingTestMicrophoneDeviceHelper* _micTestHelper;
    id<ZoomSDKSettingAudioDeviceDelegate>       _delegate;
}
/**
 * @brief Delegate to receive audio device status changes.
 */
@property(nonatomic, assign, nullable)id<ZoomSDKSettingAudioDeviceDelegate> delegate;
/**
 * @brief Get the object of ZoomSDKSettingTestSpeakerDeviceHelper.
 * @return If the function succeeds, it will return a ZoomSDKSettingTestSpeakerDeviceHelper object. Otherwise returns nil.
 */
- (ZoomSDKSettingTestSpeakerDeviceHelper*_Nullable)getSettingSpeakerTestHelper;
/**
 * @brief Get the object of ZoomSDKSettingTestMicrophoneDeviceHelper.
 * @return If the function succeeds, it will return a ZoomSDKSettingTestMicrophoneDeviceHelper object. Otherwise returns nil.
 */
- (ZoomSDKSettingTestMicrophoneDeviceHelper*_Nullable)getSettingMicrophoneTestHelper;
/**
 * @brief Get the list of audio device. 
 * @param mic YES means microphone device, No speaker device.
 * @return If the function succeeds, it will return an array containing \link SDKDeviceInfo \endlink elements, otherwise returns nil.
 */
- (NSArray*_Nullable)getAudioDeviceList:(BOOL)mic;

/**
 * @brief Get the volume of audio device. 
 * @param mic YES means the volume of microphone device, No speaker device.
 * @return If the function succeeds, it will return an int value(0-100), otherwise returns nil. 
 */
- (float)getAudioDeviceVolume:(BOOL)mic;

/**
 * @brief Set the volume of audio device. 
 * @param mic YES means the volume of microphone device, No speaker device.
 * @param volume The volume of device, varies from 0 to 100. 
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)setAudioDeviceVolume:(BOOL)mic Volume:(float)volume;

/**
 * @brief Select an audio device.
 * @param mic YES means microphone device, No speaker device.
 * @param deviceID The ID of the device.
 * @param deviceName The name of the device.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)selectAudioDevice:(BOOL)mic DeviceID:(NSString *)deviceID DeviceName:(NSString*)deviceName;

/**
 * @brief Set to enable stereo in the meeting. 
 * @param enable YES means to enable stereo, No to disable.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)enableStero:(BOOL)enable;

/**
 * @brief Enable to join meeting with the audio of computer.
 * @param enable YES means enabled, NO disabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)enableAutoJoinVoip:(BOOL)enable;

/**
 * @brief Mute user's microphone when he joins the meeting with the audio of computer.
 * @param enable YES means enabled, NO disabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)enableMuteMicJoinVoip:(BOOL)enable;
/**
 * @brief Enable the feature that attendee can speak by pressing the Spacebar when he is muted.
 * @param enable YES means enabled, NO disabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)enablePushToTalk:(BOOL)enable;

/**
 * @brief Set whether to disable the prompt dialog of joining meeting with third party audio. 
 * @param disable YES means disabled, NO enabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)disablePromptJoinAudioDialogWhenUse3rdPartyAudio:(BOOL)disable;

/**
 * @brief Determine if the meeting supports to pop up the dialog when user joins meeting with third party audio. 
 * @return YES means supported, otherwise not.
 */
- (BOOL)isSupportPromptJoinAudioDialogWhenUse3rdPartyAudio;

/**
 * @brief Determine if the dialog pops up when user joins meeting with third party audio. 
 * @return YES means that the dialog will not pop up, otherwise not. 
 */
- (BOOL)isPromptJoinAudioDialogWhenUse3rdPartyAudioDisable;

/**
 * @brief Enable auto-adjust microphone.
 * @param enable YES means enabled, NO means disabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)enableAutoAdjustMic:(BOOL)enable;

/**
 * @brief Determine if auto-adjust microphone is enabled or not. 
 * @return YES means enabled, otherwise it is not enabled.
 */
- (BOOL)isAutoAdjustMicOn;
/**
 * @brief Determine if user joins meeting with audio on. 
 * @return YES means to enable the audio, otherwise not.
 */
- (BOOL)isJoinAudioWhenJoinMeetingOn;
/**
 * @brief Determine if user joins meeting with microphone muted. 
 * @return YES means muted, otherwise not. 
 */
- (BOOL)isMuteMicWhenJoinMeetingOn;

/**
 * @brief Determine if stereophonic sound is enabled. 
 * @return YES means enabled, otherwise not. 
 */
- (BOOL)isEnableStereoOn;
/**
 * @brief Set whether to enable the feature that attendee can speak by pressing the Spacebar when he is muted.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isTemporarilyUnmuteOn;


/**
 * @brief Enable show original sound option in meeting UI.
 * @param enable YES means enabled, NO disabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)enableShowOriginalSoundOptionInMeetingUI:(BOOL)enable;

/**
 * @brief Determine if show original sound option in meeting UI is enabled or not.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isShowOriginalSoundOptionInMeetingUIOn;

/**
 * @brief Determine if the meeting supports show original sound option in meeting UI.
 * @return YES means supported, otherwise not.
 */
- (BOOL)isSupportShowOriginalSoundOptionInMeetingUI;

/**
 * @brief Set echo cancellation level.
 * @param level The level to be set.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)setEchoCancellationLevel:(ZoomSDKAudioEchoCancellationLevel)level;
/**
 * @brief Get echo cancellation level.
 * @return The level of echo cancellation.
 */
- (ZoomSDKAudioEchoCancellationLevel)getEchoCancellationLevel;
/**
 * @brief Enable echo cancellation.
 * @param enable YES means enabled, NO disabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)enableEchoCancellation:(BOOL)enable;

/**
 * @brief Determine if echo cancellation is enabled or not.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isEchoCancellationOn;

/**
 * @brief Determine if the meeting supports echo cancellation.
 * @return YES means supported, otherwise not.
 */
- (BOOL)isSupportEchoCancellation;

/**
 * @brief Get the level of suppressed background noise.
 * @return The level of suppressed background noise.
 */
- (ZoomSDKSuppressBackgroundNoiseLevel)getSuppressBackgroundNoiseLevel;

/**
 * @brief Set the level of to suppress background noise.
 * @param level The level to be set.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)setSuppressBackgroundNoise:(ZoomSDKSuppressBackgroundNoiseLevel)level;

/**
 * @brief Determine if used separate audio device to play ringtone simultaneously.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isAlwaysUseSeparateRingSpkOn;

/**
 * @brief Enable use separate audio device to play ringtone simultaneously.
 * @param enable YES means enabled, NO disabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)enableAlwaysUseSeparateRingSpk:(BOOL)enable;

/**
 * @brief Get use separate audio device to play ringtone simultaneously devices.
 * @return If the function succeeds, it will return an array containing SDKDeviceInfo elements, otherwise returns nil.
 */
- (NSArray *_Nullable)getRingSpkDeviceList;

/**
 * @brief Get the volume of audio device.
 * @return If the function succeeds, it will return an float value(0.0-255.0), otherwise returns 0.0.
 */
- (float)getRingSpkVolume;

/**
 * @brief Set the volume of audio device.
 * @param value The value of device, varies from 0.0 to 255.0.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)setRingSpkVolume:(float)value;

/**
 * @brief Select an audio device.
 * @param deviceId The id of device.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)setRingSpkDevice:(NSString*)deviceId;

/**
 * @brief Set enable sync buttons on headset.
 * @param enable YES means enabled, NO disabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)setSyncHeadsetButtonStatus:(BOOL)enable;

/**
 * @brief Determine if sync buttons on headset option on or off.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isSyncHeadsetButtonStatus;

/**
 * @brief Set the "Enable Original Sound" option is high fidelity music model.
 * @param enable YES means enabled, NO disabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)setHighFideMusicMode:(BOOL)enable;

/**
 * @brief Determine if use high fidelity music model.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isHighFideMusicMode;

/**
 * @brief Select the same audio device as system.
 * @param mic YES means microphone device, No speaker device.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)selectSameAudioDeviceAsSystem:(BOOL)mic;
@end


/**
 * @class ZoomSDK3DAvatarImageInfo
 * @brief Represents information of a 3D avatar image.
 */
@interface ZoomSDK3DAvatarImageInfo: NSObject

/**
 * @brief Determine if the current image is being used.
 */
@property(nonatomic,assign,readonly)BOOL isSelected;

/**
 * @brief Determine if it is the most recently used image.
 */
@property(nonatomic,assign,readonly)BOOL isLastUsed;

/**
 * @brief Get the file path of the current image.
 */
@property(nonatomic,copy,readonly,nullable)NSString* imageFilePath;

/**
 * @brief Get the name of the current image.
 */
@property(nonatomic,copy,readonly,nullable)NSString* imageName;

/**
 * @brief Get the index of the current image.
 */
@property(nonatomic,assign,readonly)int index;
@end


/**
 * @class ZoomSDKVideoLipSyncAvatarPreviewHelper
 * @brief Helper class to control preview of lip-sync avatars.
 */
@interface ZoomSDKVideoLipSyncAvatarPreviewHelper : NSObject

/**
 * @brief Start to preview lip-sync avatar.
 * @param lipsyncPreview The view of show lip-sync avatar preview.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)startLipSyncAvatarPreview:(NSView *)lipsyncPreview;

/**
 * @brief Stop to preview lip-sync avatar.
 * @param lipsyncPreview The view of show lip-sync avatar preview.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)stopLipSyncAvatarPreview:(NSView *)lipsyncPreview;
@end


/**
 * @class ZoomSDKVideoPreferenceSetting
 * @brief Settings for video preference modes including frame rates and resolution adaptation.
 *
 * When custom modes are set, the developer provides the maximum and minimum frame rates. If the current bandwidth cannot maintain the minimum frame rate, the video system will drop to the next lower resolution. The default maximum and minimum frame rates for other modes are 0.
 */
@interface ZoomSDKVideoPreferenceSetting : NSObject
/**
 * @brief Preferred video mode.
 */
@property (nonatomic, assign, readwrite) ZoomSDKVideoPreferenceMode mode;
    
/**
 * @brief 0 for the default value, minimumFrameRate should be less than maximumFrameRate,
 * @note range: from 0 to 30. Out of range for frame rate will use default frame rate of Zoom.
 */
@property (nonatomic, assign, readwrite) unsigned int minimumFrameRate;
    
/**
 * @brief 0 for the default value, maximumFrameRate should be less and equal than 30,
 * @note range: from 0 to 30. Out of range for frame rate will use default frame rate of Zoom.
 */
@property (nonatomic, assign, readwrite) unsigned int maximumFrameRate;
@end


/**
 * @class ZoomSDKVideoSetting
 * @brief Interface for video settings including camera selection, video effects, beauty mode, and auto-framing.
 */
@interface ZoomSDKVideoSetting: NSObject
{
    ZoomSDKSettingTestVideoDeviceHelper* settingVideoTestHelper;
}
/**
 * @brief Get the object to test video device.  
 * @return If the function succeeds, it will return a ZoomSDKSettingTestVideoDeviceHelper object, otherwise failed, returns nil.
 */
- (ZoomSDKSettingTestVideoDeviceHelper*_Nullable)getSettingVideoTestHelper;

/**
 * @brief Get the list of camera device.
 * @return If the function succeeds, it will return an array containing SDKDeviceInfo element.
 */
- (NSArray*_Nullable)getCameraList;

/**
 * @brief Select a camera.
 * @param deviceID The ID of camera.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed. 
 */
- (ZoomSDKError)selectCamera:(NSString*)deviceID;

/**
 * @brief Determine if mirror effect is enabled. 
 * @return YES means enabled, otherwise not. 
 */
- (BOOL)isMirrorEffectEnabled;

/**
 * @brief Set to enable/disable mirror effect. 
 * @param enable YES means enabled, No disabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed. 
 */
- (ZoomSDKError)enableMirrorEffect:(BOOL)enable;

/**
 * @brief Determine if facial beauty effect is enabled. 
 * @return YES means enabled, otherwise not. 
 */
- (BOOL)isBeautyFaceEnabled;

/**
 * @brief Enable/disable facial beauty effect.
 * @param enable YES means enabled, No disabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed. 
 */
- (ZoomSDKError)enableBeautyFace:(BOOL)enable;

/**
 * @brief Get beauty face value.
 * @return If the function succeeds, it will return an int value(0-100), otherwise returns 0.
 */
- (int)getBeautyFaceValue;

/**
 * @brief Set beauty face value.
 * @param value The value type is int, varies from 0 to 100.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)setBeautyFaceValue:(int)value;
/**
 * @brief Turn off the participant's video when he joins meeting. 
 * @param disable YES means that the video is turned off, otherwise not. 
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed. 
 */
- (ZoomSDKError)disableVideoJoinMeeting:(BOOL)disable;

/**
 * @brief Display/Hide username on the video window. 
 * @param display YES means showing always username on the video window, otherwise not.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed. 
 */
- (ZoomSDKError)displayUserNameOnVideo:(BOOL)display;

/**
 * @brief Enable or disable HD video. 
 * @param enable YES means enabled, NO disabled. 
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed. 
 */
- (ZoomSDKError)enableCatchHDVideo:(BOOL)enable;
/**
 * @brief Set to capture video ratio: original or 16:9. 
 * @param originalSize YES means original video, NO 16:9.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed. 
 */
- (ZoomSDKError)onVideoCaptureOriginalSizeOr16To9:(BOOL)originalSize;
/**
 * @brief Enable to spotlight the current user's video in the main interface when he speaks in the meeting. 
 * @param enable YES means spotlighting always the current user's video, NO not.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed. 
 */
- (ZoomSDKError)onSpotlightMyVideoWhenISpeaker:(BOOL)enable;
/**
 * @brief Enable or disable to show the participants in Gallery View up to 49 per screen.
 * @param enable YES indicates to show the participants in Gallery View up to 49 per screen, otherwise not.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed. 
 */
- (ZoomSDKError)onDisplayUpTo49InWallView:(BOOL)enable;
/**
 * @brief Enable or disable to hide the non-video participants.
 * @param hide YES means hiding, NO means displaying.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed. 
 */
- (ZoomSDKError)hideNoVideoUser:(BOOL)hide;
/**
 * @brief Determine if non-video participant is hided or not. 
 * @return YES means enabled, otherwise not. 
 */
- (BOOL)isHideNoVideoUser;
/**
 * @brief Query if original size of video is enabled.  
 * @return YES means enabled, otherwise not. 
 */
- (BOOL)isCaptureOriginalSize;
/**
 * @brief Determine if it is enabled to spotlight the current user's video. 
 * @return YES means enabled, otherwise not. 
 */
- (BOOL)isSpotlightMyVideoOn;
/**
 * @brief Determine if the current user's video is muted when he joins meeting. 
 * @return YES means muted, otherwise not. 
 */
- (BOOL)isMuteMyVideoWhenJoinMeetingOn;
/**
 * @brief Determine if it is enabled to display user's screen name.
 * @return YES means enaled, otherwise not. 
 */
- (BOOL)isdisplayUserNameOnVideoOn;
/**
 * @brief Determine if it is able to display up to 49 participants in video wall mode.
 * @return YES means able, otherwise not.  
 */
- (BOOL)isCanDisplayUpTo49InWallView;
/**
 * @brief Determine whether to display up to 49 participants in video wall mode. 
 * @return YES means enabled, otherwise not. 
 */
- (BOOL)isDisplayUpTo49InWallViewOn;
/**
 * @brief Determine if HD video is enabled. 
 * @return YES means enabled, otherwise not. 
 */
- (BOOL)isCatchHDVideoOn;
/**
 * @brief Determine if adjustion for low light.
 * @return If the function succeeds, it will return ZoomSDKSettingVideoLightAdaptionModel.
 */
- (ZoomSDKSettingVideoLightAdaptionModel)getLightAdjustModel;
/**
 * @brief Set the way to adjust the low light.
 * @param model The model to be set.
 * @param value The value type is int, varies from 0 to 100.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)setLightAdaptionModel:(ZoomSDKSettingVideoLightAdaptionModel)model LightAdaptionManualValue:(int)value;
/**
 * @brief Get the value of the setted.
 * @return If the function succeeds, it will return an int value(0-100), otherwise returns 0.
 */
- (int)getLightAdaptionManualValue;

/**
 * @brief Determine whether to hardware acceleration for video receive.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isHardwareAccelerationForVideoReceiveOn;

/**
 * @brief Enable or disable hardware acceleration for video receive.
 * @param enable YES means enabled, NO disabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)enableHardwareAccelerationForVideoReceive:(BOOL)enable;

/**
 * @brief Determine whether to de-noise.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isTemporalDeNoiseOn;

/**
 * @brief Enable or disable de-noise.
 * @param enable YES means enabled, NO disabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)enableTemporalDeNoise:(BOOL)enable;

/**
 * @brief Determine if optimizing received video quality is supported.
 * @return YES indicates that it is supported, NO means that it isn’t.
 */
- (BOOL)isOptimizeVideoQualitySupported;

/**
 * @brief Determine if optimizing received video quality is enabled.
 * @return YES indicates that optimization is enabled. NO means optimization is not enabled.
 */
- (BOOL)isOptimizeVideoQualityEnabled;

/**
 * @brief Enable or disable optimizing received video quality when facing network issues for a variety of reasons. Applies to the window in focus: speaker view, pinned / spotlighted videos, gallery view with a small number of videos
 * @param enable YES indicates to enable this feature. Otherwise disable this feature.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)enableOptimizeVideoQuality:(BOOL)enable;

/**
 * @brief Enable or disable to stop incoming video.
 * @param enable YES indicates to enable to stop incoming video.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)enableStopIncomingVideo:(BOOL)enable;

/**
 * @brief Get the flag to enable/disable to stop in coming video.
 * @return Enabled or disabled.
 */
- (BOOL)isStopIncomingVideoEnabled;

/**
 * @brief Enable or disable to hide the userself's view.
 * @param enable YES indicates to enable to hide the userself's view.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)enableHideSelfView:(BOOL)enable;

/**
 * @brief Get the flag to enable/disable to hide userself's view.
 * @return YES indicates enabled. NO disable.
 */
- (BOOL)isHideSelfViewEnabled;

/**
 * @brief Start my video auto-framing.
 * @param mode The auto-framing mode.
 * @param parameter The setting of video auto-framing.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)enableVideoAutoFraming:(ZoomSDKAutoFramingMode)mode setting:(ZoomSDKAutoFramingParameter *)parameter;

/**
 * @brief Stop video auto-framing.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)disableVideoAutoFraming;

/**
 * @brief Determine whether auto-framing is enabled.
 * @return YES indicates enabled. NO disable.
 */
- (BOOL)isVideoAutoFramingEnabled;

/**
 * @brief Get the auto-framing mode.
 * @return If the function succeeds, it will return the auto-framing mode, otherwise not.
 */
- (ZoomSDKAutoFramingMode)getVideoAutoFramingMode;

/**
 * @brief Get the auto-framing parameter.
 * @param mode The auto-framing mode.
 * @return If the function succeeds, it will return the auto-framing parameter, otherwise not.
 */
- (ZoomSDKAutoFramingParameter *_Nullable)getVideoAutoFramingSettingWithMode:(ZoomSDKAutoFramingMode)mode;

/**
 * @brief Set the mode of auto-framing when auto-framing is enabled.
 * @param mode The mode of auto-framing.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)setVideoAutoFramingMode:(ZoomSDKAutoFramingMode)mode;

/**
 * @brief Set the zoom in ratio of auto-framing when auto-framing is enabled.
 * @param ratio The zoom in ratio, effective range: 0.7 < ratio < 2.1.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)setVideoAutoFramingRatio:(float)ratio;

/**
 * @brief Set the fail strategy of face recognition when auto-framing is enabled(mode is @c ZoomSDKAutoFramingMode_Face_Recognition).
 * @param strategy The fail strategy of face recognition.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)setFaceRecognitionFailStrategy:(ZoomSDKFaceRecognitionFailStrategy)strategy;

/**
 * @brief Set the video quality preference that automatically adjust user's video to prioritize frame rate versus resolution based on the current bandwidth available.
 * @param preferenceSetting An instance of ZoomSDKVideoPreferenceSetting.
 * @return If the function succeeds, it will return ZMVideoSDKErrors_Success.
 */
- (ZoomSDKError)setVideoQualityPreference:(ZoomSDKVideoPreferenceSetting*)preferenceSetting;
@end


/**
 * @protocol ZoomSDKSettingRecordDelegate
 * @brief Delegate to notify cloud recording storage info.
 */
@protocol ZoomSDKSettingRecordDelegate <NSObject>
@optional
/**
 * @brief Notify of cloud recording storage info.
 * @param totalSize The total size of cloud recording storage.
 * @param usedSize The used size of cloud recording storage.
 * @param allowExceedStorage YES means allow exceed storge, NO not.
 */
- (void)onNotifyCloudRecordingStorageInfo:(long long)totalSize usedSize:(long long)usedSize isAllowExceedStorage:(BOOL)allowExceedStorage;
@end


/**
 * @class ZoomSDKRecordSetting
 * @brief Interface for recording settings.
 */
@interface ZoomSDKRecordSetting: NSObject
{
    id<ZoomSDKSettingRecordDelegate>       _delegate;
}
/**
 * @brief Delegate object for receiving recording setting events.
 */
@property(nonatomic, assign, nullable)id<ZoomSDKSettingRecordDelegate> delegate;
/**
 * @brief Set the path for saving the meeting recording file. 
 * @param path The path for saving the meeting recording file.
 * @return If the function succeeds, it will return the @c ZoomSDKError_Success, otherwise failed.
 * @note The parameter 'path' must already be present, or the path cannot be set successfully.
 */
- (ZoomSDKError)setRecordingPath:(NSString*)path;

/**
 * @brief Get the path of the current recorded meeting. 
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed. 
 */
- (NSString*_Nullable)getRecordingPath;

/**
 * @brief Determine if choose recording path when meeting ended is enabled or not.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isEnableChooseRecordingPathWhenMeetingEnd;

/**
 * @brief Enable or disable choose recording path when meeting ended.
 * @param enable YES means enabled, NO disabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)chooseRecordingPathWhenMeetingEnd:(BOOL)enable;

/**
 * @brief Determine if record audio for every attendee individually is enabled or not.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isEnableRecordAudioForEveryAttendeeIndividually;

/**
 * @brief Enable or disable record audio for every attendee individually.
 * @param enable YES means enabled, NO disabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)recordAudioForEveryAttendeeIndividually:(BOOL)enable;

/**
 * @brief Determine if optimize for third party video editor is enabled or not.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isEnableOptimizeFor3PartyVideoEditor;

/**
 * @brief Enable or disable optimize for third party video editor.
 * @param enable YES means enabled, NO disabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)OptimizeFor3PartyVideoEditor:(BOOL)enable;

/**
 * @brief Determine if add timestamp for recording is enabled or not.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isEnableAddTimestampForRecording;

/**
 * @brief Enable or disable add timestamp for recordin.
 * @param enable YES means enabled, NO disabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)addTimestampForRecording:(BOOL)enable;

/**
 * @brief Determine if recording during screen sharing is enabled or not.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isEnableRecordDuringScreenSharing;

/**
 * @brief Enable or disable record during screen sharing .
 * @param enable YES means enabled, NO disabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)recordDuringScreenSharing:(BOOL)enable;

/**
 * @brief Determine if display video next to share contents in recording file is enabled or not.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isEnableDisplayVideoNextToShareContentsInRecordingFile;

/**
 * @brief Enable or disable display video next to share contents in recording file.
 * @param enable YES means enabled, NO disabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)displayVideoNextToShareContentsInRecordingFile:(BOOL)enable;

/**
 * @brief Determine if the user have privilege to get cloud recording storage info.
 * @return YES means can get info, otherwise not.
 */
- (BOOL)canGetCloudRecordingStorageInfo;

/**
 * @brief Call to get cloud recording storage info if the user has the privilege to get cloud recording storage info.
 * @return If the function succeeds, it will return the @c ZoomSDKError_Success, otherwise failed.
 * @note The cloud recording storage info will be notified to user through delegate ZoomSDKSettingRecordDelegate when you have call this api successfully.
 */
- (ZoomSDKError)getCloudRecordingStorageInfo;

/**
 * @brief Determine if the user have privilege to get cloud recording management URL.
 * @return YES means can get url, otherwise not.
 */
- (BOOL)canGetRecordingManagementURL;

/**
 * @brief Call to get cloud recording management URL if the user has the privilege to get cloud recording management URL.
 * @return If the function succeeds, it will return the cloud recording management URL, otherwise nil.
 */
- (NSString*_Nullable)getRecordingManagementURL;

/**
 * @brief Set the cloud recording manage url in the recording setting page.
 * @param recordingMgrUrl Specify the cloud recording manage url in the recording setting page.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)setCustomizedCloudRecordingMgrUrl:(NSString *)recordingMgrUrl;
@end


/**
 * @class ZoomSDKWallpaperItem
 * @brief Represents a wallpaper item.
 */
@interface ZoomSDKWallpaperItem : NSObject
/**
 * @brief Wall-paper ID
 */
@property (copy, nonatomic, readonly, nullable) NSString *wallpaperId;
/**
 * @brief The title.
 */
@property (copy, nonatomic, readonly, nullable) NSString *title;
/**
 * @brief Thumbnail path.
 */
@property (copy, nonatomic, readonly, nullable) NSString *thumbnailPath;
/**
 * @brief Full image path.
 */
@property (copy, nonatomic, readonly, nullable) NSString *path;
/**
 * @brief 0 ~ 255, -1 means no transparency. Only possible in Meeting wall-paper.
 */
@property (assign, nonatomic, readwrite) int transparency;
/**
 * @brief Layout mode of the wall-paper.
 */
@property (assign, nonatomic, readwrite) ZoomSDKWallpaperLayoutMode layoutMode;
@end


/**
 * @protocol ZoomSDKWallpaperControllerDelegate
 * @brief Delegate for wallpaper events.
 */
@protocol ZoomSDKWallpaperControllerDelegate <NSObject>
/**
 * @brief The personal wall-paper change.
 * @param item The changed item.
 */
- (void)onPersonalWallpaperChanged:(ZoomSDKWallpaperItem * _Nullable)item;
/**
 * @brief The status of personal wall-paper download.
 * @param status The wall-paper image download status.
 * @param wallpaperId The downloaded wall-paper image ID.
 */
- (void)onPersonalWallpaperImageDownloadStatus:(ZoomSDKWallpaperSettingStatus)status wallpaper:(NSString *)wallpaperId;
/**
 * @brief The meeting wall-paper change.
 * @param item The  changed item.
 */
- (void)onMeetingWallpaperChanged:(ZoomSDKWallpaperItem * _Nullable)item;
/**
 * @brief The status of meeting wall-paper download.
 * @param status The wall-paper image download status.
 * @param wallpaperId The download wall-paper image ID.
 */
- (void)onMeetingWallpaperImageDownloadStatus:(ZoomSDKWallpaperSettingStatus)status wallpaper:(NSString *)wallpaperId;
@end


/**
 * @class ZoomSDKWallpaperController
 * @brief Controller for managing wallpapers.
 */
@interface ZoomSDKWallpaperController : NSObject
/**
 * @brief Delegate object for handling wallpaper events.
 */
@property(nonatomic, assign, nullable)id<ZoomSDKWallpaperControllerDelegate> delegate;

/**
 * @brief Determine if the personal wall-paper feature enabled by OP.
 * @return YES indicates the feature enabled. Otherwise NO.
 */
- (BOOL)isPersonalWallpaperEnabled;
/**
 * @brief  Get the current user's persional wall-paper config.
 * @return The current user's personal wall-paper.
 * @note Only login user have this config, if select None, the wall-paper ID is empty.
 */
- (ZoomSDKWallpaperItem *)getCurrentPersonalWallpaperItem;
/**
 * @brief  Get the current user's persional wall-paper config list.
 * @return The current user's personal wall-paper list.
 * @note Only login user have this config list.
 */
- (NSArray <ZoomSDKWallpaperItem *>* _Nullable)getPersonalWallpaperList;
/**
 * @brief Set the current user's persional wall-paper config.
 * @param item wall-paper item need to set.
 * @return If the function succeeds, it will return the @c ZoomSDKError_Success, otherwise failed.
 * @note Only login user have this config list.
 */
- (ZoomSDKError)setPersonalWallpaper:(ZoomSDKWallpaperItem *)item;
/**
 * @brief Get the persional wall-paper config by wall-paper ID.
 * @return The personal wall-paper with the wall-paper ID.
 * @warning Only login user have this feature.
 */
- (ZoomSDKWallpaperItem * _Nullable)getPersonalWallpaperItemById:(NSString *)wallpaperId;
/**
 * @brief Determine if the meeting wall-paper feature enabled by OP.
 * @return YES indicates the feature enabled. Otherwise NO.
 */
- (BOOL)isMeetingWallpaperEnabled;
/**
 * @brief Determine if meeting wall-paper thumbnail ready.
 * @return YES indicates ready. Otherwise NO.
 */
- (BOOL)isMeetingWallpaperThumbsReady;
/**
 * @brief Get the meeting wall-paper config.
 * @return The current used meeting wall-paper.
 * @warning if select None, the wall-paper ID is empty.
 */
- (ZoomSDKWallpaperItem * _Nullable)getCurrentMeetingWallpaperItem;
/**
 * @brief Get the meeting wall-paper config list.
 * @return The meeting wall-paper list.
 */
- (NSArray <ZoomSDKWallpaperItem *>* _Nullable)getMeetingWallpaperList;
/**
 * @brief Set the meeting wall-paper config.
 * @param item Wall-paper item need to set.
 * @return SDK error of the set opration.
 */
- (ZoomSDKError)setMeetingWallpaper:(ZoomSDKWallpaperItem *)item;
/**
 * @brief Get the meeting wall-paper config by wall-paper ID.
 * @return The meeting wall-paper with the wall-paper ID.
 */
- (ZoomSDKWallpaperItem *)getMeetingWallpaperById:(NSString *)wallpaperId;
@end


/**
 * @class ZoomSDKGeneralSetting
 * @brief General settings interface for controlling meeting behaviors and UI customizations.
 */
@interface ZoomSDKGeneralSetting: NSObject
/**
 * @brief Enable or disable meeting settings by command.
 * @param enable YES means to enable, otherwise not.
 * @param cmd An enumeration of commands that you can enable or disable them in the meeting.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed.
 */
- (ZoomSDKError)enableMeetingSetting:(BOOL)enable SettingCmd:(MeetingSettingCmd)cmd;
/**
 * @brief Set custom link for invitation.
 * @param inviteURL The URL for invitation by which user can join meeting.
 * @return If the function succeeds, it will return ZoomSDKError_Success. Otherwise failed. 
 */
- (ZoomSDKError)setCustomInviteURL:(NSString*)inviteURL;

/**
 * @brief Custom support URL.
 * @param feedbackURL support URL.
 * @return If the function succeeds, it will return @c ZoomSDKError_Success. Otherwise failed.
 * @deprecated This method is no longer used.
 */
- (ZoomSDKError)setCustomFeedbackURL:(NSString*)feedbackURL DEPRECATED_MSG_ATTRIBUTE("No longer used");

/**
 * @brief Hide setting components. 
 * @param component An enumeration of components for controlling meeting.
 * @param hide YES means to hide, NO to show.
 */
- (void)hideSettingComponent:(SettingComponent)component hide:(BOOL)hide;

/**
 * @brief This method is used to set default URL for setting components.
 * @param urlType A enum specify the url you want to modify.
 * @return A ZoomSDKError to tell client whether function call successfully or not.
 */
- (ZoomSDKError)setCustomURL:(SDKURLType)urlType urlString:(NSString*)urlString;

/**
 * @brief  Get the current status of show meeting time
 * @return If return Yes is means show meeting time
 */
- (BOOL)isShowLockMeetingTime;

/**
 * @brief Enable or disenable to set show meeting time
 * @param enable YES is means show meeting time otherwise is hidden
 * @return If the function is success will return ZoomSDKError_Success. Otherwise fail.
 */
- (ZoomSDKError)enableToShowMeetingTime:(BOOL)enable;

/**
 * @brief  Determine if can copy invite url after start meeting.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isEnableCopyInviteURL;

/**
 * @brief  to set meeting invite url can copy.
 * @param enable YES means can copy otherwise not.
 * @return If the function is success will return ZoomSDKError_Success. Otherwise fail.
 */
- (ZoomSDKError)setCopyMeetingInviteURL:(BOOL)enable;
/**
 * @brief  Determine if can comfire when leaving meeting.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isEnableConfirmLeavingMeeting;

/**
 * @brief to set confirm when user leaving meeting.
 * @param enable Yes means will confirm when user leaving meeting.
 * @return If the function is success will return ZoomSDKError_Success, otherwise fail.
 */
- (ZoomSDKError)setConfirmLeavingMeeting:(BOOL)enable;

/**
 * @brief To set the appearance of ui.
 * @param appearance The enum of appearance.
 * @return If the function is success will return ZoomSDKError_Success, otherwise fail.
 */
- (ZoomSDKError)setUIAppearance:(ZoomSDKUIAppearance)appearance;

/**
 * @brief Get the UI appearance type.
 * @return The return value is the UI appearance type.
 */
- (ZoomSDKUIAppearance)getUIAppearance;

/**
 * @brief Determine if support set ui theme.
 * @return If support, it will return YES, otherwise not.
 */
- (BOOL)isSupportSetUITheme;

/**
 * @brief Set the theme of ui.
 * @param theme The enum of theme.
 * @return If the function is success will return ZoomSDKError_Success, otherwise fail.
 */
- (ZoomSDKError)setUITheme:(ZoomSDKUITheme)theme;

/**
 * @brief Query the theme of ui.
 * @return If the function is success will return the theme of ui.
 */
- (ZoomSDKUITheme)getUITheme;

/**
 * @brief Stop my video and audio when my display is off or screen saver begins.
 * @param enable YES is means mute video and aodio when lock screen,otherewise not.
 * @return If the function is success will return ZoomSDKError_Success, otherwise fail.
 */
- (ZoomSDKError)setMuteVideoAndAudioWhenLockScreen:(BOOL)enable;

/**
 * @brief Determine if mute video and aodio when lock screen.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isMutedVideoAndAudioWhenLockScreen;

/**
 * @brief Set the skintone of the reaction.
 * @param skinTone The enum of skintone.
 * @return If the function is success will return ZoomSDKError_Success, otherwise fail.
 */
- (ZoomSDKError)setReactionSkinTone:(ZoomSDKEmojiReactionSkinTone)skinTone;

/**
 * @brief Get the skintone of the reaction.
 * @return The value of the current reaction skintone.
 */
- (ZoomSDKEmojiReactionSkinTone)getReactionSkinTone;

/**
 * @brief Hide checkbox of automatically copy invite link when meeting start.
 * @param hide YES means hide the checkbox,otherwise not.
 * @return If the function is success will return ZoomSDKError_Success, otherwise fail.
 */
- (ZoomSDKError)hideAutoCopyInviteLinkCheckBox:(BOOL)hide;
/**
 * @brief Mute attendees when they join the meeting.
 * @param bEnable YES means when attendees join the meeting is muted,otherwise not.
 * @param allow YES means attendee can unmute self,otherwise not.
 * @return If the function is success will return ZoomSDKError_Success, otherwise fail.
 */
- (ZoomSDKError)enableMuteOnEntry:(BOOL)bEnable allowUnmuteBySelf:(BOOL)allow;
/**
 * @brief Get the object of set wallpaper controller.
 * @return If the function succeeds, it will return a ZoomSDKWallpaperController object, otherwise failed, returns nil.
 */
- (ZoomSDKWallpaperController*)getWallpaperController;
@end


/**
 * @class ZoomSDKStatisticsSetting
 * @brief Interface for obtaining meeting statistics.
 */
@interface ZoomSDKStatisticsSetting: NSObject
/**
 * @brief Get the connection type of current meeting.
 * @return An enumeration of connection type.
 */
- (SettingConnectionType)getSettingConnectionType;

/**
 * @brief Get network type of current meeting.
 * @return An enumeration of network type.
 */
- (SettingNetworkType)getSettingNetworkType;
/**
 * @brief Get the proxy address of current meeting.
 * @return Proxy address if the meeting uses a proxy.
 */
- (NSString*_Nullable)getProxyAddress;
/**
 * @brief Get audio statistic information of the current meeting.
 * @return If the function succeeds, it will return an object of ZoomSDKAudioStatisticsInfo.
 */
- (ZoomSDKAudioStatisticsInfo*_Nullable)getAudioStatisticsInfo;

/**
 * @brief Get Video/AS statistic information of the current meeting.
 * @param isVideo YES means to get video statistic information, NO to get AS statistics information.
 * @return If the function succeeds, it will return an object of ZoomSDKVideoASStatisticsInfo.
 */
- (ZoomSDKVideoASStatisticsInfo*_Nullable)getVideoASStatisticsInfo:(BOOL)isVideo;
@end


/**
 * @class ZoomSDKVirtualBGImageInfo
 * @brief Information of a virtual background image or video item.
 */
@interface ZoomSDKVirtualBGImageInfo: NSObject

/**
 * @brief Determine if it is the selected virtual background image.
 * @return YES means is the selected virtual background image, otherwise not.
 */
- (BOOL)isSelected;
/**
 * @brief Get file path of the virtual background image.
 * @return If the function succeeds, it will return the image file path.
 */
- (NSString*_Nullable)getImageFilePath;
/**
 * @brief Get image file name of the virtual background image.
 * @return If the function succeeds, it will return the image file name.
 */
- (NSString*_Nullable)getImageName;
/**
 * @brief Determine if the selected virtual background is video.
 * @return YES means is the selected virtual background is video, otherwise not.
 */
- (BOOL)isVideo;

/**
 * @brief Determine if the  virtual background item can be deleted.
 * @return YES means is the selected virtual background can be deleted, otherwise it can not.
 */
- (BOOL)isAllowDelete;
@end


/**
 * @class ZoomSDKVideoFilterItemInfo
 * @brief Information of a video filter (face effect) item.
 */
@interface ZoomSDKVideoFilterItemInfo: NSObject

/**
 * @brief Determine if it is the selected virtual background image.
 * @return YES means is the selected virtual background image, otherwise not.
 */
- (BOOL)isSelected;

/**
 * @brief Get file path of the virtual background image.
 * @return If the function succeeds, it will return the image file path.
 */
- (NSString*_Nullable)getImageFilePath;

/**
 * @brief Get image file name of the virtual background image.
 * @return If the function succeeds, it will return the image file name.
 */
- (NSString*_Nullable)getImageName;

/**
 * @brief Get the type of the virtual background image or video item.
 * @return If the function succeeds, it will return the type.
 */
- (ZoomSDKVideoEffectType)getType;

/**
 * @brief Get the index of the virtual background image or video item.
 * @return If the function succeeds, it will return the index.
 */
- (int)getIndex;
@end


/**
 * @protocol ZoomSDKVirtualBackgroundSettingDelegate
 * @brief Protocol for receiving virtual background setting events.
 */
@protocol ZoomSDKVirtualBackgroundSettingDelegate <NSObject>
@optional
/**
 * @brief Notify the default virtual background image have been downloaded from web.
 * @param filePath The path of the file.
 */
- (void)onVBImageDidDownloaded:(NSString*)filePath;

/**
 * @brief Notify the virtual background was updated with selected color.
 * @param selectedColor The selected color.
 */
- (void)onGreenVBDidUpdateWithReplaceColor:(NSColor*)selectedColor;

/**
 * @brief Notify the selected virtual background image has been changed, user can get the new selected image through image list.
 */
- (void)onSelectedVBImageChanged;

/**
 * @brief Notify the result of adding video virtual background.
 * @param success YES means is successfully added, otherwise not.
 * @param error If failed adding the video virtual background, the error will be notified.
 */
- (void)onVBVideoUploadedResult:(BOOL)success failedError:(ZoomSDKSettingVBVideoError)error;

/**
 * @brief Callback event of notification that the thumbnails of all video filter items have been downloaded.
 */
- (void)onVideoFilterItemThumnailsDownloaded;

/**
 * @brief Callback event of notification that the selected video filter item is downloading.
 * @param type The type of the selected video filter item.
 * @param index The index of the selected video filter item.
 */
- (void)onVideoFilterItemDataDownloading:(ZoomSDKVideoEffectType)type index:(int)index;

/**
 * @brief Callback event of notification that the selected video filter item whether has been downloaded successfully.
 * @param ready YES indicates the selected video filter item has been downloaded successfully.
 * @param type type The type of the selected video filter item.
 * @param index The index of the selected video filter item.
 */
- (void)onVideoFilterItemDataDownloaded:(BOOL)ready type:(ZoomSDKVideoEffectType)type index:(int)index;
@end


/**
 * @class ZoomSDKVirtualBackgroundSetting
 * @brief Interface for configuring virtual background and video filter settings.
 */
@interface ZoomSDKVirtualBackgroundSetting: NSObject
{
    id<ZoomSDKVirtualBackgroundSettingDelegate>       _delegate;
}
@property(nonatomic, assign, nullable)id<ZoomSDKVirtualBackgroundSettingDelegate> delegate;
/**
 * @brief Get the object to video device test helper.
 * @return If the function succeeds, it will return a ZoomSDKSettingTestVideoDeviceHelper object, otherwise failed, returns nil.
 */
- (ZoomSDKSettingTestVideoDeviceHelper*_Nullable)getSettingVideoTestHelper;

/**
 * @brief Determine if support virtual background feature.
 * @return YES means is support, otherwise not.
 */
- (BOOL)isSupportVirtualBG;

/**
 * @brief Determine if meeting supports the video virtual video background feature.
 * @return YES means is support, otherwise not.
 */
- (BOOL)isSupportVirtualBackgroundVideo;

/**
 * @brief Determine if the machine can support the smart virtual background feature.
 * @return YES means is support, otherwise not.
 */
- (BOOL)isDeviceSupportSmartVirtualBG;

/**
 * @brief Determine if the using green screen option is enabled.
 * @return YES means is enabled, otherwise not.
 */
- (BOOL)isUsingGreenScreenOn;

/**
 * @brief Enable or disable the using green screen option.
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)setUsingGreenScreen:(BOOL)bUse;

/**
 * @brief Add virtual background image.
 * @param filePath The file path of the image user want to add.
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)addBGImage:(NSString*)filePath;

/**
 * @brief Add virtual background video.
 * @param filePath The file path of the video user want to add.
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)addBGVideo:(NSString*)filePath;

/**
 * @brief Remove virtual background item.
 * @param virtualBGImageInfo The ZoomSDKVirtualBGImageInfo object user want to remove.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)removeBGItem:(ZoomSDKVirtualBGImageInfo *)virtualBGImageInfo;

/**
 * @brief Get the array of virtual background video item.
 * @return If the function succeeds, it will return the NSArray of video item list, otherwise nil.
 */
- (NSArray*_Nullable)getBGItemList;

/**
 * @brief Use the specify image as selected virtual background images.
 * @param item The path of the virtualbackground image or video item user want to select.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)useBGItem:(ZoomSDKVirtualBGImageInfo*)item;

/**
 * @brief Get the selected replace color of virtual background images.
 * @return If the function succeeds, it will return the color, otherwise nil.
 */
- (NSColor*_Nullable)getVBReplaceColor;

/**
 * @brief Start selected replace color of virtual background images.
 * @return If the function succeeds, it will return the @c ZoomSDKError_Success, otherwise failed.
 * @note The selected replace color will be notified from callback event '- (void)onSelectedVBImageChanged'.
 */
- (ZoomSDKError)startSelectReplaceVBColor;

/**
 * @brief Determine if the machine can support the smart virtual background video feature.
 * @return YES means is support, otherwise not.
 */
- (BOOL)isDeviceSupportSmartVirtualBackgroundVideo;

/**
 * @brief Determine if the machine can support the green virtual background video feature.
 * @return YES means is support, otherwise not.
 */
- (BOOL)isDeviceSupportGreenVirtualBackgroundVideo;

/**
 * @brief Determine if the meeting supports the adding a new virtual background item feature.
 * @return YES means is allowed, otherwise not.
 */
- (BOOL)isAllowAddNewVBItem;

/**
 * @brief Determine if the meeting supports the removing a new virtual background item feature.
 * @return YES means is allowed remove, otherwise not.
 */
- (BOOL)isAllowRemoveVBItem;

/**
 * @brief Determine if face makeup feature is enabled.
 * @return YES means is enabled, otherwise not.
 */
- (BOOL)isVideoFilterEnabled;

/**
 * @brief Determine if support face makeup feature.
 * @return YES means is support, otherwise not.
 */
- (BOOL)isSupportVideoFilter;

/**
 * @brief Get the array of face makeup images.
 * @return If the function succeeds, it will return the NSArray of image list, otherwise nil.
 */
- (NSArray*_Nullable)getVideoFilterItemList;

/**
 * @brief Use the specify image as selected face makeup images.
 * @param imageInfo It is video filter object.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise failed.
 */
- (ZoomSDKError)useVideoFilterItem:(ZoomSDKVideoFilterItemInfo*)imageInfo;
@end


/**
 * @class ZoomSDKShareScreenSetting
 * @brief Interface for configuring share screen settings.
 */
@interface ZoomSDKShareScreenSetting : NSObject

/**
 * @brief Determine if it is able to silence system notifications when sharing desktop.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isDoNotDisturbInSharingOn;

/**
 * @brief Enable or disable silence system notifications when sharing desktop.
 * @param enable YES means enabled, NO disabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)enableDoNotDisturbInSharing:(BOOL)enable;

/**
 * @brief Determine if it is able to show green border when sharing.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isGreenBorderOn;

/**
 * @brief Enable or disable show green border when sharing.
 * @param enable YES means enabled, NO disabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)enableGreenBorder:(BOOL)enable;

/**
 * @brief Determine if it is able to share selected app window only.
 * @return YES means enabled, otherwise not.
 */
- (BOOL)isShareSelectedWndOnlyOn;

/**
 * @brief Enable or disable share selected app window only.
 * @param enable YES means enabled, NO disabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)enableShareSelectedWndOnly:(BOOL)enable;

/**
 * @brief Determine if it is able to using tcp connection for screen sharing.
 * @return YES means disabled, otherwise not.
 */
- (BOOL)isTCPConnectionOn;

/**
 * @brief Enable or disable  use tcp connection for screen sharing.
 * @param enable YES means enabled, NO disabled.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)enableTCPConnection:(BOOL)enable;

/**
 * @brief Set screen capture mode.
 * @param mode The mode to be set.
 * @return If the function succeeds, it will return ZoomSDKError_Success, otherwise not.
 */
- (ZoomSDKError)setScreenCaptureMode:(ZoomSDKScreenCaptureMode)mode;

/**
 * @brief Get screen capture mode
 * @return If the function succeeds will return the current screen capture mode.
 */
- (ZoomSDKScreenCaptureMode)getScreenCaptureMode;

/**
 * @brief Enable or disable meeting settings by command.
 * @param enable YES means to enable, otherwise not.
 * @param shareCmd is a enumeration to set different share screen.
 * @return If the function is success will return ZoomSDKError_Success, otherwise fail.
 */
- (ZoomSDKError)enableSetShareScreen:(BOOL)enable  SettingCmd:(shareSettingCmd)shareCmd;

/**
 * @brief Get the setting item current status.
 * @param sharingCmd is a enumeration.
 * @return  If return YES is enable otherwise is not enable.
 */
- (BOOL)isEnableToSettingShare:(shareSettingCmd)sharingCmd;

/**
 * @brief  get current FPS value.
 * @return the fps value.
 */
- (int)getLimitFPSValue;

/**
 * @brief  set the value of fps limit.
 * @param value is a number of user to set.
 */
- (ZoomSDKError)setLimitedFPSValue:(ZoomSDKFPSValue)value;
/**
 * @brief  Enable  to set limited fps.
 * @return Return YES is enable otherwise is not.
 */
- (BOOL)isEnableToSetLimitFPS;

/**
 * @brief Enable or disenable to set Limit fps value.
 * @param enable Yes means to set otherwise is not.
 */
- (ZoomSDKError)setEnableLimitFPS:(BOOL)enable;

/**
 * @brief Determine if support show zoom meeting window when share.
 * @return YES means support,otherwise not.
 */
- (BOOL)isSupportShowZoomWindowWhenShare;
/**
 * @brief Set enable show zoom meeting window when share.
 * @param show YES means show,otherwise not.
 * @return If the function is success will return ZoomSDKError_Success, otherwise fail.
 */
- (ZoomSDKError)setShowZoomWindowWhenShare:(BOOL)show;

/**
 * @brief Determine if show zoom meeting window when share.
 * @return YES means show,otherwise not.
 */
- (BOOL)isShowZoomWindowWhenShare;
/**
 * @brief Determine if enable share desktop.
 * @return YES means enable,otherwise not.
 */
- (BOOL)isShareDesktopEnabled;
/**
 * @brief Set the share option when share appliaction.
 * @param shareOption It is a enumeration of share option.
 * @return If the function is success will return ZoomSDKError_Success, otherwise fail.
 */
- (ZoomSDKError)setShareOptionWhenShareApplication:(ZoomSDKSettingShareScreenShareOption)shareOption;
/**
 * @brief Get the option of share application.
 * @return The value is a enumeration of share option.
 */
- (ZoomSDKSettingShareScreenShareOption)getShareOptionWhenShareApplication;
/**
 * @brief Set the share option when share in meeting.
 * @param shareOption It is a enumeration of share option.
 * @return If the function is success will return ZoomSDKError_Success, otherwise fail.
 */
- (ZoomSDKError)setShareOptionwWhenShareInMeeting:(ZoomSDKSettingShareScreenShareOption)shareOption;
/**
 * @brief Get the option of share in meeting.
 * @return The value is a enumeration of share option.
 */
- (ZoomSDKSettingShareScreenShareOption)getShareOptionwWhenShareInMeeting;
/**
 * @brief Set the share option when share in direct share.
 * @param shareOption It is a enumeration of share option.
 * @return If the function is success will return ZoomSDKError_Success, otherwise fail.
 */
- (ZoomSDKError)setShareOptionwWhenShareInDirectShare:(ZoomSDKSettingShareScreenShareOption)shareOption;
/**
 * @brief Get the option of share in direct share.
 * @return The value is a enumeration of share option.
 */
- (ZoomSDKSettingShareScreenShareOption)getShareOptionwWhenShareInDirectShare;

/**
 * @brief Enable/Disable the GPU acceleration when a user shares video.
 * @param bEnable YES indicates to enable the acceleration. NO indicates to not enable acceleration.
 * @return If the function is success will return ZoomSDKError_Success, otherwise fail.
*/
- (ZoomSDKError)enableHardwareAcceleratedVideoSharing:(BOOL)bEnable;

/**
 * @brief Determine if GPU acceleration is enabled when a user shares video.
 * @return YES indicates the GPU acceleration is enabled.NO indicates the GPU acceleration is not.
 */
- (BOOL)isVideoSharingHardwareAccelerated;

/**
 * @brief Enable or disable Zoom Docs features in custom UI. This is disabled by default.
 * @param enable YES means enable the Zoom Docs feature. Otherwise the feature is not enabled.
 * @return If the function succeeds, it returns the ZoomSDKError_Success. Otherwise the function fails.
 */
- (ZoomSDKError)enableZoomDocs:(BOOL)enable;
@end


/**
 * @protocol ZoomSDK3DAvatarSettingDelegate
 * @brief Delegate protocol for receiving 3D avatar setting events.
 */
@protocol ZoomSDK3DAvatarSettingDelegate <NSObject>

/**
 * @brief Callback event notifying that all 3D avatar items’ thumbnails have been downloaded.
 */
- (void)on3DAvatarItemThumbnailsDownloaded;

/**
 * @brief Callback event notifying that the selected 3d avatar item is downloading.
 * @param index The index of the selected 3d avatar item.
 */
- (void)on3DAvatarItemDataDownloading:(int)index;

/**
 * @brief Callback event notifying whether or not the selected 3d avatar item has been downloaded successfully.
 * @bSuccess YES indicates the selected 3d avatar item has been downloaded successfully.
 * @param index The index of the selected 3d avatar item.
 */
- (void)on3DAvatarItemDataDownloaded:(BOOL)isSuccess index:(int)index;
@end


/**
 * @class ZoomSDK3DAvatarSetting
 * @brief Interface for configuring 3D avatar settings.
 */
@interface ZoomSDK3DAvatarSetting : NSObject
{
    ZoomSDKVideoLipSyncAvatarPreviewHelper* lipsyncAvatarPreviewHelper;
    id<ZoomSDK3DAvatarSettingDelegate>       _delegate;
}
/**
 * @brief Delegate for receiving 3D avatar setting events.
 */
@property(nonatomic,assign, nullable)id<ZoomSDK3DAvatarSettingDelegate> delegate;

/**
 * @brief Determine if the 3D avatar feature is supported by video device.
 * @return YES indicates that the video device supports the 3D avatar feature.
 */
- (BOOL)is3DAvatarSupportedByDevice;

/**
 * @brief Determine if the 3D avatar feature is enabled.
 * @return YES indicates the video filter feature is enabled.
 */
- (BOOL)is3DAvatarEnabled;

/**
 * @brief Get the list of the video filter images.
 * @return If there are images in the list, the return value is a list of the poiters to ZoomSDK3DAvatarImageInfo.
 */
- (NSArray<ZoomSDK3DAvatarImageInfo*>*_Nullable)get3DAvatarImageList;

/**
 * @brief Specify an image to be the video filter image.
 * @param image Specify the image to use.
 * @return If the function is success will return ZoomSDKError_Success, otherwise fail.
 */
- (ZoomSDKError)set3DAvatarImage:(ZoomSDK3DAvatarImageInfo*)image;

/**
 * @brief Get the object to test video device.
 * @return If the function succeeds, it will return a ZoomSDKSettingTestVideoDeviceHelper object, otherwise failed, returns nil.
 */
- (ZoomSDKSettingTestVideoDeviceHelper*_Nullable)getTestVideoDeviceHelper;

/**
 * @brief Get the object to preview the lip-sync avatar.
 * @return If the function succeeds, it will return a ZoomSDKVideoLipSyncAvatarPreviewHelper object, otherwise failed, returns nil.
 */
- (ZoomSDKVideoLipSyncAvatarPreviewHelper*_Nullable)getLipSyncAvatarPreviewHelper;

/**
 * @brief Enable or disable 3D avatar effect when joining a meeting.
 * @param enable YES to enable 3D avatar effect. Otherwise not.
 * @return If the function is success will return ZoomSDKError_Success, otherwise the functain fails and returns a calling error.
 */
- (ZoomSDKError)enable3DAvatarEffectForAllMeeting:(BOOL)enable;

/**
 * @brief Determine whether 3D avatar is enabled when the joining a meeting.
 * @return YES if enabled, otherwise the function returns NO.
 */
- (BOOL)is3DAvatarEffectForAllMeetingEnabled;
@end


/**
 * @class ZoomSDKFaceMakeupImageInfo
 * @brief Represents information about a face makeup image in Zoom SDK.
 */
@interface ZoomSDKFaceMakeupImageInfo : NSObject

/**
 * @brief Get the type of the current image.
 */
@property(nonatomic, assign, readonly)ZoomSDKFaceMakeupType faceMakeupType;

/**
 * @brief Determine if the current face makeup item is being used.
 */
@property(nonatomic, assign, readonly)BOOL isSelected;

/**
 * @brief Get the file path of the current image.
 */
@property(nonatomic, copy, readonly)NSString *imageFilePath;

/**
 * @brief Get the name of the current image.
 */
@property(nonatomic, copy, readonly)NSString *imageName;

/**
 * @brief Get the index of the current face makeup.
 */
@property(nonatomic, assign, readonly)int  index;
@end


/**
 * @protocol ZoomSDKFaceMakeupSettingContextDelegate
 * @brief Delegate protocol to receive callbacks about face makeup setting events.
 */
@protocol ZoomSDKFaceMakeupSettingContextDelegate <NSObject>

/**
 * @brief Callback event notifying that all face makeup items' thumbnails were downloaded.
 * @param type The type of current image.
 */
- (void)onFaceMakeupItemThumbnailsDownloaded:(ZoomSDKFaceMakeupType)type;

/**
 * @brief Callback event notifying that the selected face makeup item is downloading.
 * @param type The type of current image.
 * @param index The index of the current face makeup.
 */
- (void)onFaceMakeupItemDataDownloading:(ZoomSDKFaceMakeupType)type index:(int)index;

/**
 * @brief Callback event notifying whether or not the selected face makeup item has downloaded successfully.
 * @param success YES indicates the selected face makeup item has downloaded successfully.
 * @param type The type of current image.
 * @param index The index of the current face makeup.
 */
- (void)onFaceMakeupItemDataDownloaded:(BOOL)success faceMakeupType:(ZoomSDKFaceMakeupType)type index:(int)index;

@end


/**
 * @class ZoomSDKFaceMakeupSettingContext
 * @brief Provides APIs to manage face makeup settings in Zoom SDK.
 */
@interface ZoomSDKFaceMakeupSettingContext : NSObject
{
    id<ZoomSDKFaceMakeupSettingContextDelegate>       _delegate;
}

/**
 * @brief The delegate to receive face makeup setting events.
 */
@property(nonatomic,assign, nullable)id<ZoomSDKFaceMakeupSettingContextDelegate> delegate;

/**
 * @brief Determine if the face makeup feature is enabled.
 * @return YES indicates that face makeup feature is enabled.
 */
- (BOOL)isFaceMakeupEnabled;

/**
 * @brief Determine if the meeting supports the face makeup feature.
 * @return YES indicates that the meeting supports the face makeup feature.
 */
- (BOOL)isSupportFaceMakeup;

/**
 * @brief Enable/Disable the selected face makeup effect is always used by future meetings.
 * @param enable YES indicates enabled. Otherwise not.
 * @return If the function is success will return ZoomSDKError_Success, otherwise fail.
 */
- (ZoomSDKError)enableFaceMakeupEffectForAllMeeting:(BOOL)enable;

/**
 * @brief Determine if the selected face makeup effect is always used by future meetings.
 * @return YES indicates that the selected face makeup effect still applies to future meetings.
 */
- (BOOL)isFaceMakeupEffectForAllMeetingEnabled;

/**
 * @brief Get the list of the face makeup images.
 * @return If there are images in the list, the return value is a list of the pointers to ZoomSDKFaceMakeupImageInfo.
 */
- (NSArray<ZoomSDKFaceMakeupImageInfo *> *_Nullable)getFaceMakeupImageList;

/**
 * @brief Specify an image to be face makeup image.
 * @param image Specify the image to use.
 * @return If the function is success will return ZoomSDKError_Success, otherwise fail.
 */
- (ZoomSDKError)setFaceMakeupImage:(ZoomSDKFaceMakeupImageInfo *)image;

/**
 * @brief Enable/Disable the lip face makeup effect.
 * @param enable YES indicates enabled. Otherwise not.
 * @return If the function is success will return ZoomSDKError_Success, otherwise fail.
 */
- (ZoomSDKError)setLipsFaceMakeup:(BOOL)enable;

/**
 * @brief Set the colors of face makeup effect.
 * @param color Specify the color of the face makeup effect in RGB format.
 * @param type The specified color is used on which face makeup type.
 * @return If the function is success will return ZoomSDKError_Success, otherwise fail.
 */
- (ZoomSDKError)setColor:(NSColor *)color type:(ZoomSDKFaceMakeupType)type;

/**
 * @brief Set the transparency of face makeup effect.
 * @param opactity Specify the transparency of the face makeup effect. The value should be between 0 to 100.
 * @param type The specified color is used on which face makeup type.
 * @return If the function is success will return ZoomSDKError_Success, otherwise fail.
 */
- (ZoomSDKError)setOpacity:(int)opacity type:(ZoomSDKFaceMakeupType)type;

/**
 * @brief Disable all the face makeup effect and reset color/opactity value to default value.
 * @return If the function is success will return ZoomSDKError_Success, otherwise fail.
 */
- (ZoomSDKError)resetAllFaceMakeupEffect;
/**
 * @brief Get the pointer to ZoomSDKSettingTestVideoDeviceHelper which is used to preview the video with face makeup image.
 * @return If the function succeeds, the return value is the pointer to ZoomSDKSettingTestVideoDeviceHelper.
 */
- (ZoomSDKSettingTestVideoDeviceHelper*_Nullable)getTestVideoDeviceHelper;
@end


/**
 * @class ZoomSDKSettingService
 * @brief Provides access to various Zoom SDK settings modules.
 */
@interface ZoomSDKSettingService : NSObject
{
    ZoomSDKAudioSetting* _audioSetting;
    ZoomSDKVideoSetting* _videoSetting;
    ZoomSDKRecordSetting* _recordSetting;
    ZoomSDKGeneralSetting* _generalSetting;
    ZoomSDKStatisticsSetting* _statisticsSetting;
    ZoomSDKVirtualBackgroundSetting* _virtualBGSetting;
    ZoomSDKShareScreenSetting* _shareScreenSetting;
    ZoomSDK3DAvatarSetting* _avatarSetting;
    ZoomSDKFaceMakeupSettingContext* _faceMakeupSettingContext;
}
/**
 * @brief Get the object of audio settings.
 * @return If the function succeeds, it will return an object of ZoomSDKAudioSetting.
 */
- (ZoomSDKAudioSetting*_Nullable)getAudioSetting;

/**
 * @brief Get the object of video settings.
 * @return If the function succeeds, it will return an object of ZoomSDKVideoSetting.
 */
- (ZoomSDKVideoSetting*_Nullable)getVideoSetting;

/**
 * @brief Get the object of meeting recording settings.
 * @return If the function succeeds, it will return an object of ZoomSDKRecordSetting.
 */
- (ZoomSDKRecordSetting*_Nullable)getRecordSetting;

/**
 * @brief Get the object of general settings of SDK.
 * @return If the function succeeds, it will return an object of ZoomSDKGeneralSetting.
 */
- (ZoomSDKGeneralSetting*_Nullable)getGeneralSetting;

/**
 * @brief Get the object of statistic settings.
 * @return If the function succeeds, it will return an object of ZoomSDKStatisticsSetting.
 */
- (ZoomSDKStatisticsSetting*_Nullable)getStatisticsSetting;

/**
 * @brief Get the object of virtual background settings.
 * @return If the function succeeds, it will return an object of ZoomSDKVirtualBackgroundSetting.
 */
- (ZoomSDKVirtualBackgroundSetting*_Nullable)getVirtualBGSetting;

/**
 * @brief Get the object of share screen settings.
 * @return If the function succeeds, it will return an object of ZoomSDKShareScreenSetting.
 */
- (ZoomSDKShareScreenSetting*_Nullable)getShareScreenSetting;

/**
 * @brief Get the object of 3D avatar.
 * @return If the function succeeds, it will return an object of ZoomSDK3DAvatarSetting.
 */
- (ZoomSDK3DAvatarSetting*_Nullable)get3DAvatarSettings;

/**
 * @brief Get the object of face makeup.
 * @return If the function succeeds, it will return an object of ZoomSDKFaceMakeupSettingContext.
 */
- (ZoomSDKFaceMakeupSettingContext*_Nullable)getFaceMakeupSettings;
/**
 * @brief Custom the url link show or hide on setting page.
 * @param settingPageUrl It is a enumeration of setting page url.
 * @param hide YES means is hide,otherwise not.
 */
- (void)configToShowUrlLinksInSetting:(ZoomSDKSettingPageURL)settingPageUrl isHide:(BOOL)hide;
@end

NS_ASSUME_NONNULL_END
