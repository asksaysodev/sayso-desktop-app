//
//  IZMStartMeetingHelper.h
//  PTUIBridge
//
//  Created by fistice on 2024/10/25.
//

#ifndef IZMStartMeetingHelper_h
#define IZMStartMeetingHelper_h

/// MeetingStartEntranceType
typedef NS_ENUM(int, ZMStartMeetingEntrance) {
    /// MeetingStartEntrance_Unknown = 0,
    ZMStartMeetingEntranceanceUnknown = 0,
    /// MeetingStartEntrance_Home = 1,
    ZMStartMeetingEntranceanceHome = 1,
    /// MeetingStartEntrance_Dm = 2,
    ZMStartMeetingEntranceanceDM = 2,
    /// MeetingStartEntrance_Channel = 3,
    ZMStartMeetingEntranceanceChannel = 3,
    /// MeetingStartEntrance_Dm_Sidebar = 4,
    ZMStartMeetingEntranceanceDMSideBar = 4,
    /// MeetingStartEntrance_Channel_Sidebar = 5,
    ZMStartMeetingEntranceanceChannelSidebar = 5,
    /// MeetingStartEntrance_Missed_Call = 6,
    ZMStartMeetingEntranceanceMissedCall = 6,
    /// MeetingStartEntrance_Contacts = 7,
    ZMStartMeetingEntranceanceContacts = 7,
    /// MeetingStartEntrance_Channel_Member_List = 8,
    ZMStartMeetingEntranceanceChannelMemberList = 8,
    /// MeetingStartEntrance_Profile_Card = 9,
    ZMStartMeetingEntranceanceProfileCard = 9,
    /// MeetingStartEntrance_Muc = 10,
    ZMStartMeetingEntranceanceMuc = 10,
    /// MeetingStartEntrance_Muc_Sidebar = 11,
    ZMStartMeetingEntranceanceMucSidebar = 11
};

@protocol IZMStartMeetingHelper <NSObject>

+ (void)joinMeeting;
+ (void)startMeeting;
+ (void)startMeetingWithZMEntrance:(ZMStartMeetingEntrance)entrance videoOn:(BOOL)videoOn;

@end
#endif /* IZMStartMeetingHelper_h */
//[ZMRouterCenter.shared classForProtocol:@protocol(IZMStartMeetingHelper)]
