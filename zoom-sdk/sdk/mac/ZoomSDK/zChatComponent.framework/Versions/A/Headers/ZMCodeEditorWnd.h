//
//  ZMCodeEditorWnd.h
//  SaasBeePTUIModule
//
//  Created by likevin on 11/30/17.
//  Copyright © 2017 Zipow. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include <CodeSnippet/PropSetFile.h>
#include <CodeSnippet/Scintilla.h>
#include <CodeSnippet/SciLexer.h>
#include <CodeSnippet/StyleDefinition.h>
#include <CodeSnippet/StyleWriter.h>
#include <CodeSnippet/CodeSnippetFileIO.h>

@class ScintillaView, ZMButton, ZMCodeBackColorView, ZMIMImageButton, ZMMenuBtn, ZMFileNameTextField,ZMCheckButton, ZMCodePreviewWnd,ZMChatAlwaysOverlayScrollView, ZMChatCheckDownAndUpScrollView, ZMTableViewMenu, ZMMoreInfoTextField, ZMAutoShowToolTipTextField;
@protocol ZMCMenuDelegate;

/// Codes representing the effect a line has on indentation.
enum IndentationStatus {
    isNone,		// no effect on indentation
    isBlockStart,	// indentation block begin such as "{" or VB "function"
    isBlockEnd,	// indentation end indicator such as "}" or VB "end"
    isKeyWordStart	// Keywords that cause indentation
};

struct StyleAndWords {
    int styleNumber;
    std::string words;
    StyleAndWords() : styleNumber(0) {
    }
    bool IsEmpty() const { return words.length() == 0; }
    bool IsSingleChar() const { return words.length() == 1; }
};

enum{
    Status_Enabled = 0,
    Status_Disabled,
    Status_Enabled_Try,
};
//MainCodeFile = 0, RTF	= 1,HTML = 2, Property	= 3,
typedef enum {
    Type_CodeSnippet_Main_Code              = 0,
    Type_CodeSnippet_Attachment_RTF         = 1,
    Type_CodeSnippet_Attachment_HTML        = 2,
    Type_CodeSnippet_Attachment_Property    = 3,
}Type_CodeSnippet_Attachment;

enum {
    Type_Error_NO         = 0,
    Type_Error_Exceed_16K = 1,
    Type_Error_Upload_Failed = 2,
    Type_Error_Upload_Failed_Too_Old_To_Be_Edited = 3,
};

@interface ZMCodeEditorView : ZMNonLayerBaseView<NSComboBoxDelegate,NSTextFieldDelegate> {
    NSTextField*                _tfWndTitle;
    ZMCodeBackColorView*        _bgColorView;
    ZMFileNameTextField*        _tfTextTitle;
    ZMMoreInfoTextField*        _codeTypeBtn;
    ScintillaView *             _editor;
    ZMButton*                   _cancelBtn;
    ZMButton*                   _createBtn;
    ZMTableViewMenu*            _codeTypeMenu;
    NSProgressIndicator*        _progressBar;
    NSTextField*                _errorTip;
    ZMCheckButton*              _wrapBtn;
    std::vector<char> _sub_style_bases;
    bool              _is_wrap_style;
    NSString*         _codeTypeExtension;
    bool              _is_line_number_;
    bool              _is_indent_opening_;
    bool              _is_indent_closing_;
    bool              _is_indent_maintain_;
    bool              _is_need_read_properties_;
    NSString*         _codeLanguage;
    bool              _is_indent_opening;
    bool              _is_indent_closing;
    bool              _is_indent_maintain;
    NSInteger         _int_statement_lookback;
    StyleAndWords     _statement_indent;
    StyleAndWords     _statement_end;
    StyleAndWords     _block_start;
    StyleAndWords     _block_end;
    NSUInteger        _iMarginLeftWidth;
    NSUInteger        _iMarginRightWidth;
    NSInteger        _iBytesLimit;
    NSString*         _codeFileFullPath;
    NSString*         _rtfFullPath;
    NSString*         _htmlFullPath;
    NSString*         _propertyFullPath;
    NSString*         _zipFullPath;
    NSMutableArray*  _languageArray;
    id               _localMouseEventMonitor;
    NSString*        _sessionId;
    NSString*        _msgId;
    NSString*        _editReqId;
    BOOL             _isSaveMode;
    BOOL             _isCodeTypeEdited;
    SciFnDirect     _fn_;
    sptr_t          _ptr_;
}
@property(nonatomic, readwrite, copy)NSString* codeTypeExtension;
@property(nonatomic, readwrite, copy)NSString* codeLanguage;
@property(nonatomic, readwrite, copy)NSString* codeFileFullPath;
@property(nonatomic, readwrite, copy)NSString* rtfFullPath;
@property(nonatomic, readwrite, copy)NSString* htmlFullPath;
@property(nonatomic, readwrite, copy)NSString* propertyFullPath;
@property(nonatomic, readwrite, copy)NSString* zipFullPath;
@property(nonatomic, readwrite, copy)NSString* sessionId;
@property(nonatomic, readwrite, copy)NSString* msgId;
@property(nonatomic, readwrite, copy)NSString* editReqId;
@property(nonatomic, readwrite, assign)BOOL isCodeTypeEdited;
- (void)cleanUp;
- (NSString*)codeTypeExtension;
- (void)reset;
- (void)setSavePoint;
- (BOOL)isModified;
- (void)updateCreateBtnToSave:(BOOL)isSave;
- (void)setCodeFileTitle:(NSString*)title;
- (void)tryToSetupCodeType:(NSString*)codeType;
- (void)setupLanguage:(NSString*)codeType andIsAsync:(BOOL)isAsync;
- (void)setupCodeTypeMenu;
- (void)setEditorEditable: (BOOL)isEditable;
- (void)setWrap:(bool)bWrap;
- (void)setEditorStringByURI:(NSString*)uriPath;
- (void)setEditorString:(NSString*)inString;
- (NSInteger)lengthDocument;
- (StyleDefinition)styleDefinitionFor:(NSInteger)style;
- (NSInteger)call:(NSInteger)msg wParam:(NSInteger)wParam lParam:(NSInteger)lParam;
- (NSInteger)callReturnPointer:(NSInteger)msg wParam:(NSInteger)wParam lParam:(const void*)s;
- (NSInteger)callString:(NSInteger)property parameter:(NSInteger)parameter value:(const void*)value;
@end


