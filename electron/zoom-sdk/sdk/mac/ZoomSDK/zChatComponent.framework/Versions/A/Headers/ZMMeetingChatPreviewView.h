//
//  ZMMeetingChatPreviewView.h
//  ZoomUnit
//
//  Created by Yong Zhou on 6/27/22.
//

#import <ZoomKit/ZoomKit.h>

@class ZMMeetingChatCardEntity;

NS_ASSUME_NONNULL_BEGIN

@interface ZMMeetingChatPreviewView : ZMBaseView

@property(nonatomic, retain) ZMMeetingChatCardEntity *meetingChatModel;

@end

NS_ASSUME_NONNULL_END
