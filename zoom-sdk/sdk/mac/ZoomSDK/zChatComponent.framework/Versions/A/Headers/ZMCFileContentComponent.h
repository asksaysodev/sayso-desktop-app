//
//  ZMCFileContentComponent.h
//  zChatComponent
//
//  Created by Huxley Yang on 1/2/25.
//

#import <zChatComponent/ZMCBaseComponent.h>
#include <zPTApp/SaasBeePTAppInterface.h>

NS_ASSUME_NONNULL_BEGIN

@protocol ZMCFileContentAPI <NSObject> //!FileContentMgr
@optional

///  Check share scope(anyone, same org, same account, contains external white and black lists
/// - Parameter contactJid:must be buddy jid.
- (BOOL)isFileTransferAllowed:(NSString *)contactJid;

@end

@protocol ZMCFileContentSink <NSObject> //!FileContentSink
@optional

// All external permission setting is synced with backend
- (void)indicateExternalFilePermissionSettingReady;

@end

@protocol ZMCFileContentUI <NSObject>
@optional

@end

@class ZMCFileContentAPI, ZMCFileContentSink, ZMCFileContentUI;
@interface ZMCFileContentComponent : ZMCBaseComponent <ZMCFileContentAPI, ZMCFileContentUI>

@property (nullable, strong, readonly) ZMCFileContentAPI *api;
@property (nullable, strong, readonly) ZMCFileContentSink *sink;
@property (nullable, strong, readonly) ZMCFileContentUI *UIProvider;

- (void)addSinkTarget:(id <ZMCFileContentSink>)target;
@property (readonly, nonatomic) NSArray <ZMCFileContentSink>*allTargets;

- (NS_ZOOM_MESSAGER::FileContentMgr *)pFileContentMgr;

@end

@interface ZMCFileContentComponentImp : ZMCBaseComponentImp

@property (weak, readonly, nonatomic, nullable) ZMCFileContentComponent *component;

@end

NS_ASSUME_NONNULL_END
