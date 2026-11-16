#ifndef JACK_AST_KEYWORDCONSTANTNODE_H
#define JACK_AST_KEYWORDCONSTANTNODE_H

#include "jack/ast/expressionnode.h"
#include "jack/ast/keywordconstant.h"

class KeywordConstantNode : public ExpressionNode {
public:
    KeywordConstantNode(
        const SourceSpan& sourceSpan,
        KeywordConstant value
        );

    KeywordConstant obterValor() const;

private:
    KeywordConstant value;
};

#endif // JACK_AST_KEYWORDCONSTANTNODE_H
