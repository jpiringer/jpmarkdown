//
//  HTMLRenderer.hpp
//  jpmarkdown
//
//  Created by joerg piringer on 28.08.22.
//

#ifndef HTMLRenderer_hpp
#define HTMLRenderer_hpp

#include <stdio.h>
#include <string>
#include <vector>

#include "MarkdownRenderer.hpp"

namespace jp {

class HTMLRenderer : public MarkdownRenderer<std::string> {
    
public:
    virtual std::string headline(const std::string &content, int level) override {
        return std::string("<h")+std::to_string(level)+">"+
        content+
        "</h"+std::to_string(level)+">";
    }
    
    virtual std::string newLine() override {
        return "<br/>";
    }
    
    virtual std::string text(const std::string &content) override {
        return content;
    }
    
    virtual std::string attribute(const std::string &content, enum MarkdownAttribute attr) override {
        std::string attrStr = "";
        switch (attr) {
            case Bold:
                attrStr = "b";
                break;
            case Italic:
                attrStr = "i";
                break;
                
            default:
                break;
        }
        return std::string("<")+attrStr+">"+content+"</"+attrStr+">";
    }
    
    virtual std::string code(const std::string &code, enum CodeType type) override { // fake code execution for testing
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
    
    virtual std::string link(const std::string &content, const std::string &link) override {
        return std::string("<a href=\"")+link+"\">"+content+"</a>";
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

#endif /* HTMLRenderer_hpp */
