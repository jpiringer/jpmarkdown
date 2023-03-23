//
//  TestGrammar.cpp
//  Tests
//
//  Created by joerg piringer on 02.09.22.
//

#include "TestGrammar.hpp"

jp::TestGrammar::TestGrammar()  : TestGrammar::base_type(start) {
    specialChar %= qi::char_("*[$");
    escapedChar %= qi::lit('\\') >> qi::char_;
    normalChar %= qi::char_ - specialChar;
    character %= escapedChar | normalChar;
    text %= (+character);// [qi::_val = stringFromCharArray(qi::_1)];
    
    start = text;
}
