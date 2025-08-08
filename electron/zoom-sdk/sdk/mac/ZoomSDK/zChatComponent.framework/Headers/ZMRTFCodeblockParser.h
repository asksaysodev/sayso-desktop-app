//
//  ZMRTFCodeblockParser.h
//  zChatComponent
//
//  Created by Yong Zhou on 2/11/25.
//

#import <Foundation/Foundation.h>

@interface ZMRTFCodeblockParser : NSObject

- (NSAttributedString *)parseHTML:(NSString *)htmlString;

@end

