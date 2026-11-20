#include "jack/ast/arrayaccessnode.h"

ArrayAccessNode::ArrayAccessNode(
    const SourceSpan& sourceSpan,
    const std::string& name,
    std::unique_ptr<ExpressionNode> index
    )
    : ExpressionNode(sourceSpan),
    name(name),
    index(std::move(index))
{
}

ArrayAccessNode::~ArrayAccessNode() = default;

const std::string& ArrayAccessNode::obterNome() const
{
    return name;
}

const ExpressionNode& ArrayAccessNode::obterIndice() const
{
    return *index;
}
