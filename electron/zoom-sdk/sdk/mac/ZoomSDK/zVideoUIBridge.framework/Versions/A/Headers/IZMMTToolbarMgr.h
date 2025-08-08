//
//  IZMMTToolbarMgr.h
//  VideoUI
//
//  Created by martin.yu on 9/13/22.
//  Copyright © 2022 zoom.us. All rights reserved.
//

#ifndef IZMMTToolbarMgr_h
#define IZMMTToolbarMgr_h

// Toolbar
#define ZMMTToolbarHeight 58
#define ZMMTToolbarContentMargin 4

// Fitbar
#define ZMMTFitbarHeight 58
#define ZMMTFitbarContentHeight 54
#define ZMMTFitbarContentMarginTopBottom 2
#define ZMMTFitbarContentMarginLeftRight 2

typedef NS_ENUM(NSInteger, ZPConfToolBarButtonTag) {
    ZPConfToolBarButtonTagAudio = 1,
    ZPConfToolBarButtonTagAudioOrAudioSetting,
    ZPConfToolBarButtonTagAudioMenu,
    ZPConfToolBarButtonTagVideo,
    ZPConfToolBarButtonTagParticipant,
    ZPConfToolBarButtonTagChat,
    ZPConfToolBarButtonTagBackstage,
    ZPConfToolBarButtonTagBO,
    ZPConfToolBarButtonTagNewBO,
    ZPConfToolBarButtonTagPolling,
    ZPConfToolBarButtonTagBilling,
    ZPConfToolBarButtonTagClosedCaption,
    ZPConfToolBarButtonTagInterpretation,
    ZPConfToolBarButtonTagReacitons,
#ifdef ENABLE_ZAPP
    ZPConfToolBarButtonTagZoomApps,
#endif
    ZPConfToolBarButtonTagZoomIQ,
    ZPConfToolBarButtonTagRecord,
    ZPConfToolBarButtonTagQA,
    ZPConfToolBarButtonTagMore,
    ZPConfToolBarButtonTagDoc,
    ZPConfToolBarButtonTagWhiteboard,
    ZPConfToolBarButtonTagMeetingInfo,
    ZPConfToolBarButtonTagNotes,
    ZPConfToolBarButtonTagClips,
    ZPConfToolBarButtonTagSidecar,
    ZPConfToolBarButtonTagMoreFeatures,
    ZPConfToolBarButtonTagHand,
    ZPConfToolBarButtonTagEnterCompanion,
    ZPConfToolBarButtonTagLeaveCompanion,
    ZPConfToolBarButtonTagImprovedCompanion,
    ZPConfToolBarButtonTagRoomController,
    ZPConfToolBarButtonTagMediaStream,
    ZPConfToolBarButtonTagClinicalNote,
    ZPConfToolBarButtonTagTranscript,
};

NS_ASSUME_NONNULL_BEGIN

@protocol IZPConfToolbarController <ZMRoutableObject>
- (nullable __kindof NSView *)buttonWithTag:(ZPConfToolBarButtonTag)tag;
- (void)updateButtonWithTag:(ZPConfToolBarButtonTag)tag;
- (void)updateButtonWithTag:(ZPConfToolBarButtonTag)tag needRelayoutUI:(BOOL)needRelayoutUI;
- (void)updateButtonWithTag:(ZPConfToolBarButtonTag)tag needRelayoutUI:(BOOL)needRelayoutUI needDelay:(BOOL)needDelay;

- (void)tryToPinButtonWithTag:(ZPConfToolBarButtonTag)tag;

- (void)updateAudio;

// Implementated at category.
@optional
- (BOOL)isToolbarAvailableInterpretLanguageTipShowing;
- (BOOL)isToolbarInterpretationMicStateEnabledTipShowing;
- (BOOL)isToolbarInterpretationMicStateDisabledTipShowing;

@end

#define kToolbarFirstShowTime 8
#define kToolbarDefaultShowTime 4
#define kToolbarHoveredShowTime 10

@protocol IZMMTToolbarMgr <ZMRoutableObject>
@property (nonatomic, retain, readonly, nullable) ZMMTBaseViewController<IZPConfToolbarController> *toolbarController;
@property (nonatomic, assign, readonly) BOOL needShowUserInfo;

- (BOOL)isToolbarHidden;
- (void)showToolbar:(int)inTimeInterval;
- (void)hideToolbar;

@end

NS_ASSUME_NONNULL_END

#endif /* IZMMTToolbarMgr_h */
