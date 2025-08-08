/**
 * @file ZoomSDKMeetingChatController.h
 * @brief Defines interfaces for file transfer and rich-text chat attributes in Zoom SDK.
 */


#import <Foundation/Foundation.h>
#import <ZoomSDK/ZoomSDKErrors.h>

@class ZoomSDKChatInfo;
NS_ASSUME_NONNULL_BEGIN

/**
 * @class ZoomSDKFileTransferInfo
 * @brief Provides information about the status and progress of a file transfer during a Zoom meeting.
 */
@interface ZoomSDKFileTransferInfo : NSObject
/**
 * @brief The message identify of transfer file.
 */
@property(nonatomic, copy, readonly) NSString *messageId;
/**
 * @brief The status of the file transfer.
 */
@property(nonatomic, assign, readonly) ZoomSDKFileTransferStatus transferStatus;
/**
 * @brief The time stamp of the file.
 */
@property(nonatomic, assign, readonly) time_t timeStamp;
/**
 * @brief Is the file send to all user in meeting?
 */
@property(nonatomic, assign, readonly) BOOL isSendToAll;
/**
 * @brief The bytes of transfer file size.
 */
@property(nonatomic, assign, readonly) unsigned int fileSizeBytes;
/**
 * @brief The file name of transfer file.
 */
@property(nonatomic, copy, readonly) NSString  *fileName;
/**
 * @brief The ratio of the file transfer completed.
 */
@property(nonatomic, assign, readonly) unsigned int completePercentage;
/**
 * @brief The size of the file transferred so far in bytes.
 */
@property(nonatomic, assign, readonly) unsigned int completeSize;
/**
 * @brief The speed of the file transfer in bits per second.
 */
@property(nonatomic, assign, readonly) unsigned int bitPerSecond;
@end


/**
 * @class ZoomSDKFileSender
 * @brief Provides control over file sending in a Zoom meeting chat.
 */
@interface ZoomSDKFileSender : NSObject
/**
 * @brief The basic information of transfer file.
 */
@property(nonatomic, retain, readonly) ZoomSDKFileTransferInfo *transferInfo;
/**
 * @brief Get file receiver's user ID.
 * @note 0 specify the file send to all.
 */
@property(nonatomic, assign, readonly) unsigned int receiverUserId;
/**
 * @brief Cancel the file send.
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)cancelSend;
@end


/**
 * @class ZoomSDKFileReceiver
 * @brief Provides control over receiving a file in a Zoom meeting chat.
 */
@interface ZoomSDKFileReceiver : NSObject
/**
 * @brief The basic information of transfer file.
 */
@property(nonatomic, retain, readonly) ZoomSDKFileTransferInfo *transferInfo;
/**
 * @brief Get file sender's user ID.
 */
@property(nonatomic, assign, readonly) unsigned int senderUserId;
/**
 * @brief Start receive the file.
 * @param path The path to receive the file. you need to pass the complete path, including the file name and file format.
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)startReceive:(NSString *)path;
/**
 * @brief Cancel the file receive.
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)cancelReceive;
@end


/**
 * @class ZoomSDKRichTextStyleOffset
 * @brief Describes the style range and additional metadata of a rich text segment.
 */
@interface ZoomSDKRichTextStyleOffset : NSObject
/**
 * @brief Get a certain rich-text style’s start position.
 */
@property(nonatomic,assign,readonly)unsigned int  positionStart;
/**
 * @brief Get the end position of a certain style in rich text.
 */
@property(nonatomic,assign,readonly)unsigned int  positionEnd;
/**
 * @brief Get a certain rich-text style supplementary information.
 * @note If the style is TextStyle_FontSize, possible return values are FontSize_Small, FontSize_Medium, or FontSize_Large. If style is TextStyle_Paragraph, possible return values are Paragraph_H1, Paragraph_H2, or Paragraph_H3. If the style is TextStyle_FontColor, or TextStyle_BackgroundColor, possible return values are hex string representing standard RGB data.
 */
@property(nonatomic,copy,readonly)NSString* reserve;
@end


/**
 * @class ZoomSDKRichTextStyleItem
 * @brief Represents a rich-text style and associated style ranges.
 */
@interface ZoomSDKRichTextStyleItem : NSObject
/**
 * @brief Get the rich text type of a portion of the current message.
 */
@property(nonatomic,assign,readonly)ZoomSDKRichTextStyle textStyle;
/**
 * @brief Get the current message’s rich text position info list of a certain style.
 * @return If the function succeeds, the return value is the current message’s rich text position info list of a certain style.
 */
