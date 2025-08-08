//
//  ZMMTToolbarDefinations.h
//  VideoUI
//
//  Created by martin yu on 2024/1/30.
//  Copyright © 2024 zoom.us. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ZoomKit/ZoomKit.h>

/// Button Identifier
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdAudio;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdAudioCompanionMic;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdAudioCompanionSpeaker;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdVideo;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdLeaveCompanion;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdImprovedCompanion;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdLeave;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdEnterCompanion;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdSecurity;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdParticipants;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdChat;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdShare;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdShareWhenPaused;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdPauseShare;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdResumeShare;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdAnnotate;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdRemoteControl;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdSlideControl;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdBroadcast;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdZoomIQ;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdRecording;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdRecordingStop;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdRecordingPauseResume;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdQA;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdCC;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdTranscript;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdPolling;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdBilling;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdBO;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdGR;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdReaction;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdRaiseHand;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdSupport;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdInterpretation;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdZoomApp;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdWhiteboard;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdNotes;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdSideCar;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdMoreFeatures;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdDoc;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdClips;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdRoomController;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdMeetingInfo;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdMore;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdShowMeeting;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdHideMeeting;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdMediaStream;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdClinicalNote;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonPigmentIdShareLayout;

/// Default Button Icon Identifier
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdDefault;

/// Audio Button Icon Identifier
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdAudioUnmuteTele;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdAudioMuteTele;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdAudioUnmuteVoip;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdAudioMuteVoip;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdAudioEmpty;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdAudioMicDisabled;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdAudioMicActiveKeyFormatString;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdAudioMicActiveFormatString;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdAudioMicActivePressFormatString;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdAudioNewMicActiveFormatString;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdAudioSyncTimeout;

/// Audio Companion Button Icon Identifier
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdAudioCompanionMicUnmuted;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdAudioCompanionMicMuted;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdAudioCompanionMicLevelKeyFormatString;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdAudioCompanionMicLevelFormatString;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdAudioCompanionSpeakerOff;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdAudioCompanionSpeakerOn;

/// Video Button Icon Identifier
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdVideoStop;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdVideoStart;

/// Chat Button Icon Identifier
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdChatNormal;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdChatUnread;

/// Recording Button Icon Identifier
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdRecordingPause;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdRecordingResume;

/// BO Button Icon Identifier
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdBO;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdBOAskHelp;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdBOJoin;

/// Leave Button Icon Identifier
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdLeave;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdEnd;

/// Raise/Lower hand Button Icon Identifier
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdRaiseHand;
APPKIT_EXTERN NSString *const ZMMTToolbarButtonIconIdLowerHand;
