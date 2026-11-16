#include "jack/ast/parameternode.h"

ParameterNode::ParameterNode(
    const SourceSpan& sourceSpan,
    const std::string& type,
    const std::string& name
    )
    : ASTNode(sourceSpan),
    type(type),
    name(name)
{
}

const std::string& ParameterNode::obterTipo() const
{
    return type;
}

const std::string& ParameterNode::obterNome() const
{
    return name;
}
