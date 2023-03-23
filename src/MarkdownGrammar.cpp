//
//  MarkdownGrammar.cpp
//  jpmarkdown
//
//  Created by joerg piringer on 23.03.23.
//

#include "Markdown.hpp"

#include <boost/phoenix.hpp>

namespace phx = boost::phoenix;

jp::MarkdownGrammar::MarkdownGrammar() : jp::MarkdownGrammar::base_type(start) {
    // text
    specialChar %= qi::char_("*[$%");
    escapedChar %= qi::lit('\\') >> specialChar;
    normalChar %= qi::char_ - specialChar;
    text = (+(escapedChar | normalChar)) [qi::_val = phx::new_<ASTText<std::string>>(qi::_1)];
    textWithoutNewline = (+(qi::char_ - '\n')) [qi::_val = phx::new_<ASTText<std::string>>(qi::_1)];

    // emphasis
    emphBlock = +(!qi::lit('*') >> normalBlock) [qi::_val = qi::_1];
    emph =
            (qi::char_('*') >> qi::char_('*') >> emphBlock >> qi::char_('*') >> qi::char_('*')) [qi::_val = phx::new_<ASTAttribute<std::string>>(qi::_3, Bold)];
    
    // strong
    strongBlock = +(!qi::lit("**") >> normalBlock) [qi::_val = qi::_1];
    strong =
            (qi::char_('*') >> strongBlock >> qi::char_('*')) [qi::_val = phx::new_<ASTAttribute<std::string>>(qi::_2, Italic)];
    
    attributedText %= emph | strong;
    
    // code
    codeText = (+(qi::char_ - '$')) [qi::_val = phx::new_<ASTText<std::string>>(qi::_1)];
    code =
            (qi::char_('$') >> qi::char_(':') >> codeText >> qi::char_('$')) [qi::_val = phx::new_<ASTCode<std::string>>(qi::_3, Include)] | // include
            (qi::char_('$') >> codeText >> qi::char_('$')) [qi::_val = phx::new_<ASTCode<std::string>>(qi::_2, Inline)] | // inline
            (qi::char_('$') >> qi::char_('$') >> codeText >> qi::char_('$') >> qi::char_('$')) [qi::_val = phx::new_<ASTCode<std::string>>(qi::_3, Normal)]; // normal code
    
    // link
    link1Text = (+(qi::char_ - ']')) [qi::_val = phx::new_<ASTText<std::string>>(qi::_1)];
    link2Text = (+(qi::char_ - ')')) [qi::_val = phx::new_<ASTText<std::string>>(qi::_1)];
    link =
            ('[' >> link1Text >> "](" >> link2Text >> ')')
                [qi::_val = phx::new_<ASTLink<std::string>>(qi::_1, qi::_2)] |
            ('[' >> link1Text >> ']')
                [qi::_val = phx::new_<ASTLink<std::string>>(qi::_1, nullptr)];
    
    customAttributeName %= (+(qi::char_ - ')'));
    //customAttributeBlock = +(!qi::lit("%") >> normalBlock) [qi::_val = qi::_1];
    //customAttributeBlock %= +(qi::char_ - '%');// [qi::_val = qi::_1];
    customAttribute =
            ('%' >> text >> "%(" >> customAttributeName >> ')')
                [qi::_val = phx::new_<ASTAttribute<std::string>>(qi::_1, Custom, qi::_2)];

    // headline
    headline = (+qi::char_('#') >> (+qi::blank) >> textWithoutNewline)
        [qi::_val = phx::new_<ASTHeadline<std::string>>(qi::_3, qi::_1)];

    span %=
            link | customAttribute | attributedText | code | text;

    normalBlock =
            (span >> normalBlock)
                [qi::_val = phx::new_<ASTContainer<std::string>>(qi::_1, qi::_2)] |
            span
                [qi::_val = qi::_1];
    
    blocks =
            (block >> qi::eol >> blocks)
                [qi::_val = phx::new_<ASTContainer<std::string>>(qi::_1, phx::new_<ASTNewline<std::string>>(), qi::_2)] |
            block
                [qi::_val = qi::_1];
    
    block %=
            headline | normalBlock;
    
    start %= blocks;
}
