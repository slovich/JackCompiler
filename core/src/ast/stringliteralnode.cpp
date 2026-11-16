#include "jack/ast/stringliteralnode.h"

StringLiteralNode::StringLiteralNode(
    const SourceSpan& sourceSpan,
    const std::string& value
    )
    : ExpressionNode(sourceSpan),
    value(value)
{
}

const std::string& StringLiteralNode::obterValor() const
{
    return value;
}
