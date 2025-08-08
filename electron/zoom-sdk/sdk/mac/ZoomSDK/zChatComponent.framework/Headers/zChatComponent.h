//
//  zChatComponent.h
//  zChatComponent
//
//  Created by Kevin Li on 2022/7/6.
//

#import <Foundation/Foundation.h>

//! Project version number for zChatComponent.
FOUNDATION_EXPORT double zChatComponentVersionNumber;

//! Project version string for zChatComponent.
FOUNDATION_EXPORT const unsigned char zChatComponentVersionString[];

// In this header, you should import all the public headers of your framework using statements like #import <zChatComponent/PublicHeader.h>

//Base
#import <zChatComponent/ZMIMCommonDefine.h>
#import <zChatComponent/ZMRoomCalloutDevice.h>
#import <zChatComponent/ZMMentionGroupSingleton.h>
#import <zChatComponent/ZMCommonUtil.h>
#import <zChatComponent/ZMCommonAppData.h>
#import <zChatComponent/ZMChatAppsEntity.h>

#import <zChatComponent/ZMChatComponentManager.h>

//ZMFile
#import <zChatComponent/ZMFileEntity.h>
#import <zChatComponent/ZMFileModule.h>
#import <zChatComponent/ZMFileOperateProtocal.h>
#import <zChatComponent/ZMImageOperateModule.h>
#import <zChatComponent/ZMChatFileItemSwiftAdaptor.h>
#import <zChatComponent/ZMMessageFileHelper.h>

//preview
#import <zChatComponent/ZMVideoPreViewItem.h>
#import <zChatComponent/ZMPreviewVideoItemView.h>
#import <zChatComponent/ZMFilePreViewItem.h>
#import <zChatComponent/ZMFilePreviewManager.h>
#import <zChatComponent/ZMImagePreViewItem.h>
#import <zChatComponent/ZMPreviewBaseItemView.h>
#import <zChatComponent/ZMPreviewContentView.h>
#import <zChatComponent/ZMPreviewImageItemView.h>
#import <zChatComponent/ZMChatFilePreviewManager.h>

// Session
#import <zChatComponent/ZMSessionGroupSwiftAdaptor.h>

// Buddy
#import <zChatComponent/ZMBuddySwiftAdaptor.h>

//ZMMessage
#import <zChatComponent/ZMChatMsgItemInfo.h>
#import <zChatComponent/ZMMessageSwiftAdaptor.h>
#import <zChatComponent/ZMPTGiphyData.h>
#import <zChatComponent/ZMPinMessageInfo.h>
#import <zChatComponent/ZMReactionEmojiInfo.h>
#import <zChatComponent/ZMScheduleMeetingInfo.h>
#import <zChatComponent/ZMMessageAPIHelperSwiftAdaptor.h>
#import <zChatComponent/ZMMessageMentionInfo.h>
#import <zChatComponent/ZMCTemplateCommandAPI.h>

//ZMMessage-cellview
#import <zChatComponent/ZMDLPPolicy.h>
#import <zChatComponent/ZMMessageNameCache.h>
#import <zChatComponent/ZMIMMessageLayoutManager.h>
#import <zChatComponent/ZMFileMessageCellView.h>
#import <zChatComponent/ZMVoiceMessageCellView.h>
#import <zChatComponent/ZMAppMessageCellView.h>
#import <zChatComponent/ZMIMMessageLayoutManager.h>
#import <zChatComponent/ZMIMVideoMessageCellView.h>

//ZMMessage View
#import <zChatComponent/ZMBaseMessageCellView.h>
#import <zChatComponent/ZMBubbleMessageCellView.h>
#import <zChatComponent/ZMNTextMessageCellView.h>
#import <zChatComponent/ZMPromptMessageCellView.h>
#import <zChatComponent/ZMImageMessageCellView.h>
#import <zChatComponent/ZMGiphyMessageCellView.h>
#import <zChatComponent/ZMCodeMessageCellView.h>
#import <zChatComponent/ZMMessageAccessoryView.h>
#import <zChatComponent/ZMMessageBubbleView.h>
#import <zChatComponent/ZMMessageCellViewDelegate.h>
#import <zChatComponent/ZMMessageCellViewFactory.h>
#import <zChatComponent/ZMIMMessageLayoutManager.h>
#import <zChatComponent/ZMChatMsgItemTextView.h>
#import <zChatComponent/ZMReactionItemView.h>
#import <zChatComponent/ZMAvatarView.h>
#import <zChatComponent/ZMIMAvatarView.h>
#import <zChatComponent/ZMViewAttachmentCellProtocol.h>
#import <zChatComponent/ZMScheduleMeetingCardView.h>

