//
//  ZMCmmVideoAPIProtocol.h
//  zChatComponent
//
//  Created by simon shang on 2022/8/16.
//

#import <Foundation/Foundation.h>
//#import "SBPTUI.h"
#include <zVideoApp/CmmConfAPI.h>

#ifndef ZMCmmVideoAPIProtocol_h
#define ZMCmmVideoAPIProtocol_h

@protocol ZMCmmVideoAPIProtocol <NSObject>

- (ICmmVideoAPI *)getCurrentVideoApi;

@end

#ifndef ZMShareCmmVideoAPIProtocol
#define ZMShareCmmVideoAPIProtocol (id <ZMCmmVideoAPIProtocol>)ZMSharedFor(ZMCmmVideoAPIProtocol)
#endif

#endif /* ZMCmmVideoAPIProtocol_h */
