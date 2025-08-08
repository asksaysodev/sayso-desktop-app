//
//  ZMButtonItem.h
//  zChatComponent
//
//  Created by groot.ding on 1/10/24.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMButtonItem;
@class ZMButton;

@interface NSButton (ButtonItem)

@property (nonatomic,strong) ZMButtonItem *buttonItem;

@end

@interface ZMButtonItem : NSObject

@property (nonatomic,copy,nullable) NSString *title;

@property (nonatomic,copy,nullable) NSString *tooltip;

@property (nonatomic,strong,nullable) NSImage *image;

@property (nonatomic,strong,nullable) NSImage *hoverImage;

@property (nonatomic,strong,nullable) NSImage *alternateImage;

@property (nonatomic,weak,nullable) id target;

@property (nonatomic,assign) SEL action;

@property (nonatomic,strong) NSObject *representedObject;

@property (nonatomic,copy,nullable) NSString *accessibilityRoleDescription API_AVAILABLE(macos(10.10));

@property (nonatomic,copy,nullable) NSString *accessibilityLabel API_AVAILABLE(macos(10.10));

- (ZMButton *)getButton;

@end

NS_ASSUME_NONNULL_END
