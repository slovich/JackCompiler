#ifndef JACK_AST_LETSTATEMENTNODE_H
#define JACK_AST_LETSTATEMENTNODE_H

#include "jack/ast/statementnode.h"

#include <memory>

class ExpressionNode;

class LetStatementNode : public StatementNode {
public:
    LetStatementNode(
        const SourceSpan& sourceSpan,
        std::unique_ptr<ExpressionNode> target,
        std::unique_ptr<ExpressionNode> value
        );

    ~LetStatementNode() override;

    const ExpressionNode& obterAlvo() const;
    const ExpressionNode& obterValor() const;

private:
    std::unique_ptr<ExpressionNode> target;
    std::unique_ptr<ExpressionNode> value;
};

#endif // JACK_AST_LETSTATEMENTNODE_H