- (NSArray<ZoomSDKRichTextStyleOffset *> *)getTextStyleOffsetList;
@end


/**
 * @class ZoomSDKChatMsgBoldAttrs
 * @brief Style attributes indicating text is bold.
 */
@interface ZoomSDKChatMsgBoldAttrs : NSObject
/**
 * @brief Set text style is Bold.
 */
@property(nonatomic,assign)BOOL bBold;
@end


/**
 * @class ZoomSDKChatMsgItalicAttrs
 * @brief Set italic style attributes for a chat message segment.
 */
@interface ZoomSDKChatMsgItalicAttrs : NSObject
/**
 * @brief Set text style is Italic.
 */
@property(nonatomic,assign)BOOL bItalic;
@end


/**
 * @class ZoomSDKChatMsgStrikethroughAttrs
 * @brief Set strikethrough style attributes for a chat message segment.
 */
@interface ZoomSDKChatMsgStrikethroughAttrs : NSObject
/**
 * @brief Set text style is Strikethrough.
 */
@property(nonatomic,assign)BOOL bStrikethrough;
@end


/**
 * @class ZoomSDKChatMsgBulletedListAttrs
 * @brief Set bulleted list style attributes for a chat message segment.
 */
@interface ZoomSDKChatMsgBulletedListAttrs : NSObject
/**
 * @brief Set text style is BulletedList.
 */
@property(nonatomic,assign)BOOL bBulletedList;
@end


/**
 * @class ZoomSDKChatMsgNumberedListAttrs
 * @brief Set numbered list style attributes for a chat message segment.
 */
@interface ZoomSDKChatMsgNumberedListAttrs : NSObject
/**
 * @brief Set text style is Numbered.
 */
@property(nonatomic,assign)BOOL bNumberedList;
@end


/**
 * @class ZoomSDKChatMsgUnderlineAttrs
 * @brief Set underline style attributes for a chat message segment.
 */
@interface ZoomSDKChatMsgUnderlineAttrs : NSObject
/**
 * @brief Set text style is Underline.
 */
@property(nonatomic,assign)BOOL bUnderline;
@end


/**
 * @class ZoomSDKChatMsgQuoteAttrs
 * @brief Set quote style attributes for a chat message segment.
 */
@interface ZoomSDKChatMsgQuoteAttrs : NSObject
/**
 * @brief Set text style is Quote.
 */
@property(nonatomic,assign)BOOL bQuote;
@end


/**
 * @class ZoomSDKChatMsgInsertLinkAttrs
 * @brief Set link attributes for a chat message segment.
 */
@interface ZoomSDKChatMsgInsertLinkAttrs : NSObject
/**
 * @brief if insertLinkUrl not empty, text style has insert link url.
 */
@property(nonatomic, copy, nullable)NSString* insertLinkUrl;
@end

#define FontSize_Small 8
#define FontSize_Medium 10
#define FontSize_Large 12

/**
 * @class ZoomSDKChatMsgFontSizeAttrs
 * @brief Set font size attributes for a chat message segment.
 */
@interface ZoomSDKChatMsgFontSizeAttrs : NSObject
/**
 * @brief Set font size value.
 */
@property(nonatomic,assign)int fontSize;
@end


/**
 * @class ZoomSDKChatMsgFontColorAttrs
 * @brief Set font color attributes for a chat message segment.
 * Currently supported font color combinations.
 * FontColor_Red, 235,24,7
 * FontColor_Orange, 255,138,0
 * FontColor_Yellow, 248,194,0
 * FontColor_Green, 19,138,0
 * FontColor_Blue, 0,111,250
 * FontColor_Violet, 152,70,255
 * FontColor_Rosered, 226,0,148
 * FontColor_Black 34,34,48
 */
@interface ZoomSDKChatMsgFontColorAttrs : NSObject
/**
 * @brief Font color R value.
 */
@property(nonatomic,assign)int red;
/**
 * @brief Font color G value.
 */
@property(nonatomic,assign)int green;
/**
 * @brief Font color B value.
 */
@property(nonatomic,assign)int blue;
@end


/**
 * @class ZoomSDKChatMsgBackgroundColorAttrs
 * @brief Set background color attributes for a chat message segment.
 * Currently supported background color combinations.
 * BackgroundColor_Normal, 255,255,255
 * BackgroundColor_Red, 255,67,67
 * BackgroundColor_Orange, 255,138,0
 * BackgroundColor_Yellow, 255,214,0
 * BackgroundColor_Green, 73,214,30
 * BackgroundColor_Blue, 47,139,255
 * BackgroundColor_Violet, 171,104,255
 * BackgroundColor_Rosered, 255,54,199
 * BackgroundColor_White 255,255,255
 */
