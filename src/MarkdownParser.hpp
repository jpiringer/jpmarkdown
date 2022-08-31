//
//  jpmarkdown.hpp
//  jpmarkdown
//
//  Created by joerg piringer on 28.08.22.
//

#ifndef jpmarkdown_
#define jpmarkdown_

#include <string>
#include <memory>
#include "MarkdownDefinitions.hpp"
#include "MarkdownRenderer.hpp"
#include "MarkdownGrammar.hpp"
#include "MarkdownRenderer.hpp"
#include "MarkdownAttribute.hpp"

/* The classes below are exported */
#pragma GCC visibility push(default)

namespace jp {

namespace qi = boost::spirit::qi;

template <typename T> class MarkdownRenderer;

template <typename Parser, typename Skipper, typename ... Args>
void PhraseParseOrDie(
    const std::string& input, const Parser& p, const Skipper& s,
    Args&& ... args)
{
    std::string::const_iterator begin = input.begin(), end = input.end();
    boost::spirit::qi::phrase_parse(
        begin, end, p, s, std::forward<Args>(args) ...);
    if (begin != end) {
        std::cout << "Unparseable: "
                  << std::quoted(std::string(begin, end)) << std::endl;
        throw std::runtime_error("Parse error");
    }
}

template <typename T> class MarkdownParser
{
    std::shared_ptr<MarkdownRenderer<T>> renderer;
    ASTNodePtr<T> outNode = nullptr;
    T result;
    
public:
    MarkdownParser(std::shared_ptr<MarkdownRenderer<T>> markdownRenderer) : renderer(markdownRenderer) {}
    ~MarkdownParser() {if (outNode != nullptr) {delete outNode;}}
    
    void parse(const std::string &input) {
        try {
            PhraseParseOrDie(input, MarkdownGrammar<T>(), qi::space, outNode);

            //std::cout << *outNode << std::endl;
            
            result = outNode->render(renderer);
        }
        catch (std::exception& e) {
            std::cout << "EXCEPTION: " << e.what() << std::endl;
        }
        
    }
    
    const T &getResult() {return result;}
    
    void printAST() {std::cout << *outNode << std::endl;}
};

}

#pragma GCC visibility pop
#endif
