#ifndef JACK_AST_RETURNSTATEMENTNODE_H
#define JACK_AST_RETURNSTATEMENTNODE_H

#include "jack/ast/statementnode.h"

#include <memory>

class ExpressionNode;

class ReturnStatementNode : public StatementNode {
public:
    ReturnStatementNode(
        const SourceSpan& sourceSpan,
        std::unique_ptr<ExpressionNode> expression = nullptr
        );

    ~ReturnStatementNode() override;

    bool possuiExpressao() const;
    const ExpressionNode* obterExpressao() const;

private:
    std::unique_ptr<ExpressionNode> expression;
};

#endif // JACK_AST_RETURNSTATEMENTNODE_H
