//
//  IZMShortcuts.h
//  VideoUI
//
//  Created by martin.yu on 9/8/22.
//  Copyright © 2022 zoom.us. All rights reserved.
//

#ifndef IZMShortcuts_h
#define IZMShortcuts_h

@class ZMHotkey;
@protocol IZMShortcuts <ZMRoutableObject>
- (BOOL)bindHotKey:(ZMHotkey *)hotKey global:(BOOL)isGlobal;
- (void)unbindHotkeyByIdentifier:(NSString *)identifier global:(BOOL)isGlobal;
@end

#endif /* IZMShortcuts_h */
