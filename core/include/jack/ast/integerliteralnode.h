#ifndef JACK_AST_INTEGERLITERALNODE_H
#define JACK_AST_INTEGERLITERALNODE_H

#include "jack/ast/expressionnode.h"

class IntegerLiteralNode : public ExpressionNode {
public:
    IntegerLiteralNode(
        const SourceSpan& sourceSpan,
        int value
        );

    int obterValor() const;

private:
    int value;
};

#endif // JACK_AST_INTEGERLITERALNODE_H
