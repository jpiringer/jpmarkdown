//
//  PlainTextRenderer.hpp
//  jpmarkdown
//
//  Created by joerg piringer on 28.08.22.
//
// the plain text renderer removes all formatting characters
// it is intended mainly for testing the parser

#ifndef PlainTextRenderer_hpp
#define PlainTextRenderer_hpp

#include <stdio.h>
#include <string>
#include <vector>

#include "MarkdownRenderer.hpp"

namespace jp {

class PlainTextRenderer : public MarkdownRenderer<std::string> {
    
public:
    virtual std::string headline(std::string content, int level) override {
        return content;
    }
    
    virtual std::string newLine() override {
        return "\n";
    }
    
    virtual std::string text(const std::string &content) override {
        return content;
    }
    
    virtual std::string attribute(std::string content, enum MarkdownAttribute attr) override {
        return content;
    }
    
    virtual std::string code(std::string code, enum CodeType type) override { // fake code execution for testing
        switch (type) {
            case Normal:
                return "3";
            case Inline:
                return "4";
            case Include:
                return "5";
        }
    }
    
    virtual std::string nothing() override {
        return "";
    }
    
    virtual std::string link(std::string content, std::string link) override {
        return content;
    }
    
    virtual std::string chain(const std::vector<std::string> &children) override {
        std::string accum;
        
        for (auto str : children) {
            accum += str;
        }
        
        return accum;
    }
};

}

#endif /* PlainTextRenderer_hpp */
