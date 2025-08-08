//
//  ZMMessageBodyImageView.h
//  zChatComponent
//
//  Created by Yong Zhou on 4/23/24.
//
//  This class is used to render the picture icon inside the message. 
//  Doc icons and other new features with an icon in the message body could use this class with ZMViewPresentationAttachmentCell.

#import <ZoomKit/ZoomKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMMessageBodyImageView : ZMBaseView

@property (nonatomic, copy) NSString *urlString;
@property (nonatomic, assign) NSEdgeInsets iconInsets;
@property (nonatomic, assign) BOOL disableUserInteraction;

- (instancetype)initWithIconInsets:(NSEdgeInsets)insets;

@end

NS_ASSUME_NONNULL_END
