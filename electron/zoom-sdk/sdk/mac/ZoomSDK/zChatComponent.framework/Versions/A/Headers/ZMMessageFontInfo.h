//
//  ZMMessageFontInfo.h
//  zChatComponent
//
//  Created by Felipe Bastos on 8/19/24.
//

#import <Foundation/Foundation.h>
#import <zChatComponent/ZMEmojiModel.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMMessageFontRangeInfo : NSObject

@property (nonatomic,assign) NSRange range;
@property (nonatomic,copy,nullable) NSString *value; //match _reserve1 in SDK data -> StyleOffset
@property (nonatomic,copy,nonnull) NSString *value2; // match _reserve2 in SDK data -> StyleOffset
@property (nonatomic,copy,nonnull) NSString *fileID;
@property (nonatomic, assign) ZMCustomEmojiSource emojiSource; //custom emoji

@end

NS_ASSUME_NONNULL_END


NS_ASSUME_NONNULL_BEGIN

@interface ZMMessageFontInfo : NSObject

@property (nonatomic,assign) unsigned long long style;
@property (nonatomic, retain, readonly,nonnull) NSMutableArray<ZMMessageFontRangeInfo*>* rangeInfoArray;

- (void)addRangeInfo:(ZMMessageFontRangeInfo * _Nonnull)rangeInfo;
+ (instancetype)fontInfoWithStyle:(unsigned long long)style;

@end

NS_ASSUME_NONNULL_END

