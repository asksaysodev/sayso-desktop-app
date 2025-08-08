//
//  ZMCodeblockConstants.h
//  zChatComponent
//
//  Created by Yong Zhou on 2/12/25.
//

#import <zChatComponent/ZMRichFormatConstants.h>

#ifndef ZMCodeblockConstants_h
#define ZMCodeblockConstants_h

static NSString* const ZMRTFCodeblockDefaultLanguage = @"Plain Text";

static NSString* const ZMRTFCodeblockParamContent = @"content";
static NSString* const ZMRTFCodeblockParamLanguage = @"language";
static NSString* const ZMRTFCodeblockParamID = @"id";

static NSString* const ZMRTFCodeblockMarkupPrefix = @"hljs-";
static NSString* const ZMRTFCodeblockMarkupStart = @"<span class=\"hljs-meta\">";
static NSString* const ZMRTFCodeblockMarkupEnd = @"</span>";

static inline NSDictionary* defaultCodeblockStyle(void) {
    return @{
        NSFontAttributeName:ZMTextFormatCodeBlockFont(),
        // Since lacks context, we cannot use NSColor * ZMHexColor(NSUInteger light, NSUInteger dark) instead.
        NSForegroundColorAttributeName: NSApp.isDarkMode ? NSColorFromHex(0xFFFFFF) : NSColorFromHex(0x555B62),
    };
}

static inline NSDictionary<NSString *, NSDictionary<NSString *, NSNumber *> *> *CodeHighlightLightModeStyleMap(void)
{
    return @{
        @"hljs-comment": @{@"fontValue": @(0x00), @"colorValue": @(0x3F621A)},
        @"hljs-quote": @{@"fontValue": @(0x00), @"colorValue": @(0x3F621A)},
        @"hljs-attribute": @{@"fontValue": @(0x00), @"colorValue": @(0x9F1AB1)},
        @"hljs-keyword": @{@"fontValue": @(0x00), @"colorValue": @(0x9F1AB1)},
        @"hljs-literal": @{@"fontValue": @(0x00), @"colorValue": @(0x9F1AB1)},
        @"hljs-name": @{@"fontValue": @(0x00), @"colorValue": @(0x9F1AB1)},
        @"hljs-selector-tag": @{@"fontValue": @(0x00), @"colorValue": @(0x9F1AB1)},
        @"hljs-tag": @{@"fontValue": @(0x00), @"colorValue": @(0x9F1AB1)},
        @"hljs-template-variable": @{@"fontValue": @(0x00), @"colorValue": @(0x057682)},
        @"hljs-variable": @{@"fontValue": @(0x00), @"colorValue": @(0x057682)},
        @"hljs-code": @{@"fontValue": @(0x00), @"colorValue": @(0xB42318)},
        @"hljs-metahljs-string": @{@"fontValue": @(0x00), @"colorValue": @(0xB42318)},
        @"hljs-string": @{@"fontValue": @(0x00), @"colorValue": @(0xB42318)},
        @"hljs-link": @{@"fontValue": @(0x00), @"colorValue": @(0x3E1BDA)},
        @"hljs-regexp": @{@"fontValue": @(0x00), @"colorValue": @(0x3E1BDA)},
        @"hljs-bullet": @{@"fontValue": @(0x00), @"colorValue": @(0x3E1BDA)},
        @"hljs-number": @{@"fontValue": @(0x00), @"colorValue": @(0x3E1BDA)},
        @"hljs-symbol": @{@"fontValue": @(0x00), @"colorValue": @(0x3E1BDA)},
        @"hljs-title": @{@"fontValue": @(0x00), @"colorValue": @(0x3E1BDA)},
        @"hljs-meta": @{@"fontValue": @(0x00), @"colorValue": @(0x3C2D01)},
        @"hljs-section": @{@"fontValue": @(0x00), @"colorValue": @(0x3C2D01)},
        @"hljs-built_in": @{@"fontValue": @(0x00), @"colorValue": @(0x491C96)},
        @"hljs-classhljs-title": @{@"fontValue": @(0x00), @"colorValue": @(0x491C96)},
        @"hljs-params": @{@"fontValue": @(0x00), @"colorValue": @(0x491C96)},
        @"hljs-title class_": @{@"fontValue": @(0x00), @"colorValue": @(0x491C96)},
        @"hljs-type": @{@"fontValue": @(0x00), @"colorValue": @(0x491C96)},
        @"hljs-attr": @{@"fontValue": @(0x00), @"colorValue": @(0xA37C03)},
        @"hljs-subst": @{@"fontValue": @(0x00), @"colorValue": @(0x010101)},
        @"hljs-formula": @{@"fontValue": @(0x02), @"colorValue": @(0x000000)},
        @"hljs-selector-class": @{@"fontValue": @(0x00), @"colorValue": @(0xA37C03)},
        @"hljs-selector-id": @{@"fontValue": @(0x00), @"colorValue": @(0xA37C03)},
        @"hljs-doctag": @{@"fontValue": @(0x01), @"colorValue": @(0x000000)},
        @"hljs-strong": @{@"fontValue": @(0x01), @"colorValue": @(0x000000)},
        @"hljs-emphasis": @{@"fontValue": @(0x02), @"colorValue": @(0x000000)},
    };
}

