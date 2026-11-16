#include "jack/ast/variablereferencenode.h"

VariableReferenceNode::VariableReferenceNode(
    const SourceSpan& sourceSpan,
    const std::string& name
    )
    : ExpressionNode(sourceSpan),
    name(name)
{
}

const std::string& VariableReferenceNode::obterNome() const
{
    return name;
}
