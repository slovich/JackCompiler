#ifndef JACK_AST_ASTNODE_H
#define JACK_AST_ASTNODE_H

#include "jack/ast/sourcespan.h"

class ASTNode {
public:
    explicit ASTNode(const SourceSpan& sourceSpan);

    virtual ~ASTNode() = 0;

    const SourceSpan& obterSourceSpan() const;

private:
    SourceSpan sourceSpan;
};

#endif // JACK_AST_ASTNODE_H
