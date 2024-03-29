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
#include <boost/foreach.hpp>

#include "MarkdownRenderer.hpp"
#include "MarkdownAttribute.hpp"
#include "MarkdownDefinitions.hpp"
#include "MarkdownAST.hpp"

//#include <boost/phoenix.hpp>
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/object.hpp>
#include <boost/phoenix/operator.hpp>

namespace phx = boost::phoenix;

namespace jp {

namespace qi = boost::spirit::qi;

#if 0
class MarkdownGrammar : public qi::grammar<std::string::const_iterator, ASTNodePtr<std::string>()>
{
public:
    using Iterator = std::string::const_iterator;

    MarkdownGrammar(); /* : MarkdownGrammar::base_type(start) {
        // text
        specialChar %= qi::char_("*[$%");
        escapedChar %= qi::lit('\\') >> specialChar;
        normalChar %= qi::char_ - specialChar;
        text = (+(escapedChar | normalChar)) [qi::_val = phx::new_<ASTText<T>>(qi::_1)];
        textWithoutNewline = (+(qi::char_ - '\n')) [qi::_val = phx::new_<ASTText<T>>(qi::_1)];

        // emphasis
        emphBlock = +(!qi::lit('*') >> normalBlock) [qi::_val = qi::_1];
        emph =
                (qi::char_('*') >> qi::char_('*') >> emphBlock >> qi::char_('*') >> qi::char_('*')) [qi::_val = phx::new_<ASTAttribute<T>>(qi::_3, Bold)];
        
        // strong
        strongBlock = +(!qi::lit("**") >> normalBlock) [qi::_val = qi::_1];
        strong =
                (qi::char_('*') >> strongBlock >> qi::char_('*')) [qi::_val = phx::new_<ASTAttribute<T>>(qi::_2, Italic)];
        
        attributedText %= emph | strong;
        
        // code
        codeText = (+(qi::char_ - '$')) [qi::_val = phx::new_<ASTText<T>>(qi::_1)];
        code =
                (qi::char_('$') >> qi::char_(':') >> codeText >> qi::char_('$')) [qi::_val = phx::new_<ASTCode<T>>(qi::_3, Include)] | // include
                (qi::char_('$') >> codeText >> qi::char_('$')) [qi::_val = phx::new_<ASTCode<T>>(qi::_2, Inline)] | // inline
                (qi::char_('$') >> qi::char_('$') >> codeText >> qi::char_('$') >> qi::char_('$')) [qi::_val = phx::new_<ASTCode<T>>(qi::_3, Normal)]; // normal code
        
        // link
        link1Text = (+(qi::char_ - ']')) [qi::_val = phx::new_<ASTText<T>>(qi::_1)];
        link2Text = (+(qi::char_ - ')')) [qi::_val = phx::new_<ASTText<T>>(qi::_1)];
        link =
                ('[' >> link1Text >> "](" >> link2Text >> ')')
                    [qi::_val = phx::new_<ASTLink<T>>(qi::_1, qi::_2)] |
                ('[' >> link1Text >> ']')
                    [qi::_val = phx::new_<ASTLink<T>>(qi::_1, nullptr)];
        
        customAttributeName %= (+(qi::char_ - ')'));
        //customAttributeBlock = +(!qi::lit("%") >> normalBlock) [qi::_val = qi::_1];
        //customAttributeBlock %= +(qi::char_ - '%');// [qi::_val = qi::_1];
        customAttribute =
                ('%' >> text >> "%(" >> customAttributeName >> ')')
                    [qi::_val = phx::new_<ASTAttribute<T>>(qi::_1, Custom, qi::_2)];

        // headline
        headline = (+qi::char_('#') >> (+qi::blank) >> textWithoutNewline)
            [qi::_val = phx::new_<ASTHeadline<T>>(qi::_3, qi::_1)];

        span %=
                link | customAttribute | attributedText | code | text;

        normalBlock =
                (span >> normalBlock)
                    [qi::_val = phx::new_<ASTContainer<T>>(qi::_1, qi::_2)] |
                span
                    [qi::_val = qi::_1];
        
        blocks =
                (block >> qi::eol >> blocks)
                    [qi::_val = phx::new_<ASTContainer<T>>(qi::_1, phx::new_<ASTNewline<T>>(), qi::_2)] |
                block
                    [qi::_val = qi::_1];
        
        block %=
                headline | normalBlock;
        
        start %= blocks;
    }*/
    
    qi::rule<Iterator, char()> specialChar, escapedChar, normalChar;
    qi::rule<Iterator, std::string()> customAttributeName;

    qi::rule<Iterator, ASTHeadlinePtr<std::string>()> headline;
    qi::rule<Iterator, ASTLinkPtr<std::string>()> link;
    qi::rule<Iterator, ASTCodePtr<std::string>()> code;
    qi::rule<Iterator, ASTAttributePtr<std::string>()> attributedText, emph, strong, customAttribute;

    qi::rule<Iterator, ASTTextPtr<std::string>()> text, textWithoutNewline, codeText, link1Text, link2Text;
    
