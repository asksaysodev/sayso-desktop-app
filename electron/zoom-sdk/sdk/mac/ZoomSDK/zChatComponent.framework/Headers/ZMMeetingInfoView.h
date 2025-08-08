//
//  ZMMeetingInfoView.h
//  ZoomUnit
//
//  Created by Yong Zhou on 6/30/22.
//

#import <ZoomKit/ZoomKit.h>

@class ZMMeetingChatCardEntity;

typedef NS_OPTIONS(NSUInteger, ZMMeetingInfoViewStyleMask) {
    ZMMeetingInfoViewStyleMask_Normal = 0,
    ZMMeetingInfoViewStyleMask_HiddenTitle = 1 << 0,
};

@protocol ZMMeetingInfoViewDelegate <NSObject>

- (void)onViewMoreParticipantsEvent;

@end

@interface ZMMeetingInfoView : ZMBaseView

@property(nonatomic, weak) id<ZMMeetingInfoViewDelegate> delegate;

@property(nonatomic, retain) ZMMeetingChatCardEntity *meetingChatModel;

/// You should NOT use this method to initialize. See `initWithStyleMask:` .
- (instancetype)init NS_UNAVAILABLE;
/// You should NOT use this method to initialize. See `initWithStyleMask:` .
- (instancetype)initWithFrame:(NSRect)frameRect NS_UNAVAILABLE;

- (instancetype)initWithStyleMask:(ZMMeetingInfoViewStyleMask)style;

@end
