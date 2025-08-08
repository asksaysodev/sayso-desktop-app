#import <Cocoa/Cocoa.h>
#import <ZoomKit/ZMHMenu.h>

#import <zChatComponent/ZMMentionGroupCollection.h>
#import <zChatComponent/ZMUserInfo.h>
#import <zChatComponent/ZMCheckablePeopleListView.h>

@interface ZMEditMentionGroupView : ZMBaseView
@end


@interface ZMEditMentionGroupViewCtrl : NSViewController <NSTextViewDelegate, NSTextFieldDelegate, NSTableViewDelegate, NSTableViewDataSource, ZMCheckablePeopleListViewDelegate, ZMMentionGroupCollectionDelegate, ZMHMenuDelegate>

@property (nonatomic, retain) ZMMentionGroupCollection * _Nullable mentionGroups;

// You can pass in the original MG. The view controller will create a new one if user clicks Update.
- (instancetype _Nonnull ) initWithMentionGroup:(ZMMentionGroup * _Nonnull)mentionGroup;

@end
