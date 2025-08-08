//
//  ZMDFParserModel.h
//  ChatUI
//
//  Created by Jed Zheng on 14/3/2023.
//  Copyright © 2023 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ZMDFParserModel : NSObject
@property (nonatomic, assign) BOOL isMyJid;
@property (nonatomic, retain) NSString *fullPath;
@property (nonatomic, assign) BOOL isValid;

- (void)parseZmdfPathWithUrl:(NSString *)path;
@end
