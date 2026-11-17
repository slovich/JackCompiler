#include "jack/ast/ifstatementnode.h"
#include "jack/ast/expressionnode.h"

IfStatementNode::IfStatementNode(
    const SourceSpan& sourceSpan,
    std::unique_ptr<ExpressionNode> condition
    )
    : StatementNode(sourceSpan),
    condition(std::move(condition))
{
}

IfStatementNode::~IfStatementNode() = default;

const ExpressionNode& IfStatementNode::obterCondicao() const
{
    return *condition;
}

void IfStatementNode::definirPossuiElse()
{
    hasElse = true;
}

bool IfStatementNode::possuiElse() const
{
    return hasElse;
}

void IfStatementNode::adicionarInstrucaoThen(
    std::unique_ptr<StatementNode> instrucao
    )
{
    thenBody.push_back(std::move(instrucao));
}

void IfStatementNode::adicionarInstrucaoElse(
    std::unique_ptr<StatementNode> instrucao
    )
{
    elseBody.push_back(std::move(instrucao));
}

const std::vector<std::unique_ptr<StatementNode>>&
IfStatementNode::obterCorpoThen() const
{
    return thenBody;
}

const std::vector<std::unique_ptr<StatementNode>>&
IfStatementNode::obterCorpoElse() const
{
    return elseBody;
}
