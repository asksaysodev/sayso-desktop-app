//
//  ZMPerformanceTrackerProtocol.h
//  ChatUI
//
//  Created by Mario Rao on 2022/7/11.
//  Copyright © 2022 Zoom. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <ZoomTelemetry/ZoomEventLogConsts/ZoomEventLogConsts_Desktop.h>

#ifndef ZMPerformanceTrackerProtocol_h
#define ZMPerformanceTrackerProtocol_h

using namespace ns_zoom_telemetry;

typedef NS_ENUM(NSUInteger, ZMPerformanceTrackerErrorType) {
    ZMPerformanceTrackerErrorTypeNone = 0,
    ZMPerformanceTrackerErrorTypeUnderlying = 1000,
    ZMPerformanceTrackerErrorTypeNetwork = 1001,
    ZMPerformanceTrackerErrorTypeTimeout = 1002,
    ZMPerformanceTrackerErrorTypeUserCancel = 1003
};

typedef NS_ENUM(NSUInteger, ZMPerformanceTrackerTaskStatus) {
    ZMPerformanceTrackerTaskStatusReady      = 0,
    ZMPerformanceTrackerTaskStatusStart      = 1,
    ZMPerformanceTrackerTaskStatusSuccess    = 2,
    ZMPerformanceTrackerTaskStatusOnError    = 3
};

@protocol ZMPerformanceTrackerSessionProtocol;

@protocol ZMPerformanceTrackerTaskProtocol <NSObject>

@property (nonatomic, assign, readonly) ZMPerformanceTrackerTaskStatus status;
@property (nonatomic, copy, readonly, nonnull) NSString *taskID;

//Duration
@property (nonatomic, strong, readonly, nullable) NSDate *startDate;
@property (nonatomic, strong, readonly, nullable) NSDate *endDate;

//Log params
@property (nonatomic, assign, readonly) ZoomLogClientType clientType;
@property (nonatomic, assign, readonly) ZoomLogLocation location;
@property (nonatomic, assign, readonly) ZoomLogEvent event;
@property (nonatomic, assign, readonly) ZoomLogSubEvent subevent;
///Other params can set to `params`
@property (nonatomic, strong, readonly, nonnull) NSMutableDictionary *params;

@property (nonatomic, weak, readonly, nullable) id<ZMPerformanceTrackerSessionProtocol> session;

/// End current Task. If sucess, return true.
- (BOOL)start;

/// Cancel and no log will be sent.
- (void)cancel;

/// For some case, reconnection for example, it may timeout and lost the underlying error that cause the situation. This method is to solve the case.
- (void)setUnderlyingErrorDomain:(nonnull NSString *)domain code:(NSInteger)code;

/// End current Task normally. If perform the action sucessfully, return true.
- (BOOL)endWithParams:(nullable NSDictionary *)params;

/// End current Task when something goes wrong. If perform the action sucessfully, return true.
- (BOOL)endWithErrorCode:(ZMPerformanceTrackerErrorType)errorCode underlyingError:(nullable NSError *)underlyingError;

/// For some case, when the time initializes the task, task ID is only a placeholder, so we should update the ID later.
- (void)updateTaskID:(nonnull NSString *)taskID;

@end

@protocol ZMPerformanceTrackerSessionProtocol <NSObject>

@property (nonatomic, assign, readonly) ZoomLogClientType clientType;
@property (nonatomic, assign, readonly) ZoomLogLocation location;
@property (nonatomic, assign, readonly) ZoomLogEvent event;
/// When timeout, if the network goes wrong, use network error instead of timeout error.
@property (nonatomic, assign) BOOL checkNetworkErrorWhenTimeout;

- (nonnull instancetype)initWithClientType:(ZoomLogClientType)clientType
                          location:(ZoomLogLocation)location
                             event:(ZoomLogEvent)event;

/// Create a new task and store in the current session.
- (nullable id<ZMPerformanceTrackerTaskProtocol>)taskWithSubevent:(ZoomLogSubEvent)subevent ID:(nonnull NSString *)ID;

/// Create a new task and store in the current session. default timeout is 60s.
- (nullable id<ZMPerformanceTrackerTaskProtocol>)taskWithSubevent:(ZoomLogSubEvent)subevent ID:(nonnull NSString *)ID timeout:(NSTimeInterval)timeout;

/// Get the exist task that stored in the current session.
- (nullable id<ZMPerformanceTrackerTaskProtocol>)existTaskWithID:(nonnull NSString *)ID;

@end

#endif /* ZMPerformanceTrackerProtocol_h */
