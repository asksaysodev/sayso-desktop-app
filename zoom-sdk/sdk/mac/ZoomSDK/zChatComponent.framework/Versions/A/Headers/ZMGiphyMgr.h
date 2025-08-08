//
//  ZMGiphyMgr.h
//  SaasBeePTUIModule
//
//  Created by likevin on 7/20/17.
//  Copyright © 2017 Zipow. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/zChatComponent-Swift.h>

#define HOT_GIPHY_NUM 30
#define SEARCH_GIPHY_NUM 30
#define MAX_DOWNLOAD_RETRY_COUNT 10

@class ZMPTGiphyData, ZMMessageAdapter;

@interface ZMGiphyMgr : NSObject <ZMCollectionViewDelegateWaterfallLayout>

@property(nonatomic,copy) NSString *currentSessionId;

- (void)cleanUp;
- (void)initDefaultHotEmojis;
- (BOOL)downloadGiphy:(NSString*)giphyID andGiphyMsgGuid:(NSString*)giphyMsgGuid andSessionID:(NSString*)inSessionId;
- (void)attach2GridView:(id)inGridView;
- (BOOL)loadGiphyDataByString:(NSString*)string andGiphyNum:(NSUInteger)giphyNum;

- (void)updateGiphyDataWithMessageInfo:(ZMMessageAdapter*)msgInfo outGiphyData:(ZMPTGiphyData*)giphyData;
- (BOOL)downloadWithGiphyData:(ZMPTGiphyData*)giphyData;
@end
