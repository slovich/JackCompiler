#ifndef JACK_AST_UNARYEXPRESSIONNODE_H
#define JACK_AST_UNARYEXPRESSIONNODE_H

#include "jack/ast/expressionnode.h"
#include "jack/ast/unaryoperator.h"

#include <memory>

class UnaryExpressionNode : public ExpressionNode {
public:
    UnaryExpressionNode(
        const SourceSpan& sourceSpan,
        UnaryOperator op,
        std::unique_ptr<ExpressionNode> operand
        );

    ~UnaryExpressionNode() override;

    UnaryOperator obterOperador() const;
    const ExpressionNode& obterOperando() const;

private:
    UnaryOperator op;
    std::unique_ptr<ExpressionNode> operand;
};

#endif // JACK_AST_UNARYEXPRESSIONNODE_H
