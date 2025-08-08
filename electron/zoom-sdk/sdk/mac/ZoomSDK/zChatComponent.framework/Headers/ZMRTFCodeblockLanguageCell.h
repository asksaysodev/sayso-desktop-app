//
//  ZMRTFCodeblockLanguageCell.h
//  zChatComponent
//
//  Created by Yong Zhou on 3/13/25.
//

#import <ZoomUnit/ZoomUnit.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMRTFCodeblockLanguageCell : ZMBaseTableCellView

@property (nonatomic, strong) ZMTextField *titleTextField;
@property (nonatomic, strong) NSImageView *checkImageView;
@property (nonatomic, assign) BOOL isChecked;

@end

NS_ASSUME_NONNULL_END
