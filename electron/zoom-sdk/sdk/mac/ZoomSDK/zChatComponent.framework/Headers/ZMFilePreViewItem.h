//
//  ZMFilePreViewItem.h
//  FilePreviewDemo
//
//  Created by groot.ding on 2/6/20.
//  Copyright © 2020 groot.ding. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMButtonItem.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, ZMFileDownLoadState) {
    ZMFileDownLoaded,
    ZMFileDownLoading,
    ZMFileDownLoadedFail,
};

@interface ZMFilePreViewItem : NSObject

@property (nonatomic, copy) NSString *fileId;

@property (nonatomic, copy) NSString *filePath;

@property (nonatomic, copy) NSString *previewPath;

@property (nonatomic, retain) id representedObject;

@property (nonatomic, assign) ZMFileDownLoadState downloadState;

@property (nonatomic, retain) NSArray <ZMButtonItem *> *hoverButtonItems;

@end

NS_ASSUME_NONNULL_END
