//
//  ZMSipCloudContactMgr.h
//  zSIPCallUI
//
//  Created by lisa on 2019/8/9.
//  Copyright © 2019 Evelyn. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
typedef enum : NSUInteger
{
    ZMSipCloudContactGoogleType,
    ZMSipCloudContactOutlookType,
    ZMSipCloudContactOffice365Type,
} ZMSipCloudContactType;

typedef enum : NSUInteger
{
    ZMSipCloudContactAddAction,
    ZMSipCloudContactCreateAction,
    ZMSipCloudContactNone,
} ZMSipCloudContactActionType;

typedef enum : NSUInteger
{
    ZMSipCloudContactServiceType,
    ZMSipPersonalContactServiceType,
} ZMSipContactServiceType;

typedef enum : NSUInteger
{
    ZMCloudContactSourceType_Phone,
    ZMCloudContactSourceType_Meeting,
} ZMCloudContactSourceType;

typedef NS_ENUM(NSUInteger, ZMSipCloudContactTextFieldTag)
{
    ZMSipCloudContactTextFieldNone,
    ZMSipCloudContactTextFieldEmail,
};

@interface ZMSipTextField : ZMTextField

@end

@interface ZMSipCloudContactEditView : ZMBaseView<NSTextFieldDelegate,ZMHMenuDelegate>

@property (nonatomic,strong) ZMHMenu *numberTypeList;
@property (nonatomic,strong) ZMHMenu *memberList;;
@property (nonatomic,strong) ZMHMenu *folderList;
@property (nonatomic,strong) NSMutableArray * folderArray;

@property (nonatomic,copy) NSString *tagID; //need to update info after create successful
@property (nonatomic,copy) NSString *number;

@property (nonatomic,strong) ZMSipTextField *firstNameField;
@property (nonatomic,strong) ZMSipTextField *lastNameField;
@property (nonatomic,strong) ZMSipTextField *emailField;
@property (nonatomic,strong) ZMSipTextField *companyField;
@property (nonatomic,strong) ZMSipTextField *contactNameField;
@property (nonatomic,strong) ZMSipTextField *phoneNumberField;
@property (nonatomic,strong) ZMSipTextField *clientCodeField;

@property (nonatomic,assign) ZMCloudContactSourceType sourceType;
@property (nonatomic, copy) void (^cancelButtonAction)(void);
@property (nonatomic, copy) void (^createButtonAction)(void);

@property (nonatomic, assign) BOOL titleSupportClicked;
@property (nonatomic, copy) void (^titleClickedAction)(void);

@property (nonatomic,copy) NSString *firstName;
@property (nonatomic,copy) NSString *lastName;

//phone
- (void)updateUIWithNumber:(NSString *)phoneNumber andType:(ZMSipCloudContactActionType)type serviceType:(ZMSipContactServiceType)serviceType;
- (void)updateUIWithNumber:(NSString *)number actionType:(ZMSipCloudContactActionType)actionType isSupportPersonal:(BOOL)isSupportPersonal isSupportCloud:(BOOL)isSupportCloud tag:(NSString *)tagId;

//meeting
- (void)updateUIForCreateContact;
- (void)updateUIForAddToContact;

- (void)updateWindowTitle:(NSString *)title confirmTitle:(NSString *)confirmTitle;
@end

