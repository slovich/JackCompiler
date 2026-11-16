#include "jack/ast/whilestatementnode.h"
#include "jack/ast/expressionnode.h"

WhileStatementNode::WhileStatementNode(
    const SourceSpan& sourceSpan,
    std::unique_ptr<ExpressionNode> condition
    )
    : StatementNode(sourceSpan),
    condition(std::move(condition))
{
}

WhileStatementNode::~WhileStatementNode() = default;

const ExpressionNode& WhileStatementNode::obterCondicao() const
{
    return *condition;
}

void WhileStatementNode::adicionarInstrucao(
    std::unique_ptr<StatementNode> instrucao
    )
{
    body.push_back(std::move(instrucao));
}

const std::vector<std::unique_ptr<StatementNode>>&
WhileStatementNode::obterCorpo() const
{
    return body;
}
