#ifndef PARENTHESIZEDEXPRESSIONNODE_H
#define PARENTHESIZEDEXPRESSIONNODE_H

#include <memory>

#include "jack/ast/expressionnode.h"

class ParenthesizedExpressionNode : public ExpressionNode
{
public:
    ParenthesizedExpressionNode(
        const SourceSpan& sourceSpan,
        std::unique_ptr<ExpressionNode> expression
        );

    ~ParenthesizedExpressionNode() override;

    const ExpressionNode& obterExpressao() const;

private:
    std::unique_ptr<ExpressionNode> expression;
};

#endif // PARENTHESIZEDEXPRESSIONNODE_H
