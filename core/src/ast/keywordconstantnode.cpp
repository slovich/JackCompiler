#include "jack/ast/keywordconstantnode.h"

KeywordConstantNode::KeywordConstantNode(
    const SourceSpan& sourceSpan,
    KeywordConstant value
    )
    : ExpressionNode(sourceSpan),
    value(value)
{
}

KeywordConstant KeywordConstantNode::obterValor() const
{
    return value;
}
