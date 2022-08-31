//
//  Tests.mm
//  Tests
//
//  Created by joerg piringer on 28.08.22.
//

#import <XCTest/XCTest.h>

#include "Markdown.hpp"

#define PRINT_AST 1

@interface Tests : XCTestCase

@end

@implementation Tests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

bool testPlainText(const std::string &input, const std::string &expectedOutput) {
    auto renderer = std::make_shared<jp::PlainTextRenderer>();
    
    jp::MarkdownParser<std::string> markdown(renderer);
    markdown.parse(input);
#if PRINT_AST
    markdown.printAST();
    std::cout << "----------" << std::endl;
#endif
    return markdown.getResult() == expectedOutput;
}

- (void)testPlainText {
    XCTAssert(testPlainText("normal line", "normal line"));
    XCTAssert(testPlainText("# headline\n", "headline"));
    XCTAssert(testPlainText("# headline", "headline"));
    XCTAssert(testPlainText("# headline1\n## headline2\n### headline3", "headline1\nheadline2\nheadline3"));
    XCTAssert(testPlainText("*italic*", "italic"));
    XCTAssert(testPlainText("**bold**", "bold"));
    XCTAssert(testPlainText("a**bold**", "abold"));
    XCTAssert(testPlainText("**bold**\n*italic*", "bold\nitalic"));
    XCTAssert(testPlainText("**bold**a", "bolda"));
    XCTAssert(testPlainText("that is *italic* and **bold**", "that is italic and bold"));
}

- (void)testPlainTextLink {
    XCTAssert(testPlainText("[link]", "link"));
    XCTAssert(testPlainText("[link](https://joerg.piringer.net)", "link"));
    XCTAssert(testPlainText("[link](https://joerg.piringer.net) a link was here", "link a link was here"));
    XCTAssert(testPlainText("hello this is a [link](https://joerg.piringer.net)", "hello this is a link"));
}

- (void)testPlainTextCode {
    XCTAssert(testPlainText("embedded code $:test.lua$", "embedded code 5"));
    XCTAssert(testPlainText("embedded code $1+3$", "embedded code 4"));
    XCTAssert(testPlainText("embedded code $$\nreturn 1+2\n$$", "embedded code 3"));
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
