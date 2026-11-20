#include <utility>

#include "jack/ast/parenthesizedexpressionnode.h"

ParenthesizedExpressionNode::ParenthesizedExpressionNode(
    const SourceSpan& sourceSpan,
    std::unique_ptr<ExpressionNode> expression
    )
    : ExpressionNode(sourceSpan),
    expression(std::move(expression))
{
}

ParenthesizedExpressionNode::~ParenthesizedExpressionNode() = default;

const ExpressionNode& ParenthesizedExpressionNode::obterExpressao() const
{
    return *expression;
}
