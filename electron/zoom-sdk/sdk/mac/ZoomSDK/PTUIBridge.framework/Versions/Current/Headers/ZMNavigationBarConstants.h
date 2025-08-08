//
//  ZMNavigationBarConstants.h
//  PTUIBridge
//
//  Created by fistice on 2024/11/6.
//

typedef NS_ENUM(NSUInteger, ZMHeaderNavigationButton) {
    ZMHeaderNavigationButtonNone,
    ZMHeaderNavigationButtonBack,
    ZMHeaderNavigationButtonForward,
    ZMHeaderNavigationButtonHistory,
    ZMHeaderNavigationButtonSearch,
    ZMHeaderNavigationButtonPairRoom,
    ZMHeaderNavigationButtonHelp,
    ZMHeaderNavigationButtonNotification,
    ZMHeaderNavigationButtonCalendar,
    ZMHeaderNavigationButtonAIC,
    ZMHeaderNavigationButtonProfile,
};

typedef NS_ENUM(NSUInteger, ZMNavigationTabMode) {
    ZMNavigationTabModeTopbar,
    ZMNavigationTabModeMoreMenu,
};

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
