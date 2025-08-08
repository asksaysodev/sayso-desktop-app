//
//  OndemandProductInfoProtocol.h
//  ZoomSetting
//
//  Created by Titus.Jiang on 2025/5/12.
//

#ifndef ZMOndemandProductInfoProtocol_h
#define ZMOndemandProductInfoProtocol_h

#import <Foundation/Foundation.h>

@protocol ZMOndemandProductInfoProtocol <NSObject>

@required
@property (nonatomic, readonly, copy) NSString *productName;

@optional
@property (nonatomic, readonly, copy) NSString *version;
@property (nonatomic, readonly, copy) NSString *downloadURL;
@property (nonatomic, readonly, copy) NSString *downloadPath;

@end

#endif /* ZMOndemandProductInfoProtocol_h */
