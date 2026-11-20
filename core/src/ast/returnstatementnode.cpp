#include "jack/ast/returnstatementnode.h"
#include "jack/ast/expressionnode.h"

ReturnStatementNode::ReturnStatementNode(
    const SourceSpan& sourceSpan,
    std::unique_ptr<ExpressionNode> expression
    )
    : StatementNode(sourceSpan),
    expression(std::move(expression))
{
}

ReturnStatementNode::~ReturnStatementNode() = default;

bool ReturnStatementNode::possuiExpressao() const
{
    return expression != nullptr;
}

const ExpressionNode* ReturnStatementNode::obterExpressao() const
{
    return expression.get();
}
