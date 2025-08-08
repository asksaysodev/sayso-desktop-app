//
//  ZMViewAttachmentCellProtocol.h
//  zChatComponent
//
//  Created by Yong Zhou on 5/11/23.
//

#import <Cocoa/Cocoa.h>
#import <zChatComponent/ZMMessageCellViewDelegate.h>

#ifndef ZMViewAttachmentCellProtocol_h
#define ZMViewAttachmentCellProtocol_h

NS_ASSUME_NONNULL_BEGIN

@protocol ZMViewAttachmentCellProtocol <NSObject>

@required

/// imageView which added on controlView of NSTextAttachmentCell, will retained by NSTextView  (ATTENTION⚠️:  imageView shall be removed from  NSTextView when NSTableCellView is from reuse pool)
@property (nonatomic, nullable, retain, readonly) NSView *imageView;

/// [MUST DO] remove imageView from it superView
- (void)cleanUp;

@optional

- (void)layoutTextMaxWidth:(CGFloat)maxWidth paragraphStyle:(NSParagraphStyle *)paragraphStyle;

/// Some attachment cells need to behave like the message cells. For example, interacting with the message
/// view controller. This method give the receiver a chance to retrive the message cells' delegate.
- (void)attachDelegate:(nullable id<ZMMessageCellViewDelegate>)delegate ofMessageCellView:(ZMBaseMessageCellView *)cellView;

@end

NS_ASSUME_NONNULL_END

#endif /* ZMViewAttachmentCellProtocol_h */
