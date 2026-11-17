#include "jack/ast/integerliteralnode.h"

#include <utility>

IntegerLiteralNode::IntegerLiteralNode(
    const SourceSpan& sourceSpan,
    int value,
    const std::string& lexeme
    )
    : ExpressionNode(sourceSpan),
    value(value),
    lexeme(lexeme)
{
}

int IntegerLiteralNode::obterValor() const
{
    return value;
}

const std::string& IntegerLiteralNode::obterLexema() const
{
    return lexeme;
}
