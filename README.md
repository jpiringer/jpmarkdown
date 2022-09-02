# jpmarkdown

a very basic header only markdown parser.

uses boost::spirit for parsing and lexing.

have look at the tests for an idea how to use it.

i will add more examples and use cases later.

at the moment there are these renderers:
- PlainTextRenderer: renders everything into a plain std::string. code execution does not work really. i only added some fake results for testing
- HTMLRenderer: renders everything into a very simple html std::string. code execution does not work really. i only added some fake results for testing

## markdown
this only supports a very limited and adapted markdown dialect.

it only supports a few attributes:
`*italic*`, `**bold**`, `[link](reference)`, inline code execution (not specified in the standard specification), custom attributes (also not standard)

### inline code execution
the renderer is responsible to render these correctly.
`$code$` executes a code snippet like on a command line and inserts the result.
`$$code$$` executes code. if the code returns a value this value is inserted. if not, it just executes.
`$:include.lua$` includes an external file and executes it. should not insert any text.

### custom attributes
custom attributes: `%text%(attributename)`
the renderer is responsible to render them correctly.

## tests

the tests are written in objective-c++ and there's an xcode-project for that, but you can safely ignore that if you are not on a mac or don't care about the test.
at the moment it fails one test for the html renderer because it does not correctly render linefeeds. i might change that in the future but it does not have a hight priority.
