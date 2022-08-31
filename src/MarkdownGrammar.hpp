//
//  MarkdownGrammar.hpp
//  jpmarkdown
//
//  Created by joerg piringer on 30.08.22.
//

#ifndef MarkdownGrammar_hpp
#define MarkdownGrammar_hpp

#include <ostream>
#include <iomanip>
#include <memory>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/foreach.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include "MarkdownRenderer.hpp"
#include "MarkdownAttribute.hpp"
#include "MarkdownDefinitions.hpp"
#include "MarkdownAST.hpp"

namespace jp {

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;

template <typename T> class MarkdownGrammar : public qi::grammar<std::string::const_iterator, ASTNodePtr<T>()>
{
public:
    using Iterator = std::string::const_iterator;

    MarkdownGrammar() : MarkdownGrammar::base_type(start) {
        start = chunks [qi::_val = qi::_1];
        
        chunks =
                (chunk >> qi::eol >> chunks)
                    [qi::_val = phx::new_<ASTContainer<T>>(qi::_1, phx::new_<ASTNewline<T>>(), qi::_2)] |
                chunk
                    [qi::_val = qi::_1];
        
        chunk %=
                headline | normalChunk;

        text = (+(qi::char_ - '*' - '$' - '[')) [qi::_val = phx::new_<ASTText<T>>(qi::_1)];
        textWithoutNewline = (+(qi::char_ - '\n')) [qi::_val = phx::new_<ASTText<T>>(qi::_1)];
        code = (+(qi::char_ - '$')) [qi::_val = phx::new_<ASTText<T>>(qi::_1)];
        link1Text = (+(qi::char_ - ']')) [qi::_val = phx::new_<ASTText<T>>(qi::_1)];
        link2Text = (+(qi::char_ - ')')) [qi::_val = phx::new_<ASTText<T>>(qi::_1)];

        attributedText =
                (qi::char_('*') >> qi::char_('*') >> text >> qi::char_('*') >> qi::char_('*')) [qi::_val = phx::new_<ASTAttribute<T>>(qi::_3, Bold)] |
                (qi::char_('*') >> text >> qi::char_('*')) [qi::_val = phx::new_<ASTAttribute<T>>(qi::_2, Italic)] |
                (qi::char_('$') >> qi::char_(':') >> code >> qi::char_('$')) [qi::_val = phx::new_<ASTCode<T>>(qi::_3, Include)] | // include
                (qi::char_('$') >> code >> qi::char_('$')) [qi::_val = phx::new_<ASTCode<T>>(qi::_2, Inline)] | // inline
                (qi::char_('$') >> qi::char_('$') >> code >> qi::char_('$') >> qi::char_('$')) [qi::_val = phx::new_<ASTCode<T>>(qi::_3, Normal)]; // normal code
        
        
        link =
                (qi::char_('[') >> link1Text >> ']' >> '(' >> link2Text >> ')')
                    [qi::_val = phx::new_<ASTLink<T>>(qi::_2, qi::_3)] |
                (qi::char_('[') >> link1Text >> qi::char_(']'))
                    [qi::_val = phx::new_<ASTLink<T>>(qi::_2, nullptr)];
        
        span %=
                link | attributedText | text;

        normalChunk =
                (span >> normalChunk)
                    [qi::_val = phx::new_<ASTContainer<T>>(qi::_1, qi::_2)] |
                span
        [qi::_val = qi::_1] [qi::_val = qi::_1];
        
        headline = (+qi::char_('#') >> (+qi::blank) >> textWithoutNewline)
            [qi::_val = phx::new_<ASTHeadline<T>>(qi::_3, qi::_1)];
    }

    
    qi::rule<Iterator, ASTNodePtr<T>()> start, headline, normalChunk, text, textWithoutNewline, chunks, chunk, span, attributedText, code, link, link1Text, link2Text;
};

}

#endif /* MarkdownGrammar_hpp */
