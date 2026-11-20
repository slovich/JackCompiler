#include "jack/ast/dostatementnode.h"
#include "jack/ast/subroutinecallnode.h"

DoStatementNode::DoStatementNode(
    const SourceSpan& sourceSpan,
    std::unique_ptr<SubroutineCallNode> call
    )
    : StatementNode(sourceSpan),
    call(std::move(call))
{
}

DoStatementNode::~DoStatementNode() = default;

const SubroutineCallNode& DoStatementNode::obterChamada() const
{
    return *call;
}
