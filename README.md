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
*italic*, **bold**, links, inline code execution (not specified in the standard specification), custom attributes (also not standard)

### inline code execution
the renderer is responsible to render these correctly.
$1+2$ executes a code snippet like on a command line and inserts the result.
$$code$$ executes code. if the code returns a value this value is inserted. if not, it just executes.
$:include.lua$ includes an external file and executes it. should not insert any text.

### custom attributes
custom attributes: %text%(attributename)
the renderer is responsible to render them correctly.
