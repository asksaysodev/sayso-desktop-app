#import <Cocoa/Cocoa.h>

#import <ZoomKit/ZMBaseView.h>
#import <ZoomUnit/ZMTableView.h>
#import <zChatComponent/ZMUserInfo.h>

NS_ASSUME_NONNULL_BEGIN

@protocol ZMCheckablePeopleListViewDelegate
@optional // Note, ZMViewMentionGroupViewCtrl doesn't use these.
- (void) checkmarkChangedForUser:(ZMUserInfo* _Nonnull)user checked:(BOOL)checked;
- (BOOL) searchStringDidChangeTo:(NSString* _Nullable)string;
- (void)updateHeaderLabelCount:(NSInteger)count;
@end

@interface ZMCheckablePeopleListView : ZMBaseView

- (void) updateChannel:(NSString* _Nonnull)channelID
                people:(NSArray<ZMUserInfo*>* _Nonnull)peopleArray
        filteredPeople:(NSArray<ZMUserInfo*>* _Nullable)filteredPeopleArray
               hasMore:(BOOL)hasMore
         checkedPeople:(NSArray<ZMUserInfo*>* _Nonnull)checkedPeopleArray;

- (NSArray*) checkedPeople;
- (NSArray*) uncheckedPeople;
- (void) clearFilter;

@property (nonatomic, retain) NSString* _Nonnull sessionID;
@property (nonatomic, assign) BOOL readOnly;
@property (nonatomic, assign) float fontSize;
@property (nonatomic, assign) float fieldHeight;
@property (nonatomic, assign) NSObject<ZMCheckablePeopleListViewDelegate> *delegate;
@end


@protocol ZMCheckablePersonTableCellDelegate <NSObject>
- (void)checkButtonClickedInRow:(NSInteger)row;
@end

@interface ZMCheckablePersonTableCell : NSTableCellView
- (void) setUserInfo:(ZMUserInfo *)info needsCheck:(BOOL)needsCheck;
@property (nonatomic, weak) id<ZMCheckablePersonTableCellDelegate> delegate;
@property (nonatomic) NSInteger row;
@property (nonatomic, assign) float fontSize;
@property (nonatomic, assign) BOOL readOnly;
@end

NS_ASSUME_NONNULL_END
