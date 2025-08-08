//
//  ZMPTGiphyData.h
//  ZCommonUI
//
//  Created by groot.ding on 28/07/2020.
//  Copyright © 2020 zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

enum
{
    GIPHY_Download_none, //place holder
    GIPHY_Download_Downloading, //downloading
    GIPHy_Download_Downloaded,  //normal
    GIPHy_Download_DownloadFailed,  //normal
};

@interface ZMPTGiphyData : NSObject
@property(nonatomic, readwrite, copy, nullable) NSString* msgID;
@property(nonatomic, readwrite, copy, nullable) NSString* msgSessionID;
@property(nonatomic, readwrite, copy, nullable) NSString* giphyID;
@property(nonatomic, readwrite, copy, nullable) NSString* giphyString;
@property(nonatomic, readwrite, copy, nullable) NSString* giphyUrl;
@property(nonatomic, readwrite, copy, nullable) NSString* imageLocalPath;
@property(nonatomic, readwrite, copy, nullable) NSString* bigImageLocalPath;
@property(nonatomic, readwrite, assign)NSUInteger               size;
@property(nonatomic, readwrite, assign)NSUInteger    downloadStatus;
@property(nonatomic, readwrite, assign)NSSize        giphyImgSize;
@property(nonatomic, readwrite, assign)NSUInteger    dlRetryCount;
- (NSString*)validImageLocalPath;
- (NSString*)giphyFileSize;
- (void)reset;
- (void)cleanUp;
@end

NS_ASSUME_NONNULL_END
