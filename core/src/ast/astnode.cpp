#include "jack/ast/astnode.h"

ASTNode::ASTNode(const SourceSpan& sourceSpan)
    : sourceSpan(sourceSpan)
{
}

ASTNode::~ASTNode() = default;

const SourceSpan& ASTNode::obterSourceSpan() const
{
    return sourceSpan;
}
