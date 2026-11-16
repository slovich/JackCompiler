#include "jack/ast/programnode.h"
#include "jack/ast/classnode.h"
#include "jack/ast/variabledeclarationnode.h"
#include "jack/ast/subroutinedeclarationnode.h"

ProgramNode::ProgramNode(const SourceSpan& sourceSpan)
    : ASTNode(sourceSpan)
{
}

ProgramNode::~ProgramNode() = default;

void ProgramNode::adicionarClasse(std::unique_ptr<ClassNode> classe)
{
    classes.push_back(std::move(classe));
}

const std::vector<std::unique_ptr<ClassNode>>& ProgramNode::obterClasses() const
{
    return classes;
}
