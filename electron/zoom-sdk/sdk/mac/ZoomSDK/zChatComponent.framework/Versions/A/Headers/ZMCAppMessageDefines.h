//
//  ZMCAppMessageDefines.h
//  zChatComponent
//
//  Created by Cyan.Yang on 2023/6/28.
//

typedef NS_OPTIONS(NSUInteger, ZMCommonAppBodyType)
{
    ZMCommonAppBodyType_head = 0,
    ZMCommonAppBodyType_subhead,
    ZMCommonAppBodyType_message,
    ZMCommonAppBodyType_field,
    ZMCommonAppBodyType_fields,
    ZMCommonAppBodyType_fieldKey,
    ZMCommonAppBodyType_attachmentFile,
    ZMCommonAppBodyType_attachment,
    ZMCommonAppBodyType_attachments,
    ZMCommonAppBodyType_action,
    ZMCommonAppBodyType_actions,
    ZMCommonAppBodyType_selectTitle,
    ZMCommonAppBodyType_select,
    ZMCommonAppBodyType_templateFile,
    ZMCommonAppBodyType_datePicker,
    ZMCommonAppBodyType_timePicker,
    ZMCommonAppBodyType_radioButton,
    ZMCommonAppBodyType_progressBar,
    ZMCommonAppBodyType_divider,
    ZMCommonAppBodyType_checkboxes,
    ZMCommonAppBodyType_channel,
    ZMCommonAppBodyType_alert,
    ZMCommonAppBodyType_imgs,
    ZMCommonAppBodyType_textArea,
    ZMCommonAppBodyType_meetingCard,
    ZMCommonAppBodyType_inlineVideo,
        
    // NOTE: New types should only be added above this line.
    ZMCommonAppBodyType_footer = 1000,
    ZMCommonAppBodyType_subbody,
    ZMCommonAppBodyType_inlineMessage,
    ZMCommonAppBodyType_other,
};

typedef NS_ENUM(NSUInteger, ZMCommonAppControlAlignment) {
    ZMCommonAppControlAlignmentLeft = 0,
    ZMCommonAppControlAlignmentRight,
};

typedef NS_ENUM(NSUInteger, ZMCommonAppActionButtonStyle)
{
    ZMCommonAppActionButtonStyle_Default = 0,
    ZMCommonAppActionButtonStyle_Primary,
    ZMCommonAppActionButtonStyle_Danger,
    ZMCommonAppActionButtonStyle_Disable,
    ZMCommonAppActionButtonStyle_ThumbsUp,
    ZMCommonAppActionButtonStyle_ThumbsDown,
    ZMCommonAppActionButtonStyle_ThumbsUpClicked,
    ZMCommonAppActionButtonStyle_ThumbsDownClicked,
    ZMCommonAppActionButtonStyle_Message,
    ZMCommonAppActionButtonStyle_Hyperlink,
    ZMCommonAppActionButtonStyle_HoverAction,
    
    // Alias values for some purposes. PLEASE DO NOT ADD NEW
    // CASES BELOW THIS LINE!
    ZMCommonAppActionButtonStyle_FirstThumbStyle = ZMCommonAppActionButtonStyle_ThumbsUp,
    ZMCommonAppActionButtonStyle_LastThumbStyle = ZMCommonAppActionButtonStyle_ThumbsDownClicked,
};

typedef NS_OPTIONS(NSUInteger, ZMPTCommonAppButtonCellStatus)
{
    ZMPTCommonAppButtonCellStatus_normal,
    ZMPTCommonAppButtonCellStatus_hover,
    ZMPTCommonAppButtonCellStatus_press,
};
