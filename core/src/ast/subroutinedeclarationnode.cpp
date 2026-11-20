#include "jack/ast/subroutinedeclarationnode.h"

#include "jack/ast/parameternode.h"
#include "jack/ast/variabledeclarationnode.h"
#include "jack/ast/statementnode.h"

SubroutineDeclarationNode::SubroutineDeclarationNode(
    const SourceSpan& sourceSpan,
    SubroutineKind kind,
    const std::string& returnType,
    const std::string& name
    )
    : ASTNode(sourceSpan),
    kind(kind),
    returnType(returnType),
    name(name)
{
}

SubroutineDeclarationNode::~SubroutineDeclarationNode() = default;

SubroutineKind SubroutineDeclarationNode::obterKind() const
{
    return kind;
}

const std::string& SubroutineDeclarationNode::obterTipoRetorno() const
{
    return returnType;
}

const std::string& SubroutineDeclarationNode::obterNome() const
{
    return name;
}

void SubroutineDeclarationNode::adicionarParametro(
    std::unique_ptr<ParameterNode> parametro)
{
    parameters.push_back(std::move(parametro));
}

void SubroutineDeclarationNode::adicionarVariavelLocal(
    std::unique_ptr<VariableDeclarationNode> variavel)
{
    localVariables.push_back(std::move(variavel));
}

void SubroutineDeclarationNode::adicionarInstrucao(
    std::unique_ptr<StatementNode> instrucao)
{
    body.push_back(std::move(instrucao));
}

const std::vector<std::unique_ptr<ParameterNode>>&
SubroutineDeclarationNode::obterParametros() const
{
    return parameters;
}

const std::vector<std::unique_ptr<VariableDeclarationNode>>&
SubroutineDeclarationNode::obterVariaveisLocais() const
{
    return localVariables;
}

const std::vector<std::unique_ptr<StatementNode>>&
SubroutineDeclarationNode::obterCorpo() const
{
    return body;
}
