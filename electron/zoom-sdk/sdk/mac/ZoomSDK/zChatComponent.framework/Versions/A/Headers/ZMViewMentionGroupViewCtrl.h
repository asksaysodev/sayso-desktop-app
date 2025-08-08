#import <Cocoa/Cocoa.h>
#import <ZoomKit/ZMHMenu.h>

#import <zChatComponent/ZMMentionGroupCollection.h>
#import <zChatComponent/ZMUserInfo.h>
#import <zChatComponent/ZMCheckablePeopleListView.h>
                             
@interface ZMViewMentionGroupView : ZMBaseView
@end

@protocol ZMViewMentionGroupViewCtrlDelegate
- (void) userDidEndViewingOfMentionGroup;
- (void) userDidJoinOrLeaveMentionGroup:(ZMMentionGroup*)group;
@end

@interface ZMViewMentionGroupViewCtrl : NSViewController <NSTextViewDelegate, NSTextFieldDelegate, NSTableViewDelegate, NSTableViewDataSource, ZMCheckablePeopleListViewDelegate, ZMMentionGroupCollectionDelegate, ZMHMenuDelegate>

@property (nonatomic, weak) NSObject<ZMViewMentionGroupViewCtrlDelegate> *delegate;
@property (nonatomic, retain) ZMMentionGroupCollection *mentionGroups;
@property (nonatomic, assign) BOOL hideJoinLeaveButton;

// You can pass in the original MG. The view controller will create a new one if user clicks Update.
- (instancetype) initWithMentionGroup:(ZMMentionGroup *)mentionGroup;

@end
