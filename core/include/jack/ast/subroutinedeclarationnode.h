#ifndef JACK_AST_SUBROUTINEDECLARATIONNODE_H
#define JACK_AST_SUBROUTINEDECLARATIONNODE_H

#include "jack/ast/astnode.h"
#include "jack/ast/subroutinekind.h"

#include <memory>
#include <string>
#include <vector>

class ParameterNode;
class VariableDeclarationNode;
class StatementNode;

class SubroutineDeclarationNode : public ASTNode {
public:
    SubroutineDeclarationNode(
        const SourceSpan& sourceSpan,
        SubroutineKind kind,
        const std::string& returnType,
        const std::string& name
        );

    ~SubroutineDeclarationNode() override;

    SubroutineKind obterKind() const;

    const std::string& obterTipoRetorno() const;

    const std::string& obterNome() const;

    void adicionarParametro(std::unique_ptr<ParameterNode> parametro);

    void adicionarVariavelLocal(
        std::unique_ptr<VariableDeclarationNode> variavel
        );

    void adicionarInstrucao(std::unique_ptr<StatementNode> instrucao);

    const std::vector<std::unique_ptr<ParameterNode>>&
    obterParametros() const;

    const std::vector<std::unique_ptr<VariableDeclarationNode>>&
    obterVariaveisLocais() const;

    const std::vector<std::unique_ptr<StatementNode>>&
    obterCorpo() const;

private:
    SubroutineKind kind;
    std::string returnType;
    std::string name;

    std::vector<std::unique_ptr<ParameterNode>> parameters;
    std::vector<std::unique_ptr<VariableDeclarationNode>> localVariables;
    std::vector<std::unique_ptr<StatementNode>> body;
};

#endif // JACK_AST_SUBROUTINEDECLARATIONNODE_H
