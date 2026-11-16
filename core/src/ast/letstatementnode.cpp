#include "jack/ast/letstatementnode.h"
#include "jack/ast/expressionnode.h"

LetStatementNode::LetStatementNode(
    const SourceSpan& sourceSpan,
    std::unique_ptr<ExpressionNode> target,
    std::unique_ptr<ExpressionNode> value
    )
    : StatementNode(sourceSpan),
    target(std::move(target)),
    value(std::move(value))
{
}

LetStatementNode::~LetStatementNode() = default;

const ExpressionNode& LetStatementNode::obterAlvo() const
{
    return *target;
}

const ExpressionNode& LetStatementNode::obterValor() const
{
    return *value;
}
