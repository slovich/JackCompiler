#ifndef JACK_AST_CLASSNODE_H
#define JACK_AST_CLASSNODE_H

#include "jack/ast/astnode.h"

#include <memory>
#include <string>
#include <vector>

class VariableDeclarationNode;
class SubroutineDeclarationNode;

class ClassNode : public ASTNode {
public:
    ClassNode(const SourceSpan& sourceSpan, const std::string& name);
    ~ClassNode() override;

    const std::string& obterNome() const;

    void adicionarVariavelClasse(
        std::unique_ptr<VariableDeclarationNode> variavel
        );

    void adicionarSubrotina(
        std::unique_ptr<SubroutineDeclarationNode> subrotina
        );

    const std::vector<std::unique_ptr<VariableDeclarationNode>>&
    obterVariaveisClasse() const;

    const std::vector<std::unique_ptr<SubroutineDeclarationNode>>&
    obterSubrotinas() const;

private:
    std::string name;

    std::vector<std::unique_ptr<VariableDeclarationNode>> classVariables;
    std::vector<std::unique_ptr<SubroutineDeclarationNode>> subroutines;
};

#endif // JACK_AST_CLASSNODE_H
