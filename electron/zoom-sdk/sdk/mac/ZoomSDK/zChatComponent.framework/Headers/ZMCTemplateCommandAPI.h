//
//  ZMCTemplateCommandAPI.h
//  zChatComponent
//
//  Created by Cyan.Yang on 2024/12/20.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMCommonAppSelectItemData;
@class ZMPTCommonAppTextAreaCxxItemWrapper;
@class ZMCTemplateCheckboxItem;

NS_SWIFT_NAME(TemplateCommandParameters)
@interface ZMCTemplateCommandParameters : NSObject

@property (nonatomic, copy) NSString *sessionID;
@property (nonatomic, copy) NSString *messageID;
@property (nonatomic, assign) NSInteger index;

@property (nonatomic, nullable, copy) NSString *actionID;
@property (nonatomic, nullable, copy) NSString *eventID;

- (instancetype)initWithSessionID:(NSString *)sessionID
                        messageID:(NSString *)messageID
                            index:(NSInteger)index
                         actionID:(NSString * _Nullable)actionID
                          eventID:(NSString * _Nullable)eventID;

@end

NS_SWIFT_NAME(TemplateButtonCommandParameters)
@interface ZMCTemplateButtonCommandParameters : ZMCTemplateCommandParameters

@property (nonatomic, nullable, copy) NSString *text;
@property (nonatomic, nullable, copy) NSString *value;
@property (nonatomic, nullable, copy) NSString *action;

@property (nonatomic, assign) BOOL submitButton;

@end

NS_SWIFT_NAME(TemplateSelectCommandParameters)
@interface ZMCTemplateSelectCommandParameters : ZMCTemplateCommandParameters

@property (nonatomic, nullable, copy) NSArray<ZMCommonAppSelectItemData *> *selectItems;

@end

NS_SWIFT_NAME(TemplateRadioButtonCommandParameters)
@interface ZMCTemplateRadioButtonCommandParameters : ZMCTemplateCommandParameters

@property (nonatomic, nullable, copy) NSString *text;
@property (nonatomic, nullable, copy) NSString *value;

@end

NS_SWIFT_NAME(TemplateCheckboxCommandParameters)
@interface ZMCTemplateCheckboxCommandParameters : ZMCTemplateCommandParameters

@property (nonatomic, nullable, copy) NSArray<ZMCTemplateCheckboxItem *> *checkedItems;

@end

NS_SWIFT_NAME(TemplateDateTimePickerCommandParameters)
@interface ZMCTemplateDateTimePickerCommandParameters : ZMCTemplateCommandParameters

@property (nonatomic, nullable, copy) NSDateComponents *dateComponents;

@end

NS_SWIFT_NAME(TemplateTextAreaCommandParameters)
@interface ZMCTemplateTextAreaCommandParameters : ZMCTemplateCommandParameters

@property (nonatomic, nullable, strong) ZMPTCommonAppTextAreaCxxItemWrapper *item;
@property (nonatomic, nullable, copy) NSString *value;

@end

NS_SWIFT_NAME(TemplateEditCommandParameters)
@interface ZMCTemplateEditCommandParameters : ZMCTemplateCommandParameters

@property (nonatomic, nullable, copy) NSString *oldValue;
@property (nonatomic, nullable, copy, getter=newValue) NSString *newValue;

- (NSString *)newValue __attribute__((objc_method_family(none)));

@end

NS_SWIFT_NAME(TemplateFieldsEditCommandParameters)
@interface ZMCTemplateFieldsEditCommandParameters : ZMCTemplateEditCommandParameters

@property (nonatomic, nullable, copy) NSString *key;

@end

NS_SWIFT_NAME(TemplateAlertCloseCommandParameters)
@interface ZMCTemplateAlertCloseCommandParameters : ZMCTemplateCommandParameters
@end

#pragma mark - Supporting Types

NS_SWIFT_NAME(TemplateCheckboxItem)
@interface ZMCTemplateCheckboxItem : NSObject

@property (nonatomic, nullable, copy) NSString *text;
@property (nonatomic, nullable, copy) NSString *value;

@end

#pragma mark -

NS_SWIFT_NAME(TemplateCommandAPI)
@interface ZMCTemplateCommandAPI : NSObject

+ (BOOL)sendButtonCommand:(ZMCTemplateButtonCommandParameters *)parameters;
+ (BOOL)sendSelectCommand:(ZMCTemplateSelectCommandParameters *)parameters;
+ (BOOL)sendRadioButtonCommand:(ZMCTemplateRadioButtonCommandParameters *)parameters;
+ (BOOL)sendCheckboxCommand:(ZMCTemplateCheckboxCommandParameters *)parameters;
+ (BOOL)sendTimePickerCommand:(ZMCTemplateDateTimePickerCommandParameters *)parameters;
+ (BOOL)sendDatePickerCommand:(ZMCTemplateDateTimePickerCommandParameters *)parameters;
+ (BOOL)sendTextAreaCommand:(ZMCTemplateTextAreaCommandParameters *)parameters;
+ (BOOL)sendEditCommand:(ZMCTemplateEditCommandParameters *)parameters;
+ (BOOL)sendFieldsEditCommand:(ZMCTemplateFieldsEditCommandParameters *)parameters;
+ (BOOL)sendAlertCloseCommand:(ZMCTemplateAlertCloseCommandParameters *)parameters;

+ (BOOL)sendAppBridgeAction:(NSString *)action
                    payload:(NSString *)payload
                  sessionID:(NSString *)sessionID;

@end

NS_ASSUME_NONNULL_END
