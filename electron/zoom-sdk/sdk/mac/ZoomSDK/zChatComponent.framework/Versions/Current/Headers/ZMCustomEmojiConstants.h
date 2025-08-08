//
//  ZMCustomEmojiConstants.h
//  ZoomMsgUI
//
//  Created by Yong Zhou on 4/28/22.
//

#ifndef ZMCustomEmojiConstants_h
#define ZMCustomEmojiConstants_h

static const int kEmojiCount = 20;
static const CFStringRef emojiArray[][3] =
{
    CFSTR("zm_emoji_0"),    CFSTR(":smile:"),       CFSTR(":)"),//system
    CFSTR("zm_emoji_1"),    CFSTR(":giggling:"),    CFSTR(""),  //not found
    CFSTR("zm_emoji_2"),    CFSTR(":laugh:"),       CFSTR(":D"),//laughing
    CFSTR("zm_emoji_3"),    CFSTR(":tongue_out:"),  CFSTR(":P"),//stuck_tongue_out
    CFSTR("zm_emoji_4"),    CFSTR(":blushing:"),    CFSTR(""),  //blush
    CFSTR("zm_emoji_5"),    CFSTR(":sad:"),         CFSTR(":("),//frowning2
    CFSTR("zm_emoji_6"),    CFSTR(":cry:"),         CFSTR(""),  //system
    CFSTR("zm_emoji_7"),    CFSTR(":angry:"),       CFSTR(""),  //system
    CFSTR("zm_emoji_8"),    CFSTR(":cold_sweat:"),  CFSTR(""),  //system
    CFSTR("zm_emoji_9"),    CFSTR(":sleepy:"),      CFSTR(""),  //system
    CFSTR("zm_emoji_10"),    CFSTR(":yes:"),        CFSTR(""),  //thumbsup
    CFSTR("zm_emoji_11"),    CFSTR(":no:"),         CFSTR(""),  //point_down
    CFSTR("zm_emoji_12"),    CFSTR(":handshake:"),  CFSTR(""),  //not found
    CFSTR("zm_emoji_13"),    CFSTR(":cool:"),       CFSTR(""),  //system
    CFSTR("zm_emoji_14"),    CFSTR(":surprised:"),  CFSTR(""),  //not found
    CFSTR("zm_emoji_15"),    CFSTR(":star:"),       CFSTR(""),  //system
    CFSTR("zm_emoji_16"),    CFSTR(":heart:"),      CFSTR(""),  //system
    CFSTR("zm_emoji_17"),    CFSTR(":beer:"),       CFSTR(""),  //system
    CFSTR("zm_emoji_18"),    CFSTR(":cake:"),       CFSTR(""),  //system
    CFSTR("zm_emoji_19"),    CFSTR(":party:"),      CFSTR(""),  //tada
};

#endif /* ZMCustomEmojiConstants_h */ 
