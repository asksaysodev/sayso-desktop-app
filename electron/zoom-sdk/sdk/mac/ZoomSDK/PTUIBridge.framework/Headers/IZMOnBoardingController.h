//
//  IZMOnBoardingController.h
//  PTUIBridge
//
//  Created by Slevin Zhang on 2025/4/25.
//

#ifndef IZMOnBoardingController_h
#define IZMOnBoardingController_h

@protocol IZMOnBoardingController <ZMRoutableObject>

+ (id <IZMOnBoardingController>)checklistController;
- (NSView *)checklistView;
- (void)updateAvatarImage;

@end

#endif