static inline NSDictionary<NSString *, NSDictionary<NSString *, NSNumber *> *> *CodeHighlightDarkModeStyleMap(void) {
    return @{
        @"hljs-comment": @{@"fontValue": @(0x00), @"colorValue": @(0xACDC79)},
        @"hljs-quote": @{@"fontValue": @(0x00), @"colorValue": @(0xACDC79)},
        @"hljs-attribute": @{@"fontValue": @(0x00), @"colorValue": @(0xEEAAFD)},
        @"hljs-keyword": @{@"fontValue": @(0x00), @"colorValue": @(0xEEAAFD)},
        @"hljs-literal": @{@"fontValue": @(0x00), @"colorValue": @(0xEEAAFD)},
        @"hljs-name": @{@"fontValue": @(0x00), @"colorValue": @(0xEEAAFD)},
        @"hljs-selector-tag": @{@"fontValue": @(0x00), @"colorValue": @(0xEEAAFD)},
        @"hljs-tag": @{@"fontValue": @(0x00), @"colorValue": @(0xEEAAFD)},
        @"hljs-template-variable": @{@"fontValue": @(0x00), @"colorValue": @(0x4CB2BC)},
        @"hljs-variable": @{@"fontValue": @(0x00), @"colorValue": @(0x4CB2BC)},
        @"hljs-code": @{@"fontValue": @(0x00), @"colorValue": @(0xFDA29B)},
        @"hljs-metahljs-string": @{@"fontValue": @(0x00), @"colorValue": @(0xFDA29B)},
        @"hljs-string": @{@"fontValue": @(0x00), @"colorValue": @(0xFDA29B)},
        @"hljs-link": @{@"fontValue": @(0x00), @"colorValue": @(0x826BEA)},
        @"hljs-regexp": @{@"fontValue": @(0x00), @"colorValue": @(0x826BEA)},
        @"hljs-bullet": @{@"fontValue": @(0x00), @"colorValue": @(0x826BEA)},
        @"hljs-number": @{@"fontValue": @(0x00), @"colorValue": @(0x826BEA)},
        @"hljs-symbol": @{@"fontValue": @(0x00), @"colorValue": @(0x826BEA)},
        @"hljs-title": @{@"fontValue": @(0x00), @"colorValue": @(0x826BEA)},
        @"hljs-meta": @{@"fontValue": @(0x00), @"colorValue": @(0xFFFCEB)},
        @"hljs-section": @{@"fontValue": @(0x00), @"colorValue": @(0xFFFCEB)},
        @"hljs-built_in": @{@"fontValue": @(0x00), @"colorValue": @(0xECE9FE)},
        @"hljs-classhljs-title": @{@"fontValue": @(0x00), @"colorValue": @(0xECE9FE)},
        @"hljs-title class_": @{@"fontValue": @(0x00), @"colorValue": @(0x491C96)},
        @"hljs-params": @{@"fontValue": @(0x00), @"colorValue": @(0xECE9FE)},
        @"hljs-type": @{@"fontValue": @(0x00), @"colorValue": @(0xECE9FE)},
        @"hljs-attr": @{@"fontValue": @(0x00), @"colorValue": @(0xECE9FE)},
        @"hljs-subst": @{@"fontValue": @(0x00), @"colorValue": @(0xFFFFFF)},
        @"hljs-formula": @{@"fontValue": @(0x02), @"colorValue": @(0x000000)},
        @"hljs-selector-class": @{@"fontValue": @(0x00), @"colorValue": @(0xFF1E87)},
        @"hljs-selector-id": @{@"fontValue": @(0x00), @"colorValue": @(0xFF1E87)},
        @"hljs-doctag": @{@"fontValue": @(0x01), @"colorValue": @(0x000000)},
        @"hljs-strong": @{@"fontValue": @(0x01), @"colorValue": @(0x000000)},
        @"hljs-emphasis": @{@"fontValue": @(0x02), @"colorValue": @(0x000000)},
    };
}

