//
//  ZMCodeEditMgr.h
//  SaasBeePTUIModule
//
//  Created by likevin on 2/1/18.
//  Copyright © 2018 Zipow. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <CodeSnippet/PropSetFile.h>
#include <CodeSnippet/Scintilla.h>
#include <CodeSnippet/SciLexer.h>
#include <CodeSnippet/StyleDefinition.h>
#include <CodeSnippet/StyleWriter.h>
#include <CodeSnippet/CodeSnippetFileIO.h>
#include <CodeSnippet/ILexer.h>
#include <CodeSnippet/Lexilla.h>

@class ZMCodeEditorWnd;

class ZMSourceEditController :public ISourceEditController
{
public:
    virtual PropSetFile& GetProps();
    virtual int LengthDocument();
    virtual StyleDefinition StyleDefinitionFor(int style);
    virtual int Call(unsigned int msg, uptr_t wParam=0, sptr_t lParam=0);
    virtual sptr_t CallReturnPointer(unsigned int msg, uptr_t wParam=0, sptr_t lParam=0);
    virtual int CallPointer(unsigned int msg, uptr_t wParam, void *s);
    virtual int CallString(unsigned int msg, uptr_t wParam, const char *s);
    
    bool SaveToRTF(const Cmm::CString &saveName, int start, int end);
    bool SaveToHTML(const Cmm::CString &saveName);
    bool SaveToFile(const Cmm::CString &saveName);
    bool SaveCodeSnippetProperties(const Cmm::CString &saveName, const CodeSnippetProperties &prop);
    bool ReadCodeSnippetProperties(const Cmm::CString &fileName, CodeSnippetProperties &prop);
};

@interface ZMCodeEditMgr : NSObject {
    PropSetFile*        _propSetFile;
    BOOL                _isFirstReadProperty;
    ZMCodeEditorWnd*    _codeEditorWnd;
    ZMSourceEditController* _codeEidtController;
}
@property(nonatomic, readwrite, assign)BOOL isFirstReadProperty;

+ (ZMCodeEditMgr*)sharedInstance; //lazy load
+ (void)releaseSharedInstance;
+ (ZMCodeEditMgr*)getSharedInstance; //no lazy load
+ (void)codeEditMgrValidCodeSignWithCompletion:(void (^)(BOOL result))completion;
+ (BOOL)isSupportCodeEditorFeature;
+ (BOOL)isCodeEditorEnabled:(NSString*)sessionId;
+ (BOOL)isCodeSnippetLoaded;
+ (NSString*)scintillaFrameworkFilePath:(BOOL *)isPlugin;
+ (NSString*)scintillaPropertyFilePath;
+ (BOOL)deleteCodesnippetWithSessionId:(NSString*)sessionId;
+ (Lexilla::CreateLexerFn)createLexerFn;
- (Class)scintillaViewClass;
- (ZMSourceEditController*)codeEditController;
- (ZMCodeEditorWnd*)codeEditorWindow;
- (void)closeCodeEditorWindow;
- (void)tryToCloseCodeWndWhenSessionClosed:(NSString*)inSession;
- (PropSetFile*)props;
- (NSArray*)supportLanguageArray;
- (BOOL)createCodeSnippetFile:(NSString*)strTitle andFileLength:(NSUInteger)fileLength andCodeTypeExtension:(NSString*)codeTypeExtension andTotalLine:(NSUInteger)totalLine andLanguageName:(NSString*)languageName;
- (BOOL)createEditCodeSnippetFile:(NSString*)strTitle andFileLength:(NSUInteger)fileLength andCodeTypeExtension:(NSString*)codeTypeExtension andTotalLine:(NSUInteger)totalLine andLanguageName:(NSString*)languageName andMsgId:(NSString*)msgId;
@end

@interface ZMCodeSnippetUploadDownloadMgr : NSObject{
}

@property(copy) NSString* downloadCodeSnippetSession;
@property(copy) NSString* downloadCodeSnippetMsgId;
@property(copy) NSString* downloadCodeSnippetThreadId;

+ (ZMCodeSnippetUploadDownloadMgr*)sharedInstance;
+ (void)releaseSharedInstance;
+ (NSString*)codeSnippetFileDirWithSubDir:(NSString*)subDir;
+ (NSString*)codeSnippetFileDir:(NSString*)inSessionID;
+ (NSString*)editCodeSnippetWebFileIdDir:(NSString*)inSessionID andMsgId:(NSString*)msgId;
+ (NSString*)editCodeSnippetWebFileIdDir:(NSString*)webFileId;
+ (NSDictionary*)getFileInfoWithSessionId:(NSString*)inSessionId andMsgId:(NSString*)inMsgId;
//upload
- (NSString*)editCodeSnippetMsgContentWithSessionId:(NSString*)inSession andMsgId:(NSString*)msgId andEditFilePath:(NSString*)editFilePath andEditFileName:(NSString*)editFileName;
- (NSString*)editCodeSnippetMsgTitleWithSessionId:(NSString*)inSession andMsgId:(NSString*)msgId andEditFileName:(NSString*)editFileName;

//download
- (void)cancelPreviousDownloadTimeout:(NSString*)inSession andMsgId:(NSString*)inMsgid;
- (BOOL)downloadCodeSnippetZipFile:(NSString*)inSession andMessageID:(NSString*)msgId;
- (NSDictionary*)getCodeSnippetFileDict:(NSString*)inSession andMsgID:(NSString*)msgId;
- (BOOL)checkCodeSnippetMsgFiles:(NSString*)inSession andMessageID:(NSString*)msgId;
- (BOOL)syncCheckCodeSnippetMsgFiles:(NSString*)inSession andMessageID:(NSString*)msgId WithCompletion:(void (^)(BOOL result))completion;
- (BOOL)isCodeSnippetFilesReady:(NSString*)inSession andMsgID:(NSString*)msgId;

//util
- (void)tryWriteRftFile:(NSString*)rtfPath toCodeFile:(NSString*)codePath;
@end