@interface ZoomSDKChatMsgBackgroundColorAttrs : NSObject
/**
 * @brief Background color R value.
 */
@property(nonatomic,assign)int red;
/**
 * @brief Background color G value.
 */
@property(nonatomic,assign)int green;
/**
 * @brief Background color B value.
 */
@property(nonatomic,assign)int blue;
@end

#define TextStyle_Paragraph_H1        @"Paragraph_H1"
#define TextStyle_Paragraph_H2        @"Paragraph_H2"
#define TextStyle_Paragraph_H3        @"Paragraph_H3"

/**
 * @class ZoomSDKChatMsgParagraphAttrs
 * @brief Set paragraph style attributes for a chat message segment.
 */
@interface ZoomSDKChatMsgParagraphAttrs : NSObject
/**
 * @brief if strParagraph not empty, text style has Paragraph style.
 */
@property(nonatomic, copy, nullable)NSString* strParagraph;
@end

/**
 * @class ZoomSDKChatMsgIndentAttrs
 * @brief Set indent attributes for a chat message segment.
 */
@interface ZoomSDKChatMsgIndentAttrs : NSObject
/**
 * @brief The number of times the indentation style is applied.
 */
@property(nonatomic,assign)int indent;
@end


/**
 * @class ZoomSDKChatMsgSegmentDetails
 * @brief Define detailed attributes for a chat message segment.
 */
@interface ZoomSDKChatMsgSegmentDetails : NSObject
/**
 * @brief Segment content value.
 */
@property(nonatomic, copy, readonly, nullable)NSString* strContent;
/**
 * @brief Segment BoldAttrs value.
 */
@property(nonatomic, retain, readonly, nullable)ZoomSDKChatMsgBoldAttrs* boldAttrs;
/**
 * @brief Segment ItalicAttrs value.
 */
@property(nonatomic, retain, readonly, nullable)ZoomSDKChatMsgItalicAttrs* italicAttrs;
/**
 * @brief StrikethroughAttrs content value.
 */
@property(nonatomic, retain, readonly, nullable)ZoomSDKChatMsgStrikethroughAttrs* strikethroughAttrs;
/**
 * @brief Segment BulletedListAttrs value.
 */
@property(nonatomic, retain, readonly, nullable)ZoomSDKChatMsgBulletedListAttrs* bulletedListAttrs;
/**
 * @brief Segment NumberedListAttrs value.
 */
@property(nonatomic, retain, readonly, nullable)ZoomSDKChatMsgNumberedListAttrs* numberedListAttrs;
/**
 * @brief Segment UnderlineAttrs value.
 */
@property(nonatomic, retain, readonly, nullable)ZoomSDKChatMsgUnderlineAttrs* underlineAttrs;
/**
 * @brief Segment QuoteAttrs value.
 */
@property(nonatomic, retain, readonly, nullable)ZoomSDKChatMsgQuoteAttrs* quoteAttrs;
/**
 * @brief Segment InsertLinkAttrs value.
 */
@property(nonatomic, retain, readonly, nullable)ZoomSDKChatMsgInsertLinkAttrs* insertLinkAttrs;
/**
 * @brief Segment FontSizeAttrs value.
 */
@property(nonatomic, retain, readonly, nullable)ZoomSDKChatMsgFontSizeAttrs* fontSizeAttrs;
/**
 * @brief Segment FontColorAttrs value.
 */
@property(nonatomic, retain, readonly, nullable)ZoomSDKChatMsgFontColorAttrs* fontColorAttrs;
/**
 * @brief Segment BackgroundColorAttrs value.
 */
@property(nonatomic, retain, readonly, nullable)ZoomSDKChatMsgBackgroundColorAttrs* backgroundColorAttrs;
/**
 * @brief Segment ParagraphAttrs value.
 */
@property(nonatomic, retain, readonly, nullable)ZoomSDKChatMsgParagraphAttrs* paragraphAttrs;
/**
 * @brief Segment IndentAttrs value.
 */
@property(nonatomic, retain, readonly, nullable)ZoomSDKChatMsgIndentAttrs* indentAttrs;
@end


/**
 * @class ZoomSDKChatMsgInfoBuilder
 * @brief Used to build a ZoomSDK chat message with various content and styles.
 */