static inline NSDictionary<NSString *, NSArray<NSString *> *> *CodeBlockLanguageMap(void)
{
    return @{
        @"Plain Text": @[@"Plain Text"],
        @"BASH": @[@"bash", @"sh"],
        @"C": @[@"c", @"h"],
        @"C#": @[@"csharp", @"cs", @"c#"],
        @"C++": @[@"cpp", @"cc", @"c++", @"h++", @"hpp", @"hh", @"hxx", @"cxx"],
        @"CSS": @[@"css"],
        @"Diff": @[@"diff", @"patch"],
        @"Go": @[@"go", @"golang"],
        @"GraphQL": @[@"graphql", @"gql"],
        @"HTML": @[@"html"],
        @"Java": @[@"java", @"jsp"],
        @"JavaScript": @[@"javascript", @"js", @"jsx", @"mjs", @"cjs"],
        @"JSON": @[@"json"],
        @"Kotlin": @[@"kotlin", @"kt", @"kts"],
        @"Less": @[@"less"],
        @"Lua": @[@"lua"],
        @"MATLAB": @[@"matlab"],
        @"Makefile": @[@"makefile", @"mk", @"mak", @"make"],
        @"MarkDown": @[@"markdown", @"md", @"mkdown", @"mkd"],
        @"Objective-C": @[@"objectivec", @"mm", @"objc", @"obj-c", @"objective-c", @"obj-c++", @"objective-c++", @"oc"], // @"oc" added by myself, not in standard hl-js lib
        @"PHP": @[@"php"],
        @"PHP Template": @[@"php-template", @"php template"],
        @"Perl": @[@"perl", @"pl", @"pm"],
        @"Protobuf": @[@"protobuf", @"proto"],
        @"Python": @[@"python", @"py", @"gyp", @"ipython"],
        @"Python REPL": @[@"python-repl", @"pycon", @"python repl"],
        @"R": @[@"r"],
        @"Ruby": @[@"ruby", @"rb", @"gemspec", @"podspec", @"thor", @"irb"],
        @"Rust": @[@"rust", @"rs"],
        @"SCSS": @[@"scss"],
        @"SQL": @[@"sql"],
        @"Scala": @[@"scala"],
        @"Scilab": @[@"scilab", @"sci"],
        @"Shell": @[@"shell", @"console", @"shellsession"],
        @"Swift": @[@"swift"],
        @"TOML": @[@"ini", @"toml"],
        @"TypeScript": @[@"typescript", @"ts", @"tsx", @"mts", @"cts"],
        @"VBNet": @[@"vbnet", @"vb"],
        @"WebAssembly": @[@"wasm", @"webassembly"],
        @"XHTML": @[@"xhtml"],
        @"XML": @[@"xml", @"rss", @"atom", @"xjb", @"xsd", @"xsl", @"plist", @"wsf", @"svg"],
        @"YAML": @[@"yaml", @"yml"]
    };
}

static inline NSDictionary<NSString *, NSString *> *CodeBlockLanguageKeyMap(void)
{
    return @{
        @"Plain Text": @"",
        @"BASH": @"bash",
        @"C": @"c",
        @"C#": @"csharp",
        @"C++": @"cpp",
        @"CSS": @"css",
        @"Diff": @"diff",
        @"Go": @"go",
        @"GraphQL": @"graphql",
        @"HTML": @"html",
        @"Java": @"java",
        @"JavaScript": @"javascript",
        @"JSON": @"json",
        @"Kotlin": @"kotlin",
        @"Less": @"less",
        @"Lua": @"lua",
        @"MATLAB": @"matlab",
        @"Makefile": @"makefile",
        @"MarkDown": @"markdown",
        @"Objective-C": @"objectivec",
        @"PHP": @"php",
        @"PHP Template": @"php-template",
        @"Perl": @"perl",
        @"Protobuf": @"protobuf",
        @"Python": @"python",
        @"Python REPL": @"python-repl",
        @"R": @"r",
        @"Ruby": @"ruby",
        @"Rust": @"rust",
        @"SCSS": @"scss",
        @"SQL": @"sql",
        @"Scala": @"scala",
        @"Scilab": @"scilab",
        @"Shell": @"shell",
        @"Swift": @"swift",
        @"TOML": @"ini",
        @"TypeScript": @"typescript",
        @"VBNet": @"vbnet",
        @"WebAssembly": @"wasm",
        @"XHTML": @"xhtml",
        @"XML": @"xml",
        @"YAML": @"yaml"
    };
}

static inline NSDictionary<NSString *, NSString *> *CodeBlockLanguageKeyMapSwapped(void)
{
    return @{
        @"bash": @"BASH",
        @"c": @"C",
        @"csharp": @"C#",
        @"cpp": @"C++",
        @"css": @"CSS",
        @"diff": @"Diff",
        @"go": @"Go",
        @"graphql": @"GraphQL",
        @"html": @"HTML",
        @"java": @"Java",
        @"javascript": @"JavaScript",
        @"json": @"JSON",
        @"kotlin": @"Kotlin",
        @"less": @"Less",
        @"lua": @"Lua",
        @"matlab": @"MATLAB",
        @"makefile": @"Makefile",
        @"markdown": @"MarkDown",
        @"objectivec": @"Objective-C",
        @"php": @"PHP",
        @"php-template": @"PHP Template",
        @"perl": @"Perl",
        @"protobuf": @"Protobuf",
        @"python": @"Python",
        @"python-repl": @"Python REPL",
        @"r": @"R",
        @"ruby": @"Ruby",
        @"rust": @"Rust",
        @"scss": @"SCSS",
        @"sql": @"SQL",
        @"scala": @"Scala",
        @"scilab": @"Scilab",
        @"shell": @"Shell",
        @"swift": @"Swift",
        @"ini": @"TOML",
        @"typescript": @"TypeScript",
        @"vbnet": @"VBNet",
        @"wasm": @"WebAssembly",
        @"xhtml": @"XHTML",
        @"xml": @"XML",
        @"yaml": @"YAML"
    };
}

#endif /* ZMCodeblockConstants_h */
