//
//  ZMSysEmojiToolbarView.h
//  ZoomUnit
//
//  Created by Vinson Wang on 2022/11/8.
//

#import <Cocoa/Cocoa.h>

@protocol ZMSysEmojiToolbarViewDelegate <NSObject>

- (void)onTagSelected:(NSUInteger)tag;

@end

@interface ZMSysEmojiToolbarItemModel : NSObject

@property(nonatomic, copy) NSString *iconName;
@property(nonatomic, copy) NSString *hoverIconName;
@property(nonatomic, copy) NSString *toolTipName;

@end

@interface ZMSysEmojiToolbarView : NSView <NSAccessibilitySwitch>

@property (nonatomic, readwrite, weak) id <ZMSysEmojiToolbarViewDelegate> delegate;

- (void)updateSelectedTagWhenScroll:(NSInteger)tag;

- (instancetype)initWithFrame:(CGRect)frame dataSource:(NSArray<ZMSysEmojiToolbarItemModel *> *)dataSource;

@end