@interface ZoomSDKChatMsgInfoBuilder : NSObject
/**
 * @brief Set chat message content.
 * @param content The chat message’s content.
 * @return If the function succeeds, it will return object ZoomSDKChatMsgInfoBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)setContent:(NSString*)content;
/**
 * @brief Set  who will receive the chat message.
 * @param receiver Specify the user ID to receive the chat message. The message is sent to all participants when the value is zero(0).
 * @return If the function succeeds, it will return object ZoomSDKChatMsgInfoBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)setReceiver:(unsigned int)receiver;
/**
 * @brief Set  the ID of the thread where the message will be posted.
 * @param threadId  Specify the thread ID.
 * @return If the function succeeds, it will return object ZoomSDKChatMsgInfoBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)setThreadId:(NSString*)threadId;
/**
 * @brief Set the chat message type.
 * @param type The chat message’s type.
 * @return If the function succeeds, it will return object ZoomSDKChatMsgInfoBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)setMessageType:(ZoomSDKChatMessageType)type;
/**
 * @brief Set the chat message content quote style and position.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMsgInfoBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)setQuotePosition:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;
/**
 * @brief Clear  the chat message content quote style.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMsgInfoBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)unsetQuotePosition:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;
/**
 * @brief Set the chat message content insert link style.
 * @param insertLink The segment link url.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMsgInfoBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)setInsertLink:(ZoomSDKChatMsgInsertLinkAttrs*)insertLink positionStart:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;
/**
 * @brief Clear the chat message content insert link style.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMsgInfoBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)unsetInsertLink:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;
/**
 * @brief Set the chat message content font size style.
 * @param fontSize The segment font size.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMsgInfoBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)setFontSize:(ZoomSDKChatMsgFontSizeAttrs*)fontSize positionStart:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;
/**
 * @brief Clear the chat message content font size style.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMsgInfoBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)unsetFontSize:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;
/**
 * @brief Set the chat message content italic style.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMsgInfoBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)setItalic:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;
/**
 * @brief Clear the chat message content italic style.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMsgInfoBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)unsetItalic:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;
/**
 * @brief Set the chat message content bold style.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMsgInfoBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)setBold:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;
/**
 * @brief Clear the chat message content bold style.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMsgInfoBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)unsetBold:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;
/**
 * @brief Set the chat message content strikethrough style.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMsgInfoBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)setStrikethrough:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;
/**
 * @brief Clear the chat message content strikethrough style.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMsgInfoBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)unsetStrikethrough:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;
/**
 * @brief Set the chat message content bulleted list style.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMsgInfoBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)setBulletedList:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;
/**
 * @brief Clear the chat message content bulleted list style.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMsgInfoBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)unsetBulletedList:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;
/**
 * @brief Set the chat message content numbered list style.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMsgInfoBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)setNumberedList:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;
/**
 * @brief Unset the chat message content numbered list style.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMsgInfoBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)unsetNumberedList:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;
/**
 * @brief Set the chat message content under line style.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMsgInfoBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)setUnderline:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;
/**
 * @brief Clear the chat message content under line style.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMsgInfoBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)unsetUnderline:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;
/**
 * @brief Set the chat message content font color style.
 * @param color  The segment color value.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMessageBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)setFontColor:(ZoomSDKChatMsgFontColorAttrs*)color positionStart:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;
/**
 * @brief Clear the chat message content font color style.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMessageBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)unsetFontColor:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;
/**
 * @brief Set the chat message content background color style.
 * @param color The segment color value.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMessageBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)setBackgroundColor:(ZoomSDKChatMsgBackgroundColorAttrs*)color positionStart:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;
/**
 * @brief Clear the chat message content background color style.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMessageBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)unsetBackgroundColor:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;
/**
 * @brief Increase the chat message content indent style.
 * @param indent The segment indent value.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMessageBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)increaseIndent:(ZoomSDKChatMsgIndentAttrs*)indent positionStart:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;
/**
 * @brief Decrease the chat message content indent style.
 * @param indent The segment indent value.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMessageBuilder.
 */
- (ZoomSDKChatMsgInfoBuilder*)decreaseIndent:(ZoomSDKChatMsgIndentAttrs*)indent positionStart:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;
/**
 * @brief Set the chat message content paragraph style.
 * @param paragraph The segment paragraph value.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMessageBuilder.
 * @note if paragraph.strParagraph is one of the three contents, bold and font size styles will be added internally.
 * Paragraph_H1 with bold and FontSize_Large
 * Paragraph_H2 with bold and FontSize_Medium
 * Paragraph_H3 with bold and FontSize_Small
 */
