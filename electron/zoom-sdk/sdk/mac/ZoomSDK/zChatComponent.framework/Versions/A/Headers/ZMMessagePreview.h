//
//  ZMMessagePreview.h
//  zChatComponent
//
//  Created by Cyan.Yang on 2022/10/24.
//

#import <AppKit/AppKit.h>

NS_ASSUME_NONNULL_BEGIN

/// An object that encapsulates the presentable elements for a message preview.
@interface ZMMessagePreview : NSObject

@property (nonatomic, copy) NSAttributedString *primaryAttributedString;
@property (nonatomic, strong) NSString *primaryString;
@property (nonatomic, nullable, copy) NSString *secondaryString;
@property (nonatomic, copy) NSString *timestampString;
@property (nonatomic, strong) NSImage *image;

@end

NS_ASSUME_NONNULL_END
