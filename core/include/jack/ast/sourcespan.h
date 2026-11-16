
#ifndef JACK_AST_SOURCESPAN_H
#define JACK_AST_SOURCESPAN_H

#include <cstddef>

struct SourceSpan {
    std::size_t startLine;
    std::size_t startColumn;
    std::size_t endLine;
    std::size_t endColumn;
};

#endif // JACK_AST_SOURCESPAN_H