- (ZoomSDKChatMsgInfoBuilder*)setParagraph:(ZoomSDKChatMsgParagraphAttrs*)paragraph positionStart:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;
/**
 * @brief Clear the chat message content paragraph style.
 * @param positionStart The segment start position.
 * @param positionEnd  The segment end position.
 * @return If the function succeeds, it will return object ZoomSDKChatMessageBuilder.
 * @note if paragraph.strParagraph is one of Paragraph_H1, Paragraph_H2, Paragraph_H3. bold and font size styles will be removed internally.
 */
- (ZoomSDKChatMsgInfoBuilder*)unsetParagraph:(unsigned int)positionStart positionEnd:(unsigned int)positionEnd;

/**
 * @brief Build chat message entity.
 * @return If the function succeeds, the return value is message detail info.
 */
- (ZoomSDKChatInfo*)build;
@end


/**
 * @protocol ZoomSDKMeetingChatControllerDelegate
 * @brief Callback events for in-meeting chat and file transfer activities.
 */
@protocol ZoomSDKMeetingChatControllerDelegate <NSObject>
/**
 * @brief In-meeting users receive the notification of chat messages.
 * @param chatInfo Chat information received by user.
 */
- (void)onChatMessageNotification:(ZoomSDKChatInfo*)chatInfo;

/**
 * @brief Sink the event that a chat message is edited.
 * @param chatInfo The chat message object.
 */
- (void)onChatMessageEditNotification:(ZoomSDKChatInfo*)chatInfo;

/**
 * @brief Invoked when start send file.
 * @param sender The class to sendfile object.
 */
- (void)onFileSendStart:(ZoomSDKFileSender *)sender;
/**
 * @brief Invoked when receiving a file from another user.
 * @param receiver The class to receive file object.
 */
- (void)onFileReceived:(ZoomSDKFileReceiver *)receiver;
/**
 * @brief Invoked when send or receive file status change.
 * @param info The class to basic transfer information.
 */
- (void)onFileTransferProgress:(ZoomSDKFileTransferInfo *)info;
@end


/**
 * @class ZoomSDKMeetingChatController
 * @brief Provide APIs to manage in-meeting chat messages and file transfers.
 */
@interface ZoomSDKMeetingChatController : NSObject
/**
 * @brief Set the delegate to receive chat and file transfer events.
 */
@property(nonatomic,assign,nullable) id<ZoomSDKMeetingChatControllerDelegate> delegate;
/**
 * @brief Get chat message by message ID.
 * @param messageID The message ID.
 * @return If the function succeeds, it will return object ZoomSDKChatInfo.
 */
- (ZoomSDKChatInfo*_Nullable)getChatMessageById:(NSString *)messageID;

/**
 * @brief Send a chat message.
 * @param message The message info.
 * @return If the function succeeds, it will return ZoomSDKError_Success.
 */
- (ZoomSDKError)sendChatMsgTo:(ZoomSDKChatInfo*)message;

/**
 * @brief Determine whether file transfer is enabled.
 * @return True if file transfer is enabled, otherwise false.
 */
- (BOOL)isFileTransferEnabled;

/**
 * @brief Send file to specify user in current meeting.
 * @param filePath The absolute path of the file.
 * @param userId Send the file to this user.
 * @return If the function succeeds, the return value is @c ZoomSDKError_Success.
 * @warning This interface related chat  privilege. See \link ZoomSDKChatPrivilegeType \endlink.
 */
- (ZoomSDKError)transferFile:(NSString *)filePath toUser:(unsigned int)userId;

/**
 * @brief Send file to all users in current meeting.
 * @param filePath The absolute path of the file.
 * @return If the function succeeds, the return value is @c ZoomSDKError_Success.
 * @warning This interface related chat  privilege. See \link ZoomSDKChatPrivilegeType \endlink.
 */
- (ZoomSDKError)transferFileToAll:(NSString *)filePath;

/**
 * @brief Get the list of allowed file types in transfer.
 * @return The value of allowed file types in transfer, comma-separated if there are multiple values. Exe files are, by default, forbidden from being transferred.
 */
- (NSString *_Nullable)getTransferFileTypeAllowList;

/**
 * @brief Get the maximum size for file transfer.
 * @return The maximum number of bytes for file transfer.
 */
- (unsigned long long)getMaxTransferFileSizeBytes;
@end

NS_ASSUME_NONNULL_END
