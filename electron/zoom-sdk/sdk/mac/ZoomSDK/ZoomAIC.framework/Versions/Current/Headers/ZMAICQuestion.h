//
//  ZMAICQuestion.h
//  ZoomAIC
//
//  Created by Karen Cao on 9/13/23.
//

#ifndef ZMAICQuestion_h
#define ZMAICQuestion_h

@protocol ZMAICQuestion <NSObject>

@property (nonatomic, strong, nullable) NSString* title;
@property (nonatomic, strong, nullable) NSString* desc;

@end

#endif /* ZMAICQuestion_h */
