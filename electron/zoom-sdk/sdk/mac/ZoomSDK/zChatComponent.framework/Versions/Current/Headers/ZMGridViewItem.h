//
//  ZMGridViewItem.h
//  mac-app
//
//  Created by likevin on 9/28/16.
//  Copyright © 2016 zoom.us. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class ZMGridViewItemProperty;
@class ZMStickerInfo;

@interface ZMAcceptFirstMouseImageView : NSImageView
@end

@interface ZMGiphyInfo : NSObject

@property (copy) NSString* gifID;
@property (copy) NSString* displayName;
@property (copy) NSString* url;
@property (copy) NSString* bigLocalPath;
@property (copy) NSString* localPath;
@property (assign) BOOL isDownloaded;
@property (assign) unsigned long long size;
@property (assign) NSSize pic_size;

- (id)initWithInfo:(NSDictionary*)info;

@end


@interface ZMEmojiItemData : NSObject
{
    CGFloat                 _contentInsetX;
    CGFloat                 _contentInsetY;
    NSImage*                _itemImage;
    NSString*               _tooltip;
    BOOL                    _isGifImage;
    BOOL                    _isAlwaysAnimate;
}
@property(nonatomic, readwrite, assign)  CGFloat contentInsetX;
@property(nonatomic, readwrite, assign)  CGFloat contentInsetY;
@property(nonatomic, readwrite, retain)  NSImage*   itemImage;
@property(nonatomic, readwrite, copy)    NSString* tooltip;
@property(nonatomic, readwrite, assign)  BOOL isGifImage;
@property(nonatomic, readwrite, assign)  BOOL isAlwaysAnimate;
@property(nonatomic, readwrite, assign)  NSSize itemInset;
@property (nonatomic, readonly, assign, getter=isUploading)  BOOL uploading;

@property(nonatomic, copy)  NSString *localPath;

- (void)cleanUp;
@end

enum{
    Type_Downlad_NotStarted = 0,
    Type_Downlad_Small_Started,
    Type_Downlad_Big_Started,
};
@interface ZMGiphyEmojiItemData : ZMEmojiItemData
{
    ZMGiphyInfo*            _giphyInfo;
    NSUInteger              _downloadStatus;
}

@property(retain) ZMGiphyInfo* giphyInfo;
@property(assign) NSUInteger downloadStatus;

@end

@interface ZMStickerItemData : ZMEmojiItemData
{
    ZMStickerInfo*          _stickerInfo;
    NSDate*                 _itemDate;
}

@property(nonatomic, readwrite, retain)  ZMStickerInfo* stickerInfo;
@property(nonatomic, readwrite, copy)    NSDate* itemDate;

- (void)cleanUp;
- (NSString*)keyString;
- (NSString*)stickerFiledId;

@end

@interface ZMGridViewItem : NSView
{
    NSProgressIndicator*                    _loadingSpin;
    ZMEmojiItemData*        _itemData;
    BOOL _isHovered;
    BOOL _isSelected;
}
@property(nonatomic, readwrite, retain) ZMEmojiItemData* itemData;
@property(nonatomic, readwrite, assign) BOOL isHovered;
@property(nonatomic, readwrite, assign) BOOL isSelected;

- (void)cleanUp;
- (void)setTooltip:(NSString *)tooltip;
- (NSString*)toolTip;
@end


@interface ZMCollectionViewItem : NSCollectionViewItem
{
    ZMGridViewItem*  _itemView;
}
@property(nonatomic, readonly) ZMGridViewItem*  itemView;

@property(nonatomic, assign) NSEdgeInsets contentInset;

@property(nonatomic, retain) ZMGridViewItemProperty *hoverItemProperty;

- (id)initWithBound:(NSRect)bound;

@end
