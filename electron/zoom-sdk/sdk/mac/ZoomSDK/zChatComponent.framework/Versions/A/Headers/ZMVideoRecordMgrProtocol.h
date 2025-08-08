//
//  ZMVideoRecordMgrProtocol.h
//  zChatComponent
//
//  Created by simon shang on 2022/8/17.
//

#import <Foundation/Foundation.h>

#ifndef ZMVideoRecordMgrProtocol_h
#define ZMVideoRecordMgrProtocol_h

@protocol ZMVideoRecordMgrProtocol <NSObject>

- (void)openVirtualBackgroundSettiing;

@end

#ifndef ZMShareVideoRecordMgrProtocol
#define ZMShareVideoRecordMgrProtocol (id <ZMVideoRecordMgrProtocol>)ZMSharedFor(ZMVideoRecordMgrProtocol)
#endif

#endif /* ZMVideoRecordMgrProtocol */
