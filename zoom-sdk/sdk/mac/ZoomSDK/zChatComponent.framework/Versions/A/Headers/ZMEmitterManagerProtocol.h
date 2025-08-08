//
//  ZMEmitterManagerProtocol.h
//  zChatComponent
//
//  Created by Kevin Li on 2022/7/18.
//

#ifndef ZMEmitterManagerProtocol_h
#define ZMEmitterManagerProtocol_h

@interface ZMEmitterCellEntity : NSObject{
    CGImageRef _imageRef;
}

@property (nonatomic,copy) NSString *emoji;
@property (nonatomic,assign) float birthRate;
@property (nonatomic,assign) float spinRange;
@property (nonatomic,assign) float scale;
@end

@protocol ZMEmitterManagerProtocol <NSObject>

- (BOOL)isSupportEmitter;
- (void)tryStopEmitterWithView:(NSView*)onView sessionId:(NSString*)sessionId;
/// Rains emojis in the specified view IFF the string matches one of the global emoji-rain configurations.
- (void)checkShowEmitterWithString:(NSString*)string onView:(NSView*)onView sessionId:(NSString*)sessionId;
- (BOOL)isHitEmitterString:(NSString*)string;

- (BOOL)recordHitEmitterWithSessionId:(NSString*)sessionId msgId:(NSString*)msgId msgBody:(NSString*)msgBody svrSideTime:(NSInteger)svrSideTime;
- (NSArray<NSString*>*)hitEmitterMsgIdArrayWith:(NSString*)sessionId;
- (BOOL)removeHitEmitterWithSessionId:(NSString*)sessionId;
- (void)removeAllHitEmitter;
- (BOOL)removeHitEmitterWithSessionId:(NSString*)sessionId msgId:(NSString*)msgId;
- (NSString*)copyEmitterStringWithSessionId:(NSString*)sessionId;

/// Rains the specified emoji in the specified view.
- (void)showWithEmitterCellEntity:(ZMEmitterCellEntity *)cellEntity onView:(NSView*)onView;
@end

#ifndef ZMShareEmitterManagerProtocol
#define ZMShareEmitterManagerProtocol (id<ZMEmitterManagerProtocol>)ZMSharedFor(ZMEmitterManagerProtocol)
#endif

#endif /* ZMEmitterManagerProtocol_h */
