#ifndef JACK_AST_WHILESTATEMENTNODE_H
#define JACK_AST_WHILESTATEMENTNODE_H

#include "jack/ast/statementnode.h"

#include <memory>
#include <vector>

class ExpressionNode;

class WhileStatementNode : public StatementNode {
public:
    WhileStatementNode(
        const SourceSpan& sourceSpan,
        std::unique_ptr<ExpressionNode> condition
        );

    ~WhileStatementNode() override;

    const ExpressionNode& obterCondicao() const;

    void adicionarInstrucao(std::unique_ptr<StatementNode> instrucao);

    const std::vector<std::unique_ptr<StatementNode>>& obterCorpo() const;

private:
    std::unique_ptr<ExpressionNode> condition;
    std::vector<std::unique_ptr<StatementNode>> body;
};

#endif // JACK_AST_WHILESTATEMENTNODE_H
