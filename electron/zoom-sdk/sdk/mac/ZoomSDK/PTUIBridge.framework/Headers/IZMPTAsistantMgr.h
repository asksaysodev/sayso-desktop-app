//
//  IZMPTAsistantMgr.h
//  PTUIBridge
//
//  Created by fistice on 2024/10/25.
//

#ifndef IZMPTAsistantMgr_h
#define IZMPTAsistantMgr_h

@protocol IZMPTAsistantMgr
- (void)registerPTWebserviceSink;
- (BOOL)IsInCCICall;

- (BOOL)isScheduleAdapterInstalled;
- (void)copyPMINumberToPasteBoard;
- (void)editPMIMeeting;
- (void)copyPMIInvitationToPasteBoard;
- (void)openReleaseNotesWhenSourceUpdateCompleted;
- (BOOL)isPMIItemHostVideoOff;
- (BOOL)hasPMIItem;
- (void)scheduleNewMeeting;
- (void)showCallSipView;
- (void)showSettingWithIdentifier:(NSString *__nullable)identifier;
- (NSArray *_Nullable)transferMeetings;
- (void)startDiagnosticMeeting:(void*)content;

@end
//ZMSharedFor(IZMPTAsistantMgr)
#endif /* IZMPTAsistantMgr_h */
