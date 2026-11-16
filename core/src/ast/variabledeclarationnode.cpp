#include "jack/ast/variabledeclarationnode.h"

VariableDeclarationNode::VariableDeclarationNode(
    const SourceSpan& sourceSpan,
    VariableKind kind,
    const std::string& type,
    const std::vector<std::string>& names
    )
    : ASTNode(sourceSpan),
    kind(kind),
    type(type),
    names(names)
{
}

VariableKind VariableDeclarationNode::obterKind() const
{
    return kind;
}

const std::string& VariableDeclarationNode::obterTipo() const
{
    return type;
}

const std::vector<std::string>& VariableDeclarationNode::obterNomes() const
{
    return names;
}
