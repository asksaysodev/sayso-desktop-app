//
//  ZMMTMeetingInfoHelper.h
//  VideoUIBridge
//
//  Created by Karen Cao on 12/12/24.
//

#import <zVideoUIBridge/ZMBaseHelper.h>
#import <zVideoUIBridge/ZMSmartAgendaDocInfo.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, LiveNoteStatus) {
    LiveNoteStatusNone = 0,
    LiveNoteStatusStart = 1,
    LiveNoteStatusPause = 2,
    LiveNoteStatusStop = 3,
};

@interface ZMMTMeetingInfoHelper : ZMBaseHelper

- (BOOL)isPopupNewMeetingInfoWindowEnabled;
- (void)setPopupNewMeetingInfoWindowEnable:(BOOL)enabled;
- (BOOL)isMeetingDetailEnabled;
- (BOOL)isMeetingHasAssets;
- (BOOL)isLiveNotesEnabled;
- (BOOL)isSmartAgendaEnabled;
- (LiveNoteStatus)getLiveNotesStatus;
- (BOOL)isLiveNotesOn;
- (BOOL)changeLiveNoteStatus:(LiveNoteStatus)action;
- (ZMSmartAgendaDocInfo *)getSmartAgendaDocInfo;
- (BOOL)needPromptLiveNotesDisclaimer;
- (void)agreeLiveNotesDisclaimer:(BOOL)didAgree;

@end
NS_ASSUME_NONNULL_END
