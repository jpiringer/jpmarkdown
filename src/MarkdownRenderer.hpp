//
//  MarkdownRenderer.hpp
//  jpmarkdown
//
//  Created by joerg piringer on 28.08.22.
//

#ifndef MarkdownRenderer_hpp
#define MarkdownRenderer_hpp

#include <stdio.h>
#include <string>
#include "MarkdownGrammar.hpp"
#include "MarkdownCode.hpp"
#include "MarkdownAttribute.hpp"

namespace jp {

template <typename T> class MarkdownRenderer {
public:
    virtual T headline(const T &content, int level) = 0;
    virtual T newLine() = 0;
    virtual T text(const std::string &str) = 0;
    virtual T attribute(const T &content, enum MarkdownAttribute attr) = 0;
    virtual T link(const T &content, const T &link) = 0;
    virtual T code(const T &code, enum CodeType type) = 0;
    virtual T nothing() = 0;

    virtual T chain(const std::vector<T> &children) = 0;
};

}

#endif /* MarkdownRenderer_hpp */
