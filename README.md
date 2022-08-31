# jpmarkdown

a very basic header only markdown parser.

uses boost::spirit for parsing and lexing.

have look at the tests for an idea how to use it.

i will add more examples and use cases later.

at the moment there are these renderers:
- PlainTextRenderer: renders everything into a plain std::string. code execution does not work really. i only added some fake results for testing
- HTMLRenderer: renders everything into a very simple html std::string. code execution does not work really. i only added some fake results for testing

