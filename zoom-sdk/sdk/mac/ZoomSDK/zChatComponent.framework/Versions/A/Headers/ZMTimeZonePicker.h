//
//  ZMTimeZonePicker.h
//  ZoomUnit
//
//  Created by Felipe Bastos on 5/3/23.
//

#import <Foundation/Foundation.h>

/// Sends an action when the user selects a new timezone
@interface ZMTimeZonePicker : NSControl
/// Get/set the selected timezone.
/// The possible selected timezones are based on the timezone list shipped with the zoom bundle.
/// Setting to a timezone that isn't in the bundle's list will do nothing.
/// Defaults to the current system timezone.
@property (nonatomic, copy, nonnull) NSTimeZone* selectedTimeZone;
@property (copy) NSString* jid;


- (void)updateMenuItemsWithJid:(NSString*)jid;

- (void)setBorderColor:(nonnull NSColor *)borderColor selectedColor:(nonnull NSColor *)selectedColor;

@end
