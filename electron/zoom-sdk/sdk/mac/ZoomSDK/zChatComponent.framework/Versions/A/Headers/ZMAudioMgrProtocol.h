//
//  ZMAudioMgrProtocol.h
//  zChatComponent
//
//  Created by Kevin Li on 2022/7/16.
//

#ifndef ZMAudioMgrProtocol_h
#define ZMAudioMgrProtocol_h

@protocol ZMAudioMgrProtocol <NSObject>

- (NSString*)sessionID;
- (NSString*)messageID;
- (unichar)micLevel;

- (BOOL)isPlayingWithSessionID:(NSString*)sessionID andMsgID:(NSString*)msgID;
- (BOOL)playAudioWithAudioFile:(NSString*)filePath andSessionID:(NSString*)sessionID andMsgID:(NSString*)msgID;
- (BOOL)playAudioWithAudioFile:(NSString*)filePath;
- (void)stopAudioWithSessionID:(NSString*)sessionID;
- (void)stopAudioWithSessionID:(NSString*)sessionID andMsgID:(NSString*)msgID;
- (void)stopAudio;
- (BOOL)audioVoiceEndWithHandle:(NSUInteger)handle;

-  (BOOL)startRecordMicToFile:(NSString*)filePath;
-  (BOOL)stopRecordMic;

- (void)onMicLevelChanged:(unichar)micLevel;

- (BOOL)isPlaying;

@end

#ifndef ZMShareAudioMgrProtocol
#define ZMShareAudioMgrProtocol (id <ZMAudioMgrProtocol>)ZMSharedFor(ZMAudioMgrProtocol)
#endif

#endif /* ZMAudioMgrProtocol_h */