@interface ZMCodeEditorWnd : ZMPlainWindow{
    ZMCodeEditorView* _codeEditView;
    NSString*         _attachedSession;
}
@property(nonatomic, readwrite, copy)NSString* attachedSession;
@property(nonatomic, readwrite, copy)NSString* attachedThreadID;

- (instancetype)initWithContentRect:(NSRect)contentRect styleMask:(NSWindowStyleMask)style backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag isReadOnly:(BOOL)isReadOnly;
- (ZMCodeEditorView*)codeEditView;
- (void)cleanUp;
- (void)showEditWindowOnAboveWindow:(NSWindow*)inWindow withSession:(NSString*)inSession andMsgID:(NSString*)inMsgId;
- (void)showWindowOnAboveWindow:(NSWindow*)inWnd withSession:(NSString*)inSession threadId:(NSString *)threadId andResetStatus:(BOOL)isReset;
- (void)tryToCloseWndWhenSessionClosed:(NSString*)inSession;
- (void)closeWindow;
@end


@interface ZMLineNumberRulerView : NSRulerView
@end

@interface ZMCodePreview : ZMNonLayerBaseView<NSTextViewDelegate, NSTextStorageDelegate>{
    ZMAutoShowToolTipTextField*         _tfWndTitle;
    ZMCodeBackColorView*                _bgColorView;
    ZMLineNumberRulerView*              _lineNumberRulerView;
    ZMChatCheckDownAndUpScrollView*     _scrollView;
    NSTextView*                         _textView;
    ZMCheckButton*                      _wrapBtn;
    ZMButton*                           _doneBtn;
}
- (void)cleanUp;
- (BOOL)loadRTFFile:(NSString*)rtfFilePath andFileName:(NSString*)fileName;
- (void)updateTotalLines:(NSUInteger)totalLines;
@end

@interface ZMCodePreviewWnd : ZMPlainWindow{
    ZMCodePreview*      _codePreview;
    NSString*           _attachedSessionID;
    NSString*           _attachedMsgID;
}
@property(nonatomic, readwrite, copy) NSString* attachedSessionID;
@property(nonatomic, readwrite, copy) NSString* attachedMsgID;
- (ZMCodePreview*)codePreview;
+ (void)showCodePreviewWithRTFPath:(NSString*)rtfPath andFileName:(NSString*)fileName andParentWnd:(NSWindow*)parentWnd andLinesNumber:(NSUInteger)totalLines andSessionId:(NSString*)sessionID andMsgID:(NSString*)msgId;
+ (void)closeCodePreviewWithSessionID:(NSString*)sessionID;
+ (void)closeCodePreviewWithMsgID:(NSString*)msgID;
+ (void)releaseInstance;
- (void)closeWindow;
@end

