#include "jack/ast/binaryexpressionnode.h"

BinaryExpressionNode::BinaryExpressionNode(
    const SourceSpan& sourceSpan,
    std::unique_ptr<ExpressionNode> left,
    BinaryOperator op,
    std::unique_ptr<ExpressionNode> right
    )
    : ExpressionNode(sourceSpan),
    left(std::move(left)),
    op(op),
    right(std::move(right))
{
}

BinaryExpressionNode::~BinaryExpressionNode() = default;

const ExpressionNode& BinaryExpressionNode::obterOperandoEsquerdo() const
{
    return *left;
}

BinaryOperator BinaryExpressionNode::obterOperador() const
{
    return op;
}

const ExpressionNode& BinaryExpressionNode::obterOperandoDireito() const
{
    return *right;
}
