#ifndef JACK_AST_VARIABLEDECLARATIONNODE_H
#define JACK_AST_VARIABLEDECLARATIONNODE_H

#include "jack/ast/astnode.h"
#include "jack/ast/variablekind.h"

#include <string>
#include <vector>

class VariableDeclarationNode : public ASTNode {
public:
    VariableDeclarationNode(
        const SourceSpan& sourceSpan,
        VariableKind kind,
        const std::string& type,
        const std::vector<std::string>& names
        );

    VariableKind obterKind() const;

    const std::string& obterTipo() const;

    const std::vector<std::string>& obterNomes() const;

private:
    VariableKind kind;
    std::string type;
    std::vector<std::string> names;
};

#endif // JACK_AST_VARIABLEDECLARATIONNODE_H
