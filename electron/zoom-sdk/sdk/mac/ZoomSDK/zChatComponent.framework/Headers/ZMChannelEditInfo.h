//
//  ZMChannelEditInfo.h
//  zChatComponent
//
//  Created by groot.ding on 7/24/23.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMChannelEditInfo : NSObject
@property(nonatomic, copy) NSString *gID;
@property(nonatomic, copy) NSString *gName;
@property(nonatomic, copy) NSString *aOwner;
@property(nonatomic, copy) NSString *aOwnerName;
@property(nonatomic, copy) NSString *gDesc;
@property(nonatomic, copy) NSString *classificationId;
@property(nonatomic, retain) NSDate *timeStamp;
@property(nonatomic, copy) NSString *reqId;
@property(nonatomic, assign) NSInteger flag;
@property(nonatomic, assign) BOOL isNameModified;
@property(nonatomic, assign) NSInteger descAction;
@property(nonatomic, assign) BOOL isOptionModified;
@property(nonatomic, assign) BOOL isClassificationModified;
@property(nonatomic, assign) BOOL isPMCOptionModified;
@property(nonatomic, assign) BOOL isGiphyEnableStatusModified;
@property(nonatomic, assign) BOOL isMucTransferred;
@property(nonatomic, assign) BOOL isE2EModified;
@property(nonatomic, assign) BOOL isChannelIconModified;

@end

NS_ASSUME_NONNULL_END
