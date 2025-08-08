//
//  ZMAICChatDelegate.h
//  ZoomAIC
//
//  Created by Karen Cao on 9/28/23.
//

#ifndef ZMAICChatDelegate_h
#define ZMAICChatDelegate_h

@protocol ZMAICChatSetting;
@protocol ZMAICChatDelegate <NSObject>

- (BOOL)shouldShowSettingsUI;
- (BOOL)isSettingReadOnly;
- (NSArray<id<ZMAICChatSetting>>*)getSettings;
- (id<ZMAICChatSetting>)getCurrentSelectedSetting;
- (NSImage*)getSenderAvatarWithSize:(NSSize)size;
- (NSArray<id<ZMAICQuestion>>*)getFloatingQuestions;
- (BOOL)needShowAICPreviewTip;
- (NSAttributedString*)getStringTip;
@end

#endif /* ZMAICChatDelegate_h */
