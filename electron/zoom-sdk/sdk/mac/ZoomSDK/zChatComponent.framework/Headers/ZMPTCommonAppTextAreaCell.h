//
//  ZMPTCommonAppTextAreaCell.h
//  zChatComponent
//
//  Created by Cyan.Yang on 2024/1/18.
//

#import <Foundation/Foundation.h>

#ifdef __cplusplus
namespace ns_zoom_messager {
class TextAreaItem;
}  // namespace ns_zoom_messager
#endif

NS_ASSUME_NONNULL_BEGIN

@interface ZMPTCommonAppTextAreaCxxItemWrapper : NSObject

@property (nonatomic, readonly) NSString *text;
@property (nonatomic, readonly) NSString *value;
@property (nonatomic, readonly) NSString *placeholder;
@property (nonatomic, readonly, getter=isMultiline) BOOL multiline;
@property (nonatomic, readonly) NSInteger maximumLength;

#ifdef __cplusplus
@property (nonatomic, readonly) const ns_zoom_messager::TextAreaItem *cxxItem NS_RETURNS_INNER_POINTER;

- (instancetype)initWithCxxItem:(const ns_zoom_messager::TextAreaItem *)cxxItem NS_DESIGNATED_INITIALIZER;
#endif

- (instancetype)init NS_UNAVAILABLE;

@end

NS_ASSUME_NONNULL_END
