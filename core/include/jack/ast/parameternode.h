#ifndef JACK_AST_PARAMETERNODE_H
#define JACK_AST_PARAMETERNODE_H

#include "jack/ast/astnode.h"

#include <string>

class ParameterNode : public ASTNode {
public:
    ParameterNode(
        const SourceSpan& sourceSpan,
        const std::string& type,
        const std::string& name
        );

    const std::string& obterTipo() const;

    const std::string& obterNome() const;

private:
    std::string type;
    std::string name;
};

#endif // JACK_AST_PARAMETERNODE_H