    qi::rule<Iterator, ASTNodePtr<std::string>()> normalBlock, blocks, block, span, emphBlock, strongBlock, customAttributeBlock;
    
    qi::rule<Iterator, ASTNodePtr<std::string>()> start;
};
#endif

template <typename T> class MarkdownGrammar : public qi::grammar<std::string::const_iterator, ASTNodePtr<T>()>
{
public:
    using Iterator = std::string::const_iterator;

    MarkdownGrammar() : MarkdownGrammar::base_type(start) {
        // text
        specialChar %= qi::char_("*[$%");
        escapedChar %= qi::lit('\\') >> specialChar;
        normalChar %= qi::char_ - specialChar;
        text = (+(escapedChar | normalChar)) [qi::_val = phx::new_<ASTText<T>>(qi::_1)];
        textWithoutNewline = (+(qi::char_ - '\n')) [qi::_val = phx::new_<ASTText<T>>(qi::_1)];

        // emphasis
        emphBlock = +(!qi::lit('*') >> normalBlock) [qi::_val = qi::_1];
        emph =
                (qi::char_('*') >> qi::char_('*') >> emphBlock >> qi::char_('*') >> qi::char_('*')) [qi::_val = phx::new_<ASTAttribute<T>>(qi::_3, Bold)];
        
        // strong
        strongBlock = +(!qi::lit("**") >> normalBlock) [qi::_val = qi::_1];
        strong =
                (qi::char_('*') >> strongBlock >> qi::char_('*')) [qi::_val = phx::new_<ASTAttribute<T>>(qi::_2, Italic)];
        
        attributedText %= emph | strong;
        
        // code
        codeText = (+(qi::char_ - '$')) [qi::_val = phx::new_<ASTText<T>>(qi::_1)];
        code =
                (qi::char_('$') >> qi::char_(':') >> codeText >> qi::char_('$')) [qi::_val = phx::new_<ASTCode<T>>(qi::_3, Include)] | // include
                (qi::char_('$') >> codeText >> qi::char_('$')) [qi::_val = phx::new_<ASTCode<T>>(qi::_2, Inline)] | // inline
                (qi::char_('$') >> qi::char_('$') >> codeText >> qi::char_('$') >> qi::char_('$')) [qi::_val = phx::new_<ASTCode<T>>(qi::_3, Normal)]; // normal code
        
        // link
        link1Text = (+(qi::char_ - ']')) [qi::_val = phx::new_<ASTText<T>>(qi::_1)];
        link2Text = (+(qi::char_ - ')')) [qi::_val = phx::new_<ASTText<T>>(qi::_1)];
        link =
                ('[' >> link1Text >> "](" >> link2Text >> ')')
                    [qi::_val = phx::new_<ASTLink<T>>(qi::_1, qi::_2)] |
                ('[' >> link1Text >> ']')
                    [qi::_val = phx::new_<ASTLink<T>>(qi::_1, nullptr)];
        
        customAttributeName %= (+(qi::char_ - ')'));
        //customAttributeBlock = +(!qi::lit("%") >> normalBlock) [qi::_val = qi::_1];
        //customAttributeBlock %= +(qi::char_ - '%');// [qi::_val = qi::_1];
        customAttribute =
                ('%' >> text >> "%(" >> customAttributeName >> ')')
                    [qi::_val = phx::new_<ASTAttribute<T>>(qi::_1, Custom, qi::_2)];

        // headline
        headline = (+qi::char_('#') >> (+qi::blank) >> textWithoutNewline)
            [qi::_val = phx::new_<ASTHeadline<T>>(qi::_3, qi::_1)];

        span %=
                link | customAttribute | attributedText | code | text;

        normalBlock =
                (span >> normalBlock)
                    [qi::_val = phx::new_<ASTContainer<T>>(qi::_1, qi::_2)] |
                span
                    [qi::_val = qi::_1];
        
        blocks =
                (block >> qi::eol >> blocks)
                    [qi::_val = phx::new_<ASTContainer<T>>(qi::_1, phx::new_<ASTNewline<T>>(), qi::_2)] |
                block
                    [qi::_val = qi::_1];
        
        block %=
                headline | normalBlock;
        
        start %= blocks;
    }
    
    qi::rule<Iterator, char()> specialChar, escapedChar, normalChar;
    qi::rule<Iterator, std::string()> customAttributeName;

    qi::rule<Iterator, ASTHeadlinePtr<T>()> headline;
    qi::rule<Iterator, ASTLinkPtr<T>()> link;
    qi::rule<Iterator, ASTCodePtr<T>()> code;
    qi::rule<Iterator, ASTAttributePtr<T>()> attributedText, emph, strong, customAttribute;

    qi::rule<Iterator, ASTTextPtr<T>()> text, textWithoutNewline, codeText, link1Text, link2Text;
    
    qi::rule<Iterator, ASTNodePtr<T>()> normalBlock, blocks, block, span, emphBlock, strongBlock, customAttributeBlock;
    
    qi::rule<Iterator, ASTNodePtr<T>()> start;
};

}

#endif /* MarkdownGrammar_hpp */
