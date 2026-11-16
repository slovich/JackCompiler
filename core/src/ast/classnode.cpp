#include "jack/ast/classnode.h"
#include "jack/ast/variabledeclarationnode.h"
#include "jack/ast/subroutinedeclarationnode.h"

ClassNode::ClassNode(const SourceSpan& sourceSpan, const std::string& name)
    : ASTNode(sourceSpan),
    name(name)
{
}

ClassNode::~ClassNode() = default;

const std::string& ClassNode::obterNome() const
{
    return name;
}

void ClassNode::adicionarVariavelClasse(
    std::unique_ptr<VariableDeclarationNode> variavel)
{
    classVariables.push_back(std::move(variavel));
}

void ClassNode::adicionarSubrotina(
    std::unique_ptr<SubroutineDeclarationNode> subrotina)
{
    subroutines.push_back(std::move(subrotina));
}

const std::vector<std::unique_ptr<VariableDeclarationNode>>&
ClassNode::obterVariaveisClasse() const
{
    return classVariables;
}

const std::vector<std::unique_ptr<SubroutineDeclarationNode>>&
ClassNode::obterSubrotinas() const
{
    return subroutines;
}
