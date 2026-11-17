#ifndef JACK_AST_IFSTATEMENTNODE_H
#define JACK_AST_IFSTATEMENTNODE_H

#include "jack/ast/statementnode.h"

#include <memory>
#include <vector>

class ExpressionNode;

class IfStatementNode : public StatementNode {
public:
    IfStatementNode(
        const SourceSpan& sourceSpan,
        std::unique_ptr<ExpressionNode> condition
        );

    ~IfStatementNode() override;

    const ExpressionNode& obterCondicao() const;

    void definirPossuiElse();
    bool possuiElse() const;

    void adicionarInstrucaoThen(std::unique_ptr<StatementNode> instrucao);
    void adicionarInstrucaoElse(std::unique_ptr<StatementNode> instrucao);

    const std::vector<std::unique_ptr<StatementNode>>& obterCorpoThen() const;
    const std::vector<std::unique_ptr<StatementNode>>& obterCorpoElse() const;

private:
    std::unique_ptr<ExpressionNode> condition;
    std::vector<std::unique_ptr<StatementNode>> thenBody;
    std::vector<std::unique_ptr<StatementNode>> elseBody;
    bool hasElse = false;
};

#endif // JACK_AST_IFSTATEMENTNODE_H
