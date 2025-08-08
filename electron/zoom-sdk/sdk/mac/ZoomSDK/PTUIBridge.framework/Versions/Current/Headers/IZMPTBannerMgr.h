//
//  IZMPTBannerMgr.h
//  PTUIBridge
//
//  Created by fistice on 4/28/25.
//

#ifndef IZMPTBannerMgr_h
#define IZMPTBannerMgr_h
#import <PTUIBridge/ZMBannerItem.h>

@protocol IZMPTBannerMgr <ZMPTRoutableObject>
- (void)updateBannerItems;
- (void)addBannerItem:(ZMBannerItem *)item;
- (void)removeBannerItem:(ZMBannerItem *)item;
- (BOOL)containsBannerItem:(ZMBannerItem *)item;
@end


@protocol ZMPTBannerEventProtocol
- (void)bannerViewDidLayout;
@end

#endif /* IZMPTBannerMgr_h */
