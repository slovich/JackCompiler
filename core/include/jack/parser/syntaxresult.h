#ifndef JACK_PARSER_SYNTAXRESULT_H
#define JACK_PARSER_SYNTAXRESULT_H

#include "jack/ast/programnode.h"

#include <memory>
#include <string>
#include <variant>

struct SyntaxSuccess
{
    std::unique_ptr<ProgramNode> program;
};

struct SyntaxFailure
{
    std::string message;
    SourceSpan sourceSpan;
};

using SyntaxResult = std::variant<SyntaxSuccess, SyntaxFailure>;

#endif // JACK_PARSER_SYNTAXRESULT_H
