#ifndef JACK_AST_VARIABLEREFERENCENODE_H
#define JACK_AST_VARIABLEREFERENCENODE_H

#include "jack/ast/expressionnode.h"

#include <string>

class VariableReferenceNode : public ExpressionNode {
public:
    VariableReferenceNode(
        const SourceSpan& sourceSpan,
        const std::string& name
        );

    const std::string& obterNome() const;

private:
    std::string name;
};

#endif // JACK_AST_VARIABLEREFERENCENODE_H