// App Card
#import <zChatComponent/ZMPTCommonAppTextAreaCell.h>
#import <zChatComponent/ZMPTCommonAppVideoCell.h>
#import <zChatComponent/ZMPTCommonAppMeetingCardHelper.h>

//Emoji
#import <zChatComponent/ZMEachReactionStatisticsInfo.h>
#import <zChatComponent/ZMEmojiUnicodeData.h>
#import <zChatComponent/ZMReactionInfo.h>
#import <zChatComponent/ZMSysEmojiItemView.h>
#import <zChatComponent/ZMSysEmojiToolbarView.h>
#import <zChatComponent/ZMSysEmojCollectionView.h>
#import <zChatComponent/ZMSysEmojiView.h>
#import <zChatComponent/ZMSysEmojiMgr.h>
#import <zChatComponent/ZMCustomEmojiManager.h>
#import <zChatComponent/ZMViewPresentationAttachmentCell.h>

//ZMVideoPlayer
#import <zChatComponent/ZMVideoPlayerView.h>
#import <zChatComponent/ZMVideoPlayerControlView.h>
#import <zChatComponent/ZMCBotInlineVideoPreviewHelper.h>

//input
#import <zChatComponent/ZMChatsInputFileView.h>
#import <zChatComponent/ZMInputTextView.h>
#import <zChatComponent/ZMMessageBodyImageView.h>
#import <zChatComponent/ZMChatMessageCheckerUtil.h>
#import <zChatComponent/ZMChatFeatureOptionsUtil.h>

// RTF
#import <zChatComponent/ZMRichFormatConstants.h>
#import <zChatComponent/ZMRTFShortcutWindowCtrl.h>
#import <zChatComponent/ZMRichFormatTextView.h>
#import <zChatComponent/ZMTextFormatEntity.h>
#import <zChatComponent/ZMTextFormatLayoutManager.h>
#import <zChatComponent/ZMRichFormatParagraphStyle.h>
#import <zChatComponent/NSMutableAttributedString+RichFormat.h>

// AIC
#import <zChatComponent/ZMChatAIModule.h>
#import <zChatComponent/ZMChatAIMenuItem.h>
#import <zChatComponent/ZMChatAIProtocol.h>

//Sharepoint
#import <zChatComponent/ZMIntegrationModule.h>

//Reminder Messages
#import <zChatComponent/ZMReminderMessageInfo.h>
#import <zChatComponent/ZMSipNumberFormat.h>

#import <zChatComponent/ZMGroupInviteConsentManager.h>
#import <zChatComponent/ZMChatTextScrollView.h>
#import <zChatComponent/ZMGridView.h>

#import <zChatComponent/ZMMemberInformationProtocol.h>
#import <zChatComponent/ZMSharedSpaceProtocol.h>
#import <zChatComponent/ZMChannelPreviewController.h>

#import <zChatComponent/ZMChannelModule.h>
#import <zChatComponent/ZMTranslationManagerProtocol.h>
#import <zChatComponent/ZMMessageUIMgrProtocol.h>

#import <zChatComponent/ZMUserModule.h>
#import <zChatComponent/ZMSessionModule.h>
#import <zChatComponent/ZMPhoneCallHelper.h>
#import <zChatComponent/ZMChatAppsEntity.h>
#import <zChatComponent/ZMIMAvatarHelper.h>
#import <zChatComponent/ZMGroupAvatarHelper.h>

#import <zChatComponent/ZMMessageDraftHelper.h>
#import <ZoomUnit/ZPPTSinkEventHandler.h>

#import <zChatComponent/ZMMessageFontInfo.h>
#import <zChatComponent/ZMMessageMentionInfo.h>
#import <zChatComponent/ZMShowCalendarEventParams.h>

#import <zChatComponent/ZMJumpHelper.h>

#import <zChatComponent/ZMChatsSessionViewProxy.h>

// time zone picker view
#import <zChatComponent/ZMTimeZonePicker.h>

#import <zChatComponent/ZMOPConfigHelper.h>
//Other
#import <zChatComponent/ZMCSearchTextField.h>

// Private
#import <zChatComponent/NSAttributedString+ZoomMsgUI-Swift.h>
#import <zChatComponent/NSView+Toast.h>
