//
//  ZMCMessenger.h
//  zChatComponent
//
//  Created by Huxley Yang on 11/13/24.
//

#import <Foundation/Foundation.h>
#include <zPTApp/SaasBeePTAppInterface.h>
#import <zChatComponent/ZMCE2EComponent.h>
#import <zChatComponent/ZMCSessionTabComponent.h>
#import <zChatComponent/ZMCFileContentComponent.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMCMessenger : NSObject

+ (instancetype)defaultMessenger;//Chat
+ (instancetype)meetingMessenger;//Meeting

ZMCMessenger *ZMCGetMessenger(NSString *__nullable sessionID);

//MARK: IZoomMessenger
- (NS_ZOOM_MESSAGER::IZoomMessenger*)pMessenger;

//MARK: components
@property (readonly, nonatomic, strong) ZMCE2EComponent *e2eComponent;
@property (readonly, nonatomic, strong) ZMCSessionTabComponent *sessionTabComponent;
@property (readonly, nonatomic, strong) ZMCFileContentComponent *fileContentComp;

@end

NS_ASSUME_NONNULL_END
