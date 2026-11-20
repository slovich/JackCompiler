#ifndef JACK_AST_EXPRESSIONNODE_H
#define JACK_AST_EXPRESSIONNODE_H

#include "jack/ast/astnode.h"

class ExpressionNode : public ASTNode {
public:
    explicit ExpressionNode(const SourceSpan& sourceSpan);

    ~ExpressionNode() override = default;
};

#endif // JACK_AST_EXPRESSIONNODE_H
