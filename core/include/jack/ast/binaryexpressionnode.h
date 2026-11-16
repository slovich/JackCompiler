#ifndef JACK_AST_BINARYEXPRESSIONNODE_H
#define JACK_AST_BINARYEXPRESSIONNODE_H

#include "jack/ast/expressionnode.h"
#include "jack/ast/binaryoperator.h"

#include <memory>

class BinaryExpressionNode : public ExpressionNode {
public:
    BinaryExpressionNode(
        const SourceSpan& sourceSpan,
        std::unique_ptr<ExpressionNode> left,
        BinaryOperator op,
        std::unique_ptr<ExpressionNode> right
        );

    ~BinaryExpressionNode() override;

    const ExpressionNode& obterOperandoEsquerdo() const;
    BinaryOperator obterOperador() const;
    const ExpressionNode& obterOperandoDireito() const;

private:
    std::unique_ptr<ExpressionNode> left;
    BinaryOperator op;
    std::unique_ptr<ExpressionNode> right;
};

#endif // JACK_AST_BINARYEXPRESSIONNODE_H
