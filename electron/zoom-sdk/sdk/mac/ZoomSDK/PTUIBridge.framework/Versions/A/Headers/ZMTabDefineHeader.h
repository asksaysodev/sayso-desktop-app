//
//  ZMTabDefineHeader.h
//  ChatUI
//
//  Created by Cooper.Chen on 2024/7/9.
//  Copyright © 2024 Zoom. All rights reserved.
//

#ifndef ZMTabDefineHeader_h
#define ZMTabDefineHeader_h

typedef NS_OPTIONS(NSUInteger, ZMMainTabsType) {
    ZMMainTabsInvalid = 0,
    ZMMainTabsHome = 1 << 0,
    ZMMainTabsChat = 1 << 1,
    ZMMainTabsMeet = 1 << 2,
    ZMMainTabsPhone = 1 << 3,
    ZMMainTabsDirectory = 1 << 4,
    ZMMainTabsMail = 1 << 5,
    ZMMainTabsZapp = 1 << 6,
    ZMMainTabsCCI = 1 << 8,
    ZMMainTabsCalendar = 1 << 7,
    ZMMainTabsScheduler = 1 << 17,
    ZMMainTabsWhiteBoard = 1 << 9,
    ZMMainTabsZoomIQ = 1 << 10,
    ZMMainTabsLab =1 << 11,
    ZMMainTabsSpots = 1 << 12,
    ZMMainTabsClips = 1 << 13,
    ZMMainTabsNotes = 1 << 14,
    ZMMainTabsWorkvivo = 1 << 15,
    ZMMainTabsDocs = 1 << 16,
    ZMMainTabsFax = 1 << 18,
    ZMMainTabsSpaces = 1 << 19,
    ZMMainTabsHub = 1 << 20,
    ZMMainTabsWorkFlow = 1 << 21,
    ZMMainTabsChatPWA = 1 << 22,
    ZMMainTabsTasks = 1 << 23,
    ZMMainTabsSurveys = 1 << 24,
    ZMMainTabsEvents = 1 << 25,
    ZMMainTabsClinicalNote = 1 << 26,
};

#endif /* ZMTabDefineHeader_h */
