//
//  ZMReactionInfo.h
//  ZCommonUI
//
//  Created by Frank on 2020/12/21.
//  Copyright © 2020 zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

#define SMALL_VIDEO_RENDER_WIDTH_THRESHOLD 473//384

typedef enum {
    REACTION_INFO_TYPE_UNKNOW    = 0,
    REACTION_INFO_TYPE_EMOJI     = 1,
    REACTION_INFO_TYPE_FEEDBACK  = 2
}ZMReactionInfoType;

typedef enum {
    MEETING_FEEDBACK_TYPE_UNKONW = 0,
    MEETING_FEEDBACK_TYPE_HAND   = 1,
    MEETING_FEEDBACK_TYPE_YES    = 2,
    MEETING_FEEDBACK_TYPE_NO     = 3,
    MEETING_FEEDBACK_TYPE_FAST   = 4,
    MEETING_FEEDBACK_TYPE_SLOW   = 5,
    MEETING_FEEDBACK_TYPE_COUNT  = 6
}ZRMeetingFeedbackType;

typedef enum {
    MEETING_EMOJI_SKIN_TONE_UNKOWN   = 0, // invalid
    MEETING_EMOJI_SKIN_TONE_1        = 1, // default, unicode:_T("")
    MEETING_EMOJI_SKIN_TONE_2        = 2, // unicode:_T("1f3fb")
    MEETING_EMOJI_SKIN_TONE_3        = 3, // unicode:_T("1f3fc")
    MEETING_EMOJI_SKIN_TONE_4        = 4, // unicode:_T("1f3fd")
    MEETING_EMOJI_SKIN_TONE_5        = 5, // unicode:_T("1f3fe")
    MEETING_EMOJI_SKIN_TONE_6        = 6  // unicode:_T("1f3ff")
}ZRMeetingEmojiSkinTone;

@interface ZMReactionInfo : NSObject

@property (nonatomic, assign) ZMReactionInfoType        reactionInfoType;
@property (nonatomic, assign) int                       feedback;
@property (nonatomic, copy)   NSString*                 emoji;

@end

