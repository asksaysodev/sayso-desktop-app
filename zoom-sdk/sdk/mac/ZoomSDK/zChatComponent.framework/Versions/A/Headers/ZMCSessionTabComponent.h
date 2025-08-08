//
//  ZMCSessionTabComponent.h
//  zChatComponent
//
//  Created by Huxley Yang on 12/25/24.
//

#import <zChatComponent/ZMCBaseComponent.h>
#import <zChatComponent/ZMCSessionTab.h>

NS_ASSUME_NONNULL_BEGIN

@class ZMCSessionTabAPI, ZMCSessionTabSink, ZMCSessionTabUI;
@interface ZMCSessionTabComponent : ZMCBaseComponent <ZMCSessionTabAPI, ZMCSessionTabUI>

@property (nullable, strong, readonly) ZMCSessionTabAPI *api;
@property (nullable, strong, readonly) ZMCSessionTabSink *sink;
@property (nullable, strong, readonly) ZMCSessionTabUI *UI;

- (void)addSinkTarget:(id <ZMCSessionTabSink>)target;
@property (readonly, nonatomic) NSArray <ZMCSessionTabSink>*allTargets;

- (ns_zoom_messager::IZoomSessionTabs *)pSessionTab;

@end

@interface ZMCSessionTabComponentImp : ZMCBaseComponentImp

@property (weak, readonly, nonatomic, nullable) ZMCSessionTabComponent *component;

@end

NS_ASSUME_NONNULL_END
