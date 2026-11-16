#include "jack/ast/integerliteralnode.h"

IntegerLiteralNode::IntegerLiteralNode(
    const SourceSpan& sourceSpan,
    int value
    )
    : ExpressionNode(sourceSpan),
    value(value)
{
}

int IntegerLiteralNode::obterValor() const
{
    return value;
}
