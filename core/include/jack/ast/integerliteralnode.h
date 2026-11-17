#ifndef JACK_AST_INTEGERLITERALNODE_H
#define JACK_AST_INTEGERLITERALNODE_H

#include <string>

#include "jack/ast/expressionnode.h"

class IntegerLiteralNode : public ExpressionNode {
public:
    IntegerLiteralNode(
        const SourceSpan& sourceSpan,
        int value,
        const std::string& lexeme
        );

    int obterValor() const;
    const std::string& obterLexema() const;

private:
    int value;
    std::string lexeme;
};

#endif // JACK_AST_INTEGERLITERALNODE_H
