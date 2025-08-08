//
//  ZMInputTextView+DocUrl.h
//  zChatComponent
//
//  Created by Yong Zhou on 4/18/24.
//

#import <zChatComponent/ZMInputTextView.h>

NS_ASSUME_NONNULL_BEGIN

@interface ZMInputTextView (DocUrl)

- (void)processLinksInAttributedString:(NSAttributedString *)attributedString;
- (void)fetchLinksWithContent:(NSString*)content;
- (void)fetchMutipleLinks:(NSArray *)Links;

- (void)parseMatchedLinks:(ZMInputBoxLinkInfoSyncedResult*)linkInfo;

- (void)removeDocUrlObserver;

@end

NS_ASSUME_NONNULL_END
