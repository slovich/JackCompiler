#include "jack/ast/unaryexpressionnode.h"

UnaryExpressionNode::UnaryExpressionNode(
    const SourceSpan& sourceSpan,
    UnaryOperator op,
    std::unique_ptr<ExpressionNode> operand
    )
    : ExpressionNode(sourceSpan),
    op(op),
    operand(std::move(operand))
{
}

UnaryExpressionNode::~UnaryExpressionNode() = default;

UnaryOperator UnaryExpressionNode::obterOperador() const
{
    return op;
}

const ExpressionNode& UnaryExpressionNode::obterOperando() const
{
    return *operand;
}
