//
//  ZMOndemandProductInfoWrapper.h
//  ZoomSetting
//
//  Created by Titus.Jiang on 2025/5/12.
//

#import <Foundation/Foundation.h>
#import "ZMOndemandProductInfoProtocol.h"
#include "dataModule/zASRDataDef.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZMOndemandProductInfoWrapper : NSObject<ZMOndemandProductInfoProtocol>

@property (nonatomic, copy, readonly) NSString *productName;
@property (nonatomic, copy, readonly, nullable) NSString *version;
@property (nonatomic, copy, readonly, nullable) NSString *downloadURL;
@property (nonatomic, copy, readonly, nullable) NSString *downloadPath;

- (instancetype)initWithOndemandProductInfo:(const ASRModelDownloadInfo &)productInfo;
- (instancetype)initWithProductName:(NSString *)productName;

@end

NS_ASSUME_NONNULL_END
