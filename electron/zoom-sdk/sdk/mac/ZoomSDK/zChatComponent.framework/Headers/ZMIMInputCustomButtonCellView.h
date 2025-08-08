//
//  ZMIMInputCustomButtonCellView.h
//  zChatComponent
//
//  Created by Yong Zhou on 10/25/22.
//

#import <ZoomUnit/ZoomUnit.h>

@class ZMIMInputCustomButtonItem;

NS_ASSUME_NONNULL_BEGIN

@protocol ZMIMInputCustomButtonCellViewDelegate <NSObject>
@optional
- (void)customButtonCellViewDidMoveLeft:(id)sender;
- (void)customButtonCellViewDidMoveRight:(id)sender;
@end

@interface ZMIMInputCustomButtonCellView : ZMBaseTableCellView

@property (nonatomic, retain) ZMIMInputCustomButtonItem *item;

@property (nonatomic, assign) NSInteger firstResponderIndex;

@property (weak, nonatomic) id<ZMIMInputCustomButtonCellViewDelegate> delegate;

- (void)setNextItemFirstResponder;

@end

@interface ZMIMInputCustomButtonSystemSeparateLineCell : NSTableCellView

@property (nonatomic, retain, nullable) ZMIMInputCustomButtonItem *item;

@end

@interface ZMIMInputCustomButtonSystemDescCell : NSTableCellView

@property (nonatomic, retain, nullable) ZMIMInputCustomButtonItem *item;

@end

NS_ASSUME_NONNULL_END
