//
//  ZMSendFileMenuImpl.h
//  zChatUI
//
//  Created by groot.ding on 10/7/19.
//  Copyright © 2019 Zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <zChatComponent/ZMMenuImpl.h>

const NSInteger Upload_YourComputer_File_TAG = 1001;


typedef NS_OPTIONS(NSUInteger, ZMFileMenuType)
{
    ZMFileMenuType_None    = 0,
    ZMFileMenuType_Dropbox,
    ZMFileMenuType_MicrosoftOneDrive,
    ZMFileMenuType_GoogleDrive,
    ZMFileMenuType_Box,
    ZMFileMenuType_MicrosoftSharePoint,
    ZMFileMenuType_Create_Word,
    ZMFileMenuType_Create_Excel,
    ZMFileMenuType_Create_PowerPoint,
    ZMFileMenuType_Create_Note,
    ZMFileMenuType_Create_BoxNode,
    ZMFileMenuType_Create_BoxNodeFromTemplate,
    ZMFileMenuType_Create_GoogleDocs,
    ZMFileMenuType_Create_GoogleSlides,
    ZMFileMenuType_Create_GoogleSheet,
    
    ZMFileMenuType_Create_Folder = 20,
};

@interface ZMSendFileMenuImpl : ZMMenuImpl

@property (nonatomic,copy) NSString *sessionId;

@property (nonatomic,copy) NSString *threadId;

- (BOOL)showMenuWithSessionId:(NSString *)sessionId threadId:(NSString *)threadId inView:(NSView *)inView localPoint:(CGPoint)localPoint;

- (BOOL)showMenuWithSessionId:(NSString *)sessionId threadId:(NSString *)threadId inView:(NSView *)inView localPoint:(CGPoint)localPoint nodeID:(NSString*)nodeID isFromPanel:(BOOL)isFromPanel;


+ (BOOL)isFileIntergationType:(ZMFileMenuType)type;

@end

