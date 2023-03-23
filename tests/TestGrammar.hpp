//
//  TestGrammar.hpp
//  Tests
//
//  Created by joerg piringer on 02.09.22.
//
// this is a grammar for testing out new features and finding bugs during development
// it has no use in a production environment

#ifndef TestGrammar_hpp
#define TestGrammar_hpp

#include <ostream>
#include <iomanip>
#include <memory>

/*#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/foreach.hpp>
#include <boost/spirit/include/phoenix.hpp>*/

//#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
//#include <boost/phoenix.hpp>

namespace jp {

namespace qi = boost::spirit::qi;
//namespace phx = boost::phoenix;

inline std::string stringFromCharArray(const std::vector<char> &charArray) {
    std::string s(charArray.begin(), charArray.end());
    
    return s;
}

class TestGrammar : public qi::grammar<std::string::const_iterator, std::string()>
{
public:
    using Iterator = std::string::const_iterator;
        
    qi::rule<Iterator, char()> specialChar, escapedChar, normalChar, character;
    
    qi::rule<Iterator, std::string()> text;

    qi::rule<Iterator, std::string()> start;
    
    TestGrammar();
};

}

#endif /* TestGrammar_hpp */
