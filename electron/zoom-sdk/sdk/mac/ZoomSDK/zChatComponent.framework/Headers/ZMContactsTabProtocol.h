//
//  ZMContactsProtocol.h
//  zChatComponent
//

#import <Foundation/Foundation.h>

#ifndef ZMContactsProtocol_h
#define ZMContactsProtocol_h

NS_ASSUME_NONNULL_BEGIN

@protocol ZMContactsTabProtocol <NSObject>
/// YES if the contacts tab sidebar should be a native UI element, NO if the sidebar will be part of the webview.
/// @note Even if the user preference is to use webUI, this returns NO if the webui sidebar can't be used due to OP configuration.
- (BOOL)isUsingNativeUIForSidebar;
/// Save/get user preference for web UI sidebar/native sidebar.
@property (nonatomic, assign, getter=isNativeSidebarPreferred) BOOL nativeSidebarPreferred;
@end

NS_ASSUME_NONNULL_END

#ifndef ZMContactsTabProtocol
#define ZMShareContactsTabProtocol ((id <ZMContactsTabProtocol>)ZMSharedFor(ZMContactsTabProtocol))
#endif

#endif /* ZMContactsProtocol_h */
