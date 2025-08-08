//
//  IZMSwitchAccountMgr.h
//  ChatUI
//
//  Created by davies zhao on 2023/1/31.
//  Copyright © 2023 Zoom. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <zPTApp/SaasBeePTAppConstants.h>

NS_ASSUME_NONNULL_BEGIN
@class ZMSwitchAccountCellModel;
@protocol ZMSwitchAccountModelProtocol;

@protocol IZMSwitchAccountMgr <ZMRoutableObject>
@optional
- (BOOL)isMultipleAccountsSwitchDisabled;
- (BOOL)shouldHideSwitchAccountAndSignOut;
- (NSArray<ZMSwitchAccountCellModel *> *)getAllUserAccounts;
- (SSBPTERROR)switchAccount:(ZMSwitchAccountCellModel *)model;
- (BOOL)removeUserAccount:(ZMSwitchAccountCellModel *)model;

- (SSBPTERROR)switchAccountWithModel:(id<ZMSwitchAccountModelProtocol>)model;
- (BOOL)removeUserAccountWithModel:(id<ZMSwitchAccountModelProtocol>)model;

- (BOOL)signOutAllAccounts;
- (void)addAccount;

- (void)showRemoveAccountDialog:(ZMSwitchAccountCellModel *)model;
// SystemMenu
- (void)updeteSwitchAccountOrSignOutSystemMenu:(NSMenuItem *)menuItem isSwitchAccount:(BOOL)isSwitchAccount;
// ProfileMenu
- (nullable ZMHMenuItem *)generateSwitchAccountOrSignOutMenu:(BOOL)isSwitchAccount;

- (void)showAddAccountLimitDialog;
- (void)showSignOutAllAccountsDialog;
- (void)didLogout;
@end

@protocol IZMSwitchAccountUpdateProtocol <NSObject>
@optional
- (void)switchAccountListUpdate;
- (void)mutipleSwitchAccountNotify:(SSBPTERROR)result account:(ZMSwitchAccountCellModel *)model;
@end

NS_ASSUME_NONNULL_END
