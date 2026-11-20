#ifndef JACK_AST_STRINGLITERALNODE_H
#define JACK_AST_STRINGLITERALNODE_H

#include "jack/ast/expressionnode.h"

#include <string>

class StringLiteralNode : public ExpressionNode {
public:
    StringLiteralNode(
        const SourceSpan& sourceSpan,
        const std::string& value
        );

    const std::string& obterValor() const;

private:
    std::string value;
};

#endif // JACK_AST_STRINGLITERALNODE_H
