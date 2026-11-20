#include "jack/ast/subroutinecallnode.h"

SubroutineCallNode::SubroutineCallNode(
    const SourceSpan& sourceSpan,
    const std::optional<std::string>& qualifier,
    const std::string& subroutineName
    )
    : ExpressionNode(sourceSpan),
    qualifier(qualifier),
    subroutineName(subroutineName)
{
}

SubroutineCallNode::~SubroutineCallNode() = default;

const std::optional<std::string>& SubroutineCallNode::obterQualificador() const
{
    return qualifier;
}

const std::string& SubroutineCallNode::obterNomeSubrotina() const
{
    return subroutineName;
}

void SubroutineCallNode::adicionarArgumento(
    std::unique_ptr<ExpressionNode> argumento
    )
{
    arguments.push_back(std::move(argumento));
}

const std::vector<std::unique_ptr<ExpressionNode>>&
SubroutineCallNode::obterArgumentos() const
{
    return arguments;
}
