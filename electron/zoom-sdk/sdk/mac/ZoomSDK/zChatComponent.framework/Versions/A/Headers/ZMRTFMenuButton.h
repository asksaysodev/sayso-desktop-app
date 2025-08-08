//
//  ZMRTFMenuButton.h
//  zChatComponent
//
//  Created by Yong Zhou on 10/29/24.
//

#import <Cocoa/Cocoa.h>

@protocol ZMRTFMenuButtonDelegate <NSObject>

- (void)onCodeblockCopiedWithRange:(NSRange)copyRange;

@end

@interface ZMRTFMenuButton : NSView

@property (nonatomic, assign) NSRange codeblockRange;
@property (nonatomic, copy) NSString *codeLanguage;
@property (nonatomic, weak) id<ZMRTFMenuButtonDelegate> delegate;

+ (instancetype)sharedCopyButton;

- (void)hide;

- (void)show;

@end
